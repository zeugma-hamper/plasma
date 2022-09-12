require 'weakref'

require 'Protein'
require 'Slaw'

# Plasma is a combination of:
# * A schema-free, binary envelope for message data called a Protein
# * A persistance layer for Proteins implemented as a ring buffer called a Pool
# * A network transport protocol to pass proteins to/from remote pools
module Plasma

  NO_WAIT = 0.0
  WAIT_FOREVER = -1.0

  POOL_DEFAULT_SIZE = 1048576

class PoolOperationError < StandardError ; end

#--
# XXX: Can someone document the rationale for all these to_string calls?
# Shouldn't our arguments be strings already?
# YYY: It's been suggested that someone might pass pool names as symbols?
#++
#
# A pool is a ring buffer of Proteins.
#
class Pool
  # Creates a pool with name pname.
  #
  # pname can be:
  # * a local pool, such as 'mypool'
  # * a remote networked pool, such as 'tcp://otherhost.com/mypool'
  # * a local pool accessed via the networking stack, such as 'tcp://localhost/mypool'
  def Pool.create(pname, options=nil)
    options_slaw = Slaw.new( { "size" => POOL_DEFAULT_SIZE }, :map )
    if (not options.nil?)
      options_slaw = Slaw.merge_maps( options_slaw, Slaw.new( options, :map ) )
    end
    _c_create pname.to_str, options_slaw
  end

  # Deletes the pool with name pname.
  #
  # pname can be:
  # * a local pool, such as 'mypool'
  # * a remote networked pool, such as 'tcp://otherhost.com/mypool'
  # * a local pool accessed via the networking stack, such as 'tcp://localhost/mypool'
  def Pool.dispose(pname)
    _c_dispose pname.to_str
  end

  # Returns a lists of pools contained on the given server. If server is null, lists local pools.
  #
  # The server must be a fully qualified uri and contain a trailing slash, e.g. 'tcp://otherhost.com/'
  def Pool.list_pools(server=nil)
    _c_list_pools(server)
  end

  # Participates in a pool with name pname.
  #
  # Returns a Hose object.
  #
  # pname can be:
  # * a local pool, such as 'mypool'
  # * a remote networked pool, such as 'tcp://otherhost.com/mypool'
  # * a local pool accessed via the networking stack, such as 'tcp://localhost/mypool'
  #
  # Ex. h = Pool.participate("tcp://otherhost.com/mypool")
  def Pool.participate(pname)
    Hose.new( pname.to_str )
  end

  # Renames a pool.
  #
  # Only pools which are not in use can be renamed.
  #
  # * Throws #PoolInUseError if any hoses are open to the pool.
  # * Throws #PoolExistsError if the destination exists
  # * Throws #PoolNotFoundError if the source does not.
  def Pool.rename(oldname, newname)
    _c_rename(oldname.to_str, newname.to_str)
  end

  # Returns true if pool is in use, false if it is not, or raises
  # an exception if it can't determine.  (e. g. doesn't exist)
  # Beware of TOCTOU: http://cwe.mitre.org/data/definitions/367.html
  def Pool.is_in_use?(name)
    _c_is_in_use(name.to_str)
  end

end

# A hose is a connection to a pool, local or remote. A hose allows for read, write, and seek operations on a pool.
class Hose
  attr_accessor :pool_name

  def initialize(pname)
    @pool_name = pname
     _c_initialize        # fix: initializes @_pool_hose to nil, but
                          # if that's all we do here, we should just
                          # do it in ruby, to be clearer
  end

  # Returns the pool name
  def name
    _c_get_name
  end

  # Sets the pool name to nm
  def name=(nm)
    _c_set_name(Slaw._ensure_utf8(nm))
  end

  # Retrieves the next protein from the pool, after the current index.
  def next(timeout=NO_WAIT)
    _c_await_next(timeout)
  end

  # Fetches the amount of given proteins (if available) starting at the
  # protein with the given index. If an index of -1 is given it will
  # start reading proteins at the current index.
  def fetch(amount, index=-1)
    _c_fetch(amount, index)
  end

  # implemented in c
  #
  #  deposit (protein)
  #  rewind
  #  runout
  #  tolast
  #  seekby (offset)
  #  seekto (index)
  #  nth (index)
  #  oldest_index
  #  newest_index
  #  withdraw
  #
  # note that next() is not implemented directly in c because we don't
  # know how to do default argument values in a super-simple way. we
  # could certainly do something using ruby varargs

  # Sets the "oldest" index of a pool, essentially erasing any proteins
  # prior to that index.  Returns OB_OK if at least one protein was erased.
  # Returns OB_NOTHING_TO_DO if idx_in is older than the current
  # oldest index.  Returns POOL_NO_SUCH_PROTEIN if idx_in is newer than
  # the newest index.
  def oldest_index=(idx)
    _c_advance_oldest(idx)
  end

  def add_trib__dispatch(gang)
    gang.add_standard_c_hose_tributary self
  end

  def is_empty?
    begin
      newest_index
    rescue PoolNoProteinError
      true
    else
      false
    end
  end

  # Allows some of the options that were specified in pool_create
  # to be changed after the pool is created.  options can be
  # either a protein or a slaw map, as in pool_create.
  #
  # The possible keys for the options map/protein are documented in
  # share/doc/g-speak/option-map-keys.html in the install tree,
  # or doc-non-dox/option-map-keys.html in the source tree.
  def change_options(options)
    options_slaw = Slaw.new( options, :map )
    _c_change_options(options_slaw)
  end

  # Returns a hash with information about a pool.
  #
  # Should always include an ingest "type", which is a string naming the
  # pool type, and "terminal", which is a boolean which is true if this is
  # a terminal pool type like "mmap", or false if this is a transport pool
  # type like "tcp".
  #
  # For mmap pools, should include an ingest "size", which is an integer
  # giving the size of the pool in bytes.
  # For tcp pools, should include an ingest "host", which is a string
  # naming the host the pool is on, and "port" which is an integer
  # giving the port.
  #
  # For other pool types, ingests with other relevant info can be included.
  # If "hops" is 0, means return information about this pool hose.
  # If "hops" is 1, means return information about the pool beyond this
  # hose (assuming this hose is a nonterminal type like TCP).  And higher
  # values of "hops" mean go further down the line, if multiple nonterminal
  # types are chained together.  If "hops" is -1, means return information
  # about the terminal pool, no matter how far it is.
  def info(hops=-1)
    # c function returns a protein, but really a map is nicer
    _c_get_info(hops).ingests
  end

  # True if the pool is resizable, false otherwise.
  def resizable?
    mmv = info["mmap-pool-version"]
    (! mmv.nil?) && (mmv > 0)
  end

  # Returns the size of the pool in bytes.
  def size
    info["size"]
  end

  # Sets the size of the pool to x.
  def size=(x)
    change_options({"size" => x})
  end

  # Returns the bytes used.
  def size_used
    info["size-used"]
  end

  # Returns the capacity (in # of proteins) of the pool's table of contents
  def toc_capacity
    info["toc-capacity"]
  end

  def stop_when_full
    info["stop-when-full"]
  end

  def stop_when_full=(x)
    change_options({"stop-when-full" => x})
  end

  # Boolean whether this pool is frozen.
  def frozen
    info["frozen"]
  end

  # Set whether the pool is frozen or not.
  def frozen=(x)
    change_options({"frozen" => x})
  end

  # Boolean whether this pool with be auto-disposed after withdrawal.
  def auto_dispose
    info["auto-dispose"]
  end

  # Set whether this pool should be auto-disposed after withdrawal.
  def auto_dispose=(x)
    change_options({"auto-dispose" => x})
  end

  def sync
    info["sync"]
  end

  def sync=(x)
    change_options({"sync" => x})
  end
end


class HoseNotAddableToGangError < StandardError ; end

# A HoseGang is a collection of #Hose objects that are treated as a single entity for the purposes of reading.
class HoseGang
  attr_accessor :tributaries
  attr_accessor :_awaiter

  def initialize(*args)
    @tributaries = Array.new
    args.each do |hose_or_string|
      add_tributary hose_or_string
    end
  end

  # Returns the next #Protein from whichever #Pool in the gang has an available protein first.
  def next(timeout=NO_WAIT)
    return @_awaiter ? @_awaiter.next(timeout) : nil
  end

  # Tests whether the provided object can be added to a gang. To be addable, it must behave like or truly be a #Hose.
  def is_addable?(hose_duck)
    hose_duck.respond_to?(:next) && hose_duck.respond_to?(:add_trib__dispatch)
  end

  # Ads a #Hose to the gang
  def add_tributary(hose_or_string)
    if is_addable? hose_or_string
      hose_or_string.add_trib__dispatch self
      @tributaries.push hose_or_string
      return self
    else
      h = Hose.new hose_or_string
      if is_addable? h
        h.add_trib__dispatch self
        @tributaries.push h
        return self
      end
    end
    raise HoseNotAddableToGangError,
          "hose doesn't support :next and :add_trib__dispatch"
  end

  # Removes a #Hose from the gang
  def remove_tributary(hose_or_hosename)
    return false  if  ! @_awaiter

    # support being passed either a hose object or a string which we
    # assume to be a hose name. in either case, we'll remove all
    # tributaries that correspond
    if hose_or_hosename.respond_to? :to_str
      name_to_find = hose_or_hosename.to_str
      hs = tributaries.select { |t| t.name == name_to_find }
    else
      hs = tributaries.select { |t| t == hose_or_hosename }
    end

    hs.each do |h|
      @_awaiter.remove_hose h
      tributaries.delete h
    end
    return hs.size > 0
  end

  # Adds a #Hose to the gang
  def add_standard_c_hose_tributary(hose)
    @_awaiter ||= GangBasicAwaiter.new self
    @_awaiter.add_hose hose
  end

  # Withdraw all Hose objects from all pools
  def withdraw
    hs = @tributaries.select { |t| t.respond_to?(:withdraw) }
    if _awaiter
      hs.each { |h| _awaiter.remove_hose(h) }
    end
    hs.each { |h| h.withdraw }
    @tributaries = Array.new
  end
end


class GangBasicAwaiter
  attr_accessor :_hose_gang

  def initialize(containing_gang)
    @containing_gang_ref = WeakRef.new containing_gang
    @_hose_gang ||= _c_initialize_gang
  end

  # Wait for and return the next protein.  The timeout argument specifies how long to wait
  # for a protein:
  # * timeout == POOL_WAIT_FOREVER (-1): Wait forever
  # * timeout == POOL_NO_WAIT (0)      : Don't wait at all
  # * timeout  > 0                    : Return after this many seconds.
  #
  # Returns POOL_AWAIT_TIMEDOUT if no protein arrived before the
  # timeout expired.
  def next(timeout)
    _c_await_next timeout
  end

  def add_hose(hose)
    _c_add_hose hose
  end

  def remove_hose(hose)
    _c_remove_hose hose
  end


end


end

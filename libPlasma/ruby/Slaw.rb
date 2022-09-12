# -*- coding: utf-8 -*-
# ¡¡¡¡¡¡¡¡¡ IF YOU WANT TO USE JSON, YOU NEED THE JSON GEM INSTALLED !!!!!!!!!
require 'rubyPlasma'
require 'base64'

#---
# notes -- fix:
#
#   errors need to be clearer for things like too few arguments to a
#   v234 constructor (or implicit constructor from inside a list). not
#   sure how to do this without slowing things down.
#
#   we probably need v2? v3? and v4? introspectors
#
#   there are not vXNNXXX_array kernel module constructors
#
#+++

module Plasma

class SlawTypeNoMethodError < NoMethodError; end
class SlawConsNeedsTwoArgumentsError < StandardError; end
class SlawTypeTagError < StandardError; end
class UserError < StandardError; end

# The following two exceptions are never raised anymore (bug 1871),
# but we're still going to define them, in case there is existing
# code that uses them in a rescue clause.
class ProteinDescripsAreNotListError < StandardError; end
class ProteinIngestsAreNotMapError < StandardError; end

# In c/c++ plasma, Slaw is a wrapper for all g-speak supported data types. It is necessary because of c's strong typing.
# In a duck-typed language like Ruby where all classes descend from Object, the concept of Slaw is not necessary. However,
# as these are bindings for c plasma, we do need to deal with Slaw here and there. For the most part, when using the #Pool,
# #Hose, & #Protein APIs, you can safely ignore Slaw.
class Slaw

  include Enumerable

  protected
  attr_reader :_c_slaw, :_container

  public
  attr_reader :type_tag

  def initialize(value_, type=nil)
    @ruby_value = value_

    _c_initialize

    if type == :_derive_type_tag_from_c_slaw
      # our c-level constructor code is obligated to set the type tag
      # after we return from new()
    elsif  value_.is_a? Slaw
      @_c_slaw = value_._c_slaw
      @type_tag = value_.type_tag
      # Yo, this is important!  If you don't copy this, then the underlying
      # C slaw can be freed when the original Slaw is garbage collected.
      # Resulting in at least incorrect results, and sometimes segfaults.
      # See _new_slaw_from_c_slaw() in rubyPlasma.c.
      # This is tested by test_container in slaw-tests.rb.
      @_container = value_._container
    else
      if  type
        if  !(type.is_a?(Symbol)  ||
              (value_.is_a?(Array)  &&  type.is_a?(Array)  &&
               type.all? {|x| x.is_a?(Symbol)}))
          raise SlawTypeTagError,
            "type tag must be a :symbol or an [:array, :of, :symbols]"
        else
          @type_tag = type
        end
      else
        @type_tag = _ruby_auto_type_tag
      end
      _freeze
    end
    # and throw away our passed-in value to ensure consistency between
    # values pulled out of slawx generated from ruby and slawx
    # generated from c-slawx
    @ruby_value = nil
  end

  protected

  ENCODING_UTF8 = begin
                    Encoding.find("UTF-8")
                  rescue NameError
                    # No Encoding class in Ruby 1.8
                    nil
                  end

  public

  def Slaw._ensure_utf8(str)
    # C plasma assumes all strings are utf8.  On ruby 1.8, there's not
    # much we can do about this, because 1.8 doesn't keep track of the
    # encoding of a string.  But in 1.9, we can find out the encoding
    # and transcode to UTF-8 if necessary.
    if (str.respond_to?(:encoding) and
        !(ENCODING_UTF8 == str.encoding or str.ascii_only?))
      return str.encode(ENCODING_UTF8)
    else
      return str
    end
  end

  # Constructs a slaw from YAML and returns it.
  def Slaw.from_yaml(yaml)
    _c_slaw_from_string _ensure_utf8(yaml)
  end

  # Merges all passed in map objects into one, and returns the result.
  def Slaw.merge_maps(*maps)
    if ! maps.all? {|x| x.map?}
      raise ArgumentError, "can only merge slaw maps"
    end
    _c_merge_maps(*maps)
  end

  # Takes arguments in pairs, which should be a slaw list and a range
  # of elements for that list, and creates a new list from them
  # e. g. Slaw.recombobulate_lists([list1, 0..1], [list2, 0..9], [list1, 2..3])
  def Slaw.recombobulate_lists(*args)
    a = Array.new
    args.each do |list, range|
      if not list.list?
        raise ArgumentError, "first argument of each pair must be slaw list"
      end
      if not range.respond_to?(:each)
        raise ArgumentError, "second argument of each pair must be a range"
      end
      range.each {|n| a.push(list.nth_slaw(n))}
    end
    return Slaw.new(a)
  end

  # Concatenates all passed in list objects, and returns the result.
  def Slaw.concatenate_lists(*lists)
    if ! lists.all? {|x| x.list?}
      raise ArgumentError, "can only concatenate slaw lists"
    end
    pairs = lists.map {|x| [x, 0...x.count]}
    Slaw.recombobulate_lists(*pairs)
  end

  # Returns the PORO (Plain Old Ruby Object) version of this Slaw.
  def emit
    @ruby_value ||= _unpack(self)
  end

  # Returns the binary data contained within this slaw. If the object is a Protein, returns the #rude_data.
  def emit_binary
    case @type_tag
      when :unt8_array then return _c_slaw_value_as_string
      when :int8_array then return _c_slaw_value_as_string
      when :protein    then return _c_slaw_protein_rude_data
    end
    raise SlawTypeNoMethodError,
      "emit_binary can only be called on int8 or unt8 arrays, or a protein"
  end

  # Alias for #count
  def size
    count
  end

  # Returns the size of the Slaw.
  #
  # * Returns 1 if an atomic type
  # * Returns 2 if a cons
  # * Returns number of elements if a List
  # * Returns number of elements + number of keys if a Map.
  def count
    if nil?  or  atomic?
      return 1
    elsif list? || map?
      return  _c_slaw_list_count   if  @_c_slaw
    elsif array?
      return  _c_slaw_numeric_array_count  if  @_c_slaw
    elsif cons?
      return 2
    end
  end

  # Iterate over all elements. Accepts a block.
  def each
    if list? || map? || array? || vect?
      return emit.each { |e| yield e }
    end
    raise SlawTypeNoMethodError,
      "each can only be called on a list or map slaw"
  end

  # Returns the keys when this Slaw is a map.
  def keys
    if map?
      return emit.keys
    end
    raise SlawTypeNoMethodError,
      "keys can only be called on a map slaw"
  end

  # Return the values with this Slaw is a map.
  def values
    if map?
      return emit.values
    end
    raise SlawTypeNoMethodError,
      "values can only be called on a map slaw"
  end

  # Returns the first element of a cons pair.
  def car
    if cons?
      return _c_slaw_cons_car
    end
    raise SlawTypeNoMethodError,
      "car can only be called on a cons slaw"
  end

  # Returns the second element of a cons pair.
  def cdr
    if cons?
      return _c_slaw_cons_cdr
    end
    raise SlawTypeNoMethodError,
      "cdr can only be called on a cons slaw"
  end


  # Returns the nth element of a list or map as an "emitted" result.
  #
  # If you want a Slaw, you should call #nth_slaw (for lists)
  # of find_slaw (for maps).
  def [](n)
    if list? || map?
      return emit[n]
    elsif array? || vect?
      # fix: this should probably be in c
      return emit[n]
    end
    return nil
  end

  # Returns the nth element of a list or map. See also #[].
  def nth_slaw(n)
    if list? || map?
      if (n < 0)
        n = count + n
      end
      return _c_slaw_list_nth(n)
    end
    raise SlawTypeNoMethodError,
      "nth_slaw can only be called on a list or map slaw"
  end


  def each_slaw
    count.times {|n| yield nth_slaw(n)}
  end


  # Returns (as a Slaw) the value associated with a key in a
  # slaw map.  Returns nil (the Ruby nil, not a nil Slaw) if not found.
  def find_slaw(findme)
    if not map?
      raise SlawTypeNoMethodError, "find_slaw can only be called on a map slaw"
    end
    each_slaw do |s|
      key = s.car
      if (key == findme)
        return s.cdr
      end
    end
    nil
  end

  # Returns true if this is a nil Slaw.
  def nil?
    _c_slaw_is_nil
  end

  # Returns true if an atomic type.
  def atomic?
    (!array? && !vect? && !composite?)
  end

  # Returns true if an array type.
  def array?
    _c_slaw_is_array
  end

  # Returns true if a composite type (list, map, protein, or cons).
  def composite?
    list? || map? || protein? || cons?
  end

  # Returns true if a cons type.
  def cons?
    _c_slaw_is_cons
  end

  # Returns true if a list type.
  def list?
    _c_slaw_is_list
  end

  # Returns true if a map type.
  def map?
    _c_slaw_is_map
  end

  # Returns true if a protein.
  def protein?
    @type_tag == :protein
  end

  # Returns true if a boolean.
  def boolean?
    _c_slaw_is_boolean
  end

  # Returns true if a vector.
  def vect?
    _c_slaw_is_vect
  end

  # Returns true if a string.
  def string?
    @type_tag == :string
  end

  # Returns true if a numeric type.
  def numeric?
    _c_slaw_is_numeric
  end

  # Test whether the value of two Slaw are equal.
  def ==(other)
    if (other.is_a? Slaw)
      return _c_slaw_equals(other)
    end
    e = emit
    if (e.equal?(self))
      # For proteins and conses, emit just returns the slaw unchanged.
      # If we call == on that, we will just go into a horrible infinite
      # loop.  So, don't so that.
      return false
    else
      return e == other
    end
  end

  # Test whether the two Slaw have the same value and are of the same type.
  def eql?(other)
    if (other.is_a? Slaw)
      return _c_slaw_equals(other)
    end
    return false
  end

  # Returns the underlying hash of this Slaw when it is a Map.
  def hash
    @_hash ||= _c_slaw_hash
  end

  # Comparison operator
  def <=>(other)
    return _c_slaw_spaceship(other)
  end

  # Returns a new list created by appending the list other to this list Slaw.
  def +(other)
    if not list?
      raise SlawTypeNoMethodError, "+ can only be called on a list slaw"
    end
    if not other.list?
      raise ArgumentError, "+ can only be called on a list slaw"
    end
    a = Array.new
    self.count.times  {|n| a.push(self.nth_slaw(n))}
    other.count.times {|n| a.push(other.nth_slaw(n))}
    return Slaw.new(a)
  end

  # Returns a new map created by merging the map other with this map Slaw.
  def merge(other)
    if not map?
      raise SlawTypeNoMethodError, "merge can only be called on a map slaw"
    end
    if not other.map?
      raise ArgumentError, "merge can only be called on a map slaw"
    end
    Slaw.merge_maps(self, other)
  end

  # Returns a new map based on this Slaw map, with {key => value} added to the map.
  def map_put(key, value)
    if not map?
      raise SlawTypeNoMethodError, "map_put can only be called on a map slaw"
    end
    Slaw.merge_maps(self, Slaw.new({key => value}))
  end

  # Returns a new map based on this Slaw map, less the k-v pair keyed by key.
  def map_remove(key)
    if not map?
      raise SlawTypeNoMethodError, "map_remove can only be called on a map slaw"
    end
    a = Array.new
    count.times do |n|
      s = nth_slaw(n)
      car = s.car
      if (car != key)
        cdr = s.cdr
        a.push(car)
        a.push(cdr)
      end
    end
    Slaw.new(a, :map)
  end

  # Returns a list based on this Slaw list, with elem added to the end of the list.
  def append(elem)
    if not list?
      raise SlawTypeNoMethodError, "append can only be called on a list slaw"
    end
    return self + Slaw.new([elem])
  end

  # Returns a list based on this Slaw list, with elem added to the front of the list.
  def prepend(elem)
    if not list?
      raise SlawTypeNoMethodError, "prepend can only be called on a list slaw"
    end
    return Slaw.new([elem]) + self
  end

  # Returns a list based on this Slaw list, with list inserted in this list at position index.
  def insert_list(index, list)
    if not list?
      raise SlawTypeNoMethodError, "insert_list can only be called on a list slaw"
    end
    if not list.list?
      raise ArgumentError, "insert_list can only be called on a list slaw"
    end
    Slaw.recombobulate_lists([self, 0...index],
                             [list, 0...list.count],
                             [self, index...self.count])
  end

  # Returns a new list based on this Slaw list, with the variable length parameter list inserted at position index.
  def insert(index, *elems)
    if not list?
      raise SlawTypeNoMethodError, "insert can only be called on a list slaw"
    end
    insert_list(index, Slaw.new([*elems]))
  end

  # Returns a new list based on this list, with elements removed at index_or_range
  def list_remove(index_or_range)
    if not list?
      raise SlawTypeNoMethodError, "list_remove can only be called on a list slaw"
    end
    if index_or_range.respond_to?(:integer?) and index_or_range.integer?
      index_or_range = index_or_range..index_or_range
    end
    if !index_or_range.is_a?(Range)
      raise ArgumentError, "argument must be an integer or a range"
    end
    b = index_or_range.begin
    e = index_or_range.end
    if not index_or_range.exclude_end?
      # now e is always excluded
      e = e + 1
    end
    Slaw.recombobulate_lists([self, 0...b],
                             [self, e...self.count])
  end

  # Returns the underlying Array when this Slaw is a List or Array.
  def to_ary
    if list? || array?
      return emit
    end
    raise SlawTypeNoMethodError,
      "to_ary can only be called on a list or array slaw"
  end

  # Returns the underlying Hash when this Slaw is a Map or Hash.
  def to_hash
    if map?
      return emit
    end
    raise SlawTypeNoMethodError,
      "to_hash can only be called on a hash slaw"
  end

  # Returns the underlying string when this Slaw is a string.
  def to_s
    if string?
      return emit
    end
    _c_slaw_to_string.emit
  end

  # Returns the underlying string when this Slaw is a string.
  # Raises #SlawTypeNoMethodError if not a string.
  def to_str
    if string?
      return emit
    end
    raise SlawTypeNoMethodError,
      "to_str can only be called on a string slaw"
  end

  # Returns the integer value of this Slaw. Safe to call on strings.
  def to_i
    if string? || (atomic? && numeric?)
      return emit.to_i
    end
    raise SlawTypeNoMethodError,
      "to_i can only be called on a string or atomic, numeric slaw"
  end

  # Returns the integer value of this Slaw. May not be called on strings.
  def to_int
    if (atomic? && numeric?)
      return emit.to_int
    end
    raise SlawTypeNoMethodError,
      "to_int can only be called on an atomic, numeric slaw"
  end

  # Returns the floating point value of this Slaw. Safe to call on strings.
  def to_f
    if string? || (atomic? && numeric?)
      return emit.to_f
    end
    raise SlawTypeNoMethodError,
      "to_f can only be called on a string or atomic, numeric slaw"
  end


  def respond_to?(method, include_all=false)
    case method
      when :emit_binary then return (@type_tag == :unt8_array ||
                                     @type_tag == :int8_array ||
                                     @type_tag == :protein)
      when :each then      return (list? || map? || array? || vect?)
      when :keys then      return (map?)
      when :values then    return (map?)
      when :car then       return (cons?)
      when :cdr then       return (cons?)
      when :[] then        return (list? || map? || array? || vect?)
      when :nth_slaw then  return (list? || map?)
      when :each_slaw then return (list? || map?)
      when :find_slaw then return (map?)
      when :+ then         return (list?)
      when :merge then     return (map?)
      when :map_put then   return (map?)
      when :map_remove then return (map?)
      when :append then    return (list?)
      when :prepend then   return (list?)
      when :insert_list then return (list?)
      when :insert then    return (list?)
      when :list_remove then return (list?)
      when :to_a then      return (list? || map? || array? || vect?)
      when :to_ary  then   return (list? || array?)
      when :to_hash then   return (map?)
      when :to_str then    return (string?)
      when :to_i then      return (string? || (atomic? && numeric?))
      when :to_int then    return (atomic? && numeric?)
      when :to_f then      return (string? || (atomic? && numeric?))
    end
    super
  end


  protected

  # given a slaw, recurse, pulling the slawx apart into constituent
  # ruby values and returning the whole assemblage.
  def _unpack(s)
    if (s.atomic? || s.vect? || s.array?)
      return s._c_slaw_value
    elsif s.protein? || s.cons?
      return s
    elsif (s.list?)
      return s._c_slaw_value.map { |entry| _unpack(entry) }
    elsif (s.map?)
      unpacked = Hash.new
      s._c_slaw_value.each_pair do |key, value|
        unpacked[_unpack(key)] = _unpack(value)
      end
      return unpacked
    end

  end



  def _freeze
    if @_c_slaw
      # we're immutable, you know. we can only freeze once
      return
    elsif type_tag == :protein
      _freeze_protein
      return
    else
      case @ruby_value
      when Array
        if  type_tag.to_s =~ /^(.+)_array/
          _freeze_numeric_array
          return
        elsif  type_tag.to_s =~ /^v([234])(.+)/
          _freeze_v234
          return
        elsif  type_tag == :map
          sorted_keys = Array.new
          map_pairs = Hash.new
          (0..@ruby_value.size-1).each do |index|
            if (index%2) == 0
              sorted_keys.push @ruby_value[index]
              map_pairs[@ruby_value[index]] = @ruby_value[index+1]
            end
          end
          @ruby_value = map_pairs
          _freeze_map( sorted_keys )
        elsif  type_tag == :cons
          _freeze_cons
        else
          _freeze_list
          return
        end

      when Hash
        _freeze_map
        return
      else
        ruval = @ruby_value
        if (type_tag == :string)
          if (not ruval.is_a?(String))
            ruval = ruval.to_s
          end
          ruval = Slaw._ensure_utf8(ruval)
        end
        @_c_slaw = _c_freeze_atomic( ruval, type_tag )
        return
      end
    end
  end

  def _freeze_numeric_array
    @_c_slaw = _c_freeze_array( @ruby_value, type_tag );
  end

  def _freeze_v234
    @_c_slaw = _c_freeze_vect( @ruby_value, type_tag );
  end

  def _freeze_list
    tags = IteratorFinalValueRepeater.new(@type_tag)
    @_c_slaw = _c_freeze_list( @ruby_value.map do |rv|
                                 if Slaw === rv
                                   # okay, just embed the slaw
                                   rv
                                 else
                                   Slaw.new(rv, tags.next)
                                 end
                               end )
    @type_tag = :list
  end

  def _freeze_protein
    # if (! ((@ruby_value[0].is_a? Array) ||
    #        ((@ruby_value[0].is_a? Slaw) && (@ruby_value[0].list?))))
    #   raise ProteinDescripsAreNotListError
    # end
    ing_tag = :map
    if (! ((@ruby_value[1].is_a? Hash) || (@ruby_value[1].is_a? Array))  and
        ! (((@ruby_value[1].is_a? Slaw) && (@ruby_value[1].map?))))
      # raise ProteinIngestsAreNotMapError
      ing_tag = nil
    end
    if (not @ruby_value[2].is_a?(String))
      raise ArgumentError, "rude data should be a string"
    end
    descrips = Slaw.new @ruby_value[0]
    ingests  = Slaw.new @ruby_value[1], ing_tag
    @_c_slaw = _c_freeze_protein(descrips, ingests, @ruby_value[2])
  end

  def _freeze_map(sorted_keys=nil)
    sorted_keys ||= @ruby_value.keys
    sk = Array.new
    sv = Hash.new
    sorted_keys.each do |k|
      key = (Slaw === k) ? k : Slaw.new(k)
      v = @ruby_value[k]
      val = (Slaw === v) ? v : Slaw.new(v)
      sk.push key
      sv[key] = val
    end
    @_c_slaw = _c_freeze_map(sv, sk)
    @type_tag = :map
  end

  def _freeze_cons
    raise SlawConsNeedsTwoArgumentsError  if  @ruby_value.size != 2
    car_cdr = @ruby_value.map { |r| (r.is_a? Slaw) ? r : Slaw.new(r) }
    @_c_slaw = _c_freeze_cons( *car_cdr )
    @type_tag = :cons
  end

  def _ruby_auto_type_tag
    case @ruby_value
    when Float then                 return :float64
    when Integer then               return :int64
    when String, Symbol then        return :string
    when TrueClass, FalseClass then return :boolean
    when NilClass then              return :nil
    when Hash then                  return :map
    when Array then                 return :list
    end
    # See "Unify Fixnum and Bignum into Integer",
    # https://bugs.ruby-lang.org/issues/12005
    # Ruby has (always?) had Integer
    # Ruby < 2.4 exposed an implementation detail by using
    # subclasses of Integer called Bignum and Fixnum
    # Ruby >= 2.4 hides that, always uses Integer,
    # and deprecates Bignum and Fixnum.
    # To avoid warnings, don't use those subclases if deprecated.
    if 1.class != Integer
      case @ruby_value
      when Fixnum, Bignum then      return :int64
      end
    end
    raise ArgumentError, "don't know how to treat #{@ruby_value.class} as slaw"
  end

  public

  # Returns a JSON-serialized version of this Slaw
  def to_json(*a)
    begin
      JSON
    rescue NameError
      # Instead of doing "require 'json'" at the top of the file, like
      # would normally be done, we only require it if this particular
      # method is called, since only this one method needs the JSON gem,
      # and most users will never call this method.  The only reason this
      # is an issue is because in certain rare but unknown circumstances,
      # it appears to be possible for "require 'json'" to take up to
      # 30 seconds.  We don't want all Plasma users to have to pay this
      # price for functionality only needed by a minority of users.
      # (See bug 3183 comment 4.)
      begin
        require 'rubygems'
        require 'json'
      rescue LoadError
        raise UserError, "You must install the JSON gem if you want to use " +
          "JSON.  If you have ruby 1.8.6 or later, do \"gem install json\". " +
          "If you have ruby 1.8.5, do \"gem install json_pure\".  If you are " +
          "on Mac OS X 10.5, your gem command is broken; see http://gwcoffey." +
          "posterous.com/cant-update-really-old-rubygems-101-on-mac-os"
      end
    end
    _j.to_json(*a)
  end

  protected

  def _j
    typ = @type_tag.to_s
    case typ
    when ""
      j = nil
    when "cons"
      j = _j_cons
    when "list"
      j = _j_list
    when "map"
      j = _j_map
    when "protein"
      j = _j_protein
    when /^v\d([ui]n|floa)t\d+$/
      j = _j_vector
    else
      j = emit
    end
    j
  end

  def _j_cons
    {
      JSON.create_id => "Plasma::SlawCons",
      'car'          => car._j,
      'cdr'          => cdr._j,
    }
  end

  def _j_list
    a = Array.new
    count.times do |n|
      a.push(nth_slaw(n)._j)
    end
    a
  end

  def _j_map
    h = Hash.new
    count.times do |n|
      x = nth_slaw(n)
      h[x.car._j] = x.cdr._j
    end
    h
  end

  def _j_protein
    h = {
      JSON.create_id => "Plasma::Protein",
      'descrips'     => descrips._j,
      'ingests'      => ingests._j,
    }
    rude = rude_data
    if not rude.empty?
      h["rude_data"] = Base64.encode64(rude)
    end
    h
  end

  def _j_vector
    {
      JSON.create_id => "Plasma::SlawVector",
      'v'            => emit,
    }
  end

  # protect all the underscored C methods, too
  protected :_c_freeze_array, :_c_freeze_atomic, :_c_freeze_cons
  protected :_c_freeze_list, :_c_freeze_map, :_c_freeze_protein
  protected :_c_freeze_vect, :_c_initialize, :_c_slaw_cons_car
  protected :_c_slaw_cons_cdr, :_c_slaw_equals, :_c_slaw_hash
  protected :_c_slaw_is_array, :_c_slaw_is_boolean, :_c_slaw_is_cons
  protected :_c_slaw_is_list, :_c_slaw_is_map, :_c_slaw_is_nil
  protected :_c_slaw_is_numeric, :_c_slaw_is_vect, :_c_slaw_list_count
  protected :_c_slaw_list_nth, :_c_slaw_numeric_array_count
  protected :_c_slaw_protein_descrips, :_c_slaw_protein_ingests
  protected :_c_slaw_protein_rude_data, :_c_slaw_spaceship, :_c_slaw_to_string
  protected :_c_slaw_value, :_c_slaw_value_as_string, :_c_spew_stderr
  protected :_c_type_tag

  class IteratorFinalValueRepeater
    attr :arr
    def initialize(array_or_value)
      if Array === array_or_value then @arr = Array.new(array_or_value).push(nil)
      else                             @arr = [array_or_value]
      end
    end
    def next
      nxt = if @arr.length > 1 then  @arr.shift
            else                     @arr[0]
            end
      if nxt == :list
        nxt = nil
      end
      nxt
    end
  end # IteratorFinalValueRepeater
end # Slaw

class SlawCons < Slaw
  def self.json_create(o)
    Slaw.new([o['car'],o['cdr']], :cons)
  end
end

class SlawVector < Slaw
  def self.json_create(o)
    v = o['v']
    if v.all? { |x| x.is_a?(Integer) }
      tag = "int64"
    else
      tag = "float64"
    end
    tag = "v" + v.length.to_s + tag
    Slaw.new(v, tag.to_sym)
  end
end

end # Plasma

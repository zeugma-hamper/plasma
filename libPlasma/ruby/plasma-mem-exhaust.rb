
require 'Pool'
include Plasma

h = Pool.participate( "ruby_plasma_unit_tests" );

$sl = slaw(nil)

while (true)
  h.rewind
  while (prot = h.next)
    $sl = prot.ingests.value
  end
  if ! Slaw === $sl
    p "ack: impossible error"
  end
  # p $sl[Slaw.new("slaw")].value
end


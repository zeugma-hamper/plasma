#!/usr/bin/ruby
$VERBOSE = true
require_relative 'test-helpers.rb'

ENV['PATH'] = '.' + File::PATH_SEPARATOR + ENV['PATH']

# Tests fail if address sanitizer is set to verbose
lsan_opts = ENV['LSAN_OPTIONS']
if lsan_opts
  lsan_opts = lsan_opts.gsub(/verbosity=1/, "verbosity=0")
  lsan_opts = lsan_opts.gsub(/log_threads=1/, "log_threads=0")
  ENV['LSAN_OPTIONS'] = lsan_opts
end

# Test usage message

ecode, actual = obbacktick("aptest3 -h")

expected = Regexp.new(<<'DONE', Regexp::MULTILINE)
\Ag-speak SOE \(c\) Oblong Industries - g-speak .+
Usage: aptest3.* \[options\]\s*
The wjc gstreamer plugin will select defaults for options left unspecified\.
  -h, --help         print this help, then exit
  -p, --pool         output pool for notifications \(default dvi-notifications\)
  -0, --slot-0-pools names, one per wjc card, to be used for slot 0 output
  -1, --slot-1-pools names, one per wjc card, to be used for slot 1 output
  -c, --compression  compression fraction \(default 0\.2\)
  -N, --rate-num     numerator of framerate fraction \(rate_num/rate_denom\)
                     \(default 30000\)
  -D, --rate-denom   denominator of framerate fraction \(rate_num/rate_denom\)
                     \(default 1001\)
  -t, --tiles        number of tiles to process in parallel \(default 2\)
  -k, --kernel       wavelet kernel to use
                     \(0 = IRR_9x7, 1 = REV_5x3, 2 = IRR_5x3, 3 = KERNEL_DEFAULT\)
                     \(default 3\)\Z
DONE

if (ecode != 0)
  STDERR.print("expected 0 but got #{ecode}\n")
  exit 1
end

if (not expected =~ actual)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

# Test defaults

ecode, actual = obbacktick("aptest3")

expected = <<'DONE'
output_pool: dvi-notifications
compression: 0.2
frame_rate_num: 30000
frame_rate_denom: 1001
parallel_tiles: 2
kernel: 3
slot0poolsTrove: []
slot1poolsTrove: []
non_option_args: []
DONE

if (ecode != 0)
  STDERR.print("expected 0 but got #{ecode}\n")
  exit 1
end

if (actual != expected)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

# Test too many non-option arguments

ecode, actual = obbacktick("aptest3 foo")

expected = Regexp.new(<<'DONE', Regexp::MULTILINE)
\AExpected 0 non-option arguments, but got 1
Usage: aptest3.* \[options\]\s*
The wjc gstreamer plugin will select defaults for options left unspecified\.
  -h, --help         print this help, then exit
  -p, --pool         output pool for notifications \(default dvi-notifications\)
  -0, --slot-0-pools names, one per wjc card, to be used for slot 0 output
  -1, --slot-1-pools names, one per wjc card, to be used for slot 1 output
  -c, --compression  compression fraction \(default 0\.2\)
  -N, --rate-num     numerator of framerate fraction \(rate_num/rate_denom\)
                     \(default 30000\)
  -D, --rate-denom   denominator of framerate fraction \(rate_num/rate_denom\)
                     \(default 1001\)
  -t, --tiles        number of tiles to process in parallel \(default 2\)
  -k, --kernel       wavelet kernel to use
                     \(0 = IRR_9x7, 1 = REV_5x3, 2 = IRR_5x3, 3 = KERNEL_DEFAULT\)
                     \(default 3\)\Z
DONE

if (ecode != 1)
  STDERR.print("expected 1 but got #{ecode}\n")
  exit 1
end

if (not expected =~ actual)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

# test missing argument

ecode, actual = obbacktick("aptest3 --tiles")

expected = Regexp.new(<<'DONE', Regexp::MULTILINE)
\Amissing an argument at end of command line
Usage: aptest3.* \[options\]\s*
The wjc gstreamer plugin will select defaults for options left unspecified\.
  -h, --help         print this help, then exit
  -p, --pool         output pool for notifications \(default dvi-notifications\)
  -0, --slot-0-pools names, one per wjc card, to be used for slot 0 output
  -1, --slot-1-pools names, one per wjc card, to be used for slot 1 output
  -c, --compression  compression fraction \(default 0\.2\)
  -N, --rate-num     numerator of framerate fraction \(rate_num/rate_denom\)
                     \(default 30000\)
  -D, --rate-denom   denominator of framerate fraction \(rate_num/rate_denom\)
                     \(default 1001\)
  -t, --tiles        number of tiles to process in parallel \(default 2\)
  -k, --kernel       wavelet kernel to use
                     \(0 = IRR_9x7, 1 = REV_5x3, 2 = IRR_5x3, 3 = KERNEL_DEFAULT\)
                     \(default 3\)\Z
DONE

if (ecode != 1)
  STDERR.print("expected 1 but got #{ecode}\n")
  exit 1
end

if (not expected =~ actual)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

# test range of arguments, too few

ENV['OB_MIN_NOA'] = "3"
ENV['OB_MAX_NOA'] = "5"
ecode, actual = obbacktick("aptest3 foo bar")

expected = Regexp.new(<<'DONE', Regexp::MULTILINE)
\AExpected between 3 and 5 non-option arguments, but got 2
Usage: aptest3.* \[options\]\s*
The wjc gstreamer plugin will select defaults for options left unspecified\.
  -h, --help         print this help, then exit
  -p, --pool         output pool for notifications \(default dvi-notifications\)
  -0, --slot-0-pools names, one per wjc card, to be used for slot 0 output
  -1, --slot-1-pools names, one per wjc card, to be used for slot 1 output
  -c, --compression  compression fraction \(default 0\.2\)
  -N, --rate-num     numerator of framerate fraction \(rate_num/rate_denom\)
                     \(default 30000\)
  -D, --rate-denom   denominator of framerate fraction \(rate_num/rate_denom\)
                     \(default 1001\)
  -t, --tiles        number of tiles to process in parallel \(default 2\)
  -k, --kernel       wavelet kernel to use
                     \(0 = IRR_9x7, 1 = REV_5x3, 2 = IRR_5x3, 3 = KERNEL_DEFAULT\)
                     \(default 3\)\Z
DONE

if (ecode != 1)
  STDERR.print("expected 1 but got #{ecode}\n")
  exit 1
end

if (not expected =~ actual)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

# test range of arguments, too many

ecode, actual = obbacktick("aptest3 a b c d e f")

expected = Regexp.new(<<'DONE', Regexp::MULTILINE)
\AExpected between 3 and 5 non-option arguments, but got 6
Usage: aptest3.* \[options\]\s*
The wjc gstreamer plugin will select defaults for options left unspecified\.
  -h, --help         print this help, then exit
  -p, --pool         output pool for notifications \(default dvi-notifications\)
  -0, --slot-0-pools names, one per wjc card, to be used for slot 0 output
  -1, --slot-1-pools names, one per wjc card, to be used for slot 1 output
  -c, --compression  compression fraction \(default 0\.2\)
  -N, --rate-num     numerator of framerate fraction \(rate_num/rate_denom\)
                     \(default 30000\)
  -D, --rate-denom   denominator of framerate fraction \(rate_num/rate_denom\)
                     \(default 1001\)
  -t, --tiles        number of tiles to process in parallel \(default 2\)
  -k, --kernel       wavelet kernel to use
                     \(0 = IRR_9x7, 1 = REV_5x3, 2 = IRR_5x3, 3 = KERNEL_DEFAULT\)
                     \(default 3\)\Z
DONE

if (ecode != 1)
  STDERR.print("expected 1 but got #{ecode}\n")
  exit 1
end

if (not expected =~ actual)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

# test range of arguments, just right

ecode, actual = obbacktick("aptest3 one two 3 four")

expected = <<'DONE'
output_pool: dvi-notifications
compression: 0.2
frame_rate_num: 30000
frame_rate_denom: 1001
parallel_tiles: 2
kernel: 3
slot0poolsTrove: []
slot1poolsTrove: []
non_option_args: [one,two,3,four]
DONE

if (ecode != 0)
  STDERR.print("expected 0 but got #{ecode}\n")
  exit 1
end

if (actual != expected)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

# now try just some normal options

ENV.delete("OB_MIN_NOA")
ENV.delete("OB_MAX_NOA")
ecode, actual = obbacktick("aptest3 -k 2 -t 1 -D 1 -N 15 -c 0.05 -1 x,y,z -0 blech")

expected = <<'DONE'
output_pool: dvi-notifications
compression: 0.05
frame_rate_num: 15
frame_rate_denom: 1
parallel_tiles: 1
kernel: 2
slot0poolsTrove: [blech]
slot1poolsTrove: [x,y,z]
non_option_args: []
DONE

if (ecode != 0)
  STDERR.print("expected 0 but got #{ecode}\n")
  exit 1
end

if (actual != expected)
  STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
  exit 1
end

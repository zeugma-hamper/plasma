#!/usr/bin/ruby

require 'yaml'
require 'rbconfig'

require_relative 'test-helpers.rb'

ENV['PATH'] = '.' + File::PATH_SEPARATOR + ENV['PATH']

def is_windows?
  RbConfig::CONFIG['host_os'] == "mingw32" || RbConfig::CONFIG['host_os'] == "cygwin"
end

if is_windows?
  ENV['PATH'] = Dir.getwd.sub(%r{c\+\+/tests}, "c/win32") + ";" + ENV['PATH']
else
  ENV['PATH'] = "../../c:" + ENV['PATH'] # Don't be fooled, c: is not win32 related
end

loam_cpp_ecode, cpp_output = obbacktick("ObInfoTest")
loam_c_ecode, obversion_output = obbacktick("ob-version -y")

actual = YAML.load(cpp_output)
expected = YAML.load(obversion_output)


# For each special key you must provide a `test` and a `message`.  The `message` must be
# a string, and the test must take the actual and expected values for that key, but doesn't
# need to use them
# - cpu-mhz: this can change between ObInfoTest and ob-version, since the tests may run on
#            dynamic VMs and laptops, which throttle CPU at will
special_keys = {
  'cpu-mhz' => {'test' => lambda { |actual_val, expected_val| 100 <= actual_val and actual_val <= 10_000 },
                'message' => "CPU MHz range must be between 100 and 10,000"}

}

if expected.keys.sort != actual.keys.sort
  STDERR.print("In actual but missing from expected:\n", actual.keys.sort - expected.keys.sort, "\n")
  STDERR.print("In expected but not in actual:\n", expected.keys.sort - actual.keys.sort, "\n")
  exit 1
end

expected.keys.each do |key|
  if special_keys.key? key
    if not special_keys[key]['test'].call(actual[key], expected[key])
      STDERR.print("got:\n", actual[key], "\nexpected:\n", expected,
                   "\nwhich failed test because:\n", special_keys[key]['message'], "\n")
      exit 1
    end
  else
    if actual[key] != expected[key]
      STDERR.print("got:\n", actual, "\nbut expected:\n", expected, "\n")
      exit 1
    end
  end
end

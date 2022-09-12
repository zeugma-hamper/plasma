require 'net/http'

def generate_uuid
  page = Net::HTTP.get(URI.parse('http://www.cloanto.com/uuid/?quantity=1&action=Generate'))
  if page =~ /([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})<br>/
    uuid = $1
    return uuid.upcase
  end
  raise "couldn't get a UUID"
end

def create_project(name,template,olduuid,oldns,oldfn)
  uuid = generate_uuid
  base = name.sub(/\.[Cc]/, "")
  outfile = File.dirname(template) + "/#{base}.vcxproj"
  ns = base.gsub(/[^A-Za-z0-9]/, "")
  File.open(outfile, "w") do |out|
    IO.foreach(template) do |line|
      line.sub!(/#{olduuid}/, uuid)
      line.sub!(/>#{oldns}</, ">#{ns}<")
      line.sub!(/#{oldfn}/, name)
      out.print(line)
    end
  end
  print "Created #{outfile}\n"
end

def die_usage
  print "Usage: #{$PROGRAM_NAME} test_file_base_name.c\n"
  exit 1
end

require "mkmf"

# version => micro
map = {
  '1.8.' => 'RUBY_1_8',
  '1.9.' => 'RUBY_1_9',
}
version, micro = map.find do |v, m|
  RUBY_VERSION.start_with? v
end

if micro.nil?
  abort "Not supported ruby-version: #{RUBY_VERSION}"
else
  $defs << "-D#{micro}"
end

create_makefile "revdev/revdev"

require "mkmf"

case RUBY_VERSION
when '1.8' .. '1.9'
  $defs << '-DRUBY_1_8'
when '1.9' .. '2.0'
  $defs << '-DRUBY_1_9'
else
  abort "Not supported ruby-version: #{RUBY_VERSION}"
end

create_makefile "revdev/revdev"

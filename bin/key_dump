#!/usr/bin/env ruby
# -*- coding:utf-8; mode:ruby; -*-

require "revdev"
require "optparse"

USAGE = <<__EOF
usage:
    $ #{$0} event_device
  display the infomation about event_device.

example:
    $ #{$0} /dev/input/event2

options:
  -t EV_XXX, --type EV_XXX:
      restrict event type (EV_XXX) to display

  -g, --grab:
      grab event.
__EOF

def main
  include Revdev

  spec_type = nil
  is_grab = false
  OptionParser.new do |opt|
    opt.on '-t TYPE', '--type TYPE', String do |type|
      spec_type = type
    end
    opt.on '-g', '--grab' do
      is_grab = true
    end
    opt.parse! ARGV
  end

  if ARGV.length != 1
    puts USAGE
    exit false
  end

  evdev = EventDevice.new ARGV.first
  puts "## Device Name: #{evdev.device_name}"
  puts "spec_type: #{spec_type}" if $DEBUG

  trap :INT do
    puts "# recieve :INT"
    evdev.ungrab if is_grab
    exit true
  end

  evdev.grab if is_grab

  loop do
    ie = evdev.read_input_event
    next if spec_type and spec_type != ie.hr_type.to_s
    t = ie.hr_type ? "#{ie.hr_type.to_s}(#{ie.type})" : ie.type
    c = ie.hr_code ? "#{ie.hr_code.to_s}(#{ie.code})" : ie.code
    v = ie.hr_value ? "#{ie.hr_value.to_s}(#{ie.value})" : ie.value
    puts "type:#{t}	code:#{c}	value:#{v}"
  end

end

main

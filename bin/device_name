#!/usr/bin/env ruby
# -*- coding:utf-8; mode:ruby; -*-

require "revdev"
require "optparse"

USAGE=<<__EOF
usage:
    #{$0} event_device [ event_device [ ... ] ]

  display device name(s) for event_device(s)

example:
    #{$0} /dev/input/event*
__EOF

def main
  include Revdev

  if ARGV.length == 0
    puts USAGE
    exit false
  end

  STDOUT.sync = true
  ARGV.each do |devname|
    begin
      evdev = EventDevice.new devname
      print "#{devname}	: "
      puts evdev.device_name
    rescue => e
      p e
    end
  end

end

main

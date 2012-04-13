# -*- coding:utf-8; mode:ruby; -*-

require "revdev/revdev.so"

require "revdev/each_values_equal"

require "revdev/version"
require "revdev/event_device"
require "revdev/input_event"
require "revdev/input_id"

# just import linux/input.h
module Revdev
  class EventDevice; end
  class InputEvent; end
  class InputId; end

  PREFIX_CONVERTER = {
    'BTN'	=> 'KEY',
    'MT'	=> nil, # TODO how to use MT prefix values
    'ID'	=> nil, # TODO how to use ID prefix values
    'BUS'	=> nil  # TODO how to use BUS prefix values
  }

  REVERSE_MAPS = Hash.new
  constants.each do |const|
    m = const.to_s.match(/^(FF_STATUS|[A-Z]{2,4})_/)
    next if m.nil?

    prefix = m[1]
    if prefix == 'EV'
      REVERSE_MAPS[:EV] ||= Hash.new
      REVERSE_MAPS[:EV][const_get const] = const.to_sym
      next
    end
    prefix = PREFIX_CONVERTER[prefix] if PREFIX_CONVERTER.has_key? prefix
    next if prefix.nil?

    ev = "EV_#{prefix}".to_sym
    REVERSE_MAPS[ev] ||= Hash.new
    REVERSE_MAPS[ev][const_get const] = const.to_sym
  end

end

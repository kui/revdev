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

  def Revdev.extract_prefix const_name
    return nil if not const_name
    m = const_name.to_s.match(/^(FF_STATUS|[A-Z]{2,4})_/)
    m && m[1] && m[1].to_sym
  end

  def Revdev.extract_suffix const_name
    return nil if not const_name
    m = const_name.to_s.match(/^(?:FF_STATUS|[A-Z]{2,4})_(.+)$/)
    m && m[1] && m[1].to_sym
  end

  PREFIX_CONVERTER = {
    :BTN	=> :KEY,
    :MT		=> nil, # TODO how to use 'MT_' prefix values
    :ID		=> nil, # TODO how to use 'ID_' prefix values
    #:BUS	=> nil  # TODO how to use 'BUS_' prefix values
  }

  REVERSE_MAPS = Hash.new
  constants.each do |const|
    prefix = Revdev.extract_prefix const
    prefix = PREFIX_CONVERTER[prefix] if PREFIX_CONVERTER.has_key? prefix
    next if prefix.nil?

    REVERSE_MAPS[prefix] ||= Hash.new
    REVERSE_MAPS[prefix][const_get const] = const.to_sym
  end

end

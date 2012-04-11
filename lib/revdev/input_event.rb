# -*- coding:utf-8; mode:ruby; -*-

module Revdev

  # wraper of "struct input_event" on "input.h"
  class InputEvent

    # arg: String(byte string), Hash, Time
    # type, code, value: Integer (ignored values if arg is String or Hash)
    def initialize arg, type = nil, code = nil, value = nil
      if arg.kind_of? String
        raw_initialize arg
      elsif arg.kind_of? Hash
        @time = arg['time'] || arg[:time] || Time.now
        @type = arg['type'] || arg[:type]
        @code = arg['code'] || arg[:code]
        @value = arg['value'] || arg[:value]
      else
        @time = arg || Time.now
        @type = type
        @code = code
        @value = value
      end
    end

    # human readable @type of input event
    def hr_type
      REVERSE_MAPS[:EV][@type]
    end

    # human readable @code of input event
    def hr_code
      ht = hr_type
      return nil if ht.nil?
      map = REVERSE_MAPS[ht]
      map && map[@code]
    end

    # human readable @code of input event
    # not implement
    def hr_value
      nil
    end

    PREFIX_CONVERTER = {
      'BTN'	=> 'KEY',
      'MT'	=> nil,
      'ID'	=> nil,
      'BUS'	=> nil
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

      ev = ('EV_'+prefix).to_sym
      REVERSE_MAPS[ev] ||= Hash.new
      REVERSE_MAPS[ev][const_get const] = const.to_sym
    end

  end
end

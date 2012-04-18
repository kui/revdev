# -*- coding:utf-8; mode:ruby; -*-

module Revdev

  # wraper of "struct input_event" on "input.h"
  class InputEvent
    include EachValuesEqual
    attr_accessor :time, :type, :code, :value

    # arg: String(byte string), Hash, Time
    # type, code, value: Integer (ignored values if arg is String or Hash)
    def initialize arg=nil, type = nil, code = nil, value = nil
      if arg.kind_of? String
        raw_initialize arg
      elsif arg.kind_of? Hash
        [:time, :type, :code, :value].each do |iv|
          instance_variable_set("@#{iv}", arg[iv] || arg[iv.to_s])
        end
        @time ||= Time.now
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
      suffix = Revdev.extract_suffix ht
      return nil if suffix.nil?
      map = REVERSE_MAPS[suffix]
      map && map[@code]
    end

    # human readable @code of input event
    # not implement
    def hr_value
      nil
    end

  end
end

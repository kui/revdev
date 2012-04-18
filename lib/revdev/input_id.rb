# -*- coding:utf-8; mode:ruby; -*-

module Revdev

  # wraper of "struct input_id" on "input.h"
  class InputId
    include Revdev::EachValuesEqual
    attr_accessor :bustype, :vendor, :product, :version

    def initialize(arg=nil, vendor=nil, product=nil, version=nil)
      if arg.kind_of? String
        raw_initialize arg
      elsif arg.kind_of? Hash
        [:bustype, :vendor, :product, :version].each do |iv|
          instance_variable_set("@#{iv}", arg[iv] || arg[iv.to_s])
        end
      elsif not ( arg.nil? or vendor.nil? or product.nil? or version.nil? )
        @bustype = arg
        @vendor = vendor
        @product = product
        @version = version
      else
        raise ArgumentError, "expected a Hash"
      end
    end

    def hr_bustype
      REVERSE_MAPS[:BUS][@bustype]
    end

    def hr_version
      v = @version
      v_arr = []
      v_arr << (v & 0xff)
      2.times do
        v = (v >> 8)
        v_arr << (v & 0xff)
      end
      v_arr.reverse.join '.'
    end

  end

end

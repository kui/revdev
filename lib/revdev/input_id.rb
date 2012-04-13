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
        @bustype = arg[:bustype] || arg['bustype']
        @vendor = arg[:vendor] || arg['vendor']
        @product = arg[:product] || arg['product']
        @version = arg[:version] || arg['version']
      else
        @bustype = arg
        @vendor = vendor
        @product = product
        @version = version
      end
    end

  end

end

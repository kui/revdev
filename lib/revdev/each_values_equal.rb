
# -*- coding:utf-8; mode:ruby; -*-

module Revdev
  module EachValuesEqual
    def == obj
      obj.instance_variables.delete_if do |name|
        obj.instance_variable_get(name) == instance_variable_get(name)
      end.empty?
    end
  end
end

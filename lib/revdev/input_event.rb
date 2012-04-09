# -*- coding:utf-8; mode:ruby; -*-

module Revdev
  class InputEvent

    PREFIX_CONVERTER = {
      'BTN'	=> 'KEY',
      'MT'	=> nil,
      'ID'	=> nil,
      'BUS'	=> nil
    }

    REVERSE_MAPS = Hash.new
    constants.each do |const|
      m = const.to_s.match /^(FF_STATUS|[A-Z]{2,4})_/
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
    def hr_value
      REVERSE_MAPS[:EV_KEY] && REVERSE_MAPS[:EV_KEY][@value] if hr_code == :MSC_SCAN
    end
  end
end

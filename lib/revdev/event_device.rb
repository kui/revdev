# -*- coding:utf-8; mode:ruby; -*-

module Revdev
  class EventDevice

    def initialize arg
      if arg.kind_of? File
        @file = arg
      else
        @file = File.new arg, 'r+'
      end
    end

    def read_ioctl_with command
      r = ""
      @file.ioctl command, r
      return r
    end

    def driver_version
      r = read_ioctl_with EVIOCGVERSION
      r.unpack('i').first
    end

    def read_ioctl_as_string_with command
      r = read_ioctl_with command
      r.unpack('Z*').first
    end

    def device_name
      read_ioctl_as_string_with EVIOCGNAME
    end

    def physical_location
      read_ioctl_as_string_with EVIOCGPHYS
    end

    def uniq_id
      read_ioctl_as_string_with EVIOCGUNIQ
    end

    def device_prop
      read_ioctl_as_string_with EVIOCGPROP
    end

    def global_key_state
      read_ioctl_as_string_with EVIOCGKEY
    end

    def all_leds_status
      read_ioctl_as_string_with EVIOCGLED
    end

    def all_sounds_status
      read_ioctl_as_string_with EVIOCGSND
    end

    def all_switch_status
      read_ioctl_as_string_with EVIOCGSW
    end

    def read_input_event
      InputEvent.new @file.read InputEvent::SIZEOF
    end

    def write_input_event ie
      @file.syswrite ie.to_byte_string
    end

  end
end

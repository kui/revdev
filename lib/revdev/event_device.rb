# -*- coding:utf-8; mode:ruby; -*-

module Revdev
  class EventDevice

    def initialize arg
      if arg === File
        @file = arg
      else
        @file = File.new arg
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

    def all_sounds_status
      read_ioctl_as_string_with EVIOCGSND
    end

    def all_swich_status
      read_ioctl_as_string_with EVIOCGSW
    end

    def read_input_event
      byte = @file.read InputEvent::SIZEOF
      InputEvent.new byte
    end

  end
end

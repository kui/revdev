# -*- coding:utf-8; mode:ruby; -*-

module Revdev
  class EventDevice
    attr_reader :file

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
      r.unpack('C3')
    end

    def device_id
      InputId.new read_ioctl_with EVIOCGID
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
      r = read_ioctl_with EVIOCGLED
      r = r.unpack("b#{LED_MAX}")[0]
      m = {}
      one_code = '1'[0]
      LED_MAX.times do |i|
        break if REVERSE_MAPS[:LED][i].nil?
        m[REVERSE_MAPS[:LED][i]] = (r[i] == one_code)
      end
      m
    end

    def all_sounds_status
      read_ioctl_as_string_with EVIOCGSND
    end

    def all_switch_status
      read_ioctl_as_string_with EVIOCGSW
    end

    # grab all input events of the event device
    def grab
      @file.ioctl EVIOCGRAB, 1
    end

    # release the grabbed event device
    def ungrab
      @file.ioctl EVIOCGRAB, 0
    end

    def read_input_event
      ie = InputEvent.new @file.sysread InputEvent::SIZEOF
      puts "read  #{ie.hr_type}\t#{ie.hr_code}\t#{ie.value}" if $DEBUG
      ie
    end

    def write_input_event ie
      puts "write #{ie.hr_type}\t#{ie.hr_code}\t#{ie.value}" if $DEBUG
      @file.syswrite ie.to_byte_string
    end

  end
end

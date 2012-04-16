# -*- coding:utf-8; mode:ruby; -*-

require 'test/unit'
require 'rubygems'
require 'revdev'

class EventDeviceTest < Test::Unit::TestCase
  include Revdev

  @@target = "test/tmp/file"

  def initialize *args
    super

    @@target = File.expand_path @@target
    dir = File.dirname @@target
    if not File.directory? dir
      Dir.mkdir dir
    end
    FileUtils.touch @@target

  end

  def test_consts
    puts "## InputEvent Constants"
    puts "# of consts: #{Revdev.constants.length}"
    puts "# of REVERSE_MAP entries: #{Revdev::REVERSE_MAPS.size}"
    puts "each # of REVERSE_MAP entries"
    REVERSE_MAPS.each do |k,v|
      puts "%15s => %4s" % [k.inspect,v.size]
    end
  end

  def test_init
    assert_nothing_raised do
      EventDevice.new File.new @@target
      EventDevice.new @@target
    end
  end

  def test_ioctl_string_getters
    return_data = "abc\000\234\90\444"
    expected_data = "abc"
    file = File.new @@target
    $data = return_data

    def file.ioctl command, data
      if command.kind_of? Numeric
        data << $data
      else
        raise "invalid command:#{command.class}"
      end
    end

    [:device_name, :physical_location, :uniq_id, :device_prop, :global_key_state,
    :all_leds_status, :all_sounds_status, :all_switch_status].each do |m|
      evdev = EventDevice.new file
      assert_equal expected_data, evdev.method(m).call
    end
  end

  def test_read_input_event
    file = File.new @@target
    ie = InputEvent.new nil, 1, 2, 3
    $data = ie.to_byte_string
    $self = self

    def file.sysread len
      $self.assert_equal $data.length, len
      $data
    end

    evdev = EventDevice.new file
    assert_nothing_raised do
      iee = evdev.read_input_event
      assert_equal ie, iee
    end
  end

  def test_write_key_input_event
    file = File.new @@target
    ie = InputEvent.new nil, 1, 2, 3
    $data = ie.to_byte_string
    $self = self

    def file.syswrite data
      $self.assert_equal $data, data
    end

    evdev = EventDevice.new file
    assert_nothing_raised do
      evdev.write_input_event ie
    end
  end

end

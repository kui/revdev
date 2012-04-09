# -*- coding:utf-8; mode:ruby; -*-

require 'test/unit'
require 'revdev'

class RevdevTest < Test::Unit::TestCase

  # DEVICE_FILE = "/dev/input/event0"
  DEVICE_FILE = "/dev/input/event1"
  # DEVICE_FILE = "/dev/input/event2"
  # DEVICE_FILE = "/dev/input/event3"
  # DEVICE_FILE = "/dev/input/event4"
  # DEVICE_FILE = "/dev/input/event5"
  DEVICE_NAME = "AT Translated Set 2 keyboard"

  def initialize test_method_name
    super
  end

  def setup
    @evdev = Revdev::EventDevice.new DEVICE_FILE
  end

  def teardown
  end

  def test_driver_version
    r = @evdev.driver_version
    puts "\tdriver_version:\t"+r.to_s
  end

  def test_device_name
    r = @evdev.device_name
    puts "\tdevice_name:\t"+r
    assert_equal String, r.class
    assert_not_equal 0, r.length
  end

  def test_physical_location
    begin
      r = @evdev.physical_location
      puts "\tphysical_location:\t"+r
      assert_equal String, r.class
    rescue Errno::ENOENT
      # no assertion
    end
  end

  def test_uniq_id
    begin
      r = @evdev.uniq_id
      puts "\tuniq_id:\t"+r
      assert_equal String, r.class
    rescue Errno::ENOENT
      # no assertion
    end
  end

  def test_device_prop
    begin
      r = @evdev.device_prop
      puts "\tdevice_prop:\t"+r
      assert_equal String, r.class
    rescue Errno::ENOENT
      # no assertion
    end
  end

  def test_global_key_state
    begin
      r = @evdev.global_key_state
      puts "\tglobal_key_state:\t"+r
      assert_equal String, r.class
    rescue Errno::ENOENT
      # no assertion
    end
  end

  def test_all_leds_status
    begin
      r = @evdev.all_leds_status
      puts "\tall_leds_status:\t"+r
      assert_equal String, r.class
    rescue Errno::ENOENT
      # no assertion
    end
  end

  def test_all_sounds_status
    begin
      r = @evdev.all_sounds_status
      puts "\tall_sounds_status:\t"+r
      assert_equal String, r.class
    rescue Errno::ENOENT
      # no assertion
    end
  end

  def test_all_switch_status
    begin
      r = @evdev.all_switch_status
      puts "\tall_switch_status:\t"+r
    rescue Errno::ENOENT
      # no assertion
    end
  end

  def test#_read_input_event
    Dir.glob("/dev/input/event{2,3,4}").each do |file|
      evdev = Revdev::EventDevice.new file
      puts "listening #{evdev.device_name} ..."
      ie = evdev.read_input_event
      assert_equal Time, ie.time.class
      assert_equal Fixnum, ie.type.class
      assert_equal Fixnum, ie.code.class
      assert_equal Fixnum, ie.value.class
      p ie
      puts "hr_type:	#{ie.hr_type.inspect}"
      puts "hr_code:	#{ie.hr_code.inspect}"
      puts "hr_val:	#{ie.hr_value.inspect}"
    end
  end


end

# -*- coding:utf-8; mode:ruby; -*-

require 'test/unit'
require 'revdev'

class InputEventTest < Test::Unit::TestCase
  include Revdev

  U16_MAX = 2**16 - 1
  S32_MAX = 2**31 - 1

  def setup
  end

  def teardown
  end

  def test_init
    puts
    ie = iee = ieee = nil
    assert_nothing_raised do
      t = Time.now
      ie =  InputEvent.new t, 1, 3, S32_MAX
      p ie
      iee = InputEvent.new :time => t,:type => 1, :code => 3, :value => S32_MAX
      p iee
      ieee = InputEvent.new ie.to_byte_string
      p ieee
    end
    assert_equal ie, iee
    assert_equal ie, ieee
  end

  def test_hr_blar
    htype = :EV_KEY
    type = self.class.const_get htype
    hcode = :KEY_2
    code = self.class.const_get hcode
    value = 0

    ie = InputEvent.new nil, type, code, value

    assert_nothing_raised do
      assert_equal htype, ie.hr_type
      assert_equal hcode, ie.hr_code
      assert_equal nil, ie.hr_value
    end
  end

  def test_to_byte_string
    t = Time.now
    ie =  InputEvent.new t, 1, 3, 0
    iee = nil
    assert_nothing_raised do
      iee = InputEvent.new ie.to_byte_string
    end
    assert_equal ie, iee
  end

end

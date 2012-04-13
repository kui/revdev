# -*- coding:utf-8; mode:ruby; -*-

require 'test/unit'
require 'revdev'

class InputEventTest < Test::Unit::TestCase
  include Revdev

  def setup
  end

  def teardown
  end

  def test_consts
    puts "## InputEvent Constants"
    puts "# of consts:	#{Revdev.constants.length}"
    puts "# of REVERSE_MAP entries:	#{Revdev::REVERSE_MAPS.size}"
    puts "each size of REVERSE_MAP"
    REVERSE_MAPS.each do |k,v|
      puts "#{k.inspect}	=> #{v.size}"
    end
  end

  def test_init
    assert_nothing_raised do
      t = Time.now
      ie =  InputEvent.new t, 1, 3, 0
      p ie
      iee = InputEvent.new :type => 1, :code => 3, :value => 0, :time => t
      p iee
      ieee = InputEvent.new ie.to_byte_string
      p ieee
      assert_equal ie, iee
      assert_equal ie, ieee
    end
  end

  def test_hr_blar
    assert_nothing_raised do
      htype = :EV_KEY
      type = self.class.const_get htype
      hcode = :KEY_2
      code = self.class.const_get hcode
      value = 0

      ie = InputEvent.new nil, type, code, value

      assert_equal htype, ie.hr_type
      assert_equal hcode, ie.hr_code
      assert_equal nil, ie.hr_value
    end
  end

  def test_to_byte_string
    assert_nothing_raised do
      t = Time.now
      ie =  InputEvent.new t, 1, 3, 0
      iee = InputEvent.new ie.to_byte_string
      assert_equal ie, iee
    end
  end

end

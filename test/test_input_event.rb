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
      p InputEvent.new "\355\247\205O\000\000\000\000\374M\005\000\000\000\000"+
        "\000\001\000.\000\001\000\000\000"
      p InputEvent.new :type => 1, :code => 3, :value => 0
      p InputEvent.new nil, 1, 3, 0
    end
  end

  def test_hr_blar
    ie = InputEvent.new nil, 1, 3, 0
    assert_equal :EV_KEY, ie.hr_type
    assert_equal :KEY_2, ie.hr_code
    assert_equal nil, ie.hr_value
  end

  def test_to_byte_string
    bytes = "\355\247\205O\000\000\000\000\374M\005\000\000\000\000"+
        "\000\001\000.\000\001\000\000\000"
    ie = InputEvent.new bytes
    assert_equal bytes, ie.to_byte_string
  end

end

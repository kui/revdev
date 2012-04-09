# -*- coding:utf-8; mode:ruby; -*-

require 'test/unit'
require 'revdev'

class InputEventTest < Test::Unit::TestCase

  def setup
  end

  def teardown
  end

  def test_consts
    puts "# of consts:	#{Revdev::InputEvent.constants.length}"
    puts "# of REVERSE_MAP entries:	#{Revdev::InputEvent::REVERSE_MAPS.size}"
    puts "each size of REVERSE_MAP"
    Revdev::InputEvent::REVERSE_MAPS.each do |k,v|
      puts "#{k.inspect}	=> #{v.size}"
    end
  end

  def test_hr_type
  end

end

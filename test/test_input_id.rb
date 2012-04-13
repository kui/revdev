# -*- coding:utf-8; mode:ruby; -*-

require 'test/unit'
require 'revdev'

class InputIdTest < Test::Unit::TestCase
  include Revdev

  U16_MAX = 2**16 - 1

  def setup
  end

  def teardown
  end

  def test_init
    puts

    i = ii = iii = nil
    assert_nothing_raised do
      i = InputId.new 1, 2, 3, U16_MAX
      p i
      ii = InputId.new :bustype => 1, :vendor => 2, :product => 3, :version => U16_MAX
      p ii
      iii = InputId.new i.to_byte_string
      p iii
    end

    assert_equal i, ii
    assert_equal i, iii
  end

end

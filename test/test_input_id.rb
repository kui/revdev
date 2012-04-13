# -*- coding:utf-8; mode:ruby; -*-

require 'test/unit'
require 'revdev'

class InputIdTest < Test::Unit::TestCase
  include Revdev

  def setup
  end

  def teardown
  end

  def test_init
    assert_nothing_raised do
      i = InputId.new 1, 2, 3, 4
      p i
      ii = InputId.new :bustype => 1, :vendor => 2, :product => 3, :version => 4
      p ii
      iii = InputId.new i.to_byte_string
      p iii

      assert_equal i, ii
      assert_equal i, iii
    end
  end

end

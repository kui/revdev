require "mkmf"

$v = RUBY_VERSION.split('.').map{|s| s.to_i}

def chk_ver b, e=nil
  b.zip($v).all? do |bv, vv|
    bv ||= 0; vv ||= 0
    if    bv  < vv then break true
    elsif bv == vv then next true
    else                break false
    end
  end or return false

  e.nil? or $v.zip(e).any? do |vv, ev|
    vv ||= 0; ev ||= 0
    if    vv  < ev then break true
    elsif vv == ev then next  false
    else                break false
    end
  end
end

micro =
  if    chk_ver([1, 8, 6], [1, 9]) then 'RUBY_1_8'
  elsif chk_ver([1, 9, 1]        ) then 'RUBY_1_9'
  else  abort "Not supported ruby-version: #{RUBY_VERSION}"
  end

$defs << "-D#{micro}"

create_makefile "revdev/revdev"

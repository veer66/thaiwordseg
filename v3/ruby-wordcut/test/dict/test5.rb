#!/usr/bin/env ruby
require 'wordcut'
begin
  dict=Wordcut::Dict.new ("bas2.etd")
  iter=dict.root
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  iter.transit("a")
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  iter.transit("b")
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  pos=iter.pos
  pos.each_id { |id| print "Pos ID = #{id}\n" }
  pos.each_str { |str| print "Pos = #{str}\n" }
  iter.transit("c")
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  print "OK!!!\n"
rescue 
  print "Error : #{$!}\n"
end


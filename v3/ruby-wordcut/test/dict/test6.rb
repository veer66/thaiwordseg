#!/usr/bin/env ruby
require 'wordcut'
begin
  dict=Wordcut::Dict.new ("dict.etd")
  str="µ“°≈¡º¡¬“«"
  map=dict.get_map(str)
begin
  for i in 0..str.length-1 do
    print "i=#{i}"
    map.each_assoc(i) do |x,pos|
      print "\tx=#{x}"
      pos.each_str {|s| print "<#{s}>"}
    end 
    print "\n"
  end
end 
rescue 
  print "Error : #{$!}\n"
end

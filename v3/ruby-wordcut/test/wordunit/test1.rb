#!/usr/bin/env ruby
require 'wordcut'
str="ตากลม"
map=Wordcut::Wordunit.find(str)
for i in 0..str.length-1 do
		print "#{i}\t#{map[i]}\n"
end

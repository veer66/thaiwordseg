#!/usr/bin/env ruby
require 'wordcut'
str="µ“°≈¡π—Ëß¥Ÿ¥”¥Ÿ¥’"
map=Wordcut::Wordunit.find(str)
for i in 0..str.length-1 do
	for j in i..str.length-1 do
		print "#{i}\t#{j}\t#{map.break(i,j)}\n"
	end
end

#!/usr/bin/env ruby
require 'wordcut'
str="�ҡ��ABCD"
Wordcut::Split::split(str) do |tok,lang|
	if lang == Wordcut::Split::Thai
		print "THAI : #{tok}\n"
	else
		print "ENG  : #{tok}\n"
	end
end

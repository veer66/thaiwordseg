#!/usr/bin/env ruby
require 'wordcut'
wc=Wordcut::Wordcut.new
#print "#{wc.cutline("�ҡ�� abcd")}\n"
#wc.cut("�ҡ��") 
#.each { |sur,pos,type| print "!!!!!!!!!!!\n" }

wc.cut(ARGV[0]).each do |sur,pos,type| 
	print "Surface=#{sur}\t< "
	if pos != nil then
	  	pos.each_str {|p| print "#{p} "}
	end
	print ">\t#{type}\n"
end

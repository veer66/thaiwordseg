#!/usr/bin/env ruby
require 'latexparser'
LatexParser::parse('lsh.tex') do |txt,type|
  print "Txt=#{txt}\tType=#{type}\n\n"
end

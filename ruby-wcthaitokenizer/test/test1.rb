#!/usr/bin/env ruby
require 'wcthaitokenizer'
x=WcThaiTokenizer::tok("�ҡ��foobar") do |tok,lang|
  if(lang==WcThaiTokenizer::Thai)
    print "#{tok}\t#{lang}\n"
  end
end

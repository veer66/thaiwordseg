#!/usr/bin/env ruby
require 'wordcut'
wc=TestWordcut::Wordcut.new(WcDict.new(DictPath))

while gets do
  print wc.cutline($_)
end

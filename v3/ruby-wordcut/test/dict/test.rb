#!/usr/bin/env ruby
require 'wordcut'
if ARGV.length <= 0 then
  $stderr.print "Invalid argument.\n"
  exit 1
end

begin
  dict=Wordcut::Dict.new (ARGV[0])
  print "OK\n"
rescue 
  print "Error : #{$!}\n"
end


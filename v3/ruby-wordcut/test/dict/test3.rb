#!/usr/bin/env ruby
require 'wordcut'
if ARGV.length <= 0 then
  $stderr.print "Invalid argument.\n"
  exit 1
end

begin
  dict=Wordcut::Dict.new (ARGV[0])
  iter=dict.root
  print "#{Wordcut::Dict::Iterator::ACTIVE}\n"
  print "#{Wordcut::Dict::Iterator::DEAD}\n"
  print "#{iter.status}\n"
  print "#{iter.terminator}\n"
  print "OK\n"
rescue 
  print "Error : #{$!}\n"
end


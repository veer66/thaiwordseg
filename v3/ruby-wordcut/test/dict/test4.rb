#!/usr/bin/env ruby
require 'wordcut'
if ARGV.length <= 0 then
  $stderr.print "Invalid argument.\n"
  exit 1
end

begin
  dict=Wordcut::Dict.new (ARGV[0])
  iter=dict.root
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  iter.transit("a")
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  iter.transit("b")
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  iter.transit("c")
  print "Iter Status=#{iter.status}\t#{iter.terminator}\n"
  print "OK\n"
rescue 
  print "Error : #{$!}\n"
end


#!/usr/bin/env ruby
require 'wcdict'
if ARGV.length <= 0 then
  $stderr.print "Invalid argument.\n"
  exit 1
end

begin
  dict=WcDict.new (ARGV[0])
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


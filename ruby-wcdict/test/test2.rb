#!/usr/bin/env ruby
require 'wcdict'
if ARGV.length <= 0 then
  $stderr.print "Invalid argument.\n"
  exit 1
end

begin
  dict=WcDict.new (ARGV[0])
  iter=dict.root
  print "OK\n"
rescue 
  print "Error : #{$!}\n"
end


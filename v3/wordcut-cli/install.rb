#!/usr/bin/env ruby
require 'getoptlong'
require 'ftools'
opt=GetoptLong.new(['--prefix','-d',GetoptLong::REQUIRED_ARGUMENT]);
$option=Hash.new(false)
opt.each do |name,arg|
  case name
  when '--prefix'
    $option['prefix']=arg
  when '--tag'
    $option['tag']=true
  else
  end
end
if not $option['prefix'] then
  $prefix="/usr/local"
else
$prefix=$option['prefix']
end
$bin="#{$prefix}/bin"
File.install("wordcut.rb","#{$bin}/wordcut",0755,true)

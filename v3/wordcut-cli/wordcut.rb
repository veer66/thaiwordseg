#!/usr/bin/env ruby
require 'wordcut'
require 'getoptlong'

$option=Hash.new(false)

opt=GetoptLong.new(['--debug','-d',GetoptLong::NO_ARGUMENT],
		   ['--no-unk-join','-j',GetoptLong::NO_ARGUMENT],
		   ['--tag','-T',GetoptLong::NO_ARGUMENT])
opt.each do |name,arg|
  case name
  when '--debug'
    $option['debug']=true
    print "DEBUG\n"
  when '--tag'
    $option['tag']=true
  when '--no-unk-join'
    $option['no-unk-join']=true
  else
    print "Else : #{name}\n"
  end
end

#ARGV.each {|x| print "X=#{x}\n" }
begin
  wc=Wordcut::Wordcut.new

  if $option['debug'] then
    wc.set_debug_select_path
  end

  if $option['no-unk-join'] then
    wc.no_unk_join
  end

  input_file=STDIN

  tag=$option['tag']



  while input_file.gets do
    if tag then
      ret=wc.cutline_tag($_)
    else
      ret=wc.cutline($_)
    end
    print ret
  end

  if (input_file != STDIN) then
    input_file.close
  end
rescue 
  print "Error: #{$!}"
end


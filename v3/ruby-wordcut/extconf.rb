#!/usr/bin/env ruby
require "mkmf"
module PkgConfig
  def PkgConfig.find(pkg_name,version=nil)
    found=false
    if version != nil then
      print "checking for package #{pkg_name} >= #{version} ... "
      if system "pkg-config #{pkg_name} --atleast-version #{version}" then
	found=true
      end
    else 
      print "checking for package #{pkg_name} ... "
      if system "pkg-config #{pkg_name}"
	found=true
      end
    end
    if found then
      
      print "yes\n"
      @found = true
      pipe = IO.popen "pkg-config #{pkg_name} --cflags"
      @cflags = pipe.gets.chomp
      pipe.close
      pipe = IO.popen "pkg-config #{pkg_name} --libs"
      @libs = pipe.gets.chomp
      pipe.close
      
      $CFLAGS=$CFLAGS+@cflags
      $LDFLAGS=$LDFLAGS+@libs
    else
      print "no\n"
      exit
    end
  end
end

PkgConfig.find("wordcut","0.1.1")
create_makefile "wordcut"


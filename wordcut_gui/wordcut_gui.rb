#!/usr/bin/env ruby

=begin
Wordcut GUI
Copyright (C) 2002 Vee Satayamas

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=end
 
require 'libglade2'
require 'gnome2'
require 'wordcut'
require 'iconv'

class OpenSel
	def initialize(wordcut_gui)
		@glade=GladeXML.new('wordcut_gui.glade',
			'opensel') do |callback|
			method(callback)
		end
		@filesel=@glade.get_widget('opensel')
		@wordcut_gui=wordcut_gui
	end

	def on_cancelbut_clicked
		close
	end

	def on_okbut_clicked
		filename=@filesel.filename
		@wordcut_gui.readtext(filename)
		close
	end

	def close
		@filesel.hide
		@wordcut_gui.unlock_opensel
	end
end

class SaveSel
	def initialize(wordcut_gui)
		@glade=GladeXML.new('wordcut_gui.glade',
			'savesel') do |callback|
			method(callback)
		end
		@filesel=@glade.get_widget('savesel')
		@wordcut_gui=wordcut_gui
	end

	def on_cancelbut_clicked
		close
	end

	def on_okbut_clicked
		filename=@filesel.filename
		@wordcut_gui.save_as_text(filename)
		close
	end

	def close
		@filesel.hide
		@wordcut_gui.unlock_savesel
	end
end


class WordcutGUI
	def initialize
		@glade=GladeXML.new('wordcut_gui.glade','mainwin') do |callback|
			method(callback)
		end
		@txtview=@glade.get_widget('textview')
		@bcd=Iconv.new("TIS-620","UTF-8")
		@cd=Iconv.new("UTF-8","TIS-620")
		@wc=ThaiWordcut::Wordcut.new
		@opensel_lock=false
		@savesel_lock=false
	end

	def save_as_text(filename)
		save_text(filename)		
	end

	def save_text(filename)
		f=open(filename,"w")
		buf=@txtview.buffer
		txt=buf.get_text(buf.start_iter,
			buf.end_iter,true)
		tistxt=@bcd.iconv(txt)
		f.puts(tistxt)
		f.close
	end

	def readtext(filename)
		print "Filename = #{filename}\n"
		begin
			f=open(filename)
			buf=@txtview.buffer
			buf.set_text('')
			while f.gets do
				buf.insert_at_cursor(@cd.iconv($_))	
			end
		rescue IOError
			print "can't read file!!!"
		end
	end

	def on_quitbut_clicked 
		quit
	end

	def on_mainwin_delete_event
		quit
	end

	def on_openbut_clicked
		if not @opensel_lock then
			opensel=OpenSel.new(self)
			@opensel_lock=true
		end
	end

	def on_savebut_clicked
		if not @savesel_lock then
			savesel=SaveSel.new(self)
			@savesel_lock=true
		end
	end

	def unlock_opensel
		@opensel_lock=false
	end

	def unlock_savesel
		@savesel_lock=false
	end

	def on_exebut_clicked
		buf=@txtview.buffer
		txt=buf.get_text(buf.start_iter,
			buf.end_iter,true)
		tistxt=@bcd.iconv txt
		buf.set_text(@cd.iconv(@wc.ccutline(tistxt)))
	end

	def on_quit1_activate
		quit
	end

	def quit
		Gtk.main_quit
	end

	def on_aboutmni_activate
		about=AboutDialog.new
	end
end

class AboutDialog
	def initialize
		@glade=GladeXML.new('wordcut_gui.glade',
			'aboutdia') do |callback|
			method(callback)
		end
	end

	def on_closebut_clicked
		dia=@glade.get_widget('aboutdia')
		dia.hide
	end
end


if $0 == __FILE__ then
	Gtk.init
	wordcut_gui = WordcutGUI.new
	Gtk.main
end

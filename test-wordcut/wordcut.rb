#!/usr/bin/env ruby

=begin
Limitation Thai string can't be longer than 65535 characters.
=end

require 'wcdict'
require 'wcthaitokenizer'

DictPath='/home/vee/tmp/dict.etd'
module TestWordcut
  class Token
    attr_reader :iter , :start
    attr_writer :iter , :start
    def initialize
    end
  end
  
  class ForestNode
    attr_reader :cut_point , :pos
    attr_reader :cut_point , :pos
    def initialize(cut_point,pos)
      @cut_point=cut_point
      @pos=pos
    end
  end
  
  class Forest < Hash
    
    attr_reader :str
    
    def build_token(start)
      tok=Token.new
      tok.iter=@dict.root
      tok.start=start
      return tok
    end
    
    def initialize(dict,str)
      @dict=dict
      @str=str
      @token_pool=[]
      
      for i in 0..str.length-1 do 
	@token_pool << build_token(i);
	@token_pool.each { |tok| tok.iter.transit(str[i..i]) }
	@token_pool.reject! { |tok| tok.iter.status==WcDictIter::DEAD }
	@token_pool.select { |tok| tok.iter.terminator }.
	  each do |tok| 
	  if (self[i]==nil) then
	    self[i]=[]
	  end
	  self[i]<< ForestNode.new(tok.start,tok.iter.pos)
	end
      end
    end
  end
  

  class GraphNode
    attr_reader :weight , :cut_point , :pos , :type
    attr_writer :weight , :cut_point , :pos , :type
    def set(weight,cut_point,pos=nil,type=nil)
      @weight=weight
      @cut_point=cut_point
      @pos=pos
      @type=type
    end
    
    def p
      print "Weight=#{@weight}\tCut point=#{cut_point}"
    end
  end
  

  
  class Graph < Array

    def initialize(forest)
      @forest=forest
      cut
    end
    
    def cut
      max=@forest.str.length
      for i in 0..max-1 do

	if self[i]==nil then

	  self[i]=GraphNode.new
	  self[i].set(max*i*2,i,nil,Word::UNKNOWN)

	  if(@forest[i]!=nil) then #forest search 

	    @forest[i].each do |node|
	      s=node.cut_point
	      if (s==0) then
		self[i].set(1,0,node.pos,Word::DICT)
	      else
		if (self[s-1].weight+1 < self[i].weight) then
		  self[i].set(self[s-1].weight+1,s,node.pos,Word::DICT)
		end
	      end 
	    end
	    
	  end #end forest search
	  
	end  

      end #end for
    end # end def
    
    def p
      for i in 0..@forest.str.length-1 do 
	print "#{self[i].p}\n"
      end
    end
    
    def add_s(i)
      if i>=0 then
	@buf.push(@delimiter)
	p=self[i].cut_point
	pos=self[i].pos
	@buf.push(">")
	first=true
	pos.each_str do |posstr| 
	  if (not first) 
	    @buf.push(",")
	  end
	  first =false
	  @buf.push(posstr) 
	end
	@buf.push("<")					
	@buf.push(@str[p..i])
	add_s(p-1)
      end
    end
    
    def to_s(delimiter="|")
      @delimiter=delimiter
      @str=@forest.str
      @buf=[]
      add_s(@str.length-1)
      @buf.reverse[0..-2].to_s
    end

    def to_result

      str=@forest.str
      last=str.length-1
      i=last
      
      result=WordcutResult.new

      while i>=0 do
	c=self[i].cut_point
	result.add(str[c..i],self[i].pos)
	i=c-1
      end

      return result.reverse

    end
  end
  
  class Wordcut
    def initialize(dict)
      @dict=dict
      @forest_maker=lambda{ |str| Forest.new(@dict,str)  }
    end
    
    def cut(str)
      #print "\n**** Cut str = ---#{str}---\n"
      g=Graph.new(Forest.new(@dict,str))
      return post_wordcut(g.to_result)

    end        

    def cutline(str,delimiter=' | ')

      buf=""

      WcThaiTokenizer::tok(str) do |tok,lang|

	if (lang==WcThaiTokenizer::Thai)
	  result=cut(tok)
	  result.each do |w|
	    buf << w.surface << delimiter
	  end
	else
	  buf <<  tok << delimiter
	end

      end
      return buf

      
    end

    def post_wordcut(result)
      i=0
      while i<result.length do
	deleted=false
	#print "i=#{i}\t#{result[i].surface}\n"
	if(result[i].surface.length==1)
	  
	  if(i==0)
	    if(result.length>1)
	      print "JOIN #{result[i].surface} + #{result[i+1].surface}\n"
	      result[i+1].surface=result[i].surface + result[i+1].surface
	      result[i+1].pos=nil
	      result[i+1].type=Word::JOIN
	      result.delete_at(i)
	      deleted=true
	    end
	  else
	      print "JOIN #{result[i-1].surface} + #{result[i].surface}\n"
	      result[i-1].surface=result[i-1].surface + result[i].surface
	      result[i-1].pos=nil
	      result[i-1].type=Word::JOIN
	      result.delete_at(i)
	      deleted=true
	  end
	end
	if not deleted then
	  i=i+1	
	end
      end
      return result
    end

  end

  class Word 
    attr_reader :surface , :pos , :type
    attr_writer :surface , :pos , :type
    
    DICT=1
    UNKNOWN=2
    JOIN=3
    
    def initialize(surface,pos,type)
      @surface=surface
      @pos=pos
    end
  end


  class WordcutResult < Array

    def add(surface,pos=nil,type=nil)
      self << Word.new(surface,pos,type)
    end
    
    def dump

      self.each do |w|
	print "#{w.surface}"
	w.pos.each_str { |ps| print "\t#{ps}" }
	print "\n"
      end

    end

  end

  class WordUnit
    def initialize(filename='unit.rul')
      f=open(filename)
      @rule=[]
      while f.gets do 
     	@rule << $_.chomp 
      end
      f.close
    end	

    def cut(str)      
      marker=0
      tab=[]
      @rule.each do |rul|
	
      end
    end

    def mark(tab,start,stop)
      
    end

    def dump_rule
      @rule.each { |rul| print "#{rul}\n" }
    end
  end


end

# main
if $0 == __FILE__ then
  #  wc=TestWordcut::Wordcut.new(WcDict.new(DictPath))
  #  result=wc.cut("ตากลม")
  #  result.dump
  unit=TestWordcut::WordUnit.new
end

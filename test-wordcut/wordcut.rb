#!/usr/bin/env ruby
require 'wcdict'

DictPath='/home/vee/tmp/dict.etd'

class Token
  attr_reader :iter , :start
  attr_writer :iter , :start
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
  attr_reader :weight , :cut_point , :pos
  attr_writer :weight , :cut_point , :pos
  def set(weight,cut_point)
    @weight=weight
    @cut_point=cut_point
  end

  def p
    print "Weight=#{@weight}\tCut point=#{cut_point}"
  end
end

class Wordcut
  def initialize(dict)
    @dict=dict
    @forest_maker=lambda{ |str| Forest.new(@dict,str)  }
  end
  
  def cut(str)
    Graph.new(Forest.new(@dict,str))
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
	self[i].set(max,i)
	if(@forest[i]!=nil) then
	  @forest[i].each do |node|
	    s=node.cut_point
	    if (s==0) then
	      self[i].weight=1
	      self[i].cut_point=0
	      self[i].pos=node.pos
	    else
	      if (self[s-1].weight+1 < self[i].weight) then
		self[i].weight=self[s-1].weight+1
		self[i].cut_point=s
		self[i].pos=node.pos
	      end
	    end
	  end
	end
      end
    end
  end

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
  

  def cutline(line)
  end
end

class WordcutResult
  
end

# main
if $0 == __FILE__ then
  wc=Wordcut.new(WcDict.new(DictPath))
  print "#{wc.cut("µÒ¡ÅÁ¼ÁÂÒÇ").to_s}\n"
end



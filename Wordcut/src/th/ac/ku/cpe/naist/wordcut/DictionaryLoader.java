/*
 * DictionaryLoader.java
 * Copyright (C) 2002 Vee Satayamas
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
package th.ac.ku.cpe.naist.wordcut;
import java.io.*;

import org.xml.sax.*;
import org.xml.sax.helpers.DefaultHandler;

import javax.xml.parsers.SAXParserFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;


public class DictionaryLoader extends DefaultHandler
{
    // state
    final static int START      = 0;
    final static int DICTIONARY = 1;
    final static int WORD       = 2;
    final static int TEXT       = 3;
    final static int POS        = 4;
    final static int FINISH     = 5;
    
    
    int state=START;
    
    
    private Locator locator;
    Word[] words = new Word[60000];
    int word_count=0;
    String[] pos_list = new String[8];
    int pos_count=0;

    String text;
    String pos;

    Dictionary dictionary;
    DictionaryLoader (Dictionary dictionary) {
        this.dictionary = dictionary;
    }

    static public void load (File file,Dictionary dictionary) 
    throws ParserConfigurationException , SAXException , IOException {
        DefaultHandler handler = new DictionaryLoader(dictionary);
        SAXParserFactory factory = SAXParserFactory.newInstance();
        SAXParser saxParser = factory.newSAXParser();
        saxParser.parse(file , handler);
    }

    static public void load (String uri,Dictionary dictionary) 
    throws ParserConfigurationException , SAXException , IOException {
        DefaultHandler handler = new DictionaryLoader(dictionary);
        SAXParserFactory factory = SAXParserFactory.newInstance();
        SAXParser saxParser = factory.newSAXParser();
        saxParser.parse(uri , handler);
    }




    public static void main (String args[]) throws Exception {
        try {
  //          Dictionary dict=new Dictionary();
            Dictionary dict=new Dictionary(new File(args[0]));
            DictionaryLoader.load(new File (args[0]),dict);
            //dict.dump();
        } catch (SAXParseException e) {
            System.out.println ("Error at Line number = " + e.getLineNumber());
            System.out.println ("Message = " + e.getMessage());
        }
    }
    
    //public void startDocument() throws SAXException {
    //}

    public void endDocument() throws SAXException {
        build_words();
    }

    public void startElement(String namespaceURI,
                             String lName, // local name
                             String qName, // qualified name
                             Attributes attrs) throws SAXException 
    {
        //System.out.println ("Start Element");
        
        String element = qName;
        
        if (element.equals("dictionary")) {
            if (state == START) {
                state = DICTIONARY; 
                //System.out.println ("start  dictionary");
                //System.out.println ("state = "+state);
            }
            else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        }else if (element.equals("word")) {
            if (state == DICTIONARY) {
                state = WORD; 
                //System.out.println ("start word");
                //System.out.println ("state = "+state);
            }
            else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        }else if (element.equals("text")) {
            if (state == WORD) { 
                state = TEXT; 
                //System.out.println ("start text");
                //System.out.println ("state = "+state);
            } else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        }else if (element.equals("pos")) {
            if (state == WORD) {
                state = POS; 
                //System.out.println ("start pos");
                //System.out.println ("state = "+state);
            }
            else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        }else {
            throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        }
    }

    public void setDocumentLocator( Locator locator ) {
        this.locator = locator;
    }
    
   
    public void endElement(String namespaceURI,
                           String sName, // simple name
                           String qName  // qualified name
                          )  throws SAXException
    {
        
        //System.out.println ("End Element");
        
        String element = qName;
        if (element.equals("dictionary")) {
            if (state == DICTIONARY) {
                state = FINISH; 
                //System.out.println ("end dictionary");
                //System.out.println ("state = "+state);
            }
            else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        } else if (element.equals("word")) {
            //System.out.println ("-->word");
            if (state == WORD) {
                state = DICTIONARY; 
                //System.out.println ("-->wordi2");
                build_word();
                //clear word
                pos=null;
                text=null;
                //System.out.println ("end word");
                //System.out.println ("state = "+state);
            }
            else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
       } else if (element.equals("text")) {
            if (state == TEXT) { 
                state = WORD; 
                //System.out.println ("end text");
                //System.out.println ("state = "+state);
            }    
            else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        } else if (element.equals("pos")) {
            if (state == POS) {
                state = WORD; 
                build_pos();
                //System.out.println ("end pos");
                //System.out.println ("state = "+state);
            }
            else throw new 
                SAXParseException("Invalid element order : " + element, 
                    locator);
        }
    }

    //debug
    //String pre,pre2;
    
    public void characters(char buf[], int offset, int len)
    throws SAXException {
        
        String str = new String(buf, offset, len);
        //System.out.println ("\t-"+str+"-");
        switch (state) {
            case TEXT:
                //System.out.println ("TEXT");
                if (text==null)    
                    text = str;
                else 
                    text=text.concat(str);
                
                /*
                if (text.equals("¡")) {
                    System.out.println ("--Text ¡ found");
                    System.out.println ("pre="+pre);
                    System.out.println ("pre2="+pre2);
                }
                pre = text;
                */
                break;
            case POS:
                //System.out.println ("POS");
                if (pos == null) 
                    pos  = str;
                else
                    pos=pos.concat(str);
                
                break;
            default: 
                //System.out.println ("invalid state = "+state);
                // System.out.println ("--"+str+"--");
                
                //if (!str.equals("\n"))throw new 
                //    SAXParseException("Invalid character at state : " 
                //                    + state,locator);
        }
        //pre2 = str;
    }


    // Utility
    void build_word() {
            /*
        Word word=new Word();
        word.setText(text);
        Object pos_tmp[] = pos_list.toArray();
        Pos[] pos_array  = new Pos[pos_tmp.length];
        for (int i=0;i<pos_tmp.length;i++) {
            pos_array[i] = new Pos ((String)pos_tmp[i]);    
        }
        word.setPosArray(pos_array);
        words.add(word);
        */
        Pos[] poses = new Pos[pos_count];
        for (int i=0;i<pos_count;i++) {
            poses[i] = new Pos(pos_list[i]);
        }
        Word word=new Word(text,poses);
        words[word_count++] = word;
        pos_count=0;
    }

    void build_words() {
            /*
        Object words_tmp[] = words.toArray();
        Word[] word_array = new Word[words_tmp.length];
        for (int i=0;i<words_tmp.length;i++) {
            word_array[i] = (Word)words_tmp[i];
        }
        dictionary.setWords(word_array);
        */
        dictionary.setWords(words,word_count);
        word_count=0;
    }
    void build_pos() {
        pos_list[pos_count++] = pos; 
    }
 
}


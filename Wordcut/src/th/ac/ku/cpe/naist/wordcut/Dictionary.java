/*
 * Dictionary.java
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

class Dictionary {
    Word[] words=null;

    public String[] getWordlist() {
        String[] wordlist = new String[words.length];
        for (int i=0;i<words.length;i++) {
            wordlist[i] = words[i].getText();
        }
        return wordlist;
    }
    
    
    Dictionary (Word[] words,int count) {
        setWords(words,count);
    }

    Dictionary () {
    }

    Dictionary (File dict_filename) throws org.xml.sax.SAXException , IOException ,
        javax.xml.parsers.ParserConfigurationException {
        DictionaryLoader.load (dict_filename,this);
        
    }

    Dictionary (String uri) throws org.xml.sax.SAXException , IOException ,
        javax.xml.parsers.ParserConfigurationException {
        DictionaryLoader.load (uri,this);
        
    }

    private void adjustWords(int word_count)  {
        Word[] tmp = new Word[word_count];
        for (int i=0;i<word_count;i++) {
            tmp[i] = words[i];
        }
        words=tmp;
    }

    public void setWords (Word[] words,int count) {
        this.words = words;
        adjustWords(count);
    }

    public void dump () {
        for (int i=0;i<words.length;i++) { 
            System.out.print (i+"\t"+ words[i].getText());
            Pos[] pos = words[i].getPos();
            for (int j=0;j<pos.length;j++) { 
                System.out.print ("\t"+pos[j].getText());
            }
            System.out.println ("");
        }
    }
}

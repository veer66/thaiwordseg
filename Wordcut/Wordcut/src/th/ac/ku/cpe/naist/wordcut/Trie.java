/*
 * Trie.java
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
/*
 * Trie.java
 *
 * Created on July 14, 2002, 5:23 AM
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
import java.io.*;
import java.util.*;
class Trie {
    // instances
    String _wordlist[];
    Map _index = new HashMap();
    boolean debug=false;
    public void setDebug(boolean debug) { this.debug= debug; }
    
    public Trie(String __wordlist[]) {
        build_trie(__wordlist);
    }
    
    public Trie(Object __o_wordlist[]) {
        String[] wordlist = new String[__o_wordlist.length];
        for (int i=0;i<__o_wordlist.length;i++) {
            wordlist[i] = (String)__o_wordlist[i];
        }
        build_trie(wordlist);
    }
    
    protected void build_trie(String __wordlist[]) {
        _wordlist = __wordlist;
        Arrays.sort(_wordlist);
        
        for (int i=0;i<_wordlist.length;i++) {
            
            
            if (_wordlist[i].length() >= 1) {
                String key1 = _wordlist[i].substring(0,1);
                
                if (_index.get(key1)==null) {
                    _index.put(key1,new Integer(i));
       
                }
                if (_wordlist[i].length() >= 2) {
                    String key2 = _wordlist[i].substring(0,2);
                    if (_index.get(key2)==null) _index.put(key2,new Integer(i));
                }
            }
            
        }
    }
    
    public TrieIterator trieIterator(int __start) {
        return new TrieIterator(this,__start);
    }
    
    public boolean isLeave(TriePosition __trie_position) {
        //DEBUG
        //System.out.print ("\tLeave\tX = "+(__trie_position.x));
        //System.out.print ("\tY = "+__trie_position.y);
        //System.out.println ("\tLen = "+_wordlist[__trie_position.y].length());
        if (debug) {
            System.out.println ("Isleave x="+__trie_position.x+"\tlen="+
                _wordlist[__trie_position.y].length());
        }
        return ((__trie_position.x) == _wordlist[__trie_position.y].length());
    }
    
    public boolean transit(TriePosition __trie_position,char __ch) {
        if (__trie_position.x==0) {
            if (debug) System.out.println ("T0 before word = "+_wordlist[__trie_position.y]+"\tch="+__ch+"\tx="+__trie_position.x);
            char a_ch[] = new char[1];
            a_ch[0] = __ch;
            Object index=_index.get(new String(a_ch,0,1));
            //System.out.println("--- go");
            if (index == null) return false;
            //System.out.println("--- found");
            __trie_position.x=1;
            __trie_position.y= ((Integer)index).intValue();
            if (debug) System.out.println ("T0 after word = "+_wordlist[__trie_position.y]+"\tch="+__ch+"\tx="+__trie_position.x);
            return true;
        } else if (__trie_position.x==1) {
            if (debug) System.out.println ("T1 before word = "+_wordlist[__trie_position.y]+"\tch="+__ch+"\tx="+__trie_position.x);
            char a_ch[] = new char[2];
            a_ch[1] = __ch;
            a_ch[0] =  _wordlist[__trie_position.y].charAt(0);
            Object index=_index.get(new String(a_ch,0,2));
            if (index == null) return false;
            
            __trie_position.x=2;
            __trie_position.y=((Integer)index).intValue();
            // debug
            if (debug) System.out.println ("T1 after word = "+_wordlist[__trie_position.y]+"\tch="+__ch+"\tx="+__trie_position.x);
            return true;
        } else {
            boolean found=false;
            int i= __trie_position.y;
            // DEBUG
            if (debug)  System.out.println ("TX before word = "+_wordlist[i]+"\tch="+__ch+"\tx="+__trie_position.x);
            String prefix=_wordlist[i].substring(0,__trie_position.x);
            while (i<_wordlist.length) {
                if (_wordlist[i].length() >= prefix.length() &&
                prefix.equals(_wordlist[i].substring(
                0,__trie_position.x))) {
                    if (_wordlist[i].length() > __trie_position.x
                    && _wordlist[i].charAt(__trie_position.x)==__ch) {
                        found = true;
                        break;
                    }
                } else {
                    break;
                }
                i++;
            }
            if (found) {
                __trie_position.x++;
                __trie_position.y=i;
            }
            // debug
            if (debug) System.out.println ("TX after word = "+_wordlist[i]+"\tch="+__ch+"\tx="+__trie_position.x);
            return found;
        }
    }
    
    public String[] getWordlist() {
        return _wordlist;
    }
}

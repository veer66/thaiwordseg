/*
 * DictionaryBaseForestFactory.java
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

class DictionaryBaseForestFactory implements ForestFactory {
    Trie trie;
    double weight=1;
    boolean debug=false;
    
    public void setDebug(boolean debug) {this.debug=debug;}
    public void setWeight(double weight) { this.weight=weight; }
    
    DictionaryBaseForestFactory(Dictionary dictionary) {
        trie = new Trie(dictionary.getWordlist());
        trie.setDebug(debug);
    }
    
    void test() {
        ActiveTrieIteratorList active_list = new ActiveTrieIteratorList(1024,trie);
        active_list.newToken();
        char[] s = (new String("ตากลม")).toCharArray();
        for (int i=0;i<s.length;i++) {
            active_list.transit(s[i]);
            System.out.println ("I = "+i+"\tSize = "+active_list.getCompleteList().size());
        }
    }
    
    static final int ACTIVE_LIST_SIZE = 1024;
    
    public WordcutForest cut (String str) {
        ActiveTrieIteratorList active_list = new ActiveTrieIteratorList(ACTIVE_LIST_SIZE,trie);
        active_list.setDebug(debug);
        active_list.newToken();
        char[] s = str.toCharArray();
        WordcutForest wordcut_forest = new WordcutForest(s.length);
        for (int i=0;i<s.length;i++) {
            active_list.transit(s[i]);
            TrieIteratorList complete_list = active_list.getCompleteList();
            for (int j=0;j<complete_list.size();j++) {
                int start=complete_list.getTrieIterator(j).getStart();
                wordcut_forest.put(start,i,weight); // stop = i
            }
        }
        return wordcut_forest;
    }
   
}

/*
 *
 * TestTrie.java
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
 * TestTrie.java
 *
 * Created on 15 สิงหาคม 2545, 23:40 น.
 */

/**
 *
 * @author  vee
 */

package th.ac.ku.cpe.naist.wordcut;
import java.io.*;
public class TestTrie {
    
    /** Creates a new instance of TestTrie */
    public TestTrie() {
    }
    public static void main (String[] args) throws Exception {
        Dictionary dictionary = new Dictionary(new File("/home/vee/Farm/bookindex/data/bookindex.dict"));
        Trie trie = new Trie(dictionary.getWordlist());
            
        TrieIterator trie_iterator = trie.trieIterator(0);
        char[] str="ตากลมแตดเรอะ".toCharArray();
        int status;
        for (int i=0;(status=trie_iterator.transit(str[i]))!=TrieIterator.DEAD && i < str.length;i++) {
            System.out.print ("Pos = "+i);
            System.out.println ("\tStatus = "+status);
        }
        System.out.println ("END JOB!");
    }
}

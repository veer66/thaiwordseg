/*
 *
 * TrieIteratorList.java
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
 * TrieIteratorList.java
 *
 * Created on 16 สิงหาคม 2545, 13:04 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class TrieIteratorList {
    TrieIterator[] trie_iterators;
    int size=0;
    /** Creates a new instance of TrieIteratorList */
    TrieIteratorList(int size) {
        trie_iterators=new TrieIterator[size];
    }
    
    public boolean empty() {
        return (size == 0);
    }
    
    public int size() {
        return size;
    }
    public void add (TrieIterator trie_iterator) {
        trie_iterators[size++] = trie_iterator;
    }
    
    public TrieIterator getTrieIterator(int index) {
        return trie_iterators[index];
    }
    
    public void remove (int index) {
        if (index != size-1) {
            trie_iterators[index] = trie_iterators[size-1];
        }
        size--;
    }
    
    public void clear() {
        size=0;
    }
}

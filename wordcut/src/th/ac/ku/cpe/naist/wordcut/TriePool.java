/*
 * TriePool.java
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
 * TriePool.java
 *
 * Created on 12 สิงหาคม 2545, 2:11 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */


public class TriePool {
    final static int MAXTRIE = 1024;
    final static int MAX_REMOVE_LIST = 1024;
    final static int MAX_COMPLETE_LIST = 16;
    /** Creates a new instance of TriePool */
    TrieIterator[] trie_iterators;
    int size;
    
    TriePosition[] complete_list;
    Trie trie;
    ListUpdater list_updater;
    public TriePool(Trie trie) {
        this.trie= trie;
        trie_iterators = new TrieIterator[MAXTRIE];
        size = 0;
        list_updater = new ListUpdater();
    }
    /*          Add new one
     *          Check for status , delete , adjust and report
     */
    public void transit(char ch,int start) {
        trie_iterators[size++] = new TrieIterator(trie,start);
        for (int i=0;i<size;i++) {
            addNewTrie(start);
        }
        list_updater.update(ch);
        System.out.println ("Active = "+size);
    }
    
    public TrieIterator[] getCompleteList() {
        return list_updater.getCompleteList();
    }
    
    void addNewTrie(int start) {
        TrieIterator trie_iterator = new TrieIterator(trie,start);
        
    }
    class ListUpdater {
        int remove_list[] = new int[MAX_REMOVE_LIST];
        int remove_count;
        TrieIterator complete_list_temp[] = new TrieIterator[MAX_COMPLETE_LIST];
        int complete_count=0;
        
        TrieIterator[] getCompleteList () {
            if (complete_count == 0) return null;
            TrieIterator[] complete_list = new TrieIterator[complete_count];
            for (int i=0;i<complete_count;i++) {
                complete_list[i] = complete_list_temp[i];
            }
            return complete_list;
        }
        
        void  update(char ch) {
            remove_count=0;
            complete_count = 0;
            // check status
            for (int i=0;i<size;i++) {
                int state=trie_iterators[i].transit(ch);
                if (state ==TrieIterator.DEAD) {
                    //remove_list[remove_count++] = i;
                } else if (state == TrieIterator.COMPLETE) {
                    complete_list_temp[complete_count++] = trie_iterators[i];
                }
            }
            
            // remove
            if (remove_count < size) {
                size = 0;
            } else {
                for (int i=remove_count-1;i!=0;i--) {
                    if (remove_list[i] != (size-1)) {
                        trie_iterators[remove_list[i]]  = trie_iterators[size-1];
                    } 
                    size--;
                }
            }
        }
    }
}


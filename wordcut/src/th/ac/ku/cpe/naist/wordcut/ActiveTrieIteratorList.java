/*
 * ActiveTrieIteratorList.java
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

/**
 *
 * @author  vee
 */
public class ActiveTrieIteratorList extends TrieIteratorList {
    final static int REMOVE_LIST_SIZE=16;
    final static int COMPLETE_LIST_SIZE=16;
    /** Creates a new instance of ActiveTrieIteratorList */
    TrieIteratorList complete_list= new TrieIteratorList(COMPLETE_LIST_SIZE);
    Trie trie;
    int start;
    boolean debug=false;
    public void setDebug(boolean debug) { this.debug= debug; }
    RemoveList remove_list = new RemoveList(REMOVE_LIST_SIZE);
    public ActiveTrieIteratorList(int size,Trie trie) {
        super(size);
        this.trie = trie;
    }
    
    public void newToken() {
        clear();
        this.start = 0;
    }
    
    public void transit (char ch) {
        TrieIterator new_trie_iterator = new TrieIterator(trie,start++);
        if (debug) new_trie_iterator.setDebug(true);
        add(new_trie_iterator);
        remove_list.clear();
        complete_list.clear();
        for (int i=size-1;i>=0;i--) {
            // DEBUG
            if (debug) System.out.println ("act ch="+ch+"\ti="+i+"\tstart="+trie_iterators[i].getStart());
            int status=trie_iterators[i].transit(ch);
            if (status == TrieIterator.DEAD) {
                remove_list.add(i);
            } else if (status == TrieIterator.COMPLETE) {
                if (debug) System.out.println ("com act ch="+ch+"\ti="+i+"\tstart="+trie_iterators[i].getStart()+"\tHere="+start);
                complete_list.add(trie_iterators[i]);
            }
        }
        
        int[] remove_array=remove_list.getArray();
        int remove_size = remove_list.size();
        for (int i=remove_size-1;i>=0;i--) {
            remove (remove_array[i]);
        }
        
    }
    
    public TrieIteratorList getCompleteList () {
        return complete_list;
    }
}

/*
 *
 * TrieIterator.java
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
 * TrieIterator.java
 *
 * Created on July 14, 2002, 5:25 AM
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
class TrieIterator {
    
    /** Creates a new instance of TrieIterator */
    
    int _status;
    final static int ACTIVE     = 1;
    final static int DEAD       = 2;
    final static int COMPLETE   = 3;
    
    TriePosition _trie_position = new TriePosition();
    Trie _trie;
    int _start;
    
    boolean debug=false;
    public void setDebug(boolean debug) { this.debug= debug; }
    
    TrieIterator(Trie __trie,int __start) {
        _trie   = __trie;
        
        _start = __start;
    }
    
    public int getStart() {
        return _start;
    };
    
    public TriePosition getTriePosition() {
        return _trie_position;
    };
    
    public int transit(char ch) {
        _status = ACTIVE;
        if (_trie.transit(_trie_position,ch)) {
            if (_trie.isLeave(_trie_position)) {
                _status = COMPLETE;
            }
        } else {
            _status = DEAD;
        }
        if (debug) System.out.println ("Trieiterator ch="+ch+"\t_status="+_status+
            "\tx="+_trie_position.x+"\ty="+_trie_position.y);
        return _status;
    }
}

/*
 *
 * WordcutForest.java
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
 * WordcutPath.java
 *
 * Created on 16 สิงหาคม 2545, 23:15 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class WordcutForest {
    final static int MAX_EDGES_SIZE=16;
    WordcutEdges[] edges_table;
    boolean debug=false;
    int len;
    /** Creates a new instance of WordcutPath */
    public WordcutForest(int len) {
        this.len = len;
        edges_table = new WordcutEdges[len];
    }
    
    public void setDebug(boolean debug) { this.debug = debug; }
    
    public int length() {
        return len;
    }
       
    public void put (int start,int stop) {
        
        if (edges_table[start] == null) edges_table[start] = new WordcutEdges(MAX_EDGES_SIZE);
        edges_table[start].put(stop);
    }
    
    public void put (int start,int stop,double weight) {
        if (debug) {
            System.out.println ("Start="+start+"\tStop="+stop);
        }
        if (edges_table[start] == null) edges_table[start] = new WordcutEdges(MAX_EDGES_SIZE);
        edges_table[start].put(stop,weight);
    }
    public WordcutEdges getWordcutEdges(int index) {
        return edges_table[index];
    }
    public void merge (WordcutForest forest) throws WordcutForestException {
        if (forest.length() != len) throw 
        new WordcutForestException("Cannot merge forests (invalid length)"); 
        for (int i=0;i<len;i++) {
            if (forest.getWordcutEdges(i)!=null) {
                if (edges_table[i] == null) edges_table[i] = new WordcutEdges(MAX_EDGES_SIZE);
                edges_table[i].merge(forest.getWordcutEdges(i));
            }
        }
    }
}

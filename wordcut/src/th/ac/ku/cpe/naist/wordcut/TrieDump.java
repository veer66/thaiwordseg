/*
 *
 * TrieDump.java
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
 * TrieDump.java
 *
 * Created on 18 สิงหาคม 2545, 12:34 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class TrieDump {
    
    /** Creates a new instance of TrieDump */
    public TrieDump() {
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception{
        
        Dictionary dictionary =
            new Dictionary(new java.io.File("/home/vee/Farm/bookindex/data/bookindex.dict"));
        //Trie trie = new Trie (dictionary.getWordlist());
        String[] strs = dictionary.getWordlist();
                        //trie.getWordlist();
        for (int i=0;i<strs.length;i++) {
            System.out.println (strs[i]);
        }
        
    }
    
}

/*
 * TestDictionaryCut.java
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
 * TestDictionaryCut.java
 *
 * Created on 17 สิงหาคม 2545, 11:00 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class TestDictionaryCut {
    
    /** Creates a new instance of TestDictionaryCut */
    public TestDictionaryCut() {
    }
    public static void main (String[] args) throws Exception {
         
        DictionaryBaseForestFactory dict_cut = new DictionaryBaseForestFactory (
            new Dictionary(new java.io.File("/home/vee/Farm/bookindex/data/bookindex.dict")));
            String s=args[0];
            WordcutForest forest = dict_cut.cut(s);
            for (int i=0;i<forest.length();i++) {
                WordcutEdges edges=forest.getWordcutEdges(i);
                if (edges != null) {
                    for (int j=0;j<edges.size();j++) {
                        System.out.print ("-"+s.substring(i,edges.getStop(j))+"-\t");
                        System.out.println ("i="+i+"\tj="+j+"\tstop="+edges.getStop(j));
                    }
                } else {
                    System.out.println ("Edge=NULL I= "+i);
                }
            }
    }
}

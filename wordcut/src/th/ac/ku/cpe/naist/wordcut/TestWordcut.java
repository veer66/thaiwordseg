/*
 *
 * TestWordcut.java
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
public class TestWordcut {
    
    /** Creates a new instance of TestWordcut */
    public TestWordcut() {
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {
        Wordcut wordcut = new Wordcut();
        wordcut.setMode(Integer.parseInt(args[0],10));
        wordcut.setDebug(true);
        if(args[1].equals("f")) {
            MaximumMatcher matcher = new MaximumMatcher();
            matcher.setDebug(true);
            matcher.setAloneGroup(false);
            wordcut.setMatcher(matcher);
        }
        Word[] words = wordcut.cut(args[2]);
        
        System.out.println ("Word count = "+words.length);
        for(int i=0;i<words.length;i++) {
            if (!words[i].isKnown()) System.out.print ("[");
            System.out.print (words[i].getText());
            if (!words[i].isKnown()) System.out.print ("]");
            if (i!=words.length-1) System.out.print ("-");
        }
        System.out.println ();
    }
    
}

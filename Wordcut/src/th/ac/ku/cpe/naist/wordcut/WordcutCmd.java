/*
 * WordcutCmd.java
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
 * WordcutCmd.java
 *
 * Created on 24 สิงหาคม 2545, 15:08 น.
 */

package th.ac.ku.cpe.naist.wordcut;
import java.io.*;

/**
 *
 * @author  vee
 */
public class WordcutCmd {
    
    /** Creates a new instance of WordcutCmd */
    public WordcutCmd() {
    }
    public static void main (String args[]) throws Exception {
        if (args.length == 1 &&  args[0] != null) {
            BufferedReader reader = new BufferedReader(
            new InputStreamReader(System.in));
            Wordcut wordcut = new Wordcut(args[0]);
            String line;

            while ((line=reader.readLine())!=null) {
                Word[] words = wordcut.cut(line);
                for (int i=0;i<words.length;i++) {
                    System.out.print(words[i].getText());
                    if (i!=words.length-1) System.out.print("-");
                }
                System.out.println ();
            }
        } else {
            System.out.println ("Invalid argument!!!");
            System.out.println ("Usage : WordcutCmd <dictionary uri>");
            System.exit(1);
        }
    }
}

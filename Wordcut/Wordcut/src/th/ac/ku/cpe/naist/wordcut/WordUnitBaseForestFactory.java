/*
 * WordUnitBaseForestFactory.java
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
public class WordUnitBaseForestFactory implements ForestFactory {
    double weight=0.5;
    boolean debug = false;
    
    public void setDebug(boolean debug) { this.debug = debug; }
    
    class FollowVowelGroupFinder extends StateMachine {
        FollowVowelGroupFinder () {
            final_state_table[2]=true;
            count = 2;
        }
        
        public void transit(char ch) {
            switch (state) {
                case 0:
                    if (ThaiCharacterType.isFollowVowel(ch)) {
                        state=0;
                    } else {
                        state=1;
                    }
                    break;
                case 1:
                    if (ThaiCharacterType.isFollowVowel(ch)) {
                        state=2;
                        count=2;
                    } else {
                        state=1;
                    }
                    break;
                case 2:
                    if (ThaiCharacterType.isFollowVowel(ch)) {
                        state=0;
                        count=0;
                    } else {
                        state=1;
                        count=1;
                    }
                    break;
                default:
                    break;
            }
        }    
    } // end class
    
    class LeadVowelGroupFinder extends StateMachine {
        LeadVowelGroupFinder () {
            final_state_table[2]=true;
            count = 2;
        }
        public void transit(char ch) {
            switch (state) {
                case 0:
                    if (ThaiCharacterType.isLeadVowel(ch)) {
                        state=1;
                    } else {
                        state=0;
                    }
                    break;
                case 1:
                    if (ThaiCharacterType.isLeadVowel(ch)) {
                        state=1;
                    } else {
                        state=2;
                      
                    }
                    break;
                case 2:
                    if (ThaiCharacterType.isLeadVowel(ch)) {
                        state=1;
                      
                    } else {
                        state=0;
                      
                    }
                    break;
                default:
                    break;
            }
        }
        
    }
    // end class
    
    class KarunGroupFinder extends StateMachine {
        KarunGroupFinder () {
            final_state_table[3]=true;
            final_state_table[5]=true;
            
        }
        public void transit(char ch) {
            switch (state) {
                case 0:
                    state++;
                    break;
                case 1:
                    state++;
                    break;
                case 2:
                    if (ch == '์') {
                        state=3;
                    } else if (ThaiCharacterType.isFollowVowel(ch)) {
                        state=4;
                    } else {
                        state=2;
                    }
                    break;
                case 4:
                    if (ch == '์') {
                        state=5;
                    } else {
                        state=2;
                    }
                    break;
                case 5:
                case 3:
                    state=1;
                    break;
                default:
                    break;
            }
        }
        
        public int getCount() {
            if (state==5) return 4;
            return 3;
        }
        
    }
    
    // end class
    
    class ToneGroupFinder extends StateMachine {
        ToneGroupFinder () {
            final_state_table[2]=true;
            count = 2;
        }
        public void transit(char ch) {
            switch (state) {
                case 0:
                    state=1;
                    break;
                case 1:
                    if (ThaiCharacterType.isTone(ch)) {
                        state=2;
                    } else {
                        state=1;
                    }
                    break;
                case 2:
                    state=1;
                    break;
                default:
                    break;
            }
        }
        
    }
    // end class

    class SandwishGroupFinder extends StateMachine {
        SandwishGroupFinder () {
            final_state_table[3]=true;
            count = 0;
        }
        public void transit(char ch) {
            switch (state) {
                case 0: 
                    if (ThaiCharacterType.isConsonant(ch)) {
                        state = 1;
                        
                    } else {
                        state = 0;
                    }
                    break;
                case 1:
                    if (ch == 'ื' || ch == 'ั') {
                        state = 2;
                        count = 2;
                    } else if (ThaiCharacterType.isConsonant(ch)) {
                        state = 1;
                    } else {
                        state = 0;
                    }
                    break;
                case 2:
                    if (ThaiCharacterType.isConsonant(ch)) {
                        state = 3;
                        count++;
                    } else {
                        state = 2;
                        count++;
                    }
                    break;
                case 3:
                    if (ThaiCharacterType.isConsonant(ch)) {
                        state = 1;
                    } else {
                        state = 0;
                    }
                    break;
                default:
                    break;
            }
        }
        
    }
    // end class
    
    class MaiTaiKuGroupFinder extends StateMachine {
        MaiTaiKuGroupFinder () {
            final_state_table[3]=true;
            count = 3;
        }
        public void transit(char ch) {
            switch (state) {
                case 0: 
                    if (ThaiCharacterType.isConsonant(ch) && ch !='ก') {
                        state = 1;
                    } else {
                        state = 0;
                    }
                    break;
                case 1:
                    if (ch == '็') {
                        state = 2;
                        count = 2;
                    } else if (ThaiCharacterType.isConsonant(ch) && ch !='ก') {
                        state = 1;
                    } else {
                        state = 0;
                    }
                    break;
                case 2:
                    if (ThaiCharacterType.isConsonant(ch)) {
                        state = 3;
                        count++;
                    } else {
                        state = 2;
                        count++;
                    }
                    break;
                case 3:
                    if (ThaiCharacterType.isConsonant(ch) && ch !='ก') {
                        state = 1;
                    } else {
                        state = 0;
                    }
                    break;
                default:
                    break;
            }
        }
        
    }
    // end class
    
    StateMachine state_machines[]=new StateMachine[16];
    int machine_count = 0;
    /** Creates a new instance of WordUnitBaseForestFactory */
    public WordUnitBaseForestFactory() {
        state_machines[machine_count++] = new FollowVowelGroupFinder();
        state_machines[machine_count++] = new LeadVowelGroupFinder();
        state_machines[machine_count++] = new KarunGroupFinder();
        state_machines[machine_count++] = new ToneGroupFinder();
        state_machines[machine_count++] = new SandwishGroupFinder();
        state_machines[machine_count++] = new MaiTaiKuGroupFinder();
    }
    
    public WordcutForest cut(String str) {
        char char_array[] = str.toCharArray();
        int[] connect_table = new int[char_array.length];
        int fill_count=0;
        
        for (int m=0;m<machine_count;m++) {
            for (int i=0;i<char_array.length;i++) {
                state_machines[m].transit(char_array[i]);
                if (state_machines[m].isFinalState()) {
                    if (debug) {
                        if (debug) System.out.print ("m="+m+"\ti="+i+"\tcount="+state_machines[m].getCount());
                        if (debug) System.out.println ("\tstr="+str.substring(i-state_machines[m].getCount()+1,i+1));
                    }
                    int start = i-state_machines[m].getCount()+1;
                    if (debug) {
                        System.out.println ("Start = "+start);
                    }
                    
                    int fill;
                    if (connect_table[start]!=0) {
                        fill = connect_table[start];
                    } else {
                        fill_count++;
                        fill = fill_count;
                    }
                    
                    for (int f=start;f<i/* stop = i+1*/;f++) {
                        connect_table[f]=fill;
                        
                    }
                    
                    if (connect_table[i] != 0) {
                        int t=connect_table[i];
                        for (int f=i;f<connect_table.length && connect_table[f]==t;f++) {
                            connect_table[f] = fill;
                        }
                    } else {
                        connect_table[i] = fill;
                    }
                    if (debug) dumpTable(connect_table);
                }
            }
        }
        /* debug */
        if (debug) dumpTable(connect_table);
        //System.out.println();
        return connectTable2Forest(connect_table);
    }
    
    WordcutForest connectTable2Forest(int[] table) {
        int prev=-1;
        int start=-1;
        int stop=-1;
        WordcutForest forest = new WordcutForest(table.length);
        forest.setDebug(debug);
        for (int i=0;i<table.length;i++) {
            if (prev != table[i]) {
                stop=i;
                if (start != -1) {
                    //add
                    forest.put(start,stop-1,weight);
                }
                start = i;
                prev = table[i];
            }
        }
        return forest;
    }
    void dumpTable(int[] tab) {
        System.out.print ("Dump tab = ");
        for (int i=0;i<tab.length;i++) {
            System.out.print (tab[i]);
        }
        System.out.println ();
    }
    public static void main (String[] args) {
        WordUnitBaseForestFactory word_unit_base_forest_factory =
        new WordUnitBaseForestFactory ();
        word_unit_base_forest_factory.cut("บ้ามื้น");
    }
}

/*
 * MaximumMatcher.java
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
public class MaximumMatcher implements Matcher {
    
    double[] weight_table;
    int[] unknown_count_table;
    int[] token_count_table;
    int[] status_table;
    int[] paths;
    int[] reverse_paths;
    
    boolean alone_group=true;
    boolean debug=false;
    
    final static int MAX_TABLE_SIZE=4096;
    /** Creates a new instance of MaximumMatching */
    
    public void setAloneGroup(boolean alone_group) {
        this.alone_group=alone_group;
    }
    
    public MaximumMatcher() {
        weight_table = new double[MAX_TABLE_SIZE];
        unknown_count_table = new int[MAX_TABLE_SIZE];
        token_count_table = new int[MAX_TABLE_SIZE];
        //merge_count_table = new int[MAX_TABLE_SIZE];
        paths = new int[MAX_TABLE_SIZE];
        reverse_paths = new int[MAX_TABLE_SIZE];
        status_table = new int[MAX_TABLE_SIZE];
    }
    
    public void setDebug(boolean debug) {
        this.debug = debug;
    }
    
    void resetTable(int len) {
        for (int i=0;i<len;i++) {
            unknown_count_table[i] = MAX_TABLE_SIZE;
            token_count_table[i] = MAX_TABLE_SIZE;
            weight_table[i] = 0;
            status_table[i] = Word.KNOWN;
        }
    }
    
    void maximum(WordcutForest forest,int len) {
        for (int i=len-1;i>=0;i--) {
            WordcutEdges edges;
            boolean have_path=false;
            if ((edges=forest.getWordcutEdges(i)) != null) {
                for (int j=0;j<edges.size();j++) {
                    int stop = edges.getStop(j);
                    int unknown_count_tmp = unknown_count_table[stop];
                    int token_count_tmp = token_count_table[stop]+1;
                    // TODO: Calculate weight
                    if (unknown_count_tmp <= unknown_count_table[i] &&
                    token_count_tmp < token_count_table[i])  {
                        unknown_count_table[i] = unknown_count_tmp;
                        token_count_table[i] = token_count_tmp;
                        paths[i] = stop;
                        have_path=true;
                    }
                }
            }
            
            // unknown
            if (!have_path) {
                status_table[i] = Word.UNKNOWN;
                if (i==len-1) { // last element
                    unknown_count_table[i] = 1;
                    token_count_table[i] = 1;
                    paths[i] = len;
                } else {
                    //if (status_table[i+1]==Word.UNKNOWN) {
                    //    unknown_count_table[i] = unknown_count_table[i+1]+1;
                    //    token_count_table[i] = token_count_table[i+1];
                    //    paths[i] = paths[i+1];
                    //} else {
                    unknown_count_table[i] = unknown_count_table[i+1]+1;
                    token_count_table[i] = token_count_table[i+1]+1;
                    paths[i] = i + 1;
                    //}
                }
            }
        }
    }
    
    int findAlone (int[] alone,int len) {
        int n=0;
        int alone_count=0;
        while (n != len) {
            int stop = paths[n];
            reverse_paths[stop] = n;
            if (n+1==stop) {
                alone[alone_count++] = n;
            }
            n=stop;
        }
        return alone_count;
    }
    
    void updatePath(int source,int destination) {
        paths[source] = paths[destination];
        reverse_paths[destination] = reverse_paths[source];
    }
    
    void mergeWithUnknown (int[] alone,int alone_count,int len) {
        if (debug) System.out.println("Merge alone");        
        for (int i=0;i<alone_count;i++) {
            boolean modify=false;
            if (debug) System.out.println("Alone[i] = "+alone[i]);
            if (alone[i] != len-1
            && status_table[alone[i]+1] == Word.UNKNOWN
            && status_table[alone[i]+1] != 'ä') {
                updatePath(alone[i],paths[alone[i]+1]);
                modify = true;
                if (debug) System.out.println("Merge back alone[i]="+alone[i]);
            }
            
            if (alone[i] != 0
            && status_table[reverse_paths[alone[i]]] == Word.UNKNOWN) {
                paths[reverse_paths[alone[i]]] = paths[alone[i]];
                reverse_paths[paths[alone[i]]] = reverse_paths[alone[i]];
                modify = true;
                if (debug) System.out.println("status_table[reverse_paths[alone[i]]]="
                +status_table[reverse_paths[alone[i]]]);
                if (debug) System.out.println("reverse_paths[alone[i]]="+reverse_paths[alone[i]]);
                if (debug) System.out.println("Merge front alone[i]="+alone[i]);
            }
            if (modify) alone[i] = -1;
            if (debug) dumpTable(len);
        }
    }
    
    void mergeOneSide (int[] alone,int alone_count,int len) {
            // merge one way
        
        if (debug) System.out.println("Merge one way");
        for (int i=0;i<alone_count;i++) {
            
            boolean front =false;
            boolean back = false;
            if (debug) System.out.println("Alone = "+alone[i]);
            if (alone[i] != -1) {
                
                if (alone[i] != len-1
                && status_table[alone[i]+1] == Word.UNKNOWN
                && status_table[alone[i]+1] != 'ä') {
                    
                    front = true;
                    if (debug) System.out.println("Oneway front");
                }
                if (alone[i] != 0
                && status_table[reverse_paths[alone[i]]] == Word.UNKNOWN) {
                    
                    back = true;
                    if (debug) System.out.println("Oneway front");
                    
                }
                if (!(back && front)) { // I don't know how to xor
                    if (back) {
                        updatePath(alone[i],paths[alone[i]+1]);
                        //paths[alone[i]] = paths[alone[i]+1];
                        status_table[alone[i]] = Word.MERGED;
                        alone[i] = -1;
                        if (debug) System.out.println("Done one way back");
                    } else {
                        //paths[reverse_paths[alone[i]]] = paths[alone[i]];
                        status_table[reverse_paths[alone[i]]] = Word.MERGED;
                        
                        paths[reverse_paths[alone[i]]] = paths[alone[i]];
                        reverse_paths[paths[alone[i]]] = reverse_paths[alone[i]];
                        
                        alone[i] = -1;
                        if (debug) System.out.println("Done one way front");
                    }
                    if (debug) dumpTable(len);
                }
                
            }
            
        }
    }
    
    void mergeAll (int[] alone,int alone_count,int len) {
                // just merge
        for (int i=0;i<alone_count;i++) {
            if (alone[i] != -1) {
                
                if (alone[i] != len-1
                && status_table[alone[i]+1] == Word.UNKNOWN
                && status_table[alone[i]+1] != 'ä') {
           
                    updatePath(alone[i],paths[alone[i]+1]);    
                    status_table[alone[i]] = Word.MERGED;
                    alone[i] = -1;
                    if (debug) System.out.println("Done two way back");
                    
                }
                if (alone[i] != 0
                && status_table[reverse_paths[alone[i]]] == Word.UNKNOWN) {
                    
                    paths[reverse_paths[alone[i]]] = paths[alone[i]];
                    reverse_paths[paths[alone[i]]] = reverse_paths[alone[i]];
                        
                    status_table[reverse_paths[alone[i]]] = Word.MERGED;
                    alone[i] = -1;
                    if (debug) System.out.println("Done two way front");
                    
                    
                }
            }            
        }
    } 
    
    void aloneMerge(int len) {
        
        if (debug) System.out.println("Find alone");
        int[] alone = new int [len];
        int alone_count=findAlone(alone,len);
        mergeWithUnknown (alone,alone_count,len);
        mergeOneSide (alone,alone_count,len);
        mergeAll (alone,alone_count,len);
    }
    
    Word[] path2Words(String source_string,int len) {
        // path to words
        Word[] temp = new Word[MAX_TABLE_SIZE];
        int word_count=0;
        int n=0;
        while (n != len) {
            int stop = paths[n];
            
            temp[word_count] = new Word(source_string.substring(n,stop),
            status_table[n]);
            word_count++;
            n = stop;
        }
        Word words[]=new Word[word_count];
        System.arraycopy(temp,0,words,0,word_count);
        return words;
    }
    
    void dumpTable(int len) {
        System.out.println("Maximum matcher debug len = "+len);
        System.out.println("\tpath\ttoken\tunknown\tunknown mark");
        for (int i=0;i<len;i++) {
            System.out.println(i+"\t"+paths[i]+"\t"+token_count_table[i]+"\t"+
            unknown_count_table[i]+"\t"+status_table[i]);
        }
    }
    
    public Word[] match(WordcutForest forest, String source_string) {
        int len = forest.length();
        
        resetTable(len);
        maximum(forest,len);
        if (alone_group) aloneMerge(len);
        if (debug) dumpTable(len);
        return path2Words(source_string,len);
    }
}

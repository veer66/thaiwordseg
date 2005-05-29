/*
 * WordSegmentator.java
 *
 * Copyright (c) 2005 Vee Satayamas
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, and/or sell copies of the 
 * Software, and to permit persons to whom the Software is furnished to do so, 
 * provided that the above copyright notice(s) and this permission notice appear
 * in all copies of the Software and that both the above copyright notice(s) and
 * this permission notice appear in supporting documentation.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS NOTICE BE
 * LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR 
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER 
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

package com.pompoko35.vee.text.wordseg;

/**
 *
 * @author vee
 */
public class WordSegmentator {
    
    /** Creates a new instance of WordSegmentator */
    private Trie  root;
    
    private final int NULL      = -1;
    private final int TERMINATE = -2;
    
    public WordSegmentator(Trie trie) {
        root = trie;
    }
    
    private void fillEdges(String s, Graph graph) {        
        for(int i = 0; i < s.length(); i++) {
            Trie t = root;
            boolean flag = true;
            for(int j = i; flag && j < s.length(); j++) {
                t = t.transit(s.charAt(j));
                if (t == null) {
                    
                    flag = false;
                } else {
                    
                    if(t.isBoundary()) {
                        graph.addEdge(i,j);                        
                    }
                }
            }
        }        
    }
    
    
    public void findPath(Graph graph, int len, int[] dist, int[] path) {
        for(int i = 0; i <= len; i++) {
            path[i] = NULL;
        }
        path[len] = TERMINATE;
        for(int i = len-1; i >= 0; i--) {
            System.out.println ("i = " + i);
            BoundArray boundArray = graph.boundArrayAtSource(i);
            if(boundArray != null) {
                for(int j = 0; j < boundArray.count; j++) {
                    int k = boundArray.array[j] + 1;
                    //System.out.println ("j = " + j + " k = " + k + " path[k] = " + path[k] + " dist[k] = " + dist[k]);
                    if(path[k] != NULL) {
                        //System.out.println("!!!@");
                        
                        int d = dist[k] + 1;
                        if(dist[i] > d || path[i] == NULL) {
                            dist[i] = d;
                            path[i] = k;
                        }
                    }
                }
            }
        }
        
        for (int i = 0; i <= len; i++) {
            System.out.println("!!! "+i+" "+dist[i]+" "+path[i]);
        }
        dist[len] = 0;
    }
    
    private String[] pathToArray(String s, int[] path) {
        int count = 0;
        int n;
        
        for(n = path[0]; n != NULL && n != TERMINATE; n = path[n]) {
            count++;
        }
        
        if (count == 0) {
            return null;
        }
        
        String[] array = new String[count];
        int i = 0;
        
        n = 0;
        i = 0;
        
        while (true) {            
            int m = path[n];
            if (m != NULL && m != TERMINATE) {
                array[i] = s.substring(n, m);            
                i++;
                n = m;
            } else {
                break;
            }
        }
        return array;
    }
    
    public String segment(String s, String delim) {
        Graph graph = new Graph(s.length());        
        int[] dist = new int[ s.length()+1 ];
        int[] path  = new int[ s.length()+1 ];
        String[] array;
        
        fillEdges(s , graph);
        findPath(graph, s.length(), dist, path);
        array = pathToArray(s, path);
        
        for(int i = 0; i < array.length; i++) {
            System.out.println(array[i]);
        }
        return null;
    }
    
    public String segment(String s) {
        return segment(s," | ");
    }
    
    
}

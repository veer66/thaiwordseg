/*
 * WordSegmentationGraph.java
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
public class Graph {
    
    Object[] tab;
    int[] count;
    
    final int size = 8;
    
    /** Creates a new instance of WordSegmentationGraph */
    public Graph(int len) {
        tab   = new Object[len];
        count = new int[len];
        for(int i = 0; i < tab.length; i++) {
            tab[i] = null;
        }
    }
    
    public void addEdge(int i, int j) {
        if (tab[i] == null) {
            tab[i] = new int[size];
            count[i] = 0;
        }
        
        int[] tab_i = ((int[])tab[i]);
        if (count[i] == tab_i.length) {
            int[] tmp = new int[tab_i.length * 2];
            for(int k=0; k < tab_i.length; k++) {
                tmp[k] = tab_i[k];
            }
            tab[i] = tmp;
        }
        ((int [])tab[i])[count[i]] = j;
        count[i]++;

    }
    
    public BoundArray boundArrayAtSource(int i) {
        if (tab[i] == null) {
            return null;
        }
        BoundArray boundArray = new BoundArray(((int[])tab[i]), count[i]);
        return boundArray;
    }
    
}

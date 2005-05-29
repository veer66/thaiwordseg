/*
 * TrieNode.java
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

import java.util.SortedMap;
import java.util.Map;
import java.util.Iterator;
import java.util.Arrays;




/**
 *
 * @author vee
 */
public class Trie implements Comparable {
    
    private char        label;
    private Trie[]  children;
    private boolean     boundary;
    private int childCount;
    
//    static final int  ACTIVE   = 1;
//    static final int  BOUNDARY = 2;
//    static final int  FAIL     = 3;
    
    private final int TrieNodeSize = 128;
    
    public Trie(char label) {
        this.label = label;
        children   = new Trie[128];        
        childCount = 0;
        boundary   = false;
    }
    
    public int compareTo(Object o) {
        char oc = ((Trie)o).getLabel();
        if (label > oc) {
            return 1;
        } else if (label < oc) {
            return -1;
        } else {
            return 0;
        }
    }
    
    public char getLabel() {
        return label;
    }
    
    public void addWord(String s, int si) {
        if(si >= s.length()) {
            boundary = true;
        } else {                           
            char c = s.charAt(si);
            boolean found = false;
            for(int i=0; !found && i<childCount; i++) {
                if(children[i].getLabel() == c) {
                    found = true;
                    children[i].addWord(s, si+1);
                }
            }
            if(!found) {                
                if(childCount == children.length) { 
                    Trie[] childrenNew = new Trie[childCount + childCount];
                    for(int i=0;i<childCount;i++) {
                        childrenNew[i] = children[i];
                    }
                    children = childrenNew;
                }
                Trie t = new Trie(c);
                children[childCount] = t;
                t.addWord(s, si + 1);
                childCount++;
            }            
        }
    }
    
    public void sort() {
        Arrays.sort(children,0,childCount);
        for(int i=0;i<0;i++) {
            children[i].sort();
        }
    }
    
    public void dump(int l) {
        char b = 'F';
        if(boundary) {
            b = 'T';
        }
        for(int i = 0; i < childCount; i++) {
            children[i].dump(l + 1);
        }
    }
    
    public void dumpWord() {
        char[] a = new char[1000];
        for(int i = 0; i < childCount; i++) {
            children[i].dumpWord(a,0);
        }
    }
    
    public boolean canCompressSuffix() {
        if(childCount == 0) {
            return true;
        } else if(childCount == 1) {
            return children[0].canCompressSuffix();
        } else {
            return false;
        }
    }
    
    private void dumpWord(char[] a, int a_i) {
        a[a_i] = label;
        if (boundary) {
            String s = new String(a,0,a_i+1);

        }
        for(int i = 0; i < childCount; i++) {
            children[i].dumpWord(a, a_i + 1);
        }
    }
    
    private int searchChild(char c,int l,int r) {
        if (l < r) {
            int m = (l + r)/2;
            char label = children[m].getLabel();
     //       System.out.println("!!!3   "+ c+' '+l+' '+r);
            if (c > label) {
                return searchChild(c, m+1,r);
            } else if(c < label) {
                return searchChild(c, l, m);
            } else {
                return m;
            }
        } else {
            return -1;
        }
    }
    
    public Trie transit(char c) {
        int i = searchChild(c,0,childCount);
        if (i != -1) {
            return children[i];
        } else {
            return null;
        }
    }
    
    
    public boolean isBoundary() {
        return boundary;
    }
    
    
}

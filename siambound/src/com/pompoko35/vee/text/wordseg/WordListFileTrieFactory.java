/*
 * WordListTrieFactory.java
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

import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.util.StringTokenizer;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author vee
 */
public class WordListFileTrieFactory implements TrieFactory {        
    private String filename;
    /** Creates a new instance of WordListTrieFactory */
    
    public WordListFileTrieFactory(String filename) {
        this.filename = filename;
    }
    
    public Trie createTrie() throws TrieCreationException {
        try {
            FileInputStream fis = new FileInputStream(this.filename);
            InputStreamReader isr = new InputStreamReader(fis,"UTF-8");
            BufferedReader br = new BufferedReader(isr);
            String line;

            Trie root = new Trie('@');

            while((line = br.readLine()) != null) { 
                StringTokenizer strtok = new StringTokenizer(line);
                List tokList = new ArrayList();
                while(strtok.hasMoreTokens()) {
                    String tok = (String)strtok.nextToken();
                    tokList.add(tok);
                }
                String word = ((String)tokList.get(0));
                if(word.length() < 20) {
                    root.addWord(word, 0);
                }
            }

            //root.addWord("ABCD", 0);

            root.sort();

            //root.dumpWord();

            br.close();
            isr.close();
            fis.close();
            return root;
        } catch (Exception e) {
            throw new TrieCreationException();
        }               
    }
}
/*
 * WordListFileTrieFactoryTest.java
 * JUnit based test
 *
 * Created on May 29, 2005, 10:51 PM
 */

package com.pompoko35.vee.text.wordseg;

import junit.framework.*;
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
public class WordListFileTrieFactoryTest extends TestCase {
    
    public WordListFileTrieFactoryTest(String testName) {
        super(testName);
    }

    protected void setUp() throws Exception {
    }

    protected void tearDown() throws Exception {
    }

    public static Test suite() {
        TestSuite suite = new TestSuite(WordListFileTrieFactoryTest.class);
        
        return suite;
    }

    /**
     * Test of createTrie method, of class com.pompoko35.vee.text.wordseg.WordListFileTrieFactory.
     */
    public void testCreateTrie() {
        System.out.println("testCreateTrie");
        
        // TODO add your test code below by replacing the default call to fail.
        fail("The test case is empty.");
    }
    
}

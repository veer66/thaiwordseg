/*
 * WordsegSuite.java
 * JUnit based test
 *
 * Created on May 29, 2005, 10:51 PM
 */

package com.pompoko35.vee.text.wordseg;

import junit.framework.*;

/**
 *
 * @author vee
 */
public class WordsegSuite extends TestCase {
    
    public WordsegSuite(String testName) {
        super(testName);
    }

    protected void setUp() throws Exception {
    }

    protected void tearDown() throws Exception {
    }

    /**
     * suite method automatically generated by JUnit module
     */
    public static Test suite() {
        TestSuite suite = new TestSuite("WordsegSuite");
        suite.addTest(com.pompoko35.vee.text.wordseg.BoundArrayTest.suite());
        suite.addTest(com.pompoko35.vee.text.wordseg.GraphTest.suite());
        suite.addTest(com.pompoko35.vee.text.wordseg.TrieTest.suite());
        suite.addTest(com.pompoko35.vee.text.wordseg.TrieFactoryTest.suite());
        suite.addTest(com.pompoko35.vee.text.wordseg.WordListFileTrieFactoryTest.suite());
        suite.addTest(com.pompoko35.vee.text.wordseg.WordSegmentatorTest.suite());
        return suite;
    }
    
}

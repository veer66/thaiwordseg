/*
 * TrieFactoryTest.java
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
public class TrieFactoryTest extends TestCase {
    
    public TrieFactoryTest(String testName) {
        super(testName);
    }

    protected void setUp() throws Exception {
    }

    protected void tearDown() throws Exception {
    }

    public static Test suite() {
        TestSuite suite = new TestSuite(TrieFactoryTest.class);
        
        return suite;
    }

    /**
     * Test of createTrie method, of class com.pompoko35.vee.text.wordseg.TrieFactory.
     */
    public void testCreateTrie() {
        System.out.println("testCreateTrie");
        
        // TODO add your test code below by replacing the default call to fail.
        fail("The test case is empty.");
    }

    /**
     * Generated implementation of abstract class com.pompoko35.vee.text.wordseg.TrieFactory. Please fill dummy bodies of generated methods.
     */
    private class TrieFactoryImpl implements TrieFactory {

        public com.pompoko35.vee.text.wordseg.Trie createTrie() {
            // TODO fill the body in order to provide useful implementation
            
            return null;
        }
    }

    
}

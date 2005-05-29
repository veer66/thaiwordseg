/*
 * BoundArrayTest.java
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
public class BoundArrayTest extends TestCase {
    
    public BoundArrayTest(String testName) {
        super(testName);
    }

    protected void setUp() throws Exception {
    }

    protected void tearDown() throws Exception {
    }

    public static Test suite() {
        TestSuite suite = new TestSuite(BoundArrayTest.class);
        
        return suite;
    }
    
}

/*
 *
 * Wordcut.java
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
/*
 * Class.java
 *
 * Created on 7 สิงหาคม 2545, 8:57 น.
 */

package th.ac.ku.cpe.naist.wordcut;


/**
 *
 * @author  vee
 */
public class Wordcut {
    final static int DICTIONARY_WORDUNIT = 0;
    final static int DICTIONARY_ONLY = 1;
    final static int WORDUNIT_ONLY = 2;
    ForestFactory dictionary_base_forest_factory;
    WordUnitBaseForestFactory wordunit_base_forest_factory;
    boolean debug=false;
    
    int mode=DICTIONARY_WORDUNIT;
    
    
    private Matcher matcher;
    
    public void setMatcher(Matcher matcher) {
        this.matcher = matcher;
    }
    
    public void setMode (int mode) {
        this.mode = mode;
        if (debug) {
            System.out.println ("Mode = "+this.mode);
        }
    }
    
    public Wordcut() throws org.xml.sax.SAXException  , java.io.IOException  ,
    javax.xml.parsers.ParserConfigurationException {
        dictionary_base_forest_factory = new DictionaryBaseForestFactory(
        new Dictionary(new java.io.File("/home/vee/Farm/bookindex/data/bookindex.dict")));
        wordunit_base_forest_factory = new WordUnitBaseForestFactory();
        //wordunit_base_forest_factory.setDebug(true);
        MaximumMatcher maximum_matcher = new MaximumMatcher();
        //maximum_matcher.setDebug(true);
        matcher = maximum_matcher;
    }

    public Wordcut(String uri) throws org.xml.sax.SAXException  , java.io.IOException  ,
    javax.xml.parsers.ParserConfigurationException {
        dictionary_base_forest_factory = new DictionaryBaseForestFactory(
        new Dictionary(uri));
        wordunit_base_forest_factory = new WordUnitBaseForestFactory();
        //wordunit_base_forest_factory.setDebug(true);
        MaximumMatcher maximum_matcher = new MaximumMatcher();
        //maximum_matcher.setDebug(true);
        matcher = maximum_matcher;
    }
    
    public Word[] cut(String str) throws WordcutForestException {
        char[] source = str.toCharArray();
        Word[] answers = new Word[1024];
        int answer_count=0;
        int i=0;
        int last = 0;
        do {
            for (;i<source.length && (source[i] == '\t' || source[i] == '\r' || 
            source[i] == '\n' || source[i] == ' ');i++) {
                answers[answer_count]=new Word(str.substring(i,i+1),Word.DELIMITER);
                answer_count++;
            }
            
            last=i;
            while (i<source.length && !(source[i] == '\t' || source[i] == '\r' || 
            source[i] == '\n' || source[i] == ' ')) {
                i++;
            }
            if (i>last) {
                Word[] answer_buffer=cutNoPunc(str.substring(last,i));
                System.arraycopy(answer_buffer,0,answers,answer_count,answer_buffer.length);
                answer_count = answer_count + answer_buffer.length;
            }
            last = i;
        } while (i!=source.length);
        Word[] tmp=new Word[answer_count];
        System.arraycopy(answers,0,tmp,0,answer_count);
        return tmp;
    }
    
    public void setDebug(boolean debug) {
        this.debug = debug;
    }
    
    public String[] showAllPaths(String str) {
        WordcutForest forest = dictionary_base_forest_factory.cut(str);
        
        AllPath all_path = new AllPath(forest,str);
        all_path.travel();
        return all_path.getAnswers();
    }
    
    class AllPath {
        final static int MAX_TOKEN = 1024;
        WordcutForest forest;
        String answer[] = new String[MAX_TOKEN];
        int answer_count=0;
        String str;
        java.util.List answer_list = new java.util.LinkedList();
        
        AllPath(WordcutForest forest,String str) {
            this.forest = forest;
            this.str = str;
            
        }
        
        public void travel() {
            travel(0);
        }
        
        public String[] getAnswers() {
            return (String[])answer_list.toArray(new String[0]);
        }
        
        private boolean travel(int n) {
            if (debug) System.out.println("Travel n="+n);
            
            if (n>=forest.length()) {
                StringBuffer answer_buffer=new StringBuffer();
                for (int i=0;i<answer_count;i++) {
                    if (debug) {
                        System.out.print(answer[i]);
                        if (i!=answer_count-1) System.out.print("-");
                        
                    }
                    
                    answer_buffer.append(answer[i]);
                    if (i!=answer_count-1) answer_buffer.append("-");
                    
                }
                answer_list.add(answer_buffer.toString());
              
                return true;
            } else {
                boolean ret=false;
                /*
                int m = n;
                while (m< forest.length() && forest.getWordcutEdges(m) == null) {
                    m++;
                }
                if (m!=n) {
                    // danger(maybe)
                    answer[answer_count] = new String(str.substring(n,m));
                    answer_count++;
                    if (debug) {
                        System.out.println("Count = "+answer_count);
                        System.out.println("Ans = "+answer[answer_count]);
                    }
                    if (debug) System.out.println("Allpath n="+n+"\tm="+m);
                    n=m;
                    
                }*/
                if (n < forest.length()) {
                    WordcutEdges edges = forest.getWordcutEdges(n);
                    if (edges != null) {
                        for(int i=0;i<edges.size();i++) {
                            int stop=edges.getStop(i);
                            answer[answer_count] = new String(str.substring(n,stop));
                            if (debug) {
                                System.out.println("Count = "+answer_count);
                                System.out.println("Ans = "+answer[answer_count]);
                                System.out.println("Allpath n="+n+"\tm="+stop);
                            }
                            int prev_count=answer_count;
                            answer_count++;
                            if(travel(stop)) ret = true;
                            answer_count = prev_count;
                        }
                    }
                }
                return ret;
            }
        }
    }
    
    public Word[] cutNoPunc(String str) throws WordcutForestException {
        WordcutForest forest;
        
        switch (mode) {
            case DICTIONARY_WORDUNIT: 
                forest=dictionary_base_forest_factory.cut(str);
                forest.merge(wordunit_base_forest_factory.cut(str));
                if (debug) System.out.println ("DICTIONARY_WORDUNIT");
                break;
            case DICTIONARY_ONLY:
                forest = dictionary_base_forest_factory.cut(str);
                if (debug) System.out.println ("DICTIONARY_ONLY");
                break;
            case WORDUNIT_ONLY:
                if (debug) System.out.println ("WORDUNIT_ONLY");
                forest = wordunit_base_forest_factory.cut(str);
                break;
            default:
                if (debug) System.out.println ("DEFAULT");
                forest=dictionary_base_forest_factory.cut(str);
                forest.merge(wordunit_base_forest_factory.cut(str));
                break;
        }
        
        return matcher.match(forest,str);
    }
    
    public static String WordArrayToString(Word[] words,String delimiter) {
        final int STRBUF_SIZE = 1024;
        StringBuffer strbuf = new StringBuffer(STRBUF_SIZE);
        for (int i=0;i<words.length;i++) {
            strbuf.append(words[i]);
            if (words.length-1 == i) strbuf.append(delimiter);
        }
        return strbuf.toString();
    }
}

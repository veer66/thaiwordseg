/*
 * WordcutEdges.java
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
 * WordcutPath.java
 *
 * Created on 16 สิงหาคม 2545, 23:31 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class WordcutEdges {
    int path[];
    double weights[];
    int size=0;
    /** Creates a new instance of WordcutPath */
    public WordcutEdges(int len) {
        path = new int[len];
        weights = new double[len];
    }
    public void put (int stop) {
        path[size] = stop+1;
        size++;
    }
    public void put(int stop,double weight) {
        path[size] = stop+1;
        weights[size] = weight;
        size++;
        
    }
    
    public int getStop(int index) {
        return path[index];
    }
    
    public double getWeight(int index) {
        return weights[index];
    }
    
    public int size() { return size; }
    
    public void merge(WordcutEdges edges) {
        for (int i=0;i<edges.size();i++) {
            int stop = edges.getStop(i);
            put (edges.getStop(i)-1,edges.getWeight(i));
        }
    }
}

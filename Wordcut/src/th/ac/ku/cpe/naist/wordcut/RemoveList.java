/*
 *
 * RemoveList.java
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
 * RemoveList.java
 *
 * Created on 16 สิงหาคม 2545, 19:21 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class RemoveList {
    int size;
    int[] remove_list;
    /** Creates a new instance of RemoveList */
    public RemoveList(int size) {
        remove_list = new int [size];
        this.size=0;
    }
    
    public void clear() {
        size=0;
    }
    
    public void add(int index) {
        remove_list[size++] = index;
    }
    
    public int [] getArray() { 
        return remove_list;
    }
    
    public int size() {
        return size;
    }
}

/*
 *
 * StateMachine.java
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

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */

public abstract class StateMachine { // NFA
    static final int MAX_STATE=16;
    int offset[] = new int[MAX_STATE];
    int count;
    int state;
    boolean final_state_table[] = new boolean[MAX_STATE];
    
    /** Creates a new instance of StateMachine */
    
    abstract public void transit(char ch);
    public boolean isFinalState() { return final_state_table[state]; }
    public int getCount() { return count; }
}

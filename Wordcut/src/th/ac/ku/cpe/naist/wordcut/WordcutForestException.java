/*
 *
 * WordcutForestException.java
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
 * WordcutForestException.java
 *
 * Created on 21 สิงหาคม 2545, 0:13 น.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class WordcutForestException extends java.lang.Exception {
    
    /**
     * Creates a new instance of <code>WordcutForestException</code> without detail message.
     */
    public WordcutForestException() {
    }
    
    
    /**
     * Constructs an instance of <code>WordcutForestException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public WordcutForestException(String msg) {
        super(msg);
    }
}

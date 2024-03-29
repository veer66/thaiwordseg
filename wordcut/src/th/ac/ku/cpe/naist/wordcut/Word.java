/*
 * Word.java
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
class Word 
{
    final static int KNOWN=0;
    final static int UNKNOWN=1;
    final static int MERGED=2;
    final static int DELIMITER=3;
    String text=null;
    Pos[] pos_array=null;
    int status=KNOWN;
    

    Word (String text,Pos[] pos_array) {
        this.text = text;
        this.pos_array = pos_array;
    }
    
    Word (String text,int status) {
        this.text = text;
        this.status = status;
    }

    public void setText (String text) {
        this.text = text;
    }
    public void setPoses(Pos[] pos_array) {
        this.pos_array = pos_array;
    }

    public String getText() {
        return text;
    }

    public Pos[] getPos() {
        return pos_array;
    }
    
    public boolean isKnown () {
        return (status==KNOWN);
    }
}

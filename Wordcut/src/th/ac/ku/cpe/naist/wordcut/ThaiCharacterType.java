/*
 * ThaiCharacterType.java
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
 * ThaiCharacterType.java
 *
 * Created on 20 �ԧ�Ҥ� 2545, 6:23 �.
 */

package th.ac.ku.cpe.naist.wordcut;

/**
 *
 * @author  vee
 */
public class ThaiCharacterType {
    public static boolean isMiddleFollowVowel(char ch) {
        if (ch =='�' || ch=='�'|| ch== '�' || ch== '�') {
            return true;
        } else {
            return false;
        }
    }
    
    public static boolean isLeadVowel(char ch) {
        return  (ch=='�' || ch=='�' || ch=='�' || ch=='�' || ch=='�' );
    }
    
    public static boolean isFollowVowel(char ch) {
        return  (ch =='�' || ch=='�'|| ch== '�' || ch== '�'
        || ch == '�' | ch =='�' || ch=='�' || ch == '�' ||
        ch == '�'|| ch=='�');
    }
    
    public static boolean isConsonant(char ch) {
        return (ch>='�' && ch <='�');
    }
    
    public static boolean isTone(char ch) {
        return (ch=='�' || ch=='�' || ch == '�' || ch == '�');
    }
    
    /** Creates a new instance of ThaiCharacterType */
    public ThaiCharacterType() {
    }
    
}

/*
 * CS 106A Melody Maker
 *
 * This instructor-provided file represents a musical accidental:
 * sharp, flat, or natural.
 *
 * @author Marty Stepp
 * @version Fri 2015/05/08
 * - version for 15sp; folded into spl.jar
 * @version Tue 2014/05/25
 * - initial version for 14sp
 * 
 * Your program should work properly with an UNMODIFIED version of this file.
 * If you want to modify this file for testing or for fun, that is your choice,
 * but when we grade your program we will do so with the original unmodified
 * version of this file, so your code must still work properly with that code.
 *
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */

package stanford.cs106.audio;

/**
 * An Accidental represents a musical accidental: sharp, flat, or natural.
 */
public enum Accidental {	
	SHARP, NATURAL, FLAT;

	/**
	 * Returns the Accidental that is equivalent to the given string,
	 * such as Accidental.SHARP for "SHARP", or null if the string does not
	 * match any Accidental value.
	 */
	public static Accidental getValueOf(String s) {
		s = s.intern();
		if (s == "SHARP") {
			return Accidental.SHARP;
		}
		if (s == "FLAT") {
			return Accidental.FLAT;
		}
		if (s == "NATURAL") {
			return Accidental.NATURAL;
		}
		return null;
	}
}

/*
 * CS 106A Melody Maker
 *
 * This instructor-provided file represents a musical pitch from A-G or rest.
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
 * A Pitch represents a musical pitch. R represents a rest, no pitch.
 */
public enum Pitch {
	A, B, C, D, E, F, G, R;
	
	/**
	 * Returns the Pitch that is equivalent to the given string,
	 * such as Pitch.D for "D", or null if the string does not
	 * match any Pitch value.
	 */
	public static Pitch getValueOf(String s) {
		s = s.intern();
		if (s == "A") {
			return Pitch.A;
		}
		if (s == "B") {
			return Pitch.B;
		}
		if (s == "C") {
			return Pitch.C;
		}
		if (s == "D") {
			return Pitch.D;
		}
		if (s == "E") {
			return Pitch.E;
		}
		if (s == "F") {
			return Pitch.F;
		}
		if (s == "G") {
			return Pitch.G;
		}
		if (s == "R") {
			return Pitch.R;
		}
		return null;
	}
}

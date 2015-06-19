/*
 * @author Marty Stepp
 * @version 2015/05/28
 */

package stanford.cs106.collections;

import java.util.*;

public class Lexicon {
	private Set<String> words;
	private Set<String> knownPrefixes;
	
	public Lexicon() {
		words = new HashSet<String>();
		knownPrefixes = new HashSet<String>();
		knownPrefixes.add("");
		for (char c = 'A'; c <= 'Z'; c++) {
			knownPrefixes.add(String.valueOf(c));
		}
	}
	
	public void add(String word) {
		word = word.toUpperCase();
		words.add(word);
		for (int i = 2; i <= word.length(); i++) {
			knownPrefixes.add(word.substring(0, i));
		}
	}
	
	public boolean contains(String word) {
		return words.contains(word.toUpperCase());
	}
	
	public boolean containsPrefix(String prefix) {
		return knownPrefixes.contains(prefix.toUpperCase());
	}
	
	public int size() {
		return words.size();
	}
	
	public int prefixCount() {
		return knownPrefixes.size();
	}
}

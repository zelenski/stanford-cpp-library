/*
 * @author Marty Stepp
 * @version 2016/11/03
 * - added isEmpty, toString, equals, hashCode
 * @version 2016/07/13
 * - added iterator, implementing Iterable<String> for for-each looping
 * - added constructors that take an input source
 * - added addWordsFrom(...) methods
 * 
 * Implementation notes: doesn't totally work. Has all of A-Z as prefixes,
 * regardless of contents, and doesn't support removal.
 * Implemented naively using just a set of strings.
 */

package stanford.cs106.collections;

import java.io.*;
import java.util.*;
import stanford.cs106.io.IORuntimeException;

public class Lexicon implements Iterable<String> {
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
	
	public Lexicon(String filename) {
		addWordsFromFile(filename);
	}
	
	public void add(String word) {
		word = word.toUpperCase();
		words.add(word);
		for (int i = 2; i <= word.length(); i++) {
			knownPrefixes.add(word.substring(0, i));
		}
	}
	
	public void addWordsFrom(InputStream input) {
		Scanner scan = new Scanner(input);
		addWordsFrom(scan);
	}
	
	public void addWordsFrom(Reader reader) {
		Scanner scan = new Scanner(reader);
		addWordsFrom(scan);
	}
	
	public void addWordsFrom(Scanner input) {
		while (input.hasNextLine()) {
			String line = input.nextLine().trim();
			if (!line.isEmpty()) {
				add(line);
			}
		}
	}
	
	public void addWordsFromFile(File file) {
		try {
			Scanner input = new Scanner(file);
			addWordsFrom(input);
		} catch (FileNotFoundException fnfe) {
			throw new IORuntimeException(fnfe);
		}
	}
	
	public void addWordsFromFile(String filename) {
		addWordsFromFile(new File(filename));
	}
	
	public boolean contains(String word) {
		return words.contains(word.toUpperCase());
	}
	
	public boolean containsPrefix(String prefix) {
		return knownPrefixes.contains(prefix.toUpperCase());
	}
	
	public boolean equals(Object o) {
		if (o instanceof Lexicon) {
			Lexicon lex = (Lexicon) o;
			return lex.words.equals(this.words);
		} else {
			return false;
		}
	}
	
	public int hashCode() {
		return words.hashCode();
	}

	public boolean isEmpty() {
		return size() == 0;
	}
	
	public Iterator<String> iterator() {
		return Collections.unmodifiableSet(words).iterator();
	}
	
	public int prefixCount() {
		return knownPrefixes.size();
	}
	
	public int size() {
		return words.size();
	}
	
	public String toString() {
		return words.toString();
	}
}

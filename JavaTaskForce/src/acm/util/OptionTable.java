/*
 * @(#)OptionTable.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

// REVISION HISTORY
//
// -- V2.0 --
// Code cleanup 28-May-07 (ESR)
//   1. Eliminated relationship to HashTable superclass.
//   2. Added generic type tags.

package acm.util;

import java.io.*;
import java.util.*;

/* Class: OptionTable */
/**
 * This class implements a simple tool for parsing key/value pairs from
 * a string.
 */
public class OptionTable {

/* Constructor: OptionTable(str) */
/**
 * Creates a new <code>OptionTable</code> and initializes it
 * from the specified string.  The options in <code>str</code>
 * are in two possible forms:
 *
 * <ol>
 * <li>key
 * <li>key=value
 * </ol>
 *
 * The first option sets the value associated with the key to
 * the empty string; the second supplies the value explicitly.
 *
 * @usage OptionTable options = new OptionTable(str);
 * @param str The option string that is parsed to initialize the table
 */
	public OptionTable(String str) {
		this(str, null);
	}

/* Constructor: OptionTable(str, keys) */
/**
 * Creates a new <code>OptionTable</code> from the specified string,
 * checking to make sure that all keys exist in the string array
 * <code>keys</code>.  If <code>keys</code> is <code>null</code>,
 * checking is disabled.
 *
 * @usage OptionTable options = new OptionTable(str, keys);
 * @param str The option string that is parsed to initialize the table
 * @param keys An array of strings indicating the legal keys
 */
	public OptionTable(String str, String[] keys) {
		optionTable = new HashMap<String,String>();
		try {
			StreamTokenizer tokenizer = createTokenizer(str);
			int ttype = tokenizer.nextToken();
			while (ttype != StreamTokenizer.TT_EOF) {
				if (ttype != StreamTokenizer.TT_WORD) {
					throw new ErrorException("Illegal option string: " + str);
				}
				String key = tokenizer.sval;
				if (keys != null && !keyExists(key, keys)) {
					throw new ErrorException("Unrecognized option: " + key);
				}
				ttype = tokenizer.nextToken();
				if (ttype == '=') {
					ttype = tokenizer.nextToken();
					if (ttype != StreamTokenizer.TT_WORD && ttype != '"' && ttype != '\'') {
						throw new ErrorException("Illegal option string: " + str);
					}
					optionTable.put(key, tokenizer.sval);
					ttype = tokenizer.nextToken();
				} else {
					optionTable.put(key, "");
				}
			}
		} catch (IOException ex) {
			throw new ErrorException("Illegal option string: " + str);
		}
	}

/* Constructor: OptionTable(map) */
/**
 * Creates a new <code>OptionTable</code> from an existing map.
 * Most clients will not need to use this method.
 *
 * @usage OptionTable options = new OptionTable(map);
 * @param map An existing key/value mapping
 */
	public OptionTable(Map<String,String> map) {
		optionTable = new HashMap<String,String>();
		for (Iterator<String> i = map.keySet().iterator(); i.hasNext(); ) {
			String key = i.next();
			String value = map.get(key);
			optionTable.put(key, value);
		}
	}

/* Method: isSpecified(key) */
/**
 * Returns true if the key has been specified in the option table.
 *
 * @usage if (options.isSpecified(key)) . . .
 * @param key The key being checked
 * @return <code>true</code> if <code>key</code> was specified in the option string
 */
	public boolean isSpecified(String key) {
		return optionTable.containsKey(key);
	}

/* Method: getOption(key) */
/**
 * Returns the value associated with <code>key</code> in the option
 * table, or <code>null</code> if no such value exists.
 *
 * @usage String value = options.getOption(key);
 * @param key The key
 * @return The corresponding option value
 */
	public String getOption(String key) {
		return getOption(key, null);
	}

/* Method: getOption(key, defValue) */
/**
 * Returns the value associated with <code>key</code> in the option
 * table or the specified default value if no such binding exists.
 *
 * @usage String value = options.getOption(key, defValue);
 * @param key The key
 * @param defValue The default to use if the key is not found
 * @return The corresponding option value
 */
	public String getOption(String key, String defValue) {
		String value = optionTable.get(key);
		return (value == null || value.equals("")) ? defValue : value;
	}

/* Method: getIntOption(key) */
/**
 * Returns the integer value associated with <code>key</code> in the option
 * table, or 0 if no such value exists.
 *
 * @usage int value = options.getIntOption(key);
 * @param key The key
 * @return The corresponding option value parsed as an integer
 */
	public int getIntOption(String key) {
		return getIntOption(key, 0);
	}

/* Method: getIntOption(key, defValue) */
/**
 * Returns the integer value associated with <code>key</code> in the option
 * table or the specified default value if no such binding exists.
 *
 * @usage int value = options.getIntOption(key, defValue);
 * @param key The key
 * @param defValue The default to use if the key is not found
 * @return The corresponding option value
 */
	public int getIntOption(String key, int defValue) {
		String binding = getOption(key, null);
		if (binding == null || binding.equals("")) return defValue;
		return Integer.decode(binding).intValue();
	}

/* Method: getDoubleOption(key) */
/**
 * Returns the <code>double</code> value associated with <code>key</code>
 * in the option table, or 0.0 if no such value exists.
 *
 * @usage double value = options.getDoubleOption(key);
 * @param key The key
 * @return The corresponding option value parsed as a <code>double</code>
 */
	public double getDoubleOption(String key) {
		return getDoubleOption(key, 0.0);
	}

/* Method: getDoubleOption(key, defValue) */
/**
 * Returns the <code>double</code> value associated with <code>key</code>
 * in the option table or the specified default value if no such binding
 * exists.
 *
 * @usage double value = options.getDoubleOption(key, defValue);
 * @param key The key
 * @param defValue The default to use if the key is not found
 * @return The corresponding option value
 */
	public double getDoubleOption(String key, double defValue) {
		String binding = getOption(key, null);
		if (binding == null || binding.equals("")) return defValue;
		return Double.valueOf(binding).doubleValue();
	}

/* Method: getMap() */
/**
 * Returns the <code>HashMap</code> used to associate keys and options.
 *
 * @usage HashMap<String,String> map = options.getMap();
 * @return The <code>HashMap</code> used to associate keys and options
 * @noshow
 */
	public HashMap<String,String> getMap() {
		return optionTable;
	}

/* Private method: createTokenizer(str) */
/**
 * Creates a tokenizer for the specified string.
 */
	private StreamTokenizer createTokenizer(String str) {
		StreamTokenizer t = new StreamTokenizer(new StringReader(str));
		t.resetSyntax();
		t.wordChars((char) 33, (char) ('=' - 1));
		t.wordChars((char) ('=' + 1), (char) 126);
		t.quoteChar('"');
		t.quoteChar('\'');
		t.whitespaceChars(' ', ' ');
		t.whitespaceChars('\t', '\t');
		return t;
	}

/* Private method: keyExists(key, keys) */
/**
 * Checks to see whether the key exists in the array of keys.
 */
	private boolean keyExists(String key, String[] keys) {
		for (int i = 0; i < keys.length; i++) {
			if (key.equals(keys[i])) return true;
		}
		return false;
	}

/* Private instance variables */
	private HashMap<String,String> optionTable;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}

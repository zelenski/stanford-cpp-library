package stanford.karel;

import java.util.HashMap;
import java.util.StringTokenizer;

class OptionTable {
	/* Constants used in the parseOptions finite-state machine */
	private static final int INITIAL_STATE = 0;
	private static final int KEY_SEEN = 1;
	private static final int COLON_SEEN = 2;
	private static final int VALUE_SEEN = 3;

	private HashMap<String, String> table;

	/*
	 * This method creates an empty option table and initializes it from the
	 * specified string, if any.
	 */
	public OptionTable() {
		table = new HashMap<String, String>();
	}

	public OptionTable(String options) {
		this();
		parseOptions(options);
	}

	/*
	 * This method parses a string consisting of option specifications in one of
	 * the two following forms:
	 * 
	 * /key /key:value
	 */
	public void parseOptions(String options) {
		parseOptions(options, table);
	}

	/*
	 * This method returns true if the key has been specified in the option
	 * table.
	 */
	public boolean isSpecified(String key) {
		return table.containsKey(key);
	}

	/*
	 * This method looks up an option key and returns the corresponding value.
	 * If the key is not defined, getOption returns null unless a defValue
	 * parameter is used to specify a different default.
	 */
	public String getOption(String key) {
		return getOption(key, null);
	}

	public String getOption(String key, String defValue) {
		String value = table.get(key.toLowerCase());
		return (value == null || value.equals("")) ? defValue : value;
	}

	/*
	 * This method looks up an option key and returns the corresponding value,
	 * parsed as an integer. If the key is not defined, the getIntOption method
	 * returns 0 unless a defValue parameter is used to specify a different
	 * default.
	 */
	public int getIntOption(String key) {
		return getIntOption(key, 0);
	}

	public int getIntOption(String key, int defValue) {
		String value = getOption(key, null);
		if (value == null || value.equals(""))
			return defValue;
		return (Integer.decode(value).intValue());
	}

	/*
	 * This method looks up an option key and returns the corresponding value,
	 * parsed as a double. If the key is not defined, the getDoubleOption method
	 * returns 0.0 unless a defValue parameter is used to specify a different
	 * default.
	 */
	public double getDoubleOption(String key) {
		return getDoubleOption(key, 0.0);
	}

	public double getDoubleOption(String key, double defValue) {
		String value = getOption(key, null);
		if (value == null || value.equals(""))
			return defValue;
		return (Double.valueOf(value).doubleValue());
	}

	/*
	 * This method looks up an option key and returns a boolean whose value is
	 * true if the option maps to "true", "t", or "on" and false if it maps to
	 * "false", "f", or "off". A missing option defaults to false unless the
	 * option is specified and empty, in which case it defaults to true.
	 */
	public boolean getFlagOption(String key) {
		return getFlagOption(key, false);
	}

	public boolean getFlagOption(String key, boolean def) {
		String value = table.get(key.toLowerCase());
		if (value == null)
			return def;
		value = value.toLowerCase();
		if (value.equals(""))
			return true;
		if (value.equals("true"))
			return true;
		if (value.equals("false"))
			return false;
		if (value.equals("t"))
			return true;
		if (value.equals("f"))
			return false;
		if (value.equals("on"))
			return true;
		if (value.equals("off"))
			return false;
		throw new IllegalArgumentException("parseOptions: Illegal flag value");
	}

	/* Private methods */

	/*
	 * This method parses a string consisting of option specifications in one of
	 * the two following forms:
	 * 
	 * /key /key:value
	 */
	private static void parseOptions(String options, HashMap<String, String> map) {
		StringTokenizer scanner = new StringTokenizer(options + "/", "/:", true);
		String key = null;
		String value = null;
		int state = INITIAL_STATE;
		while (scanner.hasMoreTokens()) {
			String token = scanner.nextToken();
			switch (state) {
			case INITIAL_STATE:
				if (!token.equals("/")) {
					key = token.toLowerCase();
					value = "";
					state = KEY_SEEN;
				}
				break;
			case KEY_SEEN:
				if (token.equals("/")) {
					map.put(key, value);
					state = INITIAL_STATE;
				} else if (token.equals(":")) {
					state = COLON_SEEN;
				}
				break;
			case COLON_SEEN:
				value = token;
				state = VALUE_SEEN;
				break;
			case VALUE_SEEN:
				if (token.equals("/")) {
					map.put(key, value);
					state = INITIAL_STATE;
				} else {
					String msg = "parseOptions: Illegal option string";
					throw new IllegalArgumentException(msg);
				}
				break;
			}
		}
	}
}

package stanford.cs106.util;

public final class SystemProperties {
	private SystemProperties() {
		// empty
	}
	
	public static boolean hasSystemProperty(String name) {
		String prop = getSystemProperty(name);
		return prop != null && !prop.isEmpty();
	}

	public static String getSystemProperty(String name) {
		try {
			return System.getProperty(name);
		} catch (SecurityException se) {
			return "";
		}
	}

	public static int getSystemPropertyInt(String name) {
		return Integer.parseInt(getSystemProperty(name));
	}

	public static int getSystemPropertyInt(String name, int defaultValue) {
		if (hasSystemProperty(name)) {
			return getSystemPropertyInt(name);
		} else {
			return defaultValue;
		}
	}

	public static double getSystemPropertyDouble(String name) {
		return Double.parseDouble(getSystemProperty(name));
	}

	public static double getSystemPropertyDouble(String name, double defaultValue) {
		if (hasSystemProperty(name)) {
			return getSystemPropertyDouble(name);
		} else {
			return defaultValue;
		}
	}

	public static boolean getSystemPropertyBoolean(String name) {
		return getSystemPropertyBoolean(name, false);
	}

	public static boolean getSystemPropertyBoolean(String name, boolean defaultValue) {
		if (hasSystemProperty(name)) {
			String prop = String.valueOf(getSystemProperty(name)).toLowerCase().intern();
			return prop != null && (prop == "true" || prop == "t" || prop == "1"
					|| prop == "on" || prop == "yes" || prop == "y");
		} else {
			return defaultValue;
		}
	}

	public static void setSystemProperty(String name, String value) {
		try {
			System.setProperty(name, value);
		} catch (SecurityException se) {
			// empty
		}
	}
}

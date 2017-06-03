/*
 * @author Marty Stepp
 * @version 2017/06/03
 * - If you update this file, make sure to update the @version tag above
 *   AND the String constant below! Both are needed and must be in sync. 
 * - see also: stanford/spl/LibraryUpdater.java 
 */

package stanford.spl;

public class Version {
	private static final String LIBRARY_DOCS_URL = "http://stanford.edu/~stepp/cppdoc/";
	private static final String JAVA_BACK_END_FILENAME = "spl.jar";
	private static final String JAVA_BACK_END_VERSION = "2017/06/03";
	private static final String CPP_LIB_VERSION_UNKNOWN = "(unknown)";
	private static String CPP_LIB_VERSION = CPP_LIB_VERSION_UNKNOWN;
	public static final String ABOUT_MESSAGE = "";

	/**
	 * Returns an about message describing the library and its version.
	 */
	public static String getAboutMessage() {
		String about = "Stanford Java Library (" + JAVA_BACK_END_FILENAME + ") version: " + getLibraryVersion() + "\n"
				+ (CPP_LIB_VERSION == CPP_LIB_VERSION_UNKNOWN ? "" : ("Stanford C++ Library version: " + getCppLibraryVersion() + "\n"))
				+ "\n"
				+ "Java JDK/JRE version: " + getJdkVersion() + "\n\n"
				+ "Libraries originally written by Eric Roberts,\n"
				+ "with assistance from Julie Zelenski, Keith Schwarz, et al.\n"
				+ "This version of the library is unofficially maintained by Marty Stepp.\n"
				+ "\n"
				+ "See " + LIBRARY_DOCS_URL + " for more information.";
		return about;
	}
	
	/**
	 * The full file path to spl.jar, or "" if unable to determine the path.
	 * Might be not found if the lib has been unpacked and repackaged such as is done
	 * with assignment solution demo JARs.
	 */
	public static final String getLibraryJarPath() {
		String sep = System.getProperty("path.separator");   // ":" or ";"
		String classpath = System.getProperty("java.class.path");
		for (String entry : classpath.split(sep)) {
			if (entry.endsWith(JAVA_BACK_END_FILENAME)) {
				return entry;
			}
		}
		return "";
	}
	
	/**
	 * Returns the current version of the JDK as a string such as "1.8.0_74".
	 */
	public static final String getJdkVersion() {
		String[] propNames = {"java.version", "java.runtime.version"};
		for (String propName : propNames) {
			String propValue = System.getProperty(propName);
			if (propValue != null && !propValue.isEmpty()) {
				return propValue;
			}
		}
		return "unknown";
	}
	
	/**
	 * Returns the current version of the Java library as a string such as "2017/04/26".
	 */
	public static final String getLibraryVersion() {
		return JAVA_BACK_END_VERSION;
	}
	
	/**
	 * Returns the current version of the C++ library as a string such as "2017/04/26".
	 */
	public static final String getCppLibraryVersion() {
		return CPP_LIB_VERSION;
	}
	
	/**
	 * Returns true if the current app is running as a C++ project using the Java lib
	 * as its back-end.
	 */
	public static final boolean isCppProject() {
		return !isJavaProject();
	}
	
	/**
	 * Returns true if the current app is running as a Java project using the SPL lib.
	 */
	public static final boolean isJavaProject() {
		return CPP_LIB_VERSION_UNKNOWN == CPP_LIB_VERSION;
	}
	
	/**
	 * Throws an ErrorException if the library version is not at least as new as the given version.
	 */
	public static void requireVersion(String minimumVersion) {
		String version = getLibraryVersion();
		if (version.compareTo(minimumVersion) < 0) {
			throw new acm.util.ErrorException("Stanford Java Library version " + minimumVersion
					+ " required, but found version " + version);
		}
	}
	
	/**
	 * Sets the version of the C++ libraries.
	 * Called from the back-end when C++ project loads up.
	 */
	public static final void setCppLibraryVersion(String version) {
		CPP_LIB_VERSION = version;
	}
	
	/**
	 * Prints the current library version to the console.
	 */
	public static void main(String[] args) {
		System.out.println(getLibraryVersion());
	}
}

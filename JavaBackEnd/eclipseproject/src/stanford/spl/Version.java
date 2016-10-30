/*
 * @author Marty Stepp
 * @version 2016/10/30
 * - If you update this file, make sure to update the @version tag above
 *   AND the String constant below! Both are needed and must be in sync. 
 * - see also: stanford/spl/LibraryUpdater.java 
 */

package stanford.spl;

public class Version {
	private static final String JAVA_BACK_END_VERSION = "2016/10/30";
	private static final String CPP_LIB_VERSION_UNKNOWN = "(unknown)";
	private static String CPP_LIB_VERSION = CPP_LIB_VERSION_UNKNOWN;
	public static final String ABOUT_MESSAGE = "";
	
	public static String getAboutMessage() {
		String about = "Stanford Java Library (spl.jar) version: " + getLibraryVersion() + "\n"
				+ (CPP_LIB_VERSION == CPP_LIB_VERSION_UNKNOWN ? "" : ("Stanford C++ Library version: " + getCppLibraryVersion() + "\n"))
				+ "\n"
				+ "Java JDK/JRE version: " + getJdkVersion() + "\n\n"
				+ "Libraries originally written by Eric Roberts,\n"
				+ "with assistance from Julie Zelenski, Keith Schwarz, et al.\n"
				+ "This version of the library is unofficially maintained by Marty Stepp.\n"
				+ "\n"
				+ "See http://stanford.edu/~stepp/cppdoc/ for more information.";
		return about;
	}
	
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
	
	public static final String getLibraryVersion() {
		return JAVA_BACK_END_VERSION;
	}
	
	public static final String getCppLibraryVersion() {
		return CPP_LIB_VERSION;
	}
	
	public static final boolean isCppProject() {
		return !isJavaProject();
	}
	
	public static final boolean isJavaProject() {
		return CPP_LIB_VERSION_UNKNOWN == CPP_LIB_VERSION;
	}
	
	public static void requireVersion(String minimumVersion) {
		String version = getLibraryVersion();
		if (version.compareTo(minimumVersion) < 0) {
			throw new acm.util.ErrorException("Stanford Java Library version " + minimumVersion
					+ " required, but found version " + version);
		}
	}
	
	public static final void setCppLibraryVersion(String version) {
		CPP_LIB_VERSION = version;
	}
	
	public static void main(String[] args) {
		System.out.println(getLibraryVersion());
	}
}

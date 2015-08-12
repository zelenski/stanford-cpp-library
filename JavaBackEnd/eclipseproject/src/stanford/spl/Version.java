/*
 * @author Marty Stepp
 * @version 2015/08/12
 */

package stanford.spl;

public class Version {
	private static final String JAVA_BACK_END_VERSION = "2015/08/12";
	
	public static final String getLibraryVersion() {
		return JAVA_BACK_END_VERSION;
	}
	
	public static void main(String[] args) {
		System.out.println(getLibraryVersion());
	}
}

/*
 * Stores the current version of the library for being displayed
 * in on-screen UI to the user.
 */

package stanford.karel;

public interface Version {
	public static final String VERSION = "2015/04/05";
	public static final String ABOUT_MESSAGE = "Karel the Robot\n"
			+ "version: " + VERSION + "\n"
			+ "\n"
			+ "The karel.jar library was designed and written by Stanford Prof. Eric Roberts.\n"
			+ "Minor modifications have been made by Stanford Lecturer Marty Stepp.\n"
			+ "\n"
			+ "See http://cs106a.stanford.edu/ for more information.";
}

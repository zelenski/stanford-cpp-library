/*
 * This class was added to support the client/server versions of
 * the FacePamphlet and Mini Twitter assignments.
 * 
 * @author Marty Stepp and Chris Piech
 * @version 2017/05/24
 */

package stanford.cs106.net;

/**
 * A listener that can respond to requests made to a server.
 */
public interface SimpleServerListener {
	/**
	 * Responds to the given request, returning the response to send back.
	 */
	Object requestMade(Request request);
}

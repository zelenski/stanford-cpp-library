/*
 * This class was added to support the client/server versions of
 * the FacePamphlet and Mini Twitter assignments.
 * 
 * @author Marty Stepp and Chris Piech
 * @version 2017/05/24
 */

package stanford.cs106.net;

import java.io.*;
import java.net.*;
import stanford.cs106.io.*;

/**
 * This class implements a basic synchronous HTTP client.
 * It can connect to an HTTP server on a given port (perhaps a SimpleServer?),
 * send it requests, and receive/return their responses.
 */
public class SimpleClient {
	// set to true to see debug println messages
	private static final boolean DEBUG = true;
	
	/** A special error prefix that will appear at the start of error responses. */
	public static final String ERROR_KEY = "Error:";
	
	/**
	 * A special request parameter used to indicate that the server should
	 * maintain a session with this client.
	 */
	public static final String SESSION_KEY_PARAM_NAME = "SESSION_ID";
	
	// server to connect to, e.g. "http://localhost:8080/"
	private String host;
	
	// a unique key to store session data for this client;
	// default null, but set to a hash/time string if session is started
	private String sessionKey;
	
	/**
	 * Initialiazes a new client to connect to the given HTTP server/host.
	 * @param host e.g. "http://localhost:8080/"
	 */
	public SimpleClient(String host) {
		if (!host.endsWith("/")) {
			host = host + "/";
		}
		this.host = host;
	}

//	public GImage getImage(String fileName) {
//		try {
//			URL url = new URL(host + "images/" + fileName);
//			Image image = ImageIO.read(url);
//			return new GImage(image);
//		} catch(IOException e) {
//			throw new RuntimeException(e);
//		}
//	}

	/**
	 * Initiates a synchronous HTTP request to this client's given server host,
	 * waiting for the HTTP response to be sent back, and returns the response.
	 */
	public String makeRequest(Request request) {
		if (request == null) {
			throw new NullPointerException("null request passed");
		}
		try {
			if (sessionIsInProgress()) {
				request.addParam(SESSION_KEY_PARAM_NAME, Request.encode(sessionKey));
			}
			URL destination = new URL(host + request.toGetRequest());
			if (DEBUG) System.out.println("SimpleClient: making request: " + destination.toString());
			
			HttpURLConnection conn = (HttpURLConnection) destination.openConnection();
			conn.setRequestMethod("GET");
			String response = IOUtils.readEntireStream(conn.getInputStream());
			if (DEBUG) System.out.println("SimpleClient: received response: " + response + " (" + response.length() + " chars)");
			
			if (response.startsWith(ERROR_KEY)) {
				String msg = response.substring(ERROR_KEY.length());
				throw new IORuntimeException(msg.trim());
			}
			return response;
		} catch (ConnectException e) {
			throw new IORuntimeException("Unable to connect to the server. Did you start it?");
		} catch (MalformedURLException e) {
			throw new IllegalArgumentException("Invalid URL: " + (host + request.toGetRequest()));
		} catch (IOException e) {
			throw new IORuntimeException(e);
		}
	}
	
	/**
	 * Initiates a synchronous HTTP request to this client's given server host,
	 * waiting for the HTTP response to be sent back, and returns the response.
	 */
	public String makeRequest(String command, String... params) {
		Request request = new Request(command, params);
		return makeRequest(request);
	}
	
//	public String saveImage(String fileName) {
//		try {
//			byte[] bytes = Files.readAllBytes(Paths.get("images/"+fileName));
//			String contents = Base64.getUrlEncoder().encodeToString(bytes);
//			String params = "newImg";
//			params += "?file=" + contents;
//			params += "&fileName=" + fileName;
//			if (sessionInProgress()) {
//				String key = sessionKey;
//				try {
//					key = URLEncoder.encode(sessionKey, "UTF-8");
//				} catch (UnsupportedEncodingException uee) {
//					// empty
//				}
//				params += "&SESSION_KEY=" + key;
//			}
//			URL destination = new URL(host + params);
//			HttpURLConnection conn = (HttpURLConnection) destination.openConnection();
//			conn.setRequestMethod("GET");
//			BufferedReader rd = new BufferedReader(new InputStreamReader(conn.getInputStream()));
//			String line = null;
//			StringBuilder result = new StringBuilder();
//			while ((line = rd.readLine()) != null) {
//				result.append(line);
//			}
//			String fromServer = result.toString();
//			rd.close();
//			return fromServer;
//		} catch (IOException e) {
//			throw new RuntimeException(e);
//		}
//	}
	
	/**
	 * Ends any current session between this client and the server host.
	 * If no session is in progress, has no effect.
	 */
	public void sessionEnd() {
		sessionKey = null;
	}
	
	/**
	 * Returns true if a session is currently in progress between this client
	 * and the server host.
	 */
	public boolean sessionIsInProgress() {
		return sessionKey != null;
	}
	
	/**
	 * Begins a session between this client and the server host.
	 * If a session is already in progress, has no effect.
	 */
	public void sessionStart() {
		if (!sessionIsInProgress()) {
			sessionKey = "";
			sessionKey += Integer.toString(hashCode(), /* radix */ 16);
			sessionKey += "_";
			sessionKey += Long.toString(System.currentTimeMillis(), /* radix */ 16);
		}
	}
	
	/**
	 * Returns a string representation of this client.
	 */
	@Override
	public String toString() {
		return getClass().getSimpleName() + "{"
				+ "host=\"" + host + "\""
				+ (sessionKey == null ? "" : (", sessionKey=\"" + sessionKey + "\""))
				+ "}";
	}
}

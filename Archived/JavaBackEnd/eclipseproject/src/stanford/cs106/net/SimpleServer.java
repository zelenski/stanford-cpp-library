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
import java.nio.file.*;
import java.util.*;
import com.sun.net.httpserver.*;
import stanford.cs106.io.*;

/**
 * This class implements a simple synchronous HTTP server that can listen on a
 * given port and respond to requests made by clients.
 */
public class SimpleServer {
	// set to true to see debug println messages
	private static final boolean DEBUG = true;
	
	/** Default port to listen for requests. */
	public static final int DEFAULT_PORT = 8080;
	
	// actual http server to listen for requests
	private HttpServer server;
	
	// student app that can respond to calls
	private SimpleServerListener webApp;
	
	// port to listen to
	private int port;
	
	// really this should be Map<> not HashMap<> but CS 106A students are not taught about such interfaces
	private HashMap<String, HashMap<String, String>> sessionDataAll;

	/**
	 * Constructs a new server that will listen on the given port.
	 * The server does not start listening until start() is called.
	 * @param port   the port to listen on, up to 65535
	 */
	public SimpleServer(int port) {
		this(/* webApp */ null, port);
	}
	
	/**
	 * Constructs a new server that will listen on a default port, with the given
	 * listener to be notified as each request comes in.
	 * The server does not start listening until start() is called.
	 * @param webApp listener to notify on each request
	 */
	public SimpleServer(SimpleServerListener webApp) {
		this(webApp, /* port */ DEFAULT_PORT);
	}
	
	/**
	 * Constructs a new server that will listen on the given port, with the given
	 * listener to be notified as each request comes in.
	 * The server does not start listening until start() is called.
	 * @param webApp listener to notify on each request
	 * @param port   the port to listen on, up to 65535
	 */
	public SimpleServer(SimpleServerListener webApp, int port) {
		if (port <= 0 || port > 65535) {
			throw new IllegalArgumentException("Port out of range: " + port);
		}
		
		this.webApp = webApp;
		this.port = port;
		this.sessionDataAll = new LinkedHashMap<String, HashMap<String, String>>();
	}
	
	/**
	 * Returns the port on which this server will listen.
	 */
	public int getPort() {
		return port;
	}

	/**
	 * Gets the part of the http GET request after hostname:port.
	 */
	private static String getUriString(HttpExchange exchange) {
		URI uri = exchange.getRequestURI();
		String uriStr = uri.toString();
		uriStr = uriStr.substring(1);
		return uriStr;
	}
	
	/**
	 * Returns whether the server is currently active and listening for HTTP requests,
	 * which will be true after start() is called.
	 */
	public boolean isRunning() {
		return server != null;
	}
	
	/**
	 * Returns true if this server's port is available for listening.
	 */
	public boolean portIsAvailable() {
		return portIsAvailable(this.port);
	}

	/**
	 * Returns true if the given port is available for listening.
	 */
	public static boolean portIsAvailable(int port) {
		Socket ignored = null;
		try {
			ignored = new Socket("localhost", port);
			return false;
		} catch (IOException ioe) {
			return true;
		} finally {
			if (ignored != null) {
				try {
					ignored.close();
				} catch (IOException ioe) {
					// empty
				}
			}
		}
	}
	
	/**
	 * Returns whether the given request contains an active session.
	 */
	public boolean sessionIsInProgress(Request request) {
		if (!request.hasParam(SimpleClient.SESSION_KEY_PARAM_NAME)) {
			return false;
		}
		String sessionKey = request.getParam(SimpleClient.SESSION_KEY_PARAM_NAME);
		return sessionDataAll.containsKey(sessionKey);
	}
	
	/**
	 * Returns all data associated with the given request's session.
	 * If the given request has no active session, returns null.
	 */
	public HashMap<String, String> sessionData(Request request) {
		if (!sessionIsInProgress(request)) {
			return null;
		}
		
		String sessionKey = request.getParam(SimpleClient.SESSION_KEY_PARAM_NAME);
		HashMap<String, String> sessionData = sessionDataAll.get(sessionKey);
		return sessionData;
	}
	
	/**
	 * Ends any active session for the given request, clearing its session data.
	 * If there was no session for the given request, has no effect.
	 */
	public void sessionEnd(Request request) {
		if (sessionIsInProgress(request)) {
			String sessionKey = request.getParam(SimpleClient.SESSION_KEY_PARAM_NAME);
			sessionDataAll.remove(sessionKey);
		}
	}
	
	/**
	 * Begins an active session for the given request, with empty session data.
	 * The session will use a unique key provided in the request by the client;
	 * if no such key is present, throws a SimpleServerException.
	 * If there was already a session for the given request, has no effect.
	 */
	public void sessionStart(Request request) {
		if (!sessionIsInProgress(request)) {
			String sessionKey = request.getParam(SimpleClient.SESSION_KEY_PARAM_NAME);
			if (sessionKey == null) {
				throw new SimpleServerException("missing session key for request: " + request);
			}
			sessionDataAll.put(sessionKey, new LinkedHashMap<String, String>());
		}
	}
	
	/**
	 * Starts a server on the given port. It is hard coded to handle
	 * img requests and resource requests specially by just reading the files and
	 * returning them. All other requests should be handled by the user's code.
	 * If the server is already running, this will have no effect.
	 */
	public void start() {
		if (isRunning()) {
			return;
		}
		
		if (!portIsAvailable(port)) {
			// This error occurs if the server is already running and you try to run it again.
			// If you see this, make sure to close ALL Java console program windows.
			// Then run your project again.
			throw new SimpleServerException(
					"Port " + port + " is not available. " +
					"Close all server windows and try again.");
		}
		
		try {
			server = HttpServer.create(new InetSocketAddress(port), 0);
			server.createContext("/img", new BottleFileHandler());
			server.createContext("/images", new BottleFileHandler());
			server.createContext("/favicon.ico", new FaveIconHandler());
			server.createContext("/resources", new BottleFileHandler());
			server.createContext("/newImg", new BottleImgReceiver());
			server.createContext("/", new BottleHandler());
			server.setExecutor(null);   // creates a default executor
			server.start();
		} catch (IOException e) {
			throw new SimpleServerException(e);
		}
	}
	
	/**
	 * Stops the server so that it will no longer listen to HTTP requests.
	 */
	public void stop() {
		server.stop(/* delayMS */ 0);
		server = null;
	}

	/**
	 * Returns a string representation of this server.
	 */
	@Override
	public String toString() {
		return getClass().getSimpleName() + "{"
				+ "port=" + this.port
				+ ", running=" + isRunning()
				+ (webApp == null ? "" : (", webApp=\"" + webApp + "\""))
				+ (sessionDataAll.isEmpty() ? "" : (", sessionData=" + sessionDataAll))
				+ "}";
	}

	/**
	 * This class passes on an HTTP request to the "webApp" which the
	 * user writes.
	 */
	private class BottleHandler implements HttpHandler {
		public void handle(HttpExchange t) throws IOException {
			try {
				String uriStr = getUriString(t);
				
				// turn the uri into a request
				Request request = Request.fromUrl(uriStr);
				
				// call the students method
				Object response = null;
				if (webApp != null) {
					response = webApp.requestMade(request);
				}
				
				// pass the response back to the caller
//				if (response == null) {
//					throw new NullPointerException("Server request returned null response.");
//				}

				String responseStr = String.valueOf(response);
				
				if (DEBUG) System.out.println("SimpleServer: sending response: " + responseStr);
				
				t.sendResponseHeaders(200, responseStr.length());
				OutputStream os = t.getResponseBody();
				os.write(responseStr.getBytes());
				os.close();
			} catch (IOException e) {
				e.printStackTrace();
				throw new IORuntimeException(e);
			}
		}
	}

	/**
	 * For some files (eg images and resources) you want the server to 
	 * simply return the file. This handler simply returns the file.
	 */
	private class BottleFileHandler implements HttpHandler {
		public void handle(HttpExchange exchange) throws IOException {
			try {
				String uriStr = getUriString(exchange);
				if (DEBUG) System.out.println("SimpleServer: handling request: " + uriStr);
				File file = new File(uriStr);
				exchange.sendResponseHeaders(200, file.length());
				OutputStream outputStream=exchange.getResponseBody();
				Files.copy(file.toPath(), outputStream);
				outputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
				throw new IORuntimeException(e);
			}
		}
	}

	private class BottleImgReceiver implements HttpHandler {
		public void handle(HttpExchange exchange) throws IOException {
			try {
				String uriStr = getUriString(exchange);
				Request r = Request.fromUrl(uriStr);
				String fileStr = r.getRaw("file");
				byte [] bytes = Base64.getUrlDecoder().decode(fileStr);
				String fileName = r.getRaw("fileName");
				
				Path path = Paths.get("images", fileName);
				FileOutputStream fos = new FileOutputStream(path.toString());
				fos.write(bytes);
				fos.close();

				String response = "success";
				exchange.sendResponseHeaders(200, response.length());
				OutputStream os = exchange.getResponseBody();
				os.write(response.getBytes());
				os.close();
			} catch (IOException e) {
				e.printStackTrace();
				throw new IORuntimeException(e);
			}
		}
	}

	private class FaveIconHandler implements HttpHandler {
		public void handle(HttpExchange exchange) throws IOException {
			try {
				File file = new File("images/faveicon.ico");
				if (file.exists()) {
					exchange.sendResponseHeaders(200, file.length());
					OutputStream outputStream = exchange.getResponseBody();
					Files.copy(file.toPath(), outputStream);
					outputStream.close();
				} else {
					exchange.sendResponseHeaders(0, file.length());
					exchange.getResponseBody().close();
				}
			} catch (IOException e) {
				throw new IORuntimeException(e);
			}
		}
	}

	/**
	 * An exception type that represents server-related runtime errors.
	 */
	public static class SimpleServerException extends IORuntimeException {
		public SimpleServerException(String message) {
			super(message);
		}

		public SimpleServerException(Throwable cause) {
			super(cause);
		}
	}
}

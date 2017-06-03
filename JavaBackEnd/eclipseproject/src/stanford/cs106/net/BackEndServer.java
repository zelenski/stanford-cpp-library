/*
 * This file used to be SimpleServer.java but has been renamed.
 * Really this should be merged with SimpleServer but I am keeping both of
 * them for now because the C++ lib uses this particular version of
 * the server class.
 * 
 * TODO: merge with SimpleServer and remove
 * 
 * @author Marty Stepp and Chris Piech
 * @version 2017/05/24
 */

package stanford.cs106.net;

import java.io.*;
import java.net.*;
import java.nio.file.Files;
import java.util.*;
import java.util.regex.*;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;
import stanford.cs106.io.IORuntimeException;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

public class BackEndServer {
	private static final int DEFAULT_PORT = 8080;
	
	private static BackEndServer INSTANCE;
	
	
	public static synchronized BackEndServer getInstance() {
		return getInstance(DEFAULT_PORT);
	}
	
	public static synchronized BackEndServer getInstance(int port) {
		if (INSTANCE == null) {
			INSTANCE = new BackEndServer(port);
		}
		return INSTANCE;
	}
	
	
	// This is the student application that can respond to calls
	private JavaBackEnd javaBackEnd;
	private HttpServer server;
	private int port;
	private Map<Integer, HttpExchange> requestMap = new HashMap<Integer, HttpExchange>();
	private int currentRequestID = 0;
	
	/**
	 * Method: Constructor
	 * -------------------
	 * All this method does is create an object that could be used to handle
	 * HTTP requests (but at the time of construction does nothing).
	 */
	public BackEndServer() {
		this(DEFAULT_PORT);
	}

	/**
	 * Method: Constructor
	 * -------------------
	 * All this method does is create an object that could be used to handle
	 * HTTP requests (but at the time of construction does nothing).
	 */
	public BackEndServer(int port) {
		this.port = port;
	}
	
	public int getPort() {
		return port;
	}
	
	public boolean isRunning() {
		return server != null;
	}
	
	public void sendResponse(int requestID, int httpErrorCode, String contentType, String responseText) {
//		javax.swing.JOptionPane.showMessageDialog(null, "BackEndServer.sendResponse(requestID=" + requestID
//				+ ", httpErrorCode=" + httpErrorCode + ", contentType=" + contentType + ")");
		
		if (!requestMap.containsKey(requestID)) {
			throw new IllegalArgumentException("Unknown request ID: " + requestID);
		}
		HttpExchange exchange = requestMap.get(requestID);
		try {
			exchange.sendResponseHeaders(httpErrorCode, responseText.length());
			OutputStream os = exchange.getResponseBody();
			os.write(responseText.getBytes());
			os.close();
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}

	public void sendResponseFile(int requestID, int httpErrorCode, String contentType, String responseFilePath) {
//		javax.swing.JOptionPane.showMessageDialog(null, "BackEndServer.sendResponseFile(requestID=" + requestID
//				+ ", httpErrorCode=" + httpErrorCode + ", contentType=" + contentType + ")");
		
		if (!requestMap.containsKey(requestID)) {
			throw new IllegalArgumentException("Unknown request ID: " + requestID);
		}
		HttpExchange exchange = requestMap.get(requestID);
		try {
			OutputStream os = exchange.getResponseBody();
			File file = new File(responseFilePath);
			exchange.sendResponseHeaders(httpErrorCode, file.length());
			Files.copy(file.toPath(), os);
			os.close();
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}
	
	public void setJavaBackEnd(JavaBackEnd javaBackEnd) {
		this.javaBackEnd = javaBackEnd;
	}

	/**
	 * Method: Start
	 * -------------
	 * This method starts a server on the given port. It is hard coded to handle
	 * img requests and resource requests specially by just reading the files and
	 * returning them. All other requests should be handled by the users code.
	 */
	public void start() {
		if (isRunning()) {
			stop();
		}
		
		try {
			server = HttpServer.create(new InetSocketAddress(port), 0);
			// server.createContext("/img", new BottleFileHandler());
			// server.createContext("/resources", new BottleFileHandler());
			server.createContext("/", new BottleHandler());
			server.setExecutor(null); // creates a default executor
			server.start();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * ...
	 */
	public void stop() {
		if (isRunning()) {
			server.stop(0);
			server = null;
		}
	}
	
	/**
	 * Method: Render Template
	 * -----------------------
	 * OPTIONAL: This method takes in a template (currently it takes the path to a template)
	 * and provides functionality for (a) importing other slivers of HTML and (b) inserting
	 * values from a map into all places in the template with the sequence {{key}}. See
	 * resources/index.html for an example.
	 */
	public static String renderTemplate(String templatePath, Map<String, String> args) {
		try {
			// This handles part (b)
			Scanner htmlScanner = new Scanner(new File(templatePath));
			String html = htmlScanner.useDelimiter("\\Z").next();
			htmlScanner.close();
			if(args != null) {
				for(String key : args.keySet()) {
					String value = args.get(key);
					html = html.replace("{{" + key + "}}" , value);
				}
			}
			
			// This handles inclues, part (a). Recursively applies render to included string
			String regex="\\%include\\[(.*)]";
			Pattern includePattern = Pattern.compile(regex);
			Matcher m = includePattern.matcher(html);
			String injectedHtml = html;
			while (m.find()) {
			    String includePath = m.group(1);
			    String injectHtml = renderTemplate(includePath, args);
			    injectedHtml = injectedHtml.replace("%include[" + includePath + "]", injectHtml);    
			}
			return injectedHtml;
		} catch(FileNotFoundException e) {
			throw new RuntimeException(e);
		}
	}
	
	//=--------------- Private -------------=//
	
	
	/**
	 * Method: Get URI String
	 * ----------------------
	 * Gets the part of the http GET request after localhost:8000.
	 */
	private static String getUriString(HttpExchange exchange) {
		URI uri = exchange.getRequestURI();
		String uriStr = uri.toString();
		
		// Chris was removing the leading /, but I think we want it for fidelity - Marty
		// uriStr = uriStr.substring(1);
		
		return uriStr;
	}
	
	/**
	 * Class: BottleFileHandler
	 * ------------------------
	 * For some files (eg images and resources) you want the server to 
	 * simply return the file. This handler simply returns the file.
	 */
//	private class BottleFileHandler implements HttpHandler {
//		public void handle(HttpExchange exchange) throws IOException {
//			String uriStr = getUriString(exchange);
//			File file = new File(uriStr);
//			exchange.sendResponseHeaders(200, file.length());
//			OutputStream outputStream = exchange.getResponseBody();
//			Files.copy(file.toPath(), outputStream);
//			outputStream.close();
//		}
//	}

	/**
	 * Class: BottleHandler
	 * -----------------------
	 * This class passes on an HTTP request to the "webApp" which the
	 * user writes.
	 */
	private class BottleHandler implements HttpHandler {
		public void handle(HttpExchange exchange) throws IOException {
			String requestUrl = getUriString(exchange);
			int requestID = 0;
			synchronized (BackEndServer.this) {
				requestID = currentRequestID++;
				requestMap.put(requestID, exchange);
			}
			
	    	// we will wait for the C++ code to send back a response
			javaBackEnd.acknowledgeEvent("event:serverRequest(%d, %d, \"%s\")",
					(long) javaBackEnd.getEventTime(),
	    			requestID,
	    			SplPipeDecoder.encode(requestUrl));
		}
	}
}

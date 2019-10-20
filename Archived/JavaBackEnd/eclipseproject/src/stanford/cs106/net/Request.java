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
import java.util.*;

/**
 * A Request object encapsulates a basic HTTP request along with its
 * various query parameter key/value pairs.
 */
public class Request {
	// HTTP command verb; used as "http://server:port/verb".
	private String command;
	
	// URL query parameters as key/value pairs;
	// used in URL as "http://server:port/verb?param1=value1&param2=value2...&paramN=valueN".
	private Map<String, String> params;

	/**
	 * URL-decodes the given string.
	 */
	public static String decode(String str) {
		try {
			return URLDecoder.decode(str, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			throw new IllegalArgumentException(e);
		}
	}

	/**
	 * URL-encodes the given string.
	 */
	public static String encode(String str) {
		try {
			return URLEncoder.encode(str, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			throw new IllegalArgumentException(e);
		}
	}
	
	/**
	 * Converts the given string into a Request object,
	 * parsing and separating any of its query parameters.
	 */
	public static Request fromUrl(String url) {
		String[] requestParts = url.split("\\?");
		String command = requestParts[0];
		Request request = new Request(command);
		if (requestParts.length == 2) {
			String paramStr = requestParts[1];
			String[] paramParts = paramStr.split("&");
			for (String paramPart : paramParts) {
				String key = paramPart.split("=")[0];
				String value = "";
				if (paramPart.split("=").length == 2) {
					value = paramPart.split("=")[1];
				} // else empty value or missing "=" (this is allowed)
				request.addRaw(key, value);
			}
		} 
		return request;
	}
	
	/**
	 * Converts the given URL into a Request object,
	 * parsing and separating any of its query parameters.
	 */
	public static Request fromUrl(URL url) {
		return fromUrl(url.toString());
	}

	/**
	 * Constructs a new Request object with the given command verb.
	 */
	public Request(String cmd) {
		if (cmd == null) {
			throw new NullPointerException("null command verb passed");
		}
		command = sanitize(cmd)
				.replace("?", "_")
				.replace(" ", "_")
				.replace("&", "_");
		params = new LinkedHashMap<String, String>();
	}

	/**
	 * Constructs a new Request object with the given command verb
	 * and the given set of parameter key/value pairs.
	 */
	public Request(String cmd, String... parameters) {
		this(cmd);
		addParams(parameters);
	}

	/**
	 * Adds a new query parameter with the given name and an empty value.
	 */
	public void addParam(String key) {
		addParam(key, /* value */ "");
	}

	/**
	 * Adds a new query parameter with the given name and value.
	 */
	public void addParam(String key, String value) {
		params.put(sanitize(key), encode(value));
	}
	
	/**
	 * Adds new query parameters with the given name/value pairs,
	 * passed as a var-args array in the order K1, V1, K2, V2, etc.
	 */
	public void addParams(String... params) {
		if (params != null) {
			for (int i = 0; i < params.length - 1; i += 2) {
				if (params[i] == null) {
					throw new NullPointerException("null parameter name #" + (i / 2 + 1) + " passed");
				}
				String name = String.valueOf(params[i]);
				String value = String.valueOf(params[i + 1]);
				addParam(name, value);
			}
		}
	}

	/**
	 * Adds a new query parameter with the given name and value.
	 * The value is not URL-encoded before including it.
	 */
	public void addRaw(String key, String value) {
		params.put(sanitize(key), value);
	}

	/**
	 * Returns this request's HTTP command verb.
	 */
	public String getCommand() {
		return command;
	}

	/**
	 * Returns the value of this request's query parameter with the given name.
	 * The parameter value is URL-decoded before returning it.
	 * If the request contains no such parameter, returns null.
	 */
	public String getParam(String key) {
		if (hasParam(key)) {
			return decode(params.get(sanitize(key)));
		} else {
			return null;
		}
	}
	
	/**
	 * Returns all of this request's query parameters as a map.
	 * The map returned is immutable and cannot be directly modified.
	 * If the request contains no parameters, returns an empty map.
	 */
	public Map<String, String> getParams() {
		return Collections.unmodifiableMap(params);
	}
	
	/**
	 * Returns the value of this request's query parameter with the given name.
	 * The parameter value is *not* URL-decoded before returning it.
	 * If the request contains no such parameter, returns null.
	 */
	public String getRaw(String key) {
		return params.get(sanitize(key));
	}
	
	/**
	 * Returns true if this request contains a query parameter with the given name.
	 */
	public boolean hasParam(String key) {
		return params.containsKey(sanitize(key));
	}
	
	/**
	 * Removes the query parameter with the given name from this request.
	 * If no such query parameter was present, has no effect.
	 */
	public void removeParam(String key) {
		params.remove(sanitize(key));
	}

	// strips illegal URL chars out of a parameter key name
	private String sanitize(String key) {
		return String.valueOf(key).trim();
	}

	/**
	 * Converts this request into a URL string that could be used for an
	 * HTTP GET request, such as
	 * "http://server:port/verb?param1=value1&param2=value2...&paramN=valueN".
	 */
	public String toGetRequest() {
		String getRequest = command;
		if (!params.isEmpty()) {
			boolean isFirst = true;
			for (String key : params.keySet()) {
				if (isFirst) {
					getRequest += "?" + key + "=" + params.get(key);
					isFirst = false;
				} else {
					getRequest += "&" + key + "=" + params.get(key);
				}
			}
		}
		return getRequest;
	}

	/**
	 * Returns a string representation of this request for debugging.
	 */
	@Override
	public String toString() {
		String str = command + " (";
		boolean isFirst = true;
		for (String p : params.keySet()) {
			if (!isFirst) {
				str += ", ";
			}
			str += p + "=" + getParam(p); 
			isFirst = false;
		}
		str += ")";
		return str;
	}
}

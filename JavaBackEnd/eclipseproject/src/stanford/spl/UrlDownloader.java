/*
 * A new utility class to make it easy to download the entire contents
 * of some web URL into either a string or an output file.
 * 
 * 
 * @version 2018/06/20
 * - added downloadWithHeaders
 * - added public constants for error codes
 * @version 2015/10/13
 * - initial version
 */

package stanford.spl;

import java.io.*;
import java.net.*;
import java.nio.channels.*;
import java.util.*;

public class UrlDownloader {
	/**
	 * constants to represent URL errors;
	 * these should match constants declared in C++ lib's urlstream.h
	 */
	public static final int ERR_MALFORMED_URL = -42;
	public static final int ERR_IO_EXCEPTION = -43;
	
	// singleton
	private static final UrlDownloader instance = new UrlDownloader();
	
	private UrlDownloader() {
		// empty
	}
	
	public static UrlDownloader getInstance() {
		return instance;
	}
	
	public String download(String urlString) throws IOException, HTTPDownloadException {
		URL url = new URL(urlString);
		HttpURLConnection connection = (HttpURLConnection) url.openConnection();
		int result = connection.getResponseCode();
		if (result == 200) {
			InputStream stream = url.openStream();
			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
			StringBuilder sb = new StringBuilder(65536);
			while (reader.ready()) {
				sb.append((char) reader.read());
			}
			reader.close();
			return sb.toString();
		} else {
			throw new HTTPDownloadException(result);
		}
	}
	
	public int download(String urlString, File file) throws IOException {
		Map<String, String> headers = Collections.emptyMap();
		return downloadWithHeaders(urlString, file, /* headers */ headers);
	}
	
	public int downloadWithHeaders(String urlString, File file, Map<String, String> headers) throws IOException {
		URL url = new URL(urlString);
		HttpURLConnection connection = (HttpURLConnection) url.openConnection();
		
		// set HTTP headers
		if (headers != null) {
			for (String name : headers.keySet()) {
				String value = headers.get(name);
				connection.setRequestProperty(name, value);
			}
		}
		
		// any 2xx result code indicates success
		int result = connection.getResponseCode();
		if (result >= 200 && result < 300) {
			InputStream stream = url.openStream();
			
			// http://stackoverflow.com/questions/921262/how-to-download-and-save-a-file-from-internet-using-java
			ReadableByteChannel rbc = Channels.newChannel(stream);
			FileOutputStream fos = new FileOutputStream(file);
			fos.getChannel().transferFrom(rbc, 0, Long.MAX_VALUE);
			fos.close();
		}
		
		return result;
	}
}

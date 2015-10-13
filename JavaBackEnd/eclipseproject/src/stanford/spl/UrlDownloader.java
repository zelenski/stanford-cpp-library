/*
 * A new utility class to make it easy to download the entire contents
 * of some web URL into either a string or an output file.
 * @version 2015/10/13
 */

package stanford.spl;

import java.io.*;
import java.net.*;
import java.nio.channels.*;

public class UrlDownloader {
	// singleton
	private static final UrlDownloader instance = new UrlDownloader();
	
	private UrlDownloader() {
		// empty
	}
	
	public static UrlDownloader getInstance() {
		return instance;
	}
	
	public String download(String urlString) throws IOException {
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
			throw new ConnectException("Cannot connect to " + urlString + " (HTTP error " + result + ")");
		}
	}
	
	public void download(String urlString, File file) throws IOException {
		URL url = new URL(urlString);
		HttpURLConnection connection = (HttpURLConnection) url.openConnection();
		int result = connection.getResponseCode();
		if (result == 200) {
			InputStream stream = url.openStream();
			
//			BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
//			PrintStream out = new PrintStream(file);
//			while (reader.ready()) {
//				out.write(reader.read());
//			}
//			out.close();
//			reader.close();
			
			// http://stackoverflow.com/questions/921262/how-to-download-and-save-a-file-from-internet-using-java
			ReadableByteChannel rbc = Channels.newChannel(stream);
			FileOutputStream fos = new FileOutputStream(file);
			fos.getChannel().transferFrom(rbc, 0, Long.MAX_VALUE);
			fos.close();
		} else {
			throw new ConnectException("Cannot connect to " + urlString + " (HTTP error " + result + ")");
		}
	}
}

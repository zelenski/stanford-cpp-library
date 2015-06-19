package stanford.spl;

import java.io.*;
import java.net.*;

import acm.util.*;

public class URL_download extends JBECommand {
	// URL.download("http%3A%2F%2Fmartystepp.com%2F", "temp.foo")
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String urlString = SplPipeDecoder.readEncodedString(paramTokenScanner);
		urlString = SplPipeDecoder.decode(urlString);
		paramTokenScanner.verifyToken(",");
		String tempfilename = SplPipeDecoder.readEncodedString(paramTokenScanner);
		tempfilename = SplPipeDecoder.decode(tempfilename);
		paramTokenScanner.verifyToken(")");
		
		// download from the given URL to a temp file
		File tmpDir = new File(System.getProperty("java.io.tmpdir"));
		File tmpFile = new File(tmpDir, tempfilename);
		
		int result = 0;
		try {
			URL url = new URL(urlString);
			HttpURLConnection connection = (HttpURLConnection) url.openConnection();
			result = connection.getResponseCode();
			if (result == 200) {
				InputStream stream = url.openStream();
				BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
				PrintStream out = new PrintStream(tmpFile);
				while (reader.ready()) {
					out.print((char) reader.read());
				}
				out.close();
				reader.close();
			}
		} catch (MalformedURLException mfurle) {
			result = -42;
		} catch (IOException ioe) {
			result = -43;
		}
		
		SplPipeDecoder.writeResult(result);
	}
}

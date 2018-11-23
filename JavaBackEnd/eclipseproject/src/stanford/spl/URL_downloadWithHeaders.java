/*
 * @version 2018/06/20
 * - initial version
 */

package stanford.spl;

import java.io.*;
import java.net.*;
import java.util.*;

import acm.util.*;

public class URL_downloadWithHeaders extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String urlString = SplPipeDecoder.readEncodedString(paramTokenScanner);
		urlString = SplPipeDecoder.decode(urlString);
		paramTokenScanner.verifyToken(",");
		String tempfilename = SplPipeDecoder.readEncodedString(paramTokenScanner);
		tempfilename = SplPipeDecoder.decode(tempfilename);
		paramTokenScanner.verifyToken(",");
		Map<String, String> headers = SplPipeDecoder.readEncodedMap(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		// download from the given URL to a temp file
		File tmpDir = new File(System.getProperty("java.io.tmpdir"));
		File tmpFile = new File(tmpDir, tempfilename);
		
		int result = 0;
		try {
			result = UrlDownloader.getInstance().downloadWithHeaders(urlString, tmpFile, headers);
		} catch (MalformedURLException mfurle) {
			result = UrlDownloader.ERR_MALFORMED_URL;
		} catch (IOException ioe) {
			result = UrlDownloader.ERR_IO_EXCEPTION;
		}
		
		SplPipeDecoder.writeResult(result);
	}
}

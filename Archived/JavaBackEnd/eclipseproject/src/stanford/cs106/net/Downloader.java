package stanford.cs106.net;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.URL;
import java.util.Enumeration;
import java.util.Map;
import java.util.TreeMap;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class Downloader {
	private Downloader() {
		// empty
	}
	
	// Downloads the file at the given URL and saves it to the local disk.
	// For example "www.foo.com/bar/baz/Mumble.txt" saves to "Mumble.txt".
	// Returns the file name of the saved local file.
	public static String downloadFile(URL url) throws IOException {
		String fileName = new File(url.getFile()).getName();
		// if (!(new File(fileName).exists())) {
			OutputStream output = new PrintStream(fileName);
			InputStream input = new BufferedInputStream(url.openStream());
			byte[] buffer = new byte[512000];
			int numRead;
			while ((numRead = input.read(buffer)) != -1) {
				output.write(buffer, 0, numRead);
			}
			output.close();
		// }
		return fileName;
	}

	// Downloads the contents of the .zip file at the given URL, and
	// returns them as a map from filenames to the bytes of each file.
	public static Map<String, byte[]> getZipFileContents(URL url)
			throws IOException {
		String fileName = downloadFile(url);
		ZipFile zip = new ZipFile(fileName);
		Map<String, byte[]> zipFilesMap = new TreeMap<String, byte[]>();

		// read each file entry from the zip archive
		for (Enumeration<? extends ZipEntry> enu = zip.entries(); enu
				.hasMoreElements();) {
			ZipEntry ze = enu.nextElement();
			if (ze.isDirectory()) {
				continue;
			}

			int size = (int) ze.getSize();
			if (size < 0) {
				continue; // -1 means unknown size.
			}

			InputStream input = zip.getInputStream(ze);
			byte[] b = new byte[size];
			int offset = 0;
			while (size - offset > 0) {
				int bytesRead = input.read(b, offset, size - offset);
				if (bytesRead < 0) {
					break;
				}
				offset += bytesRead;
			}

			zipFilesMap.put(ze.getName(), b);
		}
		zip.close();
		return zipFilesMap;
	}
}

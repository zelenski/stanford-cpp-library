/*
 * CS 106A
 *
 * This instructor-provided file contains utility functions related to loading
 * resource files, especially from within a JAR and/or applet.
 *
 * Author : Marty Stepp
 * Version: Tue 2014/05/11
 * 
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */

// TO DO: This file's documentation is insufficient due to time constraints.
// Improve documentation in future quarters for better style and maintainability.

package stanford.cs106.io;

import java.io.*;
import java.net.*;

public class ResourceUtils {
	// set to true to see debugging messages when trying to load resources
	private static final boolean DEBUG = false;
	
	// if running as an applet, replace this with the applet's class at runtime
	private static Class<?> resourceLoaderClass = ResourceUtils.class;
	
	public static void setResourceLoaderClass(Class<?> clazz) {
		if (clazz != null) {
			resourceLoaderClass = clazz;
		}
	}
	
	public static boolean fileExists(String filename) {
		try {
			URL url = filenameToURL(filename);
			return url != null;
		} catch (IOException ioe) {
			return false;
		}
	}
	
	public static URL filenameToURL(String filename) throws IOException {
		filename = filename.replace("\\", "/");
		if (DEBUG) System.out.println(" - filenameToURL: trying to load \"" + filename + "\"");
		
		File file = new File(filename);
		URL result = null;
		try {
			if (file.exists()) {
				result = new URL("file:" + filename.replace("\\", "/"));
				if (DEBUG) System.out.println(" - filenameToURL: found in current dir " + result);
			} else {
//				file = new File("../" + filename);  // "../simple.txt"
//				if (file.exists()) {
//					result = new URL("file:../" + filename.replace("\\", "/"));
//					if (DEBUG) System.out.println(" - filenameToURL: found in up-level dir " + result);
//				}
			}
		} catch (SecurityException sex) {
			// running as an applet
			sex.printStackTrace();
		}

		// fallback to using internal class URL (JAR or applet)
		if (result == null) {
			result = resourceLoaderClass.getResource("/" + filename);
			if (DEBUG) System.out.println(" - filenameToURL: classLoader yields " + result);
		}
		if (result == null) {
			throw new FileNotFoundException(filename);
		} else {
			return result;
		}
	}
	
	public static InputStream openFile(String filename) throws IOException {
		InputStream stream = filenameToURL(filename).openStream();
		if (stream == null) {
			throw new FileNotFoundException(filename);
		} else {
			return stream;
		}
	}
}

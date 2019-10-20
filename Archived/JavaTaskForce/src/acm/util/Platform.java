/*
 * @(#)Platform.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

// REVISION HISTORY [NOTE: Remember to update JTF_VERSION on each release]
//
// -- V2.0 --
// Feature enhancement 2-Mar-07 (ESR)
//   1. Added copyFileTypeAndCreator method.
//   2. Added getJTFVersion method.

package acm.util;

import java.awt.*;
import java.io.*;
import java.lang.reflect.*;
import java.util.*;

/* Class: Platform */
/**
 * This class contains methods to support platform-specific code.
 */
public class Platform {

/* Constant: UNKNOWN */
/** Indicates that the type of system cannot be determined. */
	public static final int UNKNOWN = 0;

/* Constant: MAC */
/** Indicates that the system is some variety of Apple Macintosh. */
	public static final int MAC = 1;

/* Constant: UNIX */
/** Indicates that the system is some variety of Unix or Linux. */
	public static final int UNIX = 2;

/* Constant: WINDOWS */
/** Indicates that the system is some variety of Microsoft Windows. */
	public static final int WINDOWS = 3;

/* Private constructor: Platform */
/**
 * Prevents anyone else from constructing this class.
 */
	private Platform() {
		/* Empty */
	}

/* Static method: getPlatform() */
/**
 * Returns an enumeration constant specifying the type of platform
 * on which this applet is running, which is one of the supported
 * types defined at the beginning of this class.
 *
 * @usage int platform = Platform.getPlatform();
 * @return A constant specifying the platform type
 */
	public static int getPlatform() {
		if (platform != -1) return platform;
		String name = System.getProperty("os.name", "").toLowerCase();
		if (name.startsWith("mac")) return platform = MAC;
		if (name.startsWith("windows")) return platform = WINDOWS;
		if (name.startsWith("microsoft")) return platform = WINDOWS;
		if (name.startsWith("ms")) return platform = WINDOWS;
		if (name.startsWith("unix")) return platform = UNIX;
		if (name.startsWith("linux")) return platform = UNIX;
		return platform = UNKNOWN;
	}

/* Static method: isMac */
/**
 * Checks whether the platform is a Macintosh.
 *
 * @usage if (Platform.isMac()) . . .
 * @return <code>true</code> if the platform is a Macintosh, <code>false</code> otherwise
 */
	public static boolean isMac() {
		return getPlatform() == MAC;
	}

/* Static method: isWindows() */
/**
 * Checks whether the platform is a Windows machine.
 *
 * @usage if (Platform.isWindows()) . . .
 * @return <code>true</code> if the platform is a Windows machine, <code>false</code> otherwise
 */
	public static boolean isWindows() {
		return getPlatform() == WINDOWS;
	}

/* Static method: isUnix() */
/**
 * Checks whether the platform is Unix.
 *
 * @usage if (Platform.isUnix()) . . .
 * @return <code>true</code> if the platform is Unix, <code>false</code> otherwise
 */
	public static boolean isUnix() {
		return getPlatform() == UNIX;
	}

/* Static method: setFileTypeAndCreator(filename, type, creator) */
/**
 * Sets the Macintosh file type and creator.  This method is ignored on non-Mac
 * platforms.
 *
 * @usage Platform.setFileTypeAndCreator(filename, type, creator);
 * @param filename The name of the file
 * @param type A four-character string indicating the file type
 * @param creator A four-character string indicating the file type
 */
	public static void setFileTypeAndCreator(String filename, String type, String creator) {
		if (!isMac()) return;
		try {
			setFileTypeAndCreator(new File(filename), type, creator);
		} catch (Exception ex) {
			/* Empty */
		}
	}

/* Static method: setFileTypeAndCreator(file, type, creator) */
/**
 * Sets the Macintosh file type and creator.  This method is ignored on non-Mac
 * platforms.
 *
 * @usage Platform.setFileTypeAndCreator(file, type, creator);
 * @param file The <code>File</code> object corresponding to the file
 * @param type A four-character string indicating the file type
 * @param creator A four-character string indicating the file type
 */
	public static void setFileTypeAndCreator(File file, String type, String creator) {
		if (!isMac()) return;
		try {
			Class<?> mrjOSTypeClass = Class.forName("com.apple.mrj.MRJOSType");
			Class<?> mrjFileUtilsClass = Class.forName("com.apple.mrj.MRJFileUtils");
			Class[] sig1 = { Class.forName("java.lang.String") };
			Constructor<?> constructor = mrjOSTypeClass.getConstructor(sig1);
			Class[] sig2 = { Class.forName("java.io.File"), mrjOSTypeClass, mrjOSTypeClass };
			Method setFileTypeAndCreator = mrjFileUtilsClass.getMethod("setFileTypeAndCreator", sig2);
			Object[] args1 = { (type + "    ").substring(0, 4) };
			Object osType = constructor.newInstance(args1);
			Object[] args2 = { (creator + "    ").substring(0, 4) };
			Object creatorType = constructor.newInstance(args2);
			Object[] args3 = { file, osType, creatorType };
			setFileTypeAndCreator.invoke(null, args3);
		} catch (Exception ex) {
			/* Empty */
		}
	}

/* Static method: copyFileTypeAndCreator(newFile, oldFile) */
/**
 * Sets the Macintosh file type and creator for the new file using the old file
 * as a model.  This method is ignored on non-Mac platforms.
 *
 * @usage Platform.copyFileTypeAndCreator(oldFile, newFile);
 * @param oldFile The <code>File</code> object corresponding to the existing file
 * @param newFile The <code>File</code> object corresponding to the new file
 */
	public static void copyFileTypeAndCreator(File oldFile, File newFile) {
		if (!isMac()) return;
		try {
			Class<?> mrjOSTypeClass = Class.forName("com.apple.mrj.MRJOSType");
			Class<?> mrjFileUtilsClass = Class.forName("com.apple.mrj.MRJFileUtils");
			Class[] sig1 = { Class.forName("java.io.File") };
			Method getFileType = mrjFileUtilsClass.getMethod("getFileType", sig1);
			Method getFileCreator = mrjFileUtilsClass.getMethod("getFileCreator", sig1);
			Class[] sig2 = { Class.forName("java.io.File"), mrjOSTypeClass, mrjOSTypeClass };
			Method setFileTypeAndCreator = mrjFileUtilsClass.getMethod("setFileTypeAndCreator", sig2);
			Object[] args1 = { oldFile };
			Object osType = getFileType.invoke(null, args1);
			Object creatorType = getFileCreator.invoke(null, args1);
			Object[] args2 = { newFile, osType, creatorType };
			setFileTypeAndCreator.invoke(null, args2);
		} catch (Exception ex) {
			/* Empty */
		}
	}

/* Static method: getJTFVersion() */
/**
 * Returns the version number of the JTF libraries as a string
 * suitable for use with the <code>compareVersion</code> method.  Note
 * that this returns the value of the version of the library that is
 * actually loaded.  Making this a constant would mean that the value
 * would be the one with which the code was compiled, which is less
 * likely to be useful.
 *
 * @usage String version = getJTFVersion();
 * @return The loaded version of the JTF libraries
 */
	public static String getJTFVersion() {
		return JTF_VERSION;
	}

/* Static method: compareVersion(version) */
/**
 * This method compares the Java version given in the system properties
 * with the specified version and returns -1, 0, or +1 depending on whether
 * the system version is earlier than, equal to, or later than the specified
 * one.  Thus, to test whether the current version of the JDK was at least
 * 1.2.1, for example, you could write
 *
 * <p><pre><code>
 * &nbsp;    if (Platform.compareVersion("1.2.1") &gt;= 0) . . .
 * </code></pre>
 *
 * @usage int cmp = Platform.compareVersion(version);
 * @param version A string consisting of integers separated by periods
 * @return -1, 0, or +1 depending on whether the system version is earlier than,
 *         equal to, or later than the specified one
 */
	public static int compareVersion(String version) {
		return compareVersion(System.getProperty("java.version"), version);
	}

/* Static method: compareVersion(v1, v2) */
/**
 * This method compares the version strings <code>v1</code> and <code>v2</code>
 * and returns -1, 0, or +1 depending on whether <code>v1</code> is earlier
 * than, equal to, or later than <code>v2</code>.
 *
 * @usage int cmp = Platform.compareVersion(v1, v2);
 * @param v1 A string consisting of integers separated by periods
 * @param v2 A second version string in the same format
 * @return -1, 0, or +1 depending on whether <code>v1</code> is earlier than,
 *         equal to, or later than <code>v2</code>
 */
	public static int compareVersion(String v1, String v2) {
		StringTokenizer t1 = new StringTokenizer(v1, ".");
		StringTokenizer t2 = new StringTokenizer(v2, ".");
		while (t1.hasMoreTokens() && t2.hasMoreTokens()) {
			int n1 = Integer.parseInt(t1.nextToken());
			int n2 = Integer.parseInt(t2.nextToken());
			if (n1 != n2) return (n1 < n2) ? -1 : +1;
		}
		if (t1.hasMoreTokens()) return +1;
		if (t2.hasMoreTokens()) return -1;
		return 0;
	}

/* Static method: isSwingAvailable() */
/**
 * Checks whether Swing is available.  Unfortunately, some browsers seem to lie
 * about the JDK version and return a 1.2 number without actually having Swing.
 * This implementation tests the version first, but then confirms the result
 * by looking for the <code>JComponent</code> class.  Checking the version first
 * means that no <nobr><code>SecurityException</code>s</nobr> will be logged in
 * Windows machines, which always log <nobr><code>SecurityException</code>s</nobr>,
 * even if the exception is caught.
 *
 * @usage if (Platform.isSwingAvailable()) . . .
 * @return <code>true</code> if Swing is available, <code>false</code> otherwise
 */
	public static boolean isSwingAvailable() {
		if (!swingChecked) {
			swingChecked = true;
			isSwingAvailable = false;
			if (compareVersion("1.2") >= 0) {
				try {
					isSwingAvailable = Class.forName("javax.swing.JComponent") != null;
				} catch (Exception ex) {
					/* Empty */
				}
			}
		}
		return isSwingAvailable;
	}

/* Static method: isSunAudioAvailable() */
/**
 * Checks whether the <code>sun.audio</code> package is available.
 *
 * @usage if (Platform.isSunAudioAvailable()) . . .
 * @return <code>true</code> if the <code>sun.audio</code> package is available,
 *         <code>false</code> otherwise
 */
	public static boolean isSunAudioAvailable() {
		if (!sunAudioChecked) {
			sunAudioChecked = true;
			try {
				isSunAudioAvailable = Class.forName("sun.audio.AudioPlayer") != null;
			} catch (Exception ex) {
				isSunAudioAvailable = false;
			}
		}
		return isSunAudioAvailable;
	}

/* Static method: isJMFAvailable() */
/**
 * Checks whether the Java Media Framework is available.
 *
 * @usage if (Platform.isJMFAvailable()) . . .
 * @return <code>true</code> if the JMF package is available, <code>false</code> otherwise
 */
	public static boolean isJMFAvailable() {
		if (!jmfChecked) {
			jmfChecked = true;
			try {
				isJMFAvailable = Class.forName("javax.media.Player") != null;
			} catch (Exception ex) {
				isJMFAvailable = false;
			}
		}
		return isJMFAvailable;
	}

/* Static method: areCollectionsAvailable() */
/**
 * Checks whether the JDK 1.2 collection classes are available.  Some browsers
 * return a version of the JDK that does not actually match what is supported.
 * This method actually checks whether the collection classes are there by
 * looking for the <code>ArrayList</code> class.
 *
 * @usage if (Platform.areCollectionsAvailable()) . . .
 * @return <code>true</code> if collections are available, <code>false</code> otherwise
 */
	public static boolean areCollectionsAvailable() {
		if (!collectionsChecked) {
			collectionsChecked = true;
			try {
				areCollectionsAvailable = Class.forName("java.util.ArrayList") != null;
			} catch (Exception ex) {
				areCollectionsAvailable = false;
			}
		}
		return areCollectionsAvailable;
	}

/* Static method: areStandardFontFamiliesAvailable() */
/**
 * Checks whether the JDK 1.2 standard font families (<code>Serif</code>,
 * <code>SansSerif</code>, and <code>Monospaced</code>) are available.
 *
 * @usage if (Platform.areStandardFontFamiliesAvailable()) . . .
 * @return <code>true</code> if the standard fonts are available, <code>false</code> otherwise
 */
	public static boolean areStandardFontFamiliesAvailable() {
		if (!fontsChecked) {
			fontsChecked = true;
			try {
				Class<?> toolkitClass = Class.forName("java.awt.Toolkit");
				Method getFontList = toolkitClass.getMethod("getFontList", new Class[0]);
				String[] fonts = (String[]) getFontList.invoke(Toolkit.getDefaultToolkit(), new Object[0]);
				int standardFontCount = 0;
				for (int i = 0; i < fonts.length; i++) {
					if (fonts[i].equals("Serif") || fonts[i].equals("SansSerif") || fonts[i].equals("Monospaced")) {
						standardFontCount++;
					}
				}
				areStandardFontFamiliesAvailable = (standardFontCount == 3);
			} catch (Exception ex) {
				areStandardFontFamiliesAvailable = false;
			}
		}
		return areStandardFontFamiliesAvailable;
	}

/* Private constants */
	private static final String JTF_VERSION = "1.99.1";

/* Private static variables */
	private static int platform = -1;
	private static boolean areStandardFontFamiliesAvailable;
	private static boolean fontsChecked;
	private static boolean isSwingAvailable;
	private static boolean swingChecked;
	private static boolean areCollectionsAvailable;
	private static boolean collectionsChecked;
	private static boolean isSunAudioAvailable;
	private static boolean sunAudioChecked;
	private static boolean isJMFAvailable;
	private static boolean jmfChecked;

}

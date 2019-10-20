/*
 * @(#)FileChooserFilter.java   1.99.1 08/12/08
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

// REVISION HISTORY
//
// Class introduced in V1.1

package acm.util;

import java.io.*;
import java.util.*;

/* Class: FileChooserFilter */
/**
 * This class extends <code>javax.swing.filechooser.FileFilter</code>
 * and exists primarily to avoid the ambiguity that arises because there is
 * also a <code>FileFilter</code> class in <code>java.io</code>.  This
 * class also supports simple wildcard matching of filenames.
 */
public class FileChooserFilter extends javax.swing.filechooser.FileFilter {

/* Constructor: FileChooserFilter() */
/**
 * Creates a default <code>FileChooserFilter</code>.  Such a
 * <code>FileChooserFilter</code> is useful only if the client
 * overrides the <code>accept</code> and <code>getDescription</code>
 * methods.
 *
 * @usage FileChooserFilter filter = new FileChooserFilter();
 */
	public FileChooserFilter() {
		filenamePattern = null;
		filenameDescription = null;
	}

/* Constructor: FileChooserFilter(pattern) */
/**
 * Creates a <code>FileChooserFilter</code> that accepts filenames
 * matching the specified pattern.  This pattern consists of a
 * wildcard pattern (or a series of wildcard patterns separated
 * by semicolons) similar to those by a Unix shell.  For example, to
 * match all files ending with the extensions <code>.html</code>
 * or <code>.htm</code>, you could use the following constructor call:
 *
 * <p><pre><code>
 * &nbsp;    new FileChooserPattern("*.html;*.htm")
 * </code></pre>
 *
 * @usage FileChooserFilter filter = new FileChooserFilter(pattern);
 * @param pattern The filename pattern
 */
	public FileChooserFilter(String pattern) {
		this(pattern, pattern + " files");
	}

/* Constructor: FileChooserFilter(pattern, description) */
/**
 * Creates a <code>FileChooserFilter</code> that accepts filenames
 * matching the specified pattern.  This pattern consists of a
 * wildcard pattern (or a series of wildcard patterns separated
 * by semicolons) similar to those by a Unix shell.  For example, to
 * match all files ending with the extensions <code>.html</code>
 * or <code>.htm</code>, you could use the following constructor call:
 *
 * <p><pre><code>
 * &nbsp;    new FileChooserPattern("*.html;*.htm")
 * </code></pre>
 *
 * <p>This version of the constructor makes it possible to set the
 * description that appears in the dialog.
 *
 * @usage FileChooserFilter filter = new FileChooserFilter(pattern, description);
 * @param pattern The filename pattern
 * @param description The description of the files for inclusion in the dialog
 */
	public FileChooserFilter(String pattern, String description) {
		filenamePattern = pattern;
		filenameDescription = description;
	}

/* Method: accept(file) */
/**
 * Returns <code>true</code> if the specified file should be accepted by
 * the filter.
 *
 * @usage if (filter.accept(file)) . . .
 * @param file The <code>File</code> object representing the file
 * @return <code>true</code> if the specified file should be accepted
 */
	public boolean accept(File file) {
		if (filenamePattern == null) {
			throw new ErrorException("No override definition for accept");
		}
		if (file.isDirectory()) return true;
		StringTokenizer tokenizer = new StringTokenizer(filenamePattern, ";");
		while (tokenizer.hasMoreTokens()) {
			String pattern = tokenizer.nextToken();
			if (pattern.length() > 0 && JTFTools.matchFilenamePattern(file.getName(), pattern)) {
				return true;
			}
		}
		return false;
	}

/* Method: getDescription() */
/**
 * Returns a description of the accepted files.
 */
	public String getDescription() {
		if (filenameDescription == null) {
			throw new ErrorException("No override definition for getDescription");
		}
		return filenameDescription;
	}

/* Private instance variables */
	String filenamePattern;
	String filenameDescription;
}

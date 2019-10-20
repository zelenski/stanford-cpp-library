/*
 * @(#)JTFTools.java   1.99.1 08/12/08
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
// -- V2.0 --
// Bug fix 2-Jan-07 (ESR, JTFBug 2007-001)
//   1. Fixed bug in the decodeFont method where it failed to take proper
//      account of the properties of the old font.
//
// Feature enhancement 2-Mar-07 (ESR)
//   1. Added methods to support the implementation of the "Export Applet" and
//      "Submit Project" menu items.
//
// Feature enhancement 4-May-07 (ESR)
//   1. Added getFontList and findFontFamily methods.
//
// Code cleanup 28-May-07 (ESR)
//   1. Added generic type tags.
//
// Feature enhancement 10-Oct-07 (ESR)
//   1. Significantly improved the operation of getMainClass by checking
//      to see whether classes in the current directory have been loaded.
//
// Feature enhancement 10-Jun-08 (ESR)
//   1. Added matchFilenamePattern method.

package acm.util;

import acm.gui.*;
import acm.io.*;
import acm.program.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.net.*;
import java.security.*;
import java.text.*;
import java.util.*;
import java.util.zip.*;
import javax.swing.*;
import javax.swing.event.*;

/* Class: JTFTools */
/**
 * This class provides a collection of static utility methods
 * that are used elsewhere in the ACM packages.
 */
public class JTFTools {

/* Private constructor */
/**
 * Prevent any clients from constructing objects of this class.
 */
	private JTFTools() {
		/* Empty */
	}

/* Static method: pause(milliseconds) */
/**
 * Delays the calling thread for the specified time, which is expressed in
 * milliseconds.  Unlike <code>Thread.sleep</code>, this method never throws an
 * exception.
 *
 * @usage JTFTools.pause(milliseconds);
 * @param milliseconds The sleep time in milliseconds
 */
	public static void pause(double milliseconds) {
		Applet applet = appletTable.get(Thread.currentThread());
		if (applet == null) {
			applet = mostRecentApplet;
			appletTable.put(Thread.currentThread(), applet);
		}
		try {
			int millis = (int) milliseconds;
			int nanos = (int) Math.round((milliseconds - millis) * 1000000);
			Thread.sleep(millis, nanos);
		} catch (InterruptedException ex) {
			/* Empty */
		}
	}

/* Static method: createEmptyContainer() */
/**
 * Returns an empty lightweight container.  Several packages need to create
 * such components as placeholders.  Defining it in JTFTools gives those
 * pacakges access to a common mechanism.
 *
 * @usage Container comp = JTFTools.createEmptyContainer();
 * @return An empty lightweight container that can be used as a placeholder
 */
	public static Container createEmptyContainer() {
		return new EmptyContainer();
	}

/* Static method: getEnclosingFrame(comp) */
/**
 * Returns the frame that encloses the specified component.
 *
 * @usage Frame frame = JTFTools.getEnclosingFrame(comp);
 * @param comp The component at which to start the search
 * @return The nearest enclosing <code>Frame</code> object
 */
	public static Frame getEnclosingFrame(Component comp) {
		if (comp instanceof Program) comp = ((Program) comp).getContentPane();
		while (comp != null && !(comp instanceof Frame)) {
			comp = comp.getParent();
		}
		return (Frame) comp;
	}

/* Static method: getStandardFont(font) */
/**
 * Returns a font that will approximate the specified font in this environment.
 * This method is required because some browsers do not support the standard
 * fonts <code>Serif</code>, <code>SansSerif</code>, and <code>Monospaced</code>.
 *
 * @usage Font newFont = JTFTools.getStandardFont(font);
 * @param font The font being checked
 * @return The new font
 */
	public static Font getStandardFont(Font font) {
		if (!fontFamilyTableInitialized) initFontFamilyTable();
		if (font == null || fontFamilyTable == null) return font;
		String family = font.getFamily();
		if (fontFamilyTable.get(trimFamilyName(family)) != null) return font;
		if (family.equals("Serif") || family.equals("Times")) {
			family = getFirstAvailableFontSubstitution(SERIF_SUBSTITUTIONS);
		} else if (family.equals("SansSerif")) {
			family = getFirstAvailableFontSubstitution(SANSSERIF_SUBSTITUTIONS);
		} else if (family.equals("Monospaced")) {
			family = getFirstAvailableFontSubstitution(MONOSPACED_SUBSTITUTIONS);
		} else {
			return font;
		}
		if (family == null) return font;
		return new Font(family, font.getStyle(), font.getSize());
	}

/* Static method: getFontList() */
/**
 * Returns an array of the available font family names.
 *
 * @usage String[] fonts = JTFTools.getFontList();
 * @return An array of the available font family names
 */
	public static String[] getFontList() {
		if (!fontFamilyTableInitialized) initFontFamilyTable();
		return fontFamilyArray;
	}

/* Static method: findFontFamily(str) */
/**
 * Finds the first font family in the string <code>str</code> that exists in
 * the current <code>GraphicsEnvironment</code>.  As in HTML font tags, the
 * <code>str</code> consists of a set of font names separated by semicolons.
 * The <code>findFontFamily</code> method returns the first family name that
 * exists in the list of loaded fonts, or <code>null</code> if there are none.
 *
 * @usage String familyName = JTFTools.findFontFamily(str);
 * @param str The list of family names separated by semicolons
 * @return The first family name that exists, or <code>null</code> if none
 */
	public static String findFontFamily(String str) {
		if (!fontFamilyTableInitialized) initFontFamilyTable();
		StringTokenizer tokenizer = new StringTokenizer(str, ";", false);
		while (tokenizer.hasMoreTokens()) {
			String familyName = fontFamilyTable.get(trimFamilyName(tokenizer.nextToken()));
			if (familyName != null) return familyName;
		}
		return null;
	}

/* Static method: decodeFont(str) */
/**
 * Decodes a font in the style of <code>Font.decode</code>.
 *
 * @usage Font font = JTFTools.decodeFont(str);
 * @param str The string to decode
 * @return The new font
 */
	public static Font decodeFont(String str) {
		return decodeFont(str, null);
	}

/* Static method: decodeFont(str, oldFont) */
/**
 * Decodes a font in the style of <code>Font.decode</code>.  The only difference
 * is that this method takes a font parameter that gives default values for
 * the different parts of the font.  If the family, size, or style is specified
 * as an asterisk, the corresponding value is taken from the supplied font.
 *
 * @usage Font font = JTFTools.decodeFont(str, oldFont);
 * @param str The string to decode
 * @param oldFont The font whose properties are used as defaults
 * @return The new font
 */
	public static Font decodeFont(String str, Font oldFont) {
		String familyName = str;
		int fontStyle = (oldFont == null) ? Font.PLAIN : oldFont.getStyle();
		int fontSize = (oldFont == null) ? 12 : oldFont.getSize();
		int hyphen = str.indexOf('-');
		if (hyphen >= 0) {
			familyName = str.substring(0, hyphen);
			str = str.substring(hyphen + 1).toLowerCase();
			String styleName = str;
			hyphen = str.indexOf('-');
			if (hyphen >= 0) {
				styleName = str.substring(0, hyphen);
				str = str.substring(hyphen + 1);
			} else {
				str = "*";
			}
			if (Character.isDigit(styleName.charAt(0))) {
				String tmp = styleName;
				styleName = str;
				str = tmp;
			}
			if (styleName.equals("plain")) {
				fontStyle = Font.PLAIN;
			} else if (styleName.equals("bold")) {
				fontStyle = Font.BOLD;
			} else if (styleName.equals("italic")) {
				fontStyle = Font.ITALIC;
			} else if (styleName.equals("bolditalic")) {
				fontStyle = Font.BOLD | Font.ITALIC;
			} else if (!styleName.equals("*")) {
				throw new ErrorException("Illegal font style");
			}
			if (!str.equals("*")) {
				try {
					fontSize = Integer.valueOf(str).intValue();
				} catch (NumberFormatException ex) {
					throw new ErrorException("Illegal font size");
				}
			}
		}
		if (familyName.equals("*")) {
			familyName = (oldFont == null) ? "Default" : oldFont.getName();
		} else {
			if (!fontFamilyTableInitialized) initFontFamilyTable();
			if (fontFamilyTable != null) {
				familyName = fontFamilyTable.get(trimFamilyName(familyName));
				if (familyName == null) familyName = "Default";
			}
		}
		return getStandardFont(new Font(familyName, fontStyle, fontSize));
	}

/* Static method: decodeColor(name) */
/**
 * Decodes a color name.  This method is similar to <code>Color.decode</code>
 * except in that it allows named colors and system colors.
 *
 * @usage color = JTFTools.decodeColor(name);
 * @param name The string name of the color
 * @return The color corresponding to the specified name
 */
	public static Color decodeColor(String name) {
		if (name.equalsIgnoreCase("desktop")) return SystemColor.desktop;
		if (name.equalsIgnoreCase("activeCaption")) return SystemColor.activeCaption;
		if (name.equalsIgnoreCase("activeCaptionText")) return SystemColor.activeCaptionText;
		if (name.equalsIgnoreCase("activeCaptionBorder")) return SystemColor.activeCaptionBorder;
		if (name.equalsIgnoreCase("inactiveCaption")) return SystemColor.inactiveCaption;
		if (name.equalsIgnoreCase("inactiveCaptionText")) return SystemColor.inactiveCaptionText;
		if (name.equalsIgnoreCase("inactiveCaptionBorder")) return SystemColor.inactiveCaptionBorder;
		if (name.equalsIgnoreCase("window")) return SystemColor.window;
		if (name.equalsIgnoreCase("windowBorder")) return SystemColor.windowBorder;
		if (name.equalsIgnoreCase("windowText")) return SystemColor.windowText;
		if (name.equalsIgnoreCase("menu")) return SystemColor.menu;
		if (name.equalsIgnoreCase("menuText")) return SystemColor.menuText;
		if (name.equalsIgnoreCase("text")) return SystemColor.text;
		if (name.equalsIgnoreCase("textText")) return SystemColor.textText;
		if (name.equalsIgnoreCase("textHighlight")) return SystemColor.textHighlight;
		if (name.equalsIgnoreCase("textHighlightText")) return SystemColor.textHighlightText;
		if (name.equalsIgnoreCase("textInactiveText")) return SystemColor.textInactiveText;
		if (name.equalsIgnoreCase("control")) return SystemColor.control;
		if (name.equalsIgnoreCase("controlText")) return SystemColor.controlText;
		if (name.equalsIgnoreCase("controlHighlight")) return SystemColor.controlHighlight;
		if (name.equalsIgnoreCase("controlLtHighlight")) return SystemColor.controlLtHighlight;
		if (name.equalsIgnoreCase("controlShadow")) return SystemColor.controlShadow;
		if (name.equalsIgnoreCase("controlDkShadow")) return SystemColor.controlDkShadow;
		if (name.equalsIgnoreCase("scrollbar")) return SystemColor.scrollbar;
		if (name.equalsIgnoreCase("info")) return SystemColor.info;
		if (name.equalsIgnoreCase("infoText")) return SystemColor.infoText;
		if (name.equalsIgnoreCase("black")) return Color.BLACK;
		if (name.equalsIgnoreCase("blue")) return Color.BLUE;
		if (name.equalsIgnoreCase("cyan")) return Color.CYAN;
		if (name.equalsIgnoreCase("darkGray")) return Color.DARK_GRAY;
		if (name.equalsIgnoreCase("DARK_GRAY")) return Color.DARK_GRAY;
		if (name.equalsIgnoreCase("gray")) return Color.GRAY;
		if (name.equalsIgnoreCase("green")) return Color.GREEN;
		if (name.equalsIgnoreCase("lightGray")) return Color.LIGHT_GRAY;
		if (name.equalsIgnoreCase("LIGHT_GRAY")) return Color.LIGHT_GRAY;
		if (name.equalsIgnoreCase("magenta")) return Color.MAGENTA;
		if (name.equalsIgnoreCase("orange")) return Color.ORANGE;
		if (name.equalsIgnoreCase("pink")) return Color.PINK;
		if (name.equalsIgnoreCase("red")) return Color.RED;
		if (name.equalsIgnoreCase("white")) return Color.WHITE;
		if (name.equalsIgnoreCase("yellow")) return Color.YELLOW;
		try {
			return Color.decode(name);
		} catch (NumberFormatException ex) {
			throw new ErrorException("Illegal color value");
		}
	}

/* Static method: matchFilenamePattern(filename, pattern) */
/**
 * Determines whether the filename matches the specified pattern.  The
 * pattern string is interpreted in much the same way that a Unix shell
 * expands filenames and supports the following wildcard options:
 *
 * <table>
 * <tr><td align=center><code>?</code></td><td>Matches any single character</td></tr>
 * <tr><td align=center><code>*</code></td><td>Matches any sequence of characters</td></tr>
 * <tr><td align=center><code>[...]</code></td><td>Matches any of the specified characters</td></tr>
 * <tr><td align=center><code>[^...]</code></td><td>Matches any character <i>except</i> the specified ones</td></tr>
 * </table>
 *
 * The last two options allow a range of characters to be specified in the form
 * <code>a-z</code>.
 *
 * @usage if (JTFTools.matchFilenamePattern(filename, pattern)) . . .
 * @param filename The filename being tested
 * @param pattern The pattern including wildcards
 * @return <code>true</code> if the filename matches the pattern
 */
	public static boolean matchFilenamePattern(String filename, String pattern) {
		return recursiveMatch(filename, 0, pattern, 0);
	}

/* Static method: registerApplet(applet) */
/**
 * Adds this applet to a table indexed by the current thread.
 *
 * @usage JTFTools.registerApplet(applet);
 * @param applet The applet being registered
 * @noshow
 */
	public static void registerApplet(Applet applet) {
		registerApplet(applet, Thread.currentThread());
		mostRecentApplet = applet;
	}

/* Static method: registerApplet(applet, thread) */
/**
 * Adds this applet to a table indexed by the specified thread.
 *
 * @usage JTFTools.registerApplet(applet);
 * @param applet The applet being registered
 * @param thread The thread used as the key
 * @noshow
 */
	public static void registerApplet(Applet applet, Thread thread) {
		appletTable.put(thread, applet);
	}

/* Static method: getApplet() */
/**
 * Returns the current applet.  This implementation may fail in
 * multithreaded programs.  Such programs should therefore not
 * rely on this facility.  It is appropriate to use this facility,
 * for example, during the <code>init</code> method of a program,
 * which is the only context in which it is applied in the JTF tools.
 *
 * @usage Applet applet JTFTools.getApplet();
 * @return The currently running applet
 * @noshow
 */
	public static Applet getApplet() {
		Applet applet = appletTable.get(Thread.currentThread());
		if (applet == null) applet = mostRecentApplet;
		return applet;
	}

/* Static method: setDebugOptions(options) */
/**
 * Sets the debugging options in place for this run.  The list of options are
 * a set of strings combined with the <code>+</code> sign character.
 *
 * @usage JTFTools.setDebugOption(options)
 * @param options The names of the enabled options separated by plus signs
 * @noshow
 */
	public static void setDebugOptions(String options) {
		debugOptions = (options == null) ? null : "+" + options.toLowerCase() + "+";
	}

/* Static method: testDebugOption(option) */
/**
 * Tests to see whether the current program was invoked with a <code>debug</code>
 * parameter that contains the specified string as a lexical unit in a string
 * delimited by <code>+</code> signs.
 *
 * @usage if (JTFTools.testDebugOption(option)) . . .
 * @param option The name of the option
 * @return The constant <code>true</code> if the specified debug option is in effect
 * @noshow
 */
	public static boolean testDebugOption(String option) {
		if (debugOptions == null) return false;
		return debugOptions.indexOf("+" + option.toLowerCase() + "+") >= 0;
	}

/* Static method: getCommandLine() */
/**
 * Returns the command line that invoked this program, or <code>null</code>
 * if no command line is available.
 *
 * @usage String line = JTFTools.getCommandLine();
 * @return The command line that invoked this program
 * @noshow
 */
	public static String getCommandLine() {
		switch (Platform.getPlatform()) {
		  case Platform.UNIX: case Platform.MAC:
			return getShellCommandLine();
		  case Platform.WINDOWS:
			return DOSCommandLine.getCommandLine();
		  default:
			return getShellCommandLine();
		}
	}

/* Static method: getMainClass() */
/**
 * Returns the name of the main class.
 *
 * @usage String className = JTFTools.getMainClass();
 * @return The name of the main class, or null if that class cannot be identified
 * @noshow
 */
	public static String getMainClass() {
		String className = null;
		try {
			className = System.getProperty("java.main");
		} catch (Exception ex) {
			/* Empty */
		}
		if (className == null) {
			className = readMainClassFromClassPath();
		}
		if (className == null) {
			String commandLine = getCommandLine();
			className = readMainClassFromCommandLine(commandLine);
		}
		return className;
	}

/* Static method: checkIfLoaded(className) */
/**
 * Returns true if the specified class is already loaded.
 *
 * @usage if (JTFTools.checkIfLoaded(className)) . . .
 * @param className The name of the class
 * @return The boolean value true if the class is already loaded, and false otherwise
 * @noshow
 */
	public static boolean checkIfLoaded(String className) {
		if (Platform.compareVersion("1.2") < 0) return false;
		boolean result = false;
		try {
			if (System.getSecurityManager() != null) return false;
			if (managerThatFails == null) {
				try {
					Class<?> managerThatFailsClass = Class.forName("acm.util.SecurityManagerThatFails");
					managerThatFails = (SecurityManager) managerThatFailsClass.newInstance();
				} catch (Exception ex) {
					return false;
				}
			}
			System.setSecurityManager(managerThatFails);
			try {
				result = (Class.forName(className) != null);
			} catch (ExceptionInInitializerError err) {
				result = true;
			} catch (NoClassDefFoundError err) {
				/* Empty */;
			} finally {
				System.setSecurityManager(null);
			}
		} catch (Exception ex) {
			/* Empty */
		}
		return result;
	}

/* Static method: terminateAppletThreads(applet) */
/**
 * Terminates all of the threads that are registered as belonging to the
 * specified applet.
 *
 * @usage JTFTools.terminateAppletThreads(applet);
 * @param applet The applet whose threads are being terminated
 * @noshow
 */
	public static void terminateAppletThreads(Applet applet) {
		try {
			Thread myThread = Thread.currentThread();
			Class<?> threadClass = Class.forName("java.lang.Thread");
			Method stop = threadClass.getMethod("stop", new Class[0]);
			Iterator<Thread> iterator = appletTable.keySet().iterator();
			while (iterator.hasNext()) {
				Thread t = iterator.next();
				if (t != myThread && t.isAlive() && isAnonymous(t) && applet == appletTable.get(t)) {
					stop.invoke(t, new Object[0]);
				}
			}
		} catch (Exception ex) {
			/* Empty */
		}
	}

/* Static method: isAnonymous(t) */
/**
 * Returns <code>true</code> if the supplied thread is an anonymous
 * one created automatically by the system.
 *
 * @usage if (JTFTools.isAnonymous(t)) . . .
 * @param t The thread being tested
 * @noshow
 */
	public static boolean isAnonymous(Thread t) {
		String name = t.getName();
		if (!name.startsWith("Thread-")) return false;
		for (int i = 7; i < name.length(); i++) {
			if (!Character.isDigit(name.charAt(i))) return false;
		}
		return true;
	}

/* Static method: openMailStream(smtpServer, from, to) */
/**
 * Opens a mail-sending process using the specified SMTP server, which will
 * send a message from the specified sender to the recipient.  The return value
 * is a <code>PrintStream</code> that the client can use to write the data
 * in the message, including the headers.  Closing the stream sends the message.
 * To cancel the message without sending it, use the <code>cancelMail</code> method.
 *
 * @usage JTFTools.openMailStream(smtpServer, from, to);
 * @param smtpServer A string indicating the host name of the SMTP server
 * @param from A string indicating the e-mail address of the sender
 * @param to A string indicating the e-mail address of this recipient
 * @return A <code>PrintStream</code> suitable for sending the message data
 * @noshow
 */
	public static PrintStream openMailStream(String smtpServer, String from, String to) {
		return new MailStream(smtpServer, from, to);
	}

/* Static method: cancelMail(out) */
/**
 * Cancels the mail-sending process without sending the message.  The output stream
 * is closed after returning.
 *
 * @usage JTFTools.cancelMail(out);
 * @param out The output stream returned by <code>openMailStream</code>
 * @noshow
 */
	public static void cancelMail(PrintStream out) {
		((MailStream) out).cancel();
	}

/* Static method: sendStandardHeaders(out, senderName, subject) */
/**
 * Adds the standard "To", "From", "Subject", and "Date" headers to the message.
 *
 * @usage JTFTools.sendStandardHeaders(out, senderName, subject);
 * @param out The output stream returned by <code>openMailStream</code>
 * @param senderName The real name of the sender
 * @param subject The subject line
 * @noshow
 */
	public static void sendStandardHeaders(PrintStream out, String senderName, String subject) {
		((MailStream) out).sendStandardHeaders(senderName, subject);
	}

/* Static method: openHexByteOutputStream(printStream) */
/**
 * Opens an output stream that allows the client to write data using pairs
 * of hex characters.  The return value is an <code>OutputStream</code> that
 * the client can use to which the client can send binary data, which is then
 * written in encoded form to the underlying  <code>PrintStream</code> object.
 *
 * @usage JTFTools.openHexByteOutputStream(printStream);
 * @param printStream The underlying <code>PrintStream</code> to which data is written
 * @return An <code>OutputStream</code> for writing binary data
 * @noshow
 */
	public static OutputStream openHexByteOutputStream(PrintStream printStream) {
		return new HexByteOutputStream(printStream);
	}

/* Static method: openBase64OutputStream(printStream) */
/**
 * Opens an output stream that allows the client to write data using Base64 encoding,
 * which is the most common encoding for mail attachments.  The return value
 * is an <code>OutputStream</code> that the client can use to which the client
 * can send binary data, which is then written in encoded form to the underlying
 * <code>PrintStream</code> object.  Closing the stream automatically pads the
 * data to complete a Base64 unit.  To pad the data without closing the stream (as
 * one would need to do for a multi-attachment message, for example), call the
 * <code>padBase64OutputStream</code> method.
 *
 * @usage JTFTools.openBase64OutputStream(printStream);
 * @param printStream The underlying <code>PrintStream</code> to which data is written
 * @return An <code>OutputStream</code> for writing binary data
 * @noshow
 */
	public static OutputStream openBase64OutputStream(PrintStream printStream) {
		return new Base64OutputStream(printStream);
	}

/* Static method: padBase64OutputStream(out) */
/**
 * Pads the data in the specified output stream without closing it.  This method
 * is automatically called when the stream is closed, but must be called explicitly
 * if you intend to write additional data to the underlying <code>PrintStream</code>.
 *
 * @usage JTFTools.padBase64OutputStream(out);
 * @param out The output stream returned by <code>openBase64OutputStream</code>
 * @noshow
 */
	public static void padBase64OutputStream(OutputStream out) {
		((Base64OutputStream) out).pad();
	}

/* Static method: exportJar(jarFile, dir, libName, transformer) */
/**
 * Exports a jar file consisting of all the <code>.class<code> files
 * in the directory, including those embedded inside jar files.
 * If the <code>transformer</code> method is not <code>null</code>, its
 * <code>transform</code> method is called to dump the bytes of the class
 * instead of simply copying them.
 *
 * @usage exportJar(jarFile, dir, libName, transformer);
 * @param jarFile A <code>File</code> object indicating the destination jar file
 * @param dir The directory being exported
 * @param imports A semicolon-separated list of <code>.jar</code> files to include (or <code>null</code>)
 * @param transformer An object to transform class data (or <code>null</code>)
 * @noshow
 */
	public static void exportJar(File jarFile, File dir, String imports, Object transformer) {
		try {
			ZipOutputStream out = new ZipOutputStream(new FileOutputStream(jarFile));
			dumpJarAndResources("", dir, out, null, imports, null, false, transformer);
			out.close();
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/* Static method: executeExportAction(program, actionCommand) */
/**
 * Checks to see if the action command corresponds to one of the export
 * actions.  If so, it executes the method and returns <code>true</code>;
 * if not, the method returns <code>false</code>.
 *
 * @usage boolean ok = JTFTools.executeExportAction(program, command);
 * @param program The program to which the action is applied
 * @param command The command name being executed
 * @return A flag indicating whether the action was recognized
 * @noshow
 */
	public static boolean executeExportAction(Program program, String command) {
		if (command.equals("Export Applet") || command.equals("Submit Project")) {
			new Thread(new ThreadedMenuAction(program, command)).start();
			return true;
		}
		return false;
	}

/* Static method: getLocalHostName() */
/**
 * Returns the canonical name of the local host.
 *
 * @usage String hostName = JTFTools.getLocalHostName();
 * @return The fully qualified name of the local host, or <code>null</code> if unavailable
 * @noshow
 */
	public static String getLocalHostName() {
		try {
			InetAddress myHost = InetAddress.getLocalHost();
			Class<?> inetAddressClass = myHost.getClass();
			Method getCanonicalHostName = inetAddressClass.getMethod("getCanonicalHostName", new Class[0]);
			return (String) getCanonicalHostName.invoke(myHost, new Object[0]);
		} catch (Exception ex) {
			return null;
		}
	}

/* Static method: copyFile(oldFile, newFile) */
/**
 * Copies a file from <code>oldFile</code> to <code>newFile</code>.
 *
 * @usage JTFTools.copyFile(oldFile, newFile);
 * @param oldFile A <code>File</code> object indicating the existing file
 * @param newFile A <code>File</code> object indicating the new file
 */
	public static void copyFile(File oldFile, File newFile) {
		try {
			BufferedInputStream in = new BufferedInputStream(new FileInputStream(oldFile));
			BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(newFile));
			copyBytes(in, out, oldFile.length());
			in.close();
			out.close();
			Platform.copyFileTypeAndCreator(oldFile, newFile);
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/* Static method: copyBytes(in, out, nBytes) */
/**
 * Copies the specified number bytes from the input stream to the output stream.
 * @param in The input stream
 * @param out The output stream
 * @param nBytes The number of bytes to copy
 */
	public static void copyBytes(InputStream in, OutputStream out, long nBytes) throws IOException {
		byte[] buffer = new byte[BUFFER_SIZE];
		while (nBytes > 0) {
			int chunkSize = (int) Math.min(BUFFER_SIZE, nBytes);
			chunkSize = in.read(buffer, 0, chunkSize);
			if (chunkSize == -1) return;
			out.write(buffer, 0, chunkSize);
			nBytes -= chunkSize;
		}
	}

/**********************************************************************/
/* Package private methods                                            */
/**********************************************************************/

/* Package private method: getURLSuffix(path) */
/**
 * Returns the last component of the URL path.
 *
 * @usage String suffix = getURLSuffix(path);
 * @param path The string version of the URL
 * @return The last component of the URL path
 */
	static String getURLSuffix(String path) {
		return path.substring(path.lastIndexOf('/') + 1);
	}

/* Package private method: exportApplet(program, progress) */
/**
 * Exports an applet version of the specified program.  The directory used for the
 * applet is chosen by a popup dialog.
 *
 * @usage submitProject(program, progress);
 * @param program The program being submitted
 * @param progress An optional progress bar to keep track of the operation
 */
	static void exportApplet(Program program, JProgressBar progress) {
		try {
			String className = program.getClass().getName();
			String programName = className.substring(className.lastIndexOf(".") + 1);
			File dir = new File(System.getProperty("user.dir"));
			File home = new File(System.getProperty("user.home"));
			ExportAppletDialog dialog = new ExportAppletDialog(home, program);
			File outDir = dialog.chooseOutputDirectory();
			if (outDir == null) return;
			if (outDir.exists()) {
				if (!outDir.isDirectory()) {
					outDir = new File(outDir.getParent());
				}
			} else {
				outDir.mkdir();
			}
			if (progress != null) {
				progress.setMaximum(countResources(dir, RESOURCE_EXTENSIONS, "acm.jar") + 1);
				ProgressBarDialog.popup(progress);
			}
			File indexFile = new File(dir, "index.html");
			if (indexFile.canRead()) {
				JTFTools.copyFile(indexFile, new File(outDir, "index.html"));
			} else {
				dumpHTMLIndex(outDir, program, className, programName);
			}
			if (progress != null) {
				if (ProgressBarDialog.hasBeenCancelled(progress)) return;
				progress.setValue(progress.getValue() + 1);
			}
			dumpJarAndResources(dir, outDir, programName + ".jar", "acm.jar", progress, dialog.exportFiles(), null);
			if (progress != null) ProgressBarDialog.dismiss(progress);
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/**********************************************************************/
/* Private methods                                                    */
/**********************************************************************/

/* Private static method: initFontFamilyTable() */
/**
 * Initializes the list of font families.
 */
	private static void initFontFamilyTable() {
		fontFamilyTableInitialized = true;
		for (int pass = 1; fontFamilyArray == null && pass <= 2; pass++) {
			try {
				if (pass == 1) {
					Class<?> classGE = Class.forName("java.awt.GraphicsEnvironment");
					Method getLocalGraphicsEnvironment = classGE.getMethod("getLocalGraphicsEnvironment", new Class[0]);
					Method getAvailableFontFamilyNames = classGE.getMethod("getAvailableFontFamilyNames", new Class[0]);
					Object ge = getLocalGraphicsEnvironment.invoke(null, new Object[0]);
					fontFamilyArray = (String[]) getAvailableFontFamilyNames.invoke(ge, new Object[0]);
				} else {
					Class<?> classToolkit = Class.forName("java.awt.Toolkit");
					Method getFontList = classToolkit.getMethod("getFontList", new Class[0]);
					fontFamilyArray = (String[]) getFontList.invoke(Toolkit.getDefaultToolkit(), new Object[0]);
				}
			} catch (Exception ex) {
				/* Empty */
			}
		}
		fontFamilyTable = new HashMap<String,String>();
		for (int i = 0; i < fontFamilyArray.length; i++) {
			fontFamilyTable.put(trimFamilyName(fontFamilyArray[i]), fontFamilyArray[i]);
		}
		fontFamilyTable.put("serif", getFirstAvailableFontSubstitution(SERIF_SUBSTITUTIONS));
		fontFamilyTable.put("sansserif", getFirstAvailableFontSubstitution(SANSSERIF_SUBSTITUTIONS));
		fontFamilyTable.put("monospaced", getFirstAvailableFontSubstitution(MONOSPACED_SUBSTITUTIONS));
	}

/* Private static method: getFirstAvailableFontSubstitution(fontOptions) */
/**
 * Returns the first family in the array of font options that is
 * actually installed.
 */
	private static String getFirstAvailableFontSubstitution(String[] fontOptions) {
		for (int i = 0; i < fontOptions.length; i++) {
			if (fontFamilyTable.get(trimFamilyName(fontOptions[i])) != null) return fontOptions[i];
		}
		return null;
	}

/* Private static method: trimFamilyName(family) */
/**
 * Creates a canonical family name by converting the string to
 * lower case and removing spaces and hyphens.
 */
	private static String trimFamilyName(String family) {
		String str = "";
		for (int i = 0; i < family.length(); i++) {
			char ch = family.charAt(i);
			if (ch != ' ' && ch != '-') str += Character.toLowerCase(ch);
		}
		return str;
	}

/* Private static method: recursiveMatch(str, sx, pattern, px) */
/**
 * This method implements the filename matcher in a way that requires no
 * string allocation.  The <code>sx</code> and <code>px</code> parameters
 * are the current index in their respective strings.
 */
	private static boolean recursiveMatch(String str, int sx, String pattern, int px) {
		int strlen = str.length();
		int patlen = pattern.length();
		if (px == patlen) return (sx == strlen);
		char pch = pattern.charAt(px);
		if (pch == '*') {
			for (int i = sx; i <= strlen; i++) {
				if (recursiveMatch(str, i, pattern, px + 1)) return true;
			}
			return false;
		}
		if (sx == strlen) return false;
		char sch = str.charAt(sx);
		if (pch == '[') {
			boolean match = false;
			boolean invert = false;
			px++;
			if (px == patlen) {
				throw new ErrorException("matchFilenamePattern: missing ]");
			}
			if (pattern.charAt(px) == '^') {
				px++;
				invert = true;
			}
			while (px < patlen && pattern.charAt(px) != ']') {
				if (px + 2 < patlen && pattern.charAt(px + 1) == '-') {
					match |= (sch >= pattern.charAt(px) && sch <= pattern.charAt(px + 2));
					px += 3;
				} else {
					match |= (sch == pattern.charAt(px));
					px++;
				}
			}
			if (px == patlen) {
				throw new ErrorException("matchFilenamePattern: missing ]");
			}
			if (match == invert) return false;
		} else if (pch != '?') {
			if (pch != sch) return false;
		}
		return recursiveMatch(str, sx + 1, pattern, px + 1);
	}

/* Private static method: getShellCommandLine() */
/**
 * Attempts to return the command line for Unix systems and MacOS X.
 * This code is adapted from a more general command-line scanner written
 * by Erik Forslin.
 *
 * @usage String line = getShellCommandLine();
 * @return The shell command line that invoked this process, or <code>null</code>
 *         if no command line is available
 */
	private static String getShellCommandLine() {
		try {
			String option = (Platform.isMac()) ? "command" : "args";
			String[] argv = { "bash", "-c", "ps -p $PPID -o " + option };
			Process p = Runtime.getRuntime().exec(argv);
			p.waitFor();
			if (p.getErrorStream().read() != -1) return null;
			BufferedReader rd = new BufferedReader(new InputStreamReader(p.getInputStream()));
			rd.readLine(); /* Ignore header */
			return rd.readLine();
		} catch (Exception ex) {
			return null;
		}
	}

/* Private static method: readMainClassFromManifest(jarName) */
/**
 * Attempts to read the name of the main class from the manifest of
 * the specified JAR file.
 *
 * @usage String className = readMainClassFromManifest(jarName);
 * @param jarName The name of the JAR file
 * @return The name of the main class, or <code>null</code>
 */
	private static String readMainClassFromManifest(String jarName) {
		try {
			if (testDebugOption("main")) {
				System.out.println("Read class from JAR manifest in " + jarName);
			}
			ZipFile jarFile = new ZipFile(jarName);
			ZipEntry entry = jarFile.getEntry("META-INF/MANIFEST.MF");
			if (entry == null) return null;
			BufferedReader rd = new BufferedReader(new InputStreamReader(jarFile.getInputStream(entry)));
			for (String line = rd.readLine(); line != null; line = rd.readLine()) {
				if (line.startsWith("Main-Class:")) {
					String mainClass = line.substring("Main-Class:".length()).trim();
					if (testDebugOption("main")) {
						System.out.println("Main class = " + mainClass);
					}
					return mainClass;
				}
			}
			return null;
		} catch (IOException ex) {
			return null;
		}
	}

/* Private static method: readMainClassFromCommandLine(line) */
/**
 * Attempts to read the name of the main class from the specified command
 * line.  This strategy is a heuristic and will probably fail in many
 * cases, but it will probably work in enough contexts to be useful.
 * As noted in the documentation for the <code>main</code> method,
 * programs can always avoid the need for this method by supplying their
 * own version of <code>main</code>.
 *
 * @usage String className = readMainClassFromCommandLine(line);
 * @param line The command line
 * @return The name of the main class, or <code>null</code>
 */
	private static String readMainClassFromCommandLine(String line) {
		if (testDebugOption("main")) {
			System.out.println("Read class from command line: " + line);
		}
		if (line == null) return null;
		boolean jarFlag = false;
		try {
			StreamTokenizer tokenizer = new StreamTokenizer(new StringReader(line));
			tokenizer.resetSyntax();
			tokenizer.wordChars(33, 255);
			tokenizer.quoteChar('"');
			tokenizer.quoteChar('\'');
			tokenizer.whitespaceChars(' ', ' ');
			tokenizer.whitespaceChars('\t', '\t');
			boolean cmdRead = false;
			while (true) {
				int tt = tokenizer.nextToken();
				String token = tokenizer.sval;
				switch (tt) {
				  case StreamTokenizer.TT_EOF:
					return null;
				  case StreamTokenizer.TT_WORD: case '"': case '\'':
					break;
				  default:
					return null;
				}
				if (cmdRead) {
					if (token.startsWith("-")) {
						if (token.equals("-jar")) {
							jarFlag = true;
						} else if (token.equals("-cp") || token.equals("-classpath")) {
							tokenizer.nextToken();
						}
					} else {
						if (jarFlag) return readMainClassFromManifest(token);
						if (testDebugOption("main")) {
							System.out.println("Main class = " + token);
						}
						return token;
					}
				} else {
					cmdRead = true;
				}
			}
		} catch (IOException ex) {
			/* Empty */
		}
		return null;
	}

/* Private static method: readMainClassFromClassPath() */
/**
 * Attempts to read the name of the main class by searching through
 * the class path to find any classes that are already loaded and that
 * extend Program.
 *
 * @usage String className = readMainClassFromClassPath();
 * @return The name of the main class, or <code>null</code>
 */
	private static String readMainClassFromClassPath() {
		String result = null;
		String classpath = System.getProperty("java.class.path");
		if (classpath == null) classpath = System.getProperty("user.dir");
		if (classpath == null) return null;
		if (testDebugOption("main")) {
			System.out.println("Read class from class path: " + classpath);
		}
		StringTokenizer tokenizer = new StringTokenizer(classpath, ":;");
		while (tokenizer.hasMoreTokens()) {
			String token = tokenizer.nextToken();
			File file = new File(token);
			String[] candidates = null;
			if (file.isDirectory()) {
				candidates = file.list();
			} else if (token.endsWith(".jar") && !nameAppears(token, SKIP_JARS)) {
				try {
					ZipFile zf = new ZipFile(file);
					ArrayList<String> list = new ArrayList<String>();
					Enumeration<?> entries = zf.entries();
					while (entries.hasMoreElements()) {
						list.add(((ZipEntry) entries.nextElement()).getName());
					}
					candidates = new String[list.size()];
					for (int i = 0; i < candidates.length; i++) {
						candidates[i] = list.get(i);
					}
				} catch (IOException ex) {
					candidates = null;
				}
			}
			if (candidates != null) {
				for (int i = 0; i < candidates.length; i++) {
					String fileName = candidates[i];
					if (fileName.endsWith(".class")) {
						String className = fileName.substring(0, fileName.lastIndexOf(".class"));
						if (className.indexOf("/") == -1 && checkIfLoaded(className)) {
							try {
								Class<?> c = Class.forName(className);
								Class[] types = { candidates.getClass() };
								if (c.getMethod("main", types) != null) {
									if (testDebugOption("main")) {
										System.out.println("Main class = " + className);
									}
									if (result != null) return null;
									result = className;
								}
							} catch (Exception ex) {
								/* Empty */
							}
						}
					}
				}
			}
		}
		return result;
	}

/* Protected static method: submitProject(program, progress) */
/**
 * Submits this project as a mail message.
 *
 * @usage submitProject(program, progress);
 * @param program The program being submitted
 * @param progress An optional progress bar to keep track of the operation
 */
	protected static void submitProject(Program program, JProgressBar progress) {
		SubmitOptions options = getOptions(program);
		if (options.popup() && options.isComplete()) {
			String className = program.getClass().getName();
			String programName = className.substring(className.lastIndexOf(".") + 1);
			String boundary = "==" + System.currentTimeMillis() + "==";
			String smtpServer = options.getSMTPServer();
			String authorName = options.getAuthorName();
			String authorEMail = options.getAuthorEMail();
			String instructor = options.getSubmissionEMail();
			String authorID = authorEMail;
			int atIndex = authorEMail.indexOf("@");
			if (atIndex != -1) authorID = authorID.substring(0, atIndex);
			String zipName = programName + "_" + authorID;
			File dir = new File(System.getProperty("user.dir"));
			if (progress != null) {
				progress.setMaximum(countResources(dir, SUBMIT_EXTENSIONS, null));
				ProgressBarDialog.popup(progress);
			}
			PrintStream out = openMailStream(smtpServer, authorEMail, instructor);
			sendStandardHeaders(out, authorName, programName);
			out.println("Mime-Version: 1.0");
			out.println("Content-Type: multipart/mixed; boundary=" + '"' + boundary + '"');
			out.println();
			out.println("--" + boundary);
			out.println("Content-Transfer-Encoding: base64");
			out.println("Content-Type: application/zip; name=" + zipName + ".zip");
			out.println("Content-Disposition: attachment; filename=" + zipName + ".zip");
			submitDirectory(out, zipName, progress);
			if (ProgressBarDialog.hasBeenCancelled(progress)) cancelMail(out);
			out.println("--" + boundary + "--");
			out.close();
			if (progress != null) ProgressBarDialog.dismiss(progress);
		}
	}

/* Private static method: dumpHTMLIndex(outDir, program, className, programName) */
/**
 * Dumps a minimal <code>index.html</code> file.
 */
	private static void dumpHTMLIndex(File outDir, Program program, String className, String programName) throws IOException {
		File outFile = new File(outDir, "index.html");
		PrintWriter wr = new PrintWriter(new FileWriter(outFile));
		Dimension size = program.getSize();
		wr.println("<html>");
		wr.println("<head>");
		wr.println("<meta name=\"generator\" content=\"ACM Java Libraries V1.1\">");
		wr.println("<title>" + programName + "</title>");
		wr.println("</head>");
		wr.println("<body>");
		wr.println("<center>");
		wr.println("<table border=2 cellpadding=0 cellspacing=0>");
		wr.println("<tr><td>");
		wr.println("<applet archive=\"" + programName + ".jar\"");
		wr.println("        code=\"" + className.replace('.', '/') + ".class\"");
		wr.println("        width=" + size.width + " height=" + size.height + ">");
		wr.println("</applet>");
		wr.println("</td></tr>");
		wr.println("</table>");
		wr.println("</center>");
		wr.println("</body>");
		wr.println("</html>");
		wr.close();
	}

/* Private static method: dumpJarAndResources(dir, outDir, filename, imports, progress, options, exportFiles, transformer) */
/**
 * Dumps all content enclosed in the directory <code>dir</code> into a jar file in
 * <code>outDir</code>.  This code creates two copies of the resources: one in the
 * jar file and a second copy in the directory.  Browsers that can read from the
 * jar file see increased performance; older browsers take the resources from the
 * code base.
 */
	private static void dumpJarAndResources(File dir, File outDir, String filename, String imports,
	                                        JProgressBar progress, boolean exportFiles,
	                                        Object transformer) throws IOException {
		File outFile = new File(outDir, filename);
		ZipOutputStream out = new ZipOutputStream(new FileOutputStream(outFile));
		dumpJarAndResources("", dir, out, outDir, imports, progress, exportFiles, transformer);
		out.close();
	}

/* Private static method: dumpJarAndResources(path, file, out, outDir, imports, progress, exportFiles, transformer) */
/**
 * Dumps the directory rooted at <code>dir</code> to the output stream.
 * The following tags describe the parameters:
 *
 * @param path The current path (/ separated)
 * @param dir The directory to be written
 * @param out The <code>ZipOutputStream</code> to which the data is written
 * @param outDir The output directory
 * @param imports A semicolon-separated list of <code>.jar</code> files to include (or <code>null</code>)
 * @param progress A <code>JProgressBar</code> to keep track of the progress
 * @param exportFiles A flag indicating whether files are to be exported
 * @param transformer An object to transform class data (or <code>null</code>)
 */
	private static void dumpJarAndResources(String path, File dir, ZipOutputStream out,
	                                        File outDir, String imports, JProgressBar progress,
	                                        boolean exportFiles, Object transformer) throws IOException {
		if (ProgressBarDialog.hasBeenCancelled(progress)) return;
		dumpTree(path, dir, out, outDir, progress, 0, exportFiles, transformer);
		StringTokenizer tokenizer = new StringTokenizer(imports, ";");
		while (tokenizer.hasMoreTokens()) {
			String jarName = tokenizer.nextToken().trim();
			File libFile = getLibrary(jarName);
			if (libFile != null) {
				ZipFile jarFile = new ZipFile(libFile);
				Enumeration<? extends ZipEntry> e = jarFile.entries();
				while (e.hasMoreElements()) {
					ZipEntry entry = e.nextElement();
					String entryName = entry.getName();
					if (!nameAppears(entryName, SKIP_FILES)) {
						InputStream in = new BufferedInputStream(jarFile.getInputStream(entry));
						if (transformer != null && entryName.endsWith(".class")) {
							entry = new ZipEntry(entryName);
							out.putNextEntry(entry);
							transformClass(transformer, in, out);
						} else {
							out.putNextEntry(entry);
							JTFTools.copyBytes(in, out, entry.getSize());
						}
						while (true) {
							int ch = in.read();
							if (ch == -1) break;
							out.write(ch);
						}
						out.closeEntry();
						in.close();
					}
					if (progress != null) progress.setValue(progress.getValue() + 1);
				}
			}
		}
	}

/* Private static method: dumpTree(path, file, out, outDir, progress, options, exportFiles, transformer) */
/**
 * Dumps the file or subdirectory tree rooted at <code>file</code> to the output stream.
 */
	private static void dumpTree(String path, File file, ZipOutputStream out, File outDir,
	                             JProgressBar progress, int level, boolean exportFiles,
	                             Object transformer) throws IOException {
		if (ProgressBarDialog.hasBeenCancelled(progress)) return;
		String filename = file.getName();
		if (file.isDirectory()) {
			String[] files = file.list();
			if (level > 0) path += filename + "/";
			for (int i = 0; i < files.length; i++) {
				dumpTree(path, new File(file, files[i]), out, outDir, progress, level + 1, exportFiles, transformer);
			}
		} else if (isResourceComponent(filename)) {
			String entryName = path + filename;
			if (entryName.startsWith("Java Classes/")) {
				entryName = entryName.substring(entryName.indexOf('/') + 1);
			}
			ZipEntry entry = new ZipEntry(entryName);
			InputStream in = new BufferedInputStream(new FileInputStream(file));
			out.putNextEntry(entry);
			if (transformer != null && filename.endsWith(".class")) {
				transformClass(transformer, in, out);
			} else {
				JTFTools.copyBytes(in, out, file.length());
			}
			in.close();
			if (exportFiles && outDir != null && !filename.endsWith(".class")) {
				in = new BufferedInputStream(new FileInputStream(file));
				outDir = new File(outDir, path);
				File outFile = new File(outDir, filename);
				outDir.mkdirs();
				BufferedOutputStream backup = new BufferedOutputStream(new FileOutputStream(outFile));
				JTFTools.copyBytes(in, backup, file.length());
				in.close();
				backup.close();
			}
			if (progress != null) progress.setValue(progress.getValue() + 1);
		}
	}

	private static void transformClass(Object transformer, InputStream in, OutputStream out) {
		Method transform = null;
		try {
			Class[] types = { Class.forName("java.io.inputStream"),  Class.forName("java.io.outputStream") };
			transform = transformer.getClass().getMethod("transform", types);
		} catch (Exception ex) {
			throw new ErrorException("exportJar: Illegal class transformer object");
		}
		try {
			Object[] args = { in, out };
			transform.invoke(transformer, args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private static method: countResources(file, extensions, libName) */
/**
 * Counts the number of files in the directory tree rooted at file whose extensions appear
 * in the specified list.  If <code>libName</code> is non-null, the number of components in
 * the corresponding jar file are also included.
 */
	private static int countResources(File file, String[] extensions, String libName) {
		String filename = file.getName();
		int count = 0;
		if (file.isDirectory()) {
			String[] files = file.list();
			for (int i = 0; i < files.length; i++) {
				count += countResources(new File(file, files[i]), extensions, null);
			}
		} else {
			for (int i = 0; i < extensions.length && count == 0; i++) {
				if (filename.endsWith(extensions[i])) count = 1;
			}
		}
		if (libName != null) {
			File libJar = getLibrary(libName);
			if (libJar != null) {
				try {
					ZipFile jarFile = new ZipFile(libJar);
					for (Enumeration<?> e = jarFile.entries(); e.hasMoreElements(); e.nextElement()) {
						count++;
					}
				} catch (IOException ex) {
					/* Empty */
				}
			}
		}
		return count;
	}

/* Private static method: getLibrary(libName) */
/**
 * Returns a <code>File</code> object for the named library on the
 * class path, or <code>null</code> if no such library is available.
 */
	private static File getLibrary(String libName) {
		if (libName == null) return null;
		File library = new File(libName);
		if (libName.startsWith(".") || library.isAbsolute()) return library;
		String classPath = System.getProperty("java.class.path");
		if (classPath == null) classPath = "";
		StringTokenizer tokenizer = new StringTokenizer(classPath, ":");
		while (tokenizer.hasMoreTokens()) {
			String token = tokenizer.nextToken();
			if (token.equals(libName) || token.endsWith("/" + libName)) {
				return new File(token);
			}
		}
		File dir = new File(System.getProperty("user.dir"));
		library = new File(dir, libName);
		return (library.exists()) ? library : null;
	}

/* Private static method: isResourceComponent(filename) */
/**
 * Checks to see whether the file should be exported by its extension.
 */
	private static boolean isResourceComponent(String filename) {
		for (int i = 0; i < RESOURCE_EXTENSIONS.length; i++) {
			if (filename.endsWith(RESOURCE_EXTENSIONS[i])) return true;
		}
		return false;
	}

/* Private static method: submitDirectory(out, zipName, progress) */
/**
 * Submits a ZIP file containing all the relevant files in the current directory.
 */
	private static void submitDirectory(PrintStream out, String zipName, JProgressBar progress) {
		try {
			OutputStream base64Stream = openBase64OutputStream(out);
			ZipOutputStream zip = new ZipOutputStream(new BufferedOutputStream(base64Stream));
			ZipOutputStream dummy = new ZipOutputStream(new NullOutputStream());
			File dir = new File(System.getProperty("user.dir"));
			dumpZip(zipName + "/", dir, zip, dummy, true, progress);
			dummy.close();
			zip.finish();
			zip.flush();
			padBase64OutputStream(base64Stream);
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/* Private static method: dumpZip(path, file, out, dummy, topLevel, progress) */
/**
 * Dumps the file or subdirectory rooted at <code>file</code> to the output stream.
 * The following tags describe the parameters:
 *
 * @param path The current path (/ separated)
 * @param file The file or subdirectory to be written
 * @param out The <code>ZipOutputStream</code> to which the data is written
 * @param dummy A <code>ZipOutputStream</code> attached to a null stream used to calculate checksums and sizes
 * @param topLevel This flag is true at the top level only
 */
	private static void dumpZip(String path, File file, ZipOutputStream out, ZipOutputStream dummy,
	                            boolean topLevel, JProgressBar progress) throws IOException {
		if (ProgressBarDialog.hasBeenCancelled(progress)) return;
		String filename = file.getName();
		if (file.isDirectory()) {
			String[] files = file.list();
			if (!topLevel) path += filename + "/";
			for (int i = 0; i < files.length; i++) {
				dumpZip(path, new File(file, files[i]), out, dummy, false, progress);
			}
		} else if (isSubmitComponent(filename)) {
			String entryName = path + filename;
			ZipEntry entry = new ZipEntry(entryName);
			InputStream in = new BufferedInputStream(new FileInputStream(file));
			if (dummy != null) {
				dummy.putNextEntry(entry);
				JTFTools.copyBytes(in, dummy, file.length());
				dummy.closeEntry();
				in.close();
				in = new BufferedInputStream(new FileInputStream(file));
			}
			out.putNextEntry(entry);
			JTFTools.copyBytes(in, out, file.length());
			out.closeEntry();
			in.close();
			if (progress != null) progress.setValue(progress.getValue() + 1);
		}
	}

/* Private static method: isSubmitComponent(filename) */
/**
 * Checks to see whether the file should be exported by its extension.
 */
	private static boolean isSubmitComponent(String filename) {
		for (int i = 0; i < SUBMIT_EXTENSIONS.length; i++) {
			if (filename.endsWith(SUBMIT_EXTENSIONS[i])) return true;
		}
		return false;
	}

/* Private static method: getOptions(program) */
/**
 * Gets the <code>SubmitOptions</code> for the specified program,
 * creating it if necessary.
 */
	private static SubmitOptions getOptions(Program program) {
		SubmitOptions options = optionsTable.get(program);
		if (options == null) {
			options = new SubmitOptions(program);
			optionsTable.put(program, options);
		}
		return options;
	}

/* Private static method: nameAppears(name, array) */
/**
 * Returns <code>true</code> if the name appears in the array.
 */
	private static boolean nameAppears(String name, String[] array) {
		for (int i = 0; i < array.length; i++) {
			if (array[i].equals(name)) return true;
		}
		return false;
	}

/**********************************************************************/
/* Private constants                                                  */
/**********************************************************************/

/* List of files to skip in copying a jar */
	private static final String[] SKIP_FILES = {
		".DS_Store",
		"FINDER.DAT",
		"RESOURCE.FRK"
	};

/* List of extensions to export to the jar file */
	private static final String[] RESOURCE_EXTENSIONS = {
		".txt",
		".dat",
		".gif",
		".jpg",
		".jpeg",
		".png",
		".au",
		".wav",
		".class",
	};

/* List of extensions to export to the submission file */
	private static final String[] SUBMIT_EXTENSIONS = {
		".java",
		".html",
		".txt",
		".dat",
		".gif",
		".jpg",
		".jpeg",
		".png",
		".au",
		".wav",
	};

/* Class variables */
	private static HashMap<Component,SubmitOptions> optionsTable = new HashMap<Component,SubmitOptions>();

/* Size of the copy buffer */
	private static final int BUFFER_SIZE = 4096;

/* Font substitutions */
	private static final String[] SERIF_SUBSTITUTIONS = {
		"Serif", "Times", "TimesRoman", "Times-Roman"
	};

	private static final String[] SANSSERIF_SUBSTITUTIONS = {
		"SansSerif", "Helvetica", "Arial"
	};

	private static final String[] MONOSPACED_SUBSTITUTIONS = {
		"Monospaced", "Courier", "Monaco"
	};

/* List of jar files to skip */
	private static final String[] SKIP_JARS = {
		"acm.jar",
		"acm11.jar",
		"swingall.jar",
		"patchJTF.jar"
	};

/* Static variables */
	private static boolean fontFamilyTableInitialized = false;
	private static String[] fontFamilyArray = null;
	private static HashMap<String,String> fontFamilyTable = null;
	private static HashMap<Thread,Applet> appletTable = new HashMap<Thread,Applet>();
	private static Applet mostRecentApplet = null;
	private static SecurityManager managerThatFails = null;
	private static String debugOptions = null;

}

/* Package class: EmptyContainer */
/**
 * This class represents a simple lightweight container.  The only difference
 * between EmptyContainer and Container is that Container is abstract and
 * can therefore not be instantiated.
 */
class EmptyContainer extends Container {

/* Override method: update(g) */
/**
 * Overrides the <code>update</code> method so that the container is painted
 * but not cleared.
 *
 * @param g The <code>Graphics</code> context passed from the component
 */
	public void update(Graphics g) {
		paint(g);
	}
}

/* Package class: SecurityManagerThatFails */
/**
 * This class is an instance of the <code>SecurityManager</code> class
 * that invariably fails if it tries to read anything, even if executed
 * inside a <code>doPrivileged</code> block.
 */
class SecurityManagerThatFails extends SecurityManager {

	public void checkRead(String file) {
		throw new SecurityException("always fail");
	}

	public void checkPermission(Permission perm) {
		/* Empty */
	}

}

/* Package class: DOSCommandLine */
/**
 * This class contains code to return the command line from a DOS prompt
 * on a Windows system and was originally designed by Erik Forslin at
 * Stanford.  The code requires the inclusion of a native library called
 * GCMDLN.DLL; if this library is not present, the code for getCommandLine
 * creates one in the current working directory.
 */
class DOSCommandLine {

/* Method: getCommandLine() */
/**
 * Returns the command line that invoked this program.
 */
	public static String getCommandLine() {
		try {
			System.loadLibrary("GCMDLN.DLL");
		} catch (UnsatisfiedLinkError err) {
			try {
				String cwd = System.getProperty("user.home");
				InputStream in = MediaTools.getHexInputStream(hexjar);
				ZipInputStream zin = new ZipInputStream(in);
				zin.getNextEntry();
				String fullPath = cwd + "\\GCMDLN.DLL";
				FileOutputStream out = new FileOutputStream(fullPath);
				for (int i = 0; i < UNCOMPRESSED_SIZE; i++) {
					out.write(zin.read());
				}
				out.close();
				System.load(fullPath);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
		return getLine();
	}

/* Native method: getLine() */
/**
 * This method header provides the link to the GCMDLN.DLL file.
 */
	private static native String getLine();

/**
 * This hex string corresponds to the compressed contents of the GCMDLN.DLL
 * file.  It would be easier to include the DLL in the acm.jar file, but
 * it is safer to make sure all data is part of the code.  Some adopters
 * may not know how to work with jar files that contain data.
 */
	private static final String[] hexjar = {
		"504B0304140008000800079966360000000000000000000000000A0000004743",
		"4D444C4E2E444C4CEDBD0B605355B6307CD2A4252D2909D042C50201AAE28098",
		"366993347DA4D0948A14022929155AA83498D6D2D6F49C429562EB69B5E9218A",
		"8E33E39DAB73EDA05E67F48EDC511966E46A4AB1056490878F2A8C96B1EAC120",
		"14A9A540ECF9D6DAE7A40F1F33DEFFCE7FFFC770CACA7EADBDF6DA7BAFBDF65A",
		"FBEC84BC3B7650728AA214008240517B28F1B1527FFFE10126CCFCD304EA95C8",
		"3FCFDA235BFAE759F9EEB21A6DB5A7EA2E4FC926ED8692CACA2A5A7BA74BEB61",
		"2AB56595DAECE50EEDA6AA52D782E8E8A80489C685E7A3C25F3D58EA0C414ABB",
		"D9B99BC45DCEAF20DCF5E6DDCE36125A9C1748A820E5BBDE34395F2161060957",
		"966D7063FD1FE2D56EA3A8A53205F5ABC77F9E15CAEBA126CC1A2F1B1745D911",
		"41CC6B9F081F1A80F524A921718D383E14351252BB243C78C2245431AD19CE27",
		"646A296A1384A67B286AEEDF1ACCA70127E26F8F373E47AB292A7774C67A711E",
		"7EE85940BBB6D0103E9E2731641FE13BF46881CA024F69095D42517F9E22D2C4",
		"4C690C861F2BFC5B20A25169384E0D12AD86EFE0F917785C15551BA05D95D837",
		"82B7E33B780BFF06EBD79E6BCFB5E7DA73EDB9F65C7BAE3DD79E6BCFB5E7DA73",
		"EDF9073EABB8B3422C387A1A3B97A7E427404ED13E257578F881F215DEBC2067",
		"5302427005B72AC885093B9E049797FBB4689F02EB7336153B2873AA9B7BA0F6",
		"9EC7E1839F02DE1ED7EFEC8AC09410AB04FACD7E75934106D9CB34DEE8382FA5",
		"E172143B1330742ABDB1733152AB6AFC4CB89FA2D8CB61DEE478C8A1558D47D0",
		"B91453ED3D4AF9BE72D94E2DC4F9D3C06AB3BF50DD848DF2EF86638A89E2AF57",
		"53D4993F63D68C09D832F66CA7E151A870161CD19D0AACDA310E91B72EE08D2A",
		"12D1F2572349646A39540B237499F1423805C8671E435A8F40397F879CA202BF",
		"8256E69B49D1B632211A4373783E7C32E1FC2C40E04B11F5568CED0006024E76",
		"308CC9766F072AE532FE6170F0B9BEE681C2427ABE53483634501A21B6D48B83",
		"E3A463F858A85628C42A8067769942F04604A6F25F40CF0AF5FE80861D94334A",
		"60F03505C9B0FA5CC1E2A27D2AAA5CE5E6A1027F170C39647B6DBD1CA3D20B96",
		"4B8CCA9C8EFCA99B5E86D6BDF5414B1F1DCE0E8531896D2618104B17AD12188D",
		"536094C251A865AEEF553735E3748999FCEC2141807CC0F370451A07C9BB02C3",
		"0B79EC908C9960E962A21C05983BF88D00ED21713973B388F82964C150C9BDF5",
		"48989E1C6A32CA31DC209B1714385B6F601A673BCB29B948FB0ABE01C8171676",
		"702E1E0BF57EFECF90017D6C53413F40C6E868A039814DD7405206239E3909D8",
		"A51314FC1BD079B740644D07830A34DC8A121CDC182279F414773F8AB810AB11",
		"D311584B384A3E63559077B85CC9D74F86D63B20F2C6248C38B9BEC0782E02B0",
		"C384A36CA7C24227283DE78B3BDA748F121AE3441A85CE823FAAA0CD37E2E043",
		"EFB7BCCD5DA8990A6C6672139A0F42D56358F592E74B141C77E64C98A91B6328",
		"EA75106B0DD7F986B20187A5B0463DBAA14B9E0B7AFFBA6260C5AD815EF0DB41",
		"9280ADBF205B7A0166523F90634E5E8D52A7C1D18C8D173B1707813719A5B1D5",
		"A6F9B51D65B35EC518CDE97381637A665B02042C0FAB0EE7221C333D511C05B3",
		"F2A9703470DAAD055EF6C4C3072F1304A1D03D17D30998BE00A2508853C2C7C1",
		"189BEB35CC2C2F691FA7FA3128D40BC852BD8656029F0D1371F8F889C0754739",
		"883D8C938AD7014976BFAAA35C0659143F4B4A0AB15A601AF4481B2E067650A0",
		"274047DC38DDD164799CF90BAA92EC044597357F3D647459634089501D2047B1",
		"73A1AAB88AB86E21763DAA10585CCC6DA1E536533EBCDC6EF9D672A3E3BB7244",
		"7A3984DE9855A72CD7F05E10E04287109BB01EE7A9784D47B9C6ADC5C9104069",
		"708C125798BA49850A2D270186709C9DCF04D551C8E5CC17538962CA20A6E688",
		"A93431152BA6B2C5D43831952BA62ECB482A3F1DB9C30C1E33CAA3F98F60200A",
		"61F2B99CF5E2B29C2C2819B525FC804FC4F3231E992416BBAFE2FF032B74D982",
		"D83F2EA7D46BEB814A7982D2BC653E1DCEE5CE1722CD5B0C1833604C87311DC6",
		"AC18B30A915CEE52C1FB0A54B684071F803626A53F8FC178B38981E63C1343CD",
		"6901C5C96761F37C3C8C0ECCA65E8021029E195CD01D1857F165629C4C2B7B59",
		"A0E7E0ACC2AA18B7429C3B3BFF2F2049E5D4E889EAE07F46F43C88D9499810BD",
		"BFC359E02E1D5ED9DC60B340DFCD25E3AA76AF86EC02E13DF7520C77AE45E97C",
		"CF6D2589F562C24412A524C1A66326B513458D99DB66C07D0273DA7418439CB6",
		"F910EB0A4742F7C7401E62BA6F8EC5B6D7127524EC4446E92CBD40046E7101FF",
		"3B6A58E0E67D5BE0A64902E7DD16335AD8B8AEC0385E0862DF70C5ABDC5B50C8",
		"7E194584AC79805ECDA62B76501A39632D57F02727E098BB8230DC67A6A034D8",
		"7A8172A4D3CE874F45CD49A6633C30C1D6F752CC74589D8128A8763E9AE8331C",
		"5A6A07B264C20DE39E285C9F1A770B3678B3D8A0FA2546E5657A6011C97250D3",
		"80A6EFD9EC0AB1B098ED54B39FF579191557D46399FE0A08DE663DD0CF46B666",
		"D5071D7CD20C912BC2C8121C8DFA5E98A24902D3534E15F0B7CE02D6F6AB9A05",
		"26D559AE0C719326CD24738339BD9783119DEEE43F05A12A0493A1B916144560",
		"026C69D0CE4DA41F5C17BF1B36D90EF66CAEE3B506D80D1D7CDD6C71D356B207",
		"40618020EED4EEC0B953C2274AA7AF05F1023358A5C0AE57529D560540781417",
		"8D68AC3FA1B11DCB2DED9E8F56390B7CD90931A0839602571BD9AD093994BAE9",
		"0C4574502EDA307FC1B8C9E756F814B77AB313347B2825541DDC1AC60D9AD3B1",
		"C1FA65AF2740E0E03F9945B8A24DACC8883742E263CAB7F998003CC8DF82F63F",
		"03F52F52D9FA4620822331FDC966615B3934AFE1285490F9AD79D440BB8C5E3A",
		"D0AE64C603A7B9E29E96C6BDC575B03D337D0A7FFBE5704EE5C3CEF90C8D624D",
		"6F44AB8D6AC951F87254EEA76125DAF9495AC25E21BD40C851B280A45864B37C",
		"BCF5E7FA93E2B8F81427A1369B213055CD273B73148D4C54F9C0EA801AC63850",
		"C775E45EEABEDECFFABF0606B2C904B2824067160CB363B801F763307FBC194C",
		"44C33285351089268C02A2CA11267C3325267E034C04A2081EA2342C62AFCAD5",
		"DB3D30FB42B2928889753DEEAFEB8A8BD6B0FB73410FBC8E4C7219205E89BE87",
		"C8B84E63EF575274946FB10A98B22353116C57423899DB41CF5F8514FE495868",
		"EC8DB0863A158D435AD800B7BE0EABCEA13F699E3E1F52CE02302A6E0633804B",
		"464B1174FC4566524097964ECB163BF94498CF421F1D216B8D48ED60BEF029FE",
		"0B5F06D9F9ABD0076ED072B5D09B6E2296208ED06A91C4AAC04F9CFC208C34C7",
		"DB1A32D20BE9EB56F16F01BEA5A3D03B8E36C2CA3D3993ACDC2E85FC6BD30CE6",
		"B840AAF117AF82F1351DA3DE995DE1C8296A71BDBFB0681DEC45423236C41F21",
		"4818C5167BD1547770452AFD71F32A6581770A77B94B8EB5E84830ED599B52E1",
		"353798663313F4079B8FAB1F6FB772C7FE7376E01621024630BC55D9A25EDCAA",
		"54EFFEC03AC09E8685AD00EB502C896B99B9D8DA741008D349CDC799BD0D97B5",
		"7478C3E548E6F750ACE85C2C50A8911A4CB04EDE83D65A95C30811D6C085DCC0",
		"6952F41AD14ACAD1CC0872FDC945FAE38130EBC206D35AFA323277E3C03E1933",
		"93AD0F52B4CA972B6B30CF66145C7B204C7F527FD09C1704B6C11939F645F371",
		"3005B1F5716B170B114B988BADCAA683742ED663A2B0FD74689F36359FA41700",
		"DA75EADDC7061E107B1501FDB40A11A4BF01B57AF7C18146B1400EB94284F58C",
		"1B244E244D0169E8F519B455C0FF019996B35A4A90AF5B73B003C77A85C359A0",
		"17CCE92839203717D00655C080BF5EED4551EA0CA75B61871162F34112DB0CC4",
		"FEF526E7A25B330E8684EB66C2B88F7CB6A0BDC0C70CEA0F721DBC05F63F8E01",
		"5F6BB0FDB33079A79D6F9B8E92256A4C22553720BE4F958155A0421456B00573",
		"0B77CE07B2207F3AAFB84680C3152BDAB640FB0ED0685CBA1BD7CE49FD80A5A3",
		"3CAC88B945789FEBB35CA4555DE18882321298219A53ECE01626927B7327E607",
		"C20905B0BB997A58084AA956DD46F30CAE939E207F73A3398EB92C065F71E9A5",
		"50DDE170CCEB741C1BB4DA9D0E07684703D4E12D013A6915FF6EBC28F5A068E8",
		"390EC72A3B9885098025BC8F363D2434FCD020D85CDEB5A0AE00C0BE5B8F4BB8",
		"2360B7BC492B2D1DB402DCA06CD87C311719E933CDA0A3ACA638E62A7E0CCCEB",
		"B472FC2A7E53BCB41A19855E088C87EEF3CA1964DB018AABB16E17EA10701160",
		"B37D02F7BE5BC04CECB2F5CEBF87D234D693A066368A3438271C28277A3C58C7",
		"E3C0B50AF925ACAD571178974F94E3FEA9723F8F34CE84111A069186E1C7D2F8",
		"320C37B2747487C39868361DFDE1708F0211CBE5AB3BF47E33D848E594FA71BF",
		"1B5FA0825AAB26DA7527EE9CF44FF87634A307E53B717F6666B807D1EF7C48F4",
		"8C1975687FAD208AB303898676F232A2F25E04C15FC5DD82D6C0369B934BDE05",
		"6910161AE5A053E5C6F7B36E7C712BA408EFBBD10F2F2751A156316C4ABCCB76",
		"C62CB2104A5B8FAC2B16088D91D2A235A399E8387CF8703AF6A366C2A56EF97E",
		"D6AF68A61EE7287BC70ADFD20465E3192CBA053F9A6522DAD97907B8FDCD32EE",
		"34F88756859DAC3A5874BC7011E4BE5EC9F571CEFCB65EB4FF0F99CD74B42FDE",
		"CAB6AB7CF1BFB4BCE539EF53587D8A072C076A145014063AEFA0BA69129AEB85",
		"CAE693DE22A5BA09D72A7B359C51B1254A749DCF0451475D95A99B3E2516F36A",
		"AF0DD6A2CA9BB39ACB52B0834A75F31B981FDB0D0DB69D820FF93ECBD1FA9B7C",
		"8A95ED3D61DCB67CB6D4A4A4B8742CE6A663F942F9C7AC5F6539B1F5AF5C1138",
		"B88D971FA6283FB76D2D13D995B3168DF44069E3E51D9047321A31C3D178F9D1",
		"504613662C6CBCFCD350067AE781C4C6CBBE50C60398A16DBCFC4828E341CC50",
		"375E7E0C33C641462BFA36B56BC1D83A51E8DDB636300EFB6A174E80D628848E",
		"A14E8914189510BB05E687E88D3D1B6A36F5A0D831D1B0289576FE2174FE0A3B",
		"44090267FCF06177CB6C4A53DA86C74776B42EBC15091A1FC0BC1E981CD05E5E",
		"176F8706C0F5C0EDDB6B1BF4D9FA4A77227A0797D757EA8DC66821B6B6A2C3B9",
		"2849B77A914E0753CB9E5542FD5541D0B9A06E2F5915781D42DDFC029E5ED806",
		"399B06E6C307D41705B91A15779FD2C1179185AD18EFA7EF638704BADEA7AAE5",
		"B21FC174A17395EF6E547907F5C7F5037A41385A540C74A124734B2EA9AB9F4E",
		"EA0296D3C1BB494209F5CB654041C90750F32A1EF12E526172341DA88CCD181E",
		"096C277765027380593657A1DC43F4682CB65B2E38F858916411C9C65D9BFBB4",
		"080418049DBB99CB8AB773597176BE2F5E44C28334DF8B09340C57E359742ADB",
		"ACCD20453E3C3E7338BDCD2DB8F91F2FD0FB2DAA3779743A22ACECE064CF05AE",
		"AFFD33B9E561CC5237C7623DE9A444DDF4313A6EFB50A990D31275D311C868BC",
		"8C32AE6EFA8292F62A9FADC1BEB2E5674DC405CE27BA85D140A6FB19177A6419",
		"C4ADF0D537705DE0D182F30FCDA615D6CEC6E4EC3854B480CBF29672B97CD0FD",
		"345429E00333B14B1009837261F759E29DF35F03215FB63CBE90F5CB0BD99E20",
		"DF0FB60B77CADDE4424BF97D70660A1C7C0F04EE0692F3EE945065077F10B335",
		"5A19E55E4DCA7690599F1F98BBD2D78C0364F7114430EF1E4176ED424A79BF68",
		"9482BEDC0A81EF591C58CE87A3C83F0DC618CC46E39BE8B31DEC1873B4F493D0",
		"D1D22C378E13EF06D16FDBDA8AA352888733E488895F890B020C50FDC01B0DB8",
		"3FA67DDDD4889566F8145F3740CC6B22C6ADA063BBE2F8176324035795C30E25",
		"6C3D0E6B082C4E36816082D6095C7070D3D15675BE8E3905C4A09DCC6E530047",
		"05C28902FECC8590C9AA6C1C7A1A1CF0ADA708E63A72621489988C1C56F448F9",
		"676021926359A56078135185D80668A008CF93FC30827C5A10ED0CA5CF409880",
		"9D6006E3298FE3F5B178F2005B2713CDB7C02C765178772790591EC5DF890BD6",
		"5B14044CB17305FC093C2D03178D9953C0DB80477ED57095720116BC9D9F3C8D",
		"6CBBD0C7A9DE8CC0B8027EC685D03152243166ADFC9720051D403F0F0F9C24A6",
		"9D3E916B36836226DBF9C5688FA16F573E91EF0F001A6C41B14DD09FE2A20EF0",
		"B1F1148BDF81DE22178DE75A5C5F1B6671C7E61D65F7292C5FD52CFB2371D02E",
		"D4847147E44756D8F9DD33C41961A6B29D9A1085B6506658718748685E3416EC",
		"C48FF6CB613E558B97647BCD6C38860AAEABB8C3DD80F2707532A94B50194579",
		"DCEA0E30130917E4A40D142758094771E03BAEC0383D069D20A641BD92B710FD",
		"D83B3C26602EF01F005207CF00568778B60938E86B5DEA2E04B787039DCCE5A4",
		"81E169095FBD1D34C1064B17BD8ECB996F3631ABC5E3A67888C659C29FC67321",
		"58C3CF9F0702B569FC51745F0AB91CDD08CA33124AB384B28CA008B5F3F91A29",
		"E7EEF3A8FCB91CAB25FC49449E0195A741E52976DE0E455C4EF62D43C4095A28",
		"21DA2DE1F9C8563C20C699F7E0F111032D94CEC47DC8CE5F4FD09CFC640C6121",
		"3879FD5920D3C72D2BB544E32993BAE959A2EB2C87E85BB9ACF942A4651FAC3C",
		"259765084574A1883514592AB4603B6D58DF9B534A42414C9AEDF3E928CE3A7F",
		"3869C0A46138A9C3A46E3869C5A4554A72D6A5420339FF3A444F364F64605F25",
		"0760E356F00CAA002EA7949CD8ACBF0BA69607DBB55CC59BF1D81067979FFD3D",
		"538B062ED4E08B8328EEB73C03E61A3D9BE515F454968FA655B9B41C84658F02",
		"DCCA8E3D93C8A742899F13315E506E05FFEF8D1E98868B2FFC56EFDFD9FB3048",
		"D86148EEDC0F1F6FF441F285175E5887CBA8F16C5CB8B489C0A6E9F4D978BB90",
		"8CF842EC0E62E5CB5EA764B8B946CB7021B63485F3D023ABA5D3D3DF6A3BD7E4",
		"EF6CC6B4964E71F8569D2F50EF8E52EFF65BDA374F9F77208BFBA0FD4C986F2F",
		"A9B0470BCFC517B8B7D8D3F28BBF5DD81AB6B0C9CF1C5BB7060D40E4C9D7CC5F",
		"41071E5BB63B7D84A8BD5CC6BF318BD892241F7261CC9CC3C54EB12AFFC99C31",
		"38C1D1386EBC7229E1BD3607B5CC5ABD7F23B7DD86CDE1D9C8948616F4EA34AD",
		"DBC3917A4B0BF508F833F103ED61B4462CD26211100D7CD2D9808514767E6F20",
		"5BEF6707B33CD7B183776C8E091139D0D0AE9548CC64074B3C93D9C17B374F08",
		"D139D070461BE81945E5752E2F58CC9BA6E1F642CC9A7832110E271E1B9FB45C",
		"8419295037E5E3C8C37660DE656D02A172B17EDDC2F43E1014CF97385DCEE199",
		"5237CF02549C7CB69E970D0B80978C8C773A0E82FAC1B350B1A1FE1C6CED7FC2",
		"F3AEBCF3AD139B8EAB9BFE0377FBDD59827AF7F1338F52229510059F6A65FB69",
		"85B7A8F7B7BE7FB1030BAD72EE62E0E656A7ACE9387D23CCB77AF7154BD7E618",
		"6E556FEB63262857EEC0FE5A2D5DB55FE7E4B446C04C9F1040BE3B946C7DAFC2",
		"D30E9A9B486457388A241A3DFCF530F43E6F36D4E52E10F17CF65922AACF06D6",
		"8983A780255CDB9F95D55027802CDE06A2A23F98C5B58B854A6B3A6EB760E474",
		"F2C760A648556F34520F4478A763A8F787A43E106D9E7E0ACF0E27F0EF002E3F",
		"484E5403723070613660BFE79F8FC3F9288F7157E372357F0D4A38AF47288FE6",
		"1FBC0C8B91BC56F1A6230934E7068798495DE198125F0E6C87A908CC6307BF19",
		"9BDD82D931EC6090518F64B73DB11D579DD2128EC3219E7C70C9784AEF654E59",
		"2ED10A7306AD716B4182F9763CD0E4FAC4FCFBF165CA61D41FE4A49EA9F266B4",
		"9129CE51B46167BD39CA36320439B0A5A266190CAFD76CE454361C828DDEA539",
		"1A6B802725E932E0A33EBAB50527BA65A92EDE1A382316E0A2AFD7B412596ED9",
		"AE9B0E496BE0332FE18EADEF1198180B89D32A275F7D065809C8BDF53DA37458",
		"0F9FDA8F3A2C9AFFE525D4612C39FDA09849E5DFF04FA352EC0A9F3F7C58D5E1",
		"B6CAC4453C72D00BBE2BBA888CAC034C61057BC3F390A0D81BD081A476EE2173",
		"EA8DC693E5AEF0BDB8F88090B5A30D8B7DAA86B65D246C094CE2F21314F3ECAA",
		"C62BE89F7A22597F8CA5DDF3053449169C86CB836D41030B4E55C00DCDAB53B1",
		"9DC1F6F36AEEEDB2830A1877DF76593630E6CDEBE326DC06EAC29B175437BF0F",
		"050E5F7C2237D9BBAA9F7306BDAB06B955FD03FB64DE221543B75F512C61AF64",
		"D6CACB33EFE06E57586E57320BD92B5A9CDB869AEBB8432702BEA5618A4B2766",
		"ADDD933D14C9CC0106665D1788F72DE9192E7972CF7EC086C2082C2C54A09BE3",
		"50726B1420A0F2BC7EEF1D0A6E958A83CF954AAF4389EFDC8F8D69B56890FD04",
		"98E9573783BB47CD630621A7FDAAA23CD3CBA86E2FB67C541BC67D24CF1B146B",
		"59DE030290F39EE543BA98CB5371F7282CF728190B7B55FB3AE1792AF7D68973",
		"97DE9B4503BF4BE50A661632951898E6BBBD87E4BF22728B4584DF7B14848A92",
		"BB4DE15D26C615DC6D4AEF32250C72400E0E2E5BDF4F314ACB87EA26B48AC0D1",
		"F3A98E718580ADF4162BBC80BD0C3E95F06959A664D6B7C2A0B4E4A987DA6114",
		"5B205E734343BD9A62267087C8989E0840A391D7913819DDEC3DD99171819B09",
		"920686751456A102D27F22E92F7C4D84EF4858C5416F84375B071E7C9F00AEFB",
		"255C99286C78867F0A198CF6438A4BC6D30E72D4D17E5A2DCF55BD86E71C8E15",
		"C27B6239A922D18EB42B4992B36AC442F699166C8C0AE50EE52E12A3B99A86BA",
		"451413C9AE570C911C768B5260CA1CB014AA55C27B244BA8D68C7D2143849C8B",
		"2522AE68C02385030471DE01DFD2896757F87263C078F5E1513228A8FD2A211A",
		"F12DE18852AB60372963C8EA206B087531E67BD391CD35EB8A0F8A4B888BC635",
		"85B6633B63F0657BEDD08A5D3CB32908B1F2C47A3CA864147AFFBA0E565C8364",
		"C54A0430EA74EFCFC237D2621560D667681457B7A5CB9BA3C1BA8145E50A377E",
		"87801C201508B18F4BA52A6692503BD21A763C70945DA614BC19DE6D0A219CB4",
		"92A3619502D759BC4E3C6725AB5909FBE73DC0FBC980FC58CFA2667FFD65AEA3",
		"CC4F9673930E97332C04DBE01D5EABD26B55B07EE512A6BF5CC15D25E766ED9F",
		"ABE5752A728425B182AF14F165D153B85BFE82AA0623EF8A775570F322EED0BC",
		"B7DBCFA87C8B35596CEEA0C0B69C4557CEB72388371F341082E1DA650DF661D2",
		"AEE86AE0218289706CE736E62DBC20631B0C1F44DF7AB9CA9BABF066297D4B20",
		"A2F46629D8D25F65537A61B177EFAF50705A7316B51E186A6FF2C37CB52C5BC4",
		"C823B72888ACC1DAFCE2D2F1597625D741CE64885253894A0DCD087013EB55DC",
		"FBF35680629BDAFE859A3BF42DC5D6CF2D0FB29FF5DD66B9E0DB6A0E7233BD79",
		"1AF0DED40FAF82D2810E30296C10917F60B9A87EC482676279C1F6CB8ADBD8CB",
		"99DEBCC1DA88F2CC4208B9750ACB3A25B388BD2CA9B969B8DC067D4B6544CFED",
		"403D77A3A8E766B2ED3DA30B9FDCF12D55B75C09D4BC185170F72BBD754A2E4F",
		"33EF882C2F8827F6EA879F256F0A83A055DA05C57256C8F42D4D0CA2D65BC715",
		"F5FBE2AF7A8B34A027B17E91C65BA8F4DEAE40C572BB123E2D854AA6A075E938",
		"502693419908A04CC6296AE21BEA27A332797BB432F165EFC8E6DE0E684999C6",
		"B7FC5B3AA489300DD9A1E5AE417D9C17F4652705BD4AEF525910D4DC2A952F47",
		"E65D18F466270D9E4923C6F319135AD2BE5B711C8B543703E56532EFED41DF5A",
		"7D1066AA7D48B18C1DCA844D04F2B04BC503B6204C00D1914C502A2605303896",
		"E5B8AF0C0DEF2B4740F3D111A1F19E30B2AF2C23FB4AC4F70E36EC2B30DE9B89",
		"CEDDAC04AAA876198D9C097A87F9111BCCEBF7A92E70753894DEFBC8F8DE0E9F",
		"4A698383618D9074F4100C6B041956A2A38FBC4186E83C8E9C990CEB111C56A2",
		"9A97F58C2AAC1B1ED665A386D5A6F1CABDD9714172161212EF18C01FA502D9F6",
		"A9ECE93E07ECD120994ED03105209E5A5FDA0360A7D58F678F0827CEB1798342",
		"201A440F16B27EE0040FCBBF8D68BF6E2FD36FF9381003FDE02C73F206E70C8D",
		"7F9B19CF76C4588A82DE22A5E773089904EEC3C0C431185188D18D08BCA55BDD",
		"84474BDEE984E0220DA761AF08A020E8186E3BE910B7754776A8620A47B2E6AC",
		"1A44CFCF979BCD25CE61FAC7BFC731FDCC54EED126B14CB005D9760567862AEF",
		"D367504F7C77D52EDD91AD17E61C613649AD400BE5DA758162F63E25457A00DC",
		"01F39EF3D8831BA10791A46484F33E4B37338157831BC47593636E3ACEC1FFE1",
		"0A7AABB76BBC32E8096B12D4CD09686AFBCFDC0BCD1E3BBDB8F9202853683DFF",
		"B2828B9A0756D18590D4E60D6E4341C1AB2E6853724B1496254A662D2C3322A1",
		"F537726FE3FA09FAB69A4014BD4567E7AC6DC9F6020CA9193D977716CC8259E3",
		"03F361518968DBC9862962CFBA6E480D3598F178389E7776D6ED0AC98C188476",
		"94DCBD0A6F9D021AE4EE457DA16EF211A32224B04B945E40588108DE154A89AF",
		"625160C70F0B6CFD9C86FAF13070442261319D8FB48C92DD60E4D6966C342900",
		"257A446E23EF438BCCB7B745925B622F9D38176904FDD67C901EEF8DF22E9D88",
		"BA005424D3273F86CB1DB29260B5F7E39D2F5F9DCC9BC14CB34CC71D989904B4",
		"2CC45A60C649D62EE47895BE850A22FB6D3D1C795DAE166F93E1412379FD03A6",
		"2CB97F0986AE0A2DE5E6E3F472BDBF353B4159C05D66AF283C89973E00AF9B56",
		"CD3BD6328EED92B16764CC007700EF48B663A081E010FB573938EB74C4C517A0",
		"7A9488F705E20199751D78FFA643BC02F724794564DDA7A4CA35EEE7D1418AE3",
		"05A18D27BC3146F10D958C4E1875F706788E756FBD6BF856DD4E09396A0F952A",
		"A57035E0CB05F1C25F4F20016FFA511CDE20945E8535D6F74EC557154A3026C8",
		"552B7072D1D978F7737250E2DE83ACF83E27C795A177570B2CC9E4E6C98DE50A",
		"FE50403A6E71F27FB84A3C27F1C0452F1EB8E0E924B9CB22D695D111E20D16E7",
		"589F84DF88CDE12D2505DFF105716BB626287A36CF26E76F8C740039C50CA346",
		"6B485ECE9098F7114C93908C9756F05D09FFA6405E951C1EF514705B1394814A",
		"DF33E40BAF9C80735970A91DBF724B4F6E95B1EDB2263F9D2E6630E7A15CF627",
		"6168689BFC28DB27E8F781C2F80D259335D23C971504C4394DBDF4B4DF804142",
		"D1137E832E281D16F8B3AF0ED4A0AF6E28A0F4D57D1390FBEA82D0884A6A637A",
		"EB446CE338BDB1652A4CBFD4D01781702F2415A42D4E26B5C64D1C6EEF34D430",
		"340DD0375DDA475A9B842169AECB3B55149F8D62A4737118359C58D7D12285A1",
		"FE637F25561242DD5D16EA6E388E893434A111FA9BFD4F82FE2788FD9F3CD2FF",
		"2C32AAF3DA3B7C5943C3B16F8663412916BACF5C807E211820F806B19D3B26EF",
		"B40CD52A2D83EA07B6828ABBD445588B817503AB87BDACF4DC7CF15921E16769",
		"19F8E6F84F58C89E5178546C8F5C7E404868B6A7C3DC27F89642F10E088F4272",
		"C77AF8D80EB00B60CEB1D6889671AD39B296C5B2D69C3020DBB2388CED94B35D",
		"84F8E161E2BEDBA8615C8225633BC3D8AE30C47A6E186B8788C376220E2E6728",
		"6D194D430F8116200E4003A004A0000681973E002EFBE15E6FF623BD10F210F2",
		"109E85F02C847D10F641D80F613F8483100E4208C6CE23419F02153025EF930F",
		"864643C04EE703AC07A03388DB51BCEE60C70EE470745CECBD94033C8E191118",
		"8E10AE8F4E0CA2BD2ACD41C2C81C447F73F1D920B4DB9D89ED5EFA0046BA2593",
		"F437342797C539998773722A83CC0922EF388BE3001113406B8E7CCEB196C572",
		"F61C19BDF364F47E3F8A34F226E18CCCD539C024B3D0320A73C7283C401A99B3",
		"7372F63CB2AC7EC009CA602CEDED103C0EF024C033002F02BC02B017E0ED4C32",
		"37F130D6F110C6411807610C8431106A20C47215842A0895102A215440A8183B",
		"37E218F1F0D9978927929426C63A6A6E72E4C0F5A8B920E9B1F3303A6FCCDC89",
		"EB075FD2E2255E77AE55BC019C337DE40D2759EF59E4D52D79F349AB511FC02E",
		"A6F186EDC199EAC077C4D9091A7BF990DB0E044A4113C353EABD812CFFEC042D",
		"B75AC955ABF085EE020B9D9040CFF5196A39D5CBDEA5094A6FAE8ADDFAB28262",
		"26B9893D92FDB292B7A21EC87F5919E8287D842814708EF19294DE5FCA9137CD",
		"8D750A6C88D1702B54DC4A9579859209DF235E225EF19AAE191CD02829022628",
		"9AC28B14DEBB55856BC46F37B067B5E4E4962B560E74C899EB4BDBF00EB4D7A6",
		"8431E17295960FBC79418FCAB25A51330E2CAA3EA05BC06D53B1020C0CDE29F8",
		"881CF37A57812BC729F1DAABD27248CDA2FBC56E5150B45CB02917E22581F76B",
		"7BC8F5A8182E67D0620BAA1F980D2816D8ADA6E15BDD3E3C97BCDAF839D50776",
		"DB40B37FDB2473DAAB831C1E2FE2EB971C4BDFD673E5F13E5B8FDD21C43E2F7D",
		"DB623DD46C046B8AA2C0DF70A0E767EB3F4C3BB9BCDE8D8DE6BC3BD4CD993884",
		"5969F2F64613FE3C82BA7901646C6CDC12371E2A60FBF33ED8C86E89A0D47FC8",
		"8D01CF2E4EFD6034A9A1B2747BD4DC0AA5FCA8E5C39A8881AC9B1A98CDE532B7",
		"693B6EA8CF88AD370B7837291A196F3EC81DDB361984B41F126613AD5CC2F260",
		"CFDCDFDF7C9CB9A55CBDC6F2D6FD61DC07FAE3CD27B74EF2297E86DDE2740B2D",
		"27BC266E68DB39F6B2A67E5C96975002991B69E4CC5343E48CF7FEE601F5C38F",
		"626BD3313BC4C80964E25EC896468A5E4046D1572D340F6CD58432E5CBEAFA9B",
		"07EA35ECA0A63EC2BA53C20884D171FA838007FCF8083F9A2CCB11E0E7CAB673",
		"211E4E9C990EC4C1F518716134E4564DDA76729170937BAE1B597D915C8EB45C",
		"5537FD941C0493BBCAF16E7257B9D9BF13B1E9AD6E9598E11EBC0BAF285BB78B",
		"5794C9BDA09DD9DBC9E59E537785EA2CC53AD1EEFD62C6CE5C48B6E1075E0245",
		"13929EEECB0BAE2857F9F2FA5694CBF02EAB581831601B9431D789D78C149EA8",
		"86BC49DAC0CD3084C910530466B65945BC29C251F41EE8096DD96246B8037304",
		"FCB6015A732063C836E9389A1A0578B706BF28E374701F707442CCA54E397191",
		"1234CC78D08BEA667273E5A65630BF64216B959E07FBF9AD2226F32979ABC494",
		"B09FC8C12A1A46030C231B90315FA2C25853BCAE43DC19E8292122EAA627A8E1",
		"3DFB0B91082D1642B58135C5A231426C1D6CF3DF4483272264F04C613B2503E3",
		"14183CF3C1E09921193C2AD1E061F67BA706E21AFF8A3F8D02B10910A3484C01",
		"8B1AA982C10A74F1447E9CD80633009D50376FC61B0712D7E54AB709E697DF7F",
		"4A7CF38B5F8AE037E2EDFCC23197C2CAE5D297512CD1F8C28891759CC10B5EBC",
		"E66DA8E757E2CB8B5CA4B3F514B9FEA43F69B9CA4C8029E1EBBE06BBF34C132E",
		"4E4665B9C8A80AF8DF1FC3AC2AD433A2DD2CAE6E6F115EE9563F74015F0629F8",
		"D61E349A8B8205FCBFF0D28B870E75D333F8CE5334B2973A0BEC7CFD97E4CB1A",
		"2808DEFADE80C9C92FE9138D6D4B07FD136E7130D76B3B65E9F484719DF60281",
		"E9E57F312008057C0E8F6F407B80C05D3C12883317F532B7037B11FA811C2FA3",
		"1ABE4DEE748CB6C645B273BE87AC15C90A4C0F3F95D08B21A6FE7260D65CD4C3",
		"CC06C272203C42B5C031FA4C13BF18859499F5E6E9E496F96A277FFF57D00DD4",
		"139FC2520ED8F427D130AC473760EF47E806C0E40EF56C9E2251FE164120152B",
		"918A76F20691147358EFE7A79E44EF45E35E8D93F5F687C0ABE4BAA402E5A91F",
		"4B2E0BEC294E7EF6E7E2B0E31541AE36C89E8BF432BDE05B32A21F8326315BDF",
		"43319305FCF257A8F95DE40A24D7C9FFC787C471E905BA518463723F00146253",
		"A30C5FB82BF8DFBE8D2FF295FCAF49A8E27F49420DFF180963F85612C6F10F60",
		"98C2DF47925ABE868409FCDD249CCB6F20E17C7E0D0975BC838406FE7612C6F3",
		"8B4868E22D6F8BF706F428B2FBAD42AD959F4BB2B2792D0973F938122E25528D",
		"57029424CCE72912AEE6078F60B896EF23E17A9E276129DF434237DF4DC20AFE",
		"2809ABF90324A4793F09B7F07B48B895DF75446461371EB5F1BFC6DCDD7814C3",
		"FF8244C965A0ED248A3781F84612C55FA8E16B49947C91F16E124525C3AF27D1",
		"2731BA8A449FC6E81212C515C36792E8F3183590B6E71787E683CEE722B8E817",
		"1F02A991D16A4BF8D687C4EFECA88E907B040AA92C4B81850D52E1F93F6361AD",
		"B28D94552BA12CB9899439F91358563C32DD8750DFE6A842745448A745A2F3EF",
		"844E8E2654A8C1C2ED5261AB5818132A8CC1C21D52E126B1302E541887858F4B",
		"8576B1303E54188F854F48857AB1501B2AD462E19352E114B1630952C712B063",
		"4F4B1DBB7298746CF4F74F9D7ABF5D7AF0FC156CD6B028BCF9B44FA67E419110",
		"B88067030705C807EFB235821475CAD43B150935E7B87676BFB6F8E0288F1C5F",
		"1CE225BC4BFBC89E91C685A5CA980550E9C6D412197343542F3DBD9DD7A46685",
		"31D741E6C4D41239A3012F94DDA788EA658E73F815A41DD7FB8FF5B07E59C7A5",
		"7DC4C28D6B0D035E80CCE7C000543A75691FFE2214FDCC460E0A90FE11C8ED44",
		"FA6F02FDFF62DBC3022DC3FC6427A0625D827C8161FC156E9AE3DF07B78736FD",
		"86509FD83A2155CDAC862EE1AEC6CBC0A39FD03AD5748CB1B52E93B516C82066",
		"4137119C453E8CF9645D710777001C213CF79977F1DFE99BB86541AE2008581A",
		"D3D78CAAFD8CA6FD0B0DA4C24C5F93EB8BD7FBD7B1DD427107F0B09F9BC2A94D",
		"87A01BD04ABCE91CF3114626379EC60DB1F1AFF86939C61C844CFCA652BCFB89",
		"4A506F7DEFA2456A4E7E1A8D94649FADD7AE171617B81FAF44DB24D62F1E1429",
		"BDE9581E881DB9641ED5158E5938506D186107C3C46B8960F336FD15D51EDE7C",
		"209B2D73CACBF49899384629BD5A8F733A455B44EF37335AF5CFFC3EC59B3856",
		"7681891362F71283CB5BDFDD2CA89BF0F08E1CCAC667721DAC5F8E570FEB4EC0",
		"BEE4E2B97E2FF38EA39C72F24F9C25FB1B6ADC40ACB41FF3255160B80F902FA9",
		"02A901666A417918FFE743E496531FACE8BBBA6C3DD86881C44BB96CB875E8F1",
		"448189B193AFBA9231808D0CD5380AF8754882B39D0A547245F1968F98086E7A",
		"0BF48AAB8F6B1620918E5D74F0CBC6E337BD0605F22E0DBF2738033D2E5F9E66",
		"0534652FE0EF198F7E4E1CE1E42F5EB4CA62469B677BC4AF540D6083CF1FC406",
		"BB7CAE03FCB67760BB383CE621E7210E2E4FF3C94D60BBD52BB94B7AFF57BFBB",
		"F481FC2D6E0876C38BCFB5E608FA83A98B85CDE174387B26ECD231AE7D8DE40B",
		"FEF8F5C9096461AA420BF3822F47105766B9C9FD3CCAD1F33029683D1F403932",
		"391CB8DF122972E39D05E8D2819028251FF8AE281D9044C95C14B3ED7A2E2F86",
		"B3696E33C5D14AABE52DB0C70EA2993EAF5D668B693B1012B553E85E81ADF38E",
		"4C12B52174D4046FFD3BDEA203DEA2C3E622EDB0BC691D0E717C41DE8A12C6C8",
		"9B56923730388E5A2EAA9BD0AF94BE72E953A44802A73C260ADC7EE97832707D",
		"48C882E3A0D193689411397B87638EA291140F75EDFCCE40C8C8127DB92EDB01",
		"A40B52D52BB28322A70D899CBAE933287438C46272E84AC60C97C23BE87E7451",
		"E801462BE85BCC45F1C029AEB5FA78F5232F113B371EA4F7DB55CFE01608BD09",
		"23BDC9947A937A74B8373D637BB340EA4DCFE8DEF4307150D7CEBFFB45C8C0A4",
		"AD5DB6C364E9802DF7ED36618A67391CC021A370380211226388E8207D2D253D",
		"32171DA623D1103CDC0944CD450730D5CBBFDE4924FDCC1A62E876C36C16ED37",
		"1729611E5BC42B396326154F2E3E1F47BEAFE8C79586EE3BDEB4B2D8B4EAA673",
		"642C7D7931B0DE3438CFBC308EAC38AFAD1B7AF00E91087B88F1FD92089C4211",
		"F835EAACA2A0345AAAB7C5D10A7AEBF73A1D05FC7C62BAAA029AD0A05D0EC741",
		"D30B386660CFCF119853FC6FF0EA0D3A8D742224A1D6C0E7A456976D3FB978C4",
		"9C2A2012D03D9A0134963B405F0C046E1447CD673B85763374D4CF678D93F4C5",
		"A58FAE1FB8F4115A987BCC45FBE939057CCB9B78CF679A5867B41E09F5CB5CD4",
		"8DE3DBCD172326D7E973BDC81B8E4A9AE4FFEA79EA76C7F79FA73EBD929CDD3D",
		"E910CF53B518EE82BC530094E3479EA722F1BF7F9E8A583F7C9E2AD16021A806",
		"7003AC07580D6007C805B03AFE51E7A9643404ECF43300BB00F63BFE37CE53E7",
		"AE1A3E4FEDCEFFC1F3D4F9F9644E1079471AC4D703B4E4FF9DF354C4FE71E7A9",
		"84EE7FEB3C55A27D0A78E805380BD00F10045040910A60E6AA7FD479AA384626",
		"F8B402D8014A57FD83CE53C55323723B147F50227DD967D6D798654E9F6DD02E",
		"C476E3E263827A6650887D075F8DF589DF1BC7F02809F196BD107B18F16CFD7A",
		"5B9FBECF9B4C0E0E6047C410C9C116EB8E8375E8AEAE865D762158D7D2ADE0DE",
		"B6C3A007F5072DEDF44C2FE81EE93D9DF825F2334A72B58168A8C039E036979E",
		"F2C652A0D0D5DC0DAEF2135594E6F577F04C27E68D2D55A1DC6ED8AD5FFF23E4",
		"B6E4F5885F2E693E85B775570C7FB36432F95A8994CD9FEE4563CAF714A6B80E",
		"3B7FEC53F48CFDE3D9C1B4DA1B30C3FF29BEC616D1593E51DE5D2E275F3271F0",
		"0F921309883C85757CD93A95A46E9F3A28D976054EBE11E8BF815F27817833C4",
		"DD878141277F2F441D4EBE060228A880407815D977F2EB1169F88B264EFE2662",
		"17A695CBF9E4FF128431F6123927D8257002583549E4C0AAF526501FA6FDF4B9",
		"F95969D3A61D6C38AD0D6B57643DD8332669EAA18F4CF3C70BEAD7FD6B860560",
		"A9C35950AE58CD9F3E00DDEDC763DC6EBCFE2B1EE3D29BB9A2533EDB2B30D13D",
		"38D1797BDAF054D057275C7A7FCED0BCCB5CDF3254E9EC6717D8CE89EA972EC8",
		"2F5A86BC79831E2B7EA7652DEC43AFE1C52491A8C00487C96A395BAFCC166C2E",
		"E2E9BF0ED8BE0489ECF6DA8274384C3AD89B83356877A6593A6BE45E2658AE80",
		"DD6E90D4C6BB538401BC181A24DF01BF01BF64ECCBEB5F61B587B04EE1976E5C",
		"CF93034267016FDF0B1DDB526A49273F23318E3F009B073708AEAB8ABD7FAE4C",
		"BD3B62DB84722518B5AE3072781F88E296E7AA772BB2D81EF2A5DB9C40B77AF7",
		"841CF6F22DDC31B0412FCF631498D6FBD9CBBAAD51EC65F3FD6098EA0272B4F3",
		"2E0BB40A2FA429B21025F017F6CA2DEB8A99B04BDDD21D2A22FCE511E49B64FD",
		"76B74283BBA9B2D3768512625F1185947CD1360A8B792BBECFE5F282FCDD2009",
		"C456F5E32A0A1E18594568321E86BD9DB3C57071DE22BFB77E3F2C178B4DA56E",
		"AA21E37274859D4BC6ABDC78744B6BD9FAA332669ACF7694180F98154DB2C649",
		"7BBBB97E3F3D8DBD2AD00AEE62007FA087FFD35F9189BE1C2F7300B7ED0380C1",
		"4C2B28704806A072C4023D60B944AF866919B13C1FEE1497C3296FD11EB4290B",
		"1CFC0B1F8F383B53434608FD0DF90D9793C470033A1F33339CE0EE7CF45FC4DD",
		"E986345A47AF1073702F3142A179FF580ED00465490FE2686D41816452E22F04",
		"A944BB0DFABA91B381057538508C9D985200CF6804AE0F3AB0C8592EE35DA461",
		"C004CF270990ECA3D12C5D78EA79987F7B2F5A94F587031AB6DE2FD05160F77B",
		"657887648FB97E2F3DCEC1FF6E2F71B50E83C1C2E5F5F2CF8105CADFD705860B",
		"DE2AE46CF805F3C6CB14B9FFA6DC1CC9DD96ABFE83222B90EFE48EB65F511670",
		"7520845F0D646F9635AC2BA6D5E5612DB66F5A56053B6D43D4EA40548B2DA8F7",
		"77DABEA1AC3004F7C408F7287C791356D8F1FBD9C0FF477D38C8F1204D61073B",
		"D47FB04D9863D3882E13791BB6147CFC037872CE44E2258C4BA7F7816D73E934",
		"D78DA5974A1362E462ECB4FCC41A28FA5E7F6DFC413A0F3DAB7AD51FB25EBE63",
		"B716F79E1BA37A5BC7D137814E9A239E1798829E08D3A9CD61619F990621D60D",
		"B14ED37E663C9E531FD31F047515F947FC8EA627ECD20763FDBAD0E3044662C6",
		"FB99B9C8341E56E88F5FBA20317A81EBE33A805D0D7780EBC4507E2CB018E2F8",
		"0D6EBCB60238C7CE1C3B79EC8B63DDE30F32FD972E82F9099D038CEC04CDA5CF",
		"E4C73C132C80B759E9515B00B73672D93C28B83E3F21467F12624A88A92E7D78",
		"A99BFD90E20E7127C03F85768AC70E886FA1B0A6237485A0348112EFC838B8EE",
		"F61E7CEB289DF3D0B1AD51E08F9A3EA0DF6E3A48AF900E7F02E3BB0BB88EF64F",
		"34CEF1DD5CD41BE4D5403B38C06FC1CE7F99BD20E03B82B7F59DEC3E45E3697C",
		"49C0C4DF40DE159CC040C6281BD1E36A60F617AF5BA3F777700B83A66E3AA5C9",
		"4F9F379DA26F6AEAA53F6FE73590150B59A7202B02B2DE09FCBC789DC8F3C221",
		"A807C1376210C440B8E1E9F5DFFE75EE6BCFB5E7FFBE27D74351770054033400",
		"3C0CF034C02B0007004E01F0008300CA1A8A9A04A0055800900A9003B01AC00D",
		"500BD002F034C06E80A3003C40184D5171004900568065006B01F087E99B009E",
		"047805E075804300EF017C06D00FA061282A1E20096021C04A8062806A800700",
		"9E00F82DC09F000E01F402F40344D5525402404AADD84FD472EC244AF338C0A2",
		"2A8F6D4B196DF7546D70D5007F9B6A3654795CAE05A51515040FF17362C47518",
		"4A3BA7C00E2EC54B219E535193037528081D2EDA5952C190F8E251F1AC0AF20B",
		"F81475B7CB53E9AAD0279106A8117E303C3D756C3BCFC58DA4E7CEA434F3678E",
		"A42B205E0DE0612AE9B24D2EADCBE3A9F268292A3A8AA2F2972E7738C41C4C52",
		"8EDB962D1E95A4B297E765DDB66C2467658A2EC91C1D758B96FC970D25D5D515",
		"651B4AE8B2AACAD07FDB80FF670353535679979676BBB4251BE8B25A97B6D6E5",
		"A9419CAA8D2437AF6C83A7AAA66A23AD5DB0CC96AF5D29F21565AF7095D4B8B4",
		"1BAA2A69A827D61F69E0A61A6D0D535D5DE5811257C926ED46E8C326187F6D59",
		"254437119C058469E4D1843C3295257756B8B47415E094D165251565F7BAB46E",
		"5749B5D837C433221EF2EDAAAC62EE726B6BAA4B36B808ED8AAACD65A32A12FA",
		"23F5527EB05E0D5DFA37EA2563BD6A06D8AE2DF3D04C4985762353B9411AC08A",
		"8A61FE0D3F487F5D55A50BA4F0D6121A032D8D5D1CA69F681EDBEFAA6A57250E",
		"684D15A44B5DB5651B46E14A63E4DA52EDDA40BB4AC9C88C9E7BC4317E0B6713",
		"534197D16E8FABA4144668C3DDDFC6FFE171912AE1FF0301E8D151DF11207749",
		"8DD6E3BA8771D5604338FB92606047689767535925F4590B7D2EABD496809455",
		"323538809B4BEA16FC23654767FEC13EB82A61D6AA2A37B92A69C0465CD30FE2",
		"9678EE6210B166986E12E26EACA882E6607954579555D2A42AE494BA4AC5311C",
		"591ACE32D2BB45F3E60D8FC3D2B23B3D259E3AC48BC2B52865DB700A664545D9",
		"C5FF4025159636B560C1022A2DF43FAA5496001653797765D5E6CA8CB17A84BD",
		"71AC1EF9C54D625AFBAD3F373557FAFB76C9F7FDE592FF00E587FF9ABEE76F2C",
		"46A36CE4EFC73EA3EB3F201BF90BFB917FA1BA387EB230B9223C629C32326ABC",
		"2A7A825A3371D2E498D82953E3AE9B767DFCF41933B5B366CF49B8E1C69BE6DE",
		"FC9379F36F5970AB2E31496F484E319ACCA996B4F48C4C6BD6C245D9B69CC5B9",
		"B72DB97D69DEB2E5F6152B1DF9AB9C05AB0BEF58B3B6A878DDFA923B3794BA36",
		"DEE52E2BBFBB62536555F53D9E1A9AA9DDBCA5EEDEFBB6D66FBB7F783E3C378F",
		"9D9F940594E6B66165B6C80331D01B0E1751215995A58EEAB2CA45552018A3F6",
		"8B4DA994E6BED4B174A22C9406361D692F2B28AB2CADDAECA0C942A0207F558D",
		"CBB3FCCE72207BDBC80AC9827A50B6B4A486CE227ADD5E55CD54933C312D92A1",
		"F28062C95DAE85555BB22806088DDEC1B0FD461B3875B6B1FCA8168F4D77403A",
		"373775D3A654DC6961EF8467BE360F1E2D46EAE0A1F2F26E2D2DBDB58EAC077B",
		"1E45650164BB36B836DDE9F260DEB2AADAE1F8F20D7415461DAE6A3A9499C5DC",
		"C5D4C0402D612A088D25A0E730CCAAF69421BB79259E0D6EDCD35D777A1869DD",
		"2D29A924516807E9235DA489B4900ED2807A754803EB635DAC43394A68C6535A",
		"4268E478CA482CDFCD786AA4BC025769A54B4AE433622CAFAA92643818292CA1",
		"B12ED6437CC4431C2CA7FED99FEFEAA0B9FFDB1A6BD49FEC47FE8DD158A3FEC2",
		"7EE4DF688DF5CFFE7C774EFFB9F6ACFFE9DFDFEB3FFEC550316352A3CB7E7497",
		"A57E8F4DFFD8FE8EF4FBDB3978B15BDC3F26E6519AD8BC91FDA46E05A5A95F21",
		"FA61B5CB29CDBDCBC578B61DF61880D1765396F64E66E34697470B7B87077D19",
		"B44AEF7481155DEAA2451378B3BB6C839BE483FBE761AA43D6AA646DDD541305",
		"861DB86E60BCD5C08EEA5AA00567C9F5EDFFDEAEA664A3ABA28E18AC65950C38",
		"655B5C1B1862079754A2995D434781B986FF0BDEB0E95BBA200A7CD931DC8598",
		"9A155A0559DA1AA00366419DE4E681CB25597ED032ECC67FAB43513FD021ED8F",
		"EF50D4DFE890F6FB3A04CF2A89BF61CE37969455A08B34AA77385FD73B29CD4C",
		"00D8EF60DB83DD0F3641D80B614BC49D91ECCDB0DDC2AE0B9B2FECC1B015C38E",
		"0C1B33ECCFB04DC36E8D9BB6F420BDA1424A137687282763FE6FBCFFE68347FA",
		"DA7BC49B24A1A76536D824D6EFE2AEBF67249E83670CEBBF9FE63FCBF98A5206",
		"D6E3A2AA4D9B4048969655BAB2A80C92C3783CE03BE5139FF1B6528AFA18739D",
		"E231820DECCA97A8D147314FC8245B36ABB4D443CE6636634E5E552953E1CA05",
		"D21540985A2ECF0F49DEF0194EEAA8D64299F9F2FCE15398EBE50ED1E2254E16",
		"4595C9168F49AF425C72A25380B1E1331D27A6864F755461B9E05B13342A82C4",
		"45F25391BAC89F64B4FF27D277D0A562268C29A673CA2A5CF975D52EEA25B1B4",
		"C44333D568986751F782255A01CBE45B9E00C58CF41F6B2F030F1046E04B0A79",
		"B08D78B10EDA03BE684D169587F8DF2DA0CEFD408D02EA21794159A96B91BBC4",
		"935F9587A7020BEB6817B5FCFBE914C0CC937E678377EFA9AAA39424B508A617",
		"2A51B572A7781A428668BD7C55A59BF4BFD4B6650318ECD021E8048D46FBE3F2",
		"02E829E913B5386CA9ABA4F63B5DA71EA16CA8AFBE93FF35F15416D9F1DB8210",
		"5B6ECBC378107317D97138A99A101FD2EC6B08972B5D62F2BAB01FF4BCA84361",
		"2BE98A55959BC103A266021EB48FC723A407D097BB5CD47D21DA2B1817FA12B9",
		"614BC1EF975C7A989B49A42D07D0A6A80A22655540DF254E33F57B71E6713051",
		"1220E7EEB0E151CFAF0A4D05F5EA583C18F7D4B0A58BF28030C9837A96D16928",
		"FF451861C8EEF21047AF7283288A30D8EF22ADFCB20D774BB269F9EE5A81A5E9",
		"272DD6D5805B955F06525643A41562545DA8C7808C8A9CA25E1BC125E37DBB6D",
		"E532DBD2B1279C234FE8FF5C7D0C74056EEE216881F476801DA843407BFF9AE8",
		"92514A84905BB7A4A4B6645DC9864DEB6017AA5897BDDC310A63DD5D2E1A7230",
		"6A35FDF7B4FDB5E7DA73EDB9F65C7BAE3DD79E6BCF3FEF53BF8A1ABE73927E13",
		"A5C95C40FDE01D947531E2990B3E0D517F9B2E1209A728FF78293D1DE20A29FE",
		"F35171FC3D7BA5147F7854FC9151F11DA3E28F8E8A3F362AFED35171BCE0330E",
		"600B00B2893EBDCE4569B05C0121FE9C657729A5C1326BA9F843933110E27F6B",
		"DFBB81D24C82D00FE164081F8710BFF4BF16C2EB203440380DC2E09D94E67A08",
		"F743180FE19310421FA9C1124A83EDF210E27B806E08EF85F00084F8BBE2FBF1",
		"4774207CA5441C6799C47328A4BE95FE7BF93F54FEDFC5FFA1F2FF279F4552F8",
		"63780AF915A31FFCA6E5E8F48E724AB3E741310F7FCDF9C7D25EF4F751FE579E",
		"FFB7F0210B5390A586BF61BDFE81BA07420797CF7D2C866DCF8A61E3BFF5A0C8",
		"53D66D0D247C1EF0DB777EB853FB03741B87C4D02A85AFFE48FC2C29DC0DF86F",
		"FFBAF7D7D3A84F7FCDFF7ACD0FD3DFD646622B4041ADF8B0F8432DB5EEC3F20F",
		"93BE8B7FA2FBA39ECB1495B84DACA90DFFE975A3CBF18E3D862843532851CF20",
		"C8A510751CEA0EFC6512FC3512FC6114D43DA89BF0B7F295521829855152D9F8",
		"516553A4BA18AA25BA1A290FF5D52409307F96949F2CA5B3A4F422296D97EAAC",
		"94F21D525820D12F94DAAE90F237619F01AA0150C779245E2324FC0689E74629",
		"7C402A67A5F226890EFEFAF4CD00BF92D26D123FCF48EDFDBB94FF0789BF2352",
		"F9FB5279DC38712C426B777B19A5690168026800D80AB0058006A8002805580B",
		"6007C8064803D001CC07980B9000A005880788038801D000A80094000A000A60",
		"D04D69CE02F4029C728BF8EF407818603FC01E805D00CF003C01F038C00E8026",
		"806A80528048C5888E41FDF35805A55900F12DA09BECDBFF3EDC2F3DA8CBBE4F",
		"772FA046E837548BF1BE0BE2FB77BB239FFA9F3CF6ECFF59FD1EE059DB327277",
		"0E9F6FC767406801B88312F7EB5F00BC0A80DF30B90830113AA4035806B04936",
		"52279512E5753345EC090AEFA79F00F80A00070A7F92334F86A770FFA32E5C7B",
		"AE3DD79E6BCFB5E7DAF3FFBF07ACA952D81FC3752E5DAD6E9BEE21DDBFE8FE5D",
		"F7B2EE635DAFEE9CEEFAC494C48CC4ECC4AD890F24FE34717FE247895F27BA93",
		"9E4B7A35E958D207491F27F149E793E47ABDDEA65FA2AFD46FD1DFAFFF37FD9F",
		"F46FEA0FEB8FEB3FD47FAA3FABEFD75FD18F37680CD71BB4865443A3C16BF899",
		"E15F0D2F1AF61AF6190E1902860B86A02132599B3C3FF997C9FB926F4CB925C5",
		"93F260CAE329BF4C7939E58F296FA61C4A399ED29DF271CAA7296753BE4AB992",
		"4219C71955C689C629C6EB8DB38C37187F62BCD5986C4C37AE35DE69DC64AC35",
		"3E6ADC693C6FFCDA1866BAD16431D59B58D323A6F74D7F310D99AE332F33AF32",
		"6F343F6EFE95B9C37CD23C25353B7565EA33A9BF4D7D2DF583D4ABA9919638CB",
		"32CB1ACB6396A72CCF5AFE60D9678949FB224D9EBE283D3F7D6DFA8674777A75",
		"7A57FA91F453E99FA50BE9E333A666CCC85892F164C6EF33CE650C6544654ECC",
		"5C98B92C73556673E6C399BB32FF90D999F997CC4F32FB3283996840E38F84CB",
		"752ADD645DAA2E4767D7ADD6DDA973EBF6E9F489A6C49589AB139549C9494FE8",
		"7F0F637754FF895E658833DC6E586FB8DFF0A8E129C37F183A0D1F187A0CB264",
		"4DF2ECE48C645BF2F2E4BAE4E6642EF977C9AF261F49EE49FE2CF97CF250F2CF",
		"5376A7F8530EA79C480933C6C2F8688DF38D06E3EDC64263B1D16DDC6BFCD0F8",
		"8DF127268769BBE9B0E91DD349D317A6CB26A579AA79817991F93FCDAF99FDE6",
		"37CD87CC7F3107CC17CC85A9AED4FAD4C753DB52F7C2E87C9EDA9FAAB0CCB0FC",
		"C492665962D968612DFF6AF9BDE5354BBBA5DF12917663DAFCB4BC3447DA9D69",
		"6569D56975693F4F7B21ED3FD3FE98E64F7B2BEDABB409E9F1E937A6EBD3D3D3",
		"97A43B6024DDE9BF4A7F2EFD77E91DE97F491F488FC898949195B13263634645",
		"464D4643C64319BE8C9F65FC6BC67319BFCBA07414C583887EA9BBAA93255E9F",
		"A84DBC1546CB96684FBC3391496C487C30914B7C34F15789AF25B6271E4E3C96",
		"F841E2C7896712CF272A92C6274D4C9A997443D24F92744919494B92DC497549",
		"F727ED48BA98F430C8D6DB295AE31A90844A73A339227572EAF4D49B526F4D4D",
		"4ECD48B5A52E4975A4AE4EE5527F95FA62EAABA97CEA9554CA926D999B6E4D5F",
		"9CBE1C246135F4A039FD85F483E947D3DF49EF0669E849EF4DE7D3CFA6A764A4",
		"655833B2337281F33F66BC937132A327A33783CF88CCD467BA322B32AB33E9CC",
		"2D995B331B329B325B328F6776679ECAECC9ECCD840E5A29AA1782345DBDEE33",
		"1D951899F81CF4E944E2ED492B92D62555243D96F49BA45D49BB93FE9CF44ED2",
		"A7496793AE244DD0CFD61BF42BF56BF425FA7BF4B5FA06FD43FA47F43FD33FA5",
		"DFA9FFADFE15FD6BFA0EFD01587F5FEBA30D330C46C35DC93B927F91FC52726F",
		"F2B494969487C97AFA3A45674C315A61E5DC657CC9F8AAB1C25C6B6E31FFD4FC",
		"8CF925F351581F5F9A07CD51A9B1303649B05296A796A4568044B4A45AD272D3",
		"0AD28AD3DC30D7F7A535A5FD2EED789A3DFDCEF4A6F447D3DF4B673376643C95",
		"F19B8C97335ECBE8CC389F3103564507AC04743CF107B5E374DB752F412F2FEA",
		"6C899D89D393B6C0ACFC22E9DF929E497A3EE9C5A47793AE265DA7D7EA13F473",
		"F5F3F53AE8A3499FA67F50BF5DBF43FFB8FE09FD903ED290609867D0190C0693",
		"A1D0C01A5E054DE237EC371C301C361C35BC63E836CC4E9E0BDA44976C486E02",
		"9DF21CAC91DDA0598E2677279F49BE987C35795E4A724A56CAED291B53AA5276",
		"A5F4805E9962FCA5F105E3CBC60BC65B4D06D32AD326539F69BCF956B3196464",
		"BF3927F52EE8F54BA93BD2F18E4F36F4E1ACAE4F17973837D10032B823F1C9C4",
		"A7135F4CDC9578005A701BB7185F34EE32FA8D6926ABC96E5A6F7A32E3E98C17",
		"337665ECC9D89BB13FE340063AD2314083D66DD1EDD71DD0A9123589E844A3EF",
		"A2D06974313A9DEEACAE5F17D4291255893189F1890989F3A1A534D0C24B13F3",
		"13D72696265624D2A0917B92FA92FA938249BB52F6A4EC8575BF3FE500ACFDA3",
		"29EF80BE3C05FDEA4DE1A16F7D29FD29832941D09B0AA31234A7C618638C33C6",
		"836E4830CE05FDA0030D6132A6811C641B738D4B8D7663BE7135C8C47A63B591",
		"366E3536189B8C2DC6EDC61DC6C78D4F189F343E0DFD5B6F2ABDF6AEFDDA73ED",
		"B9F65C7BAE3DD79E6BCFB5E7FFA3CFFF01504B070808DCFA2BA944000000A000",
		"00504B010214001400080008000799663608DCFA2BA944000000A000000A0000",
		"00000000000000000000000000000047434D444C4E2E444C4C504B0506000000",
		"000100010038000000E14400000000"
	};
	private static final int UNCOMPRESSED_SIZE = 0xA000;

}

class ExportAppletDialog extends JFileChooser {
	public ExportAppletDialog(File dir, Program program) {
		super(dir);
		setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		setDialogTitle("Export Applet");
		String className = program.getClass().getName();
		String programName = className.substring(className.lastIndexOf('.') + 1);
		setSelectedFile(new File(dir, programName));
		exportFilesCheckBox = new JCheckBox("Export resource files");
		exportFilesCheckBox.setSelected(true);
		target = program;
	}

	protected JDialog createDialog(Component parent) {
		JDialog dialog = super.createDialog(parent);
		JPanel panel = new JPanel();
		panel.setLayout(new FlowLayout(FlowLayout.LEFT));
		panel.add(exportFilesCheckBox);
		dialog.getContentPane().add(panel, BorderLayout.SOUTH);
		dialog.validate();
		return dialog;
	}

	public File chooseOutputDirectory() {
		int result = showSaveDialog(target);
		if (result == JFileChooser.CANCEL_OPTION) return null;
		return getSelectedFile();
	}

	public boolean exportFiles() {
		return exportFilesCheckBox.isSelected();
	}

/* Private instance variables */
	private JCheckBox exportFilesCheckBox;
	private Component target;

}

class ThreadedMenuAction implements Runnable {

	public ThreadedMenuAction(Program program, String action) {
		targetProgram = program;
		actionCommand = action;
	}

	public void run() {
		ProgressBarDialog dialog = new ProgressBarDialog(targetProgram);
		try {
			if (actionCommand.equals("Export Applet")) {
				JTFTools.exportApplet(targetProgram, dialog.getProgressBar());
			} else if (actionCommand.equals("Submit Project")) {
				JTFTools.submitProject(targetProgram, dialog.getProgressBar());
			}
		} catch (Exception ex) {
			dialog.setVisible(false);
			IOModel io = targetProgram.getDialog();
			if (io == null) io = targetProgram.getConsole();
			if (io == null) io = IOConsole.SYSTEM_CONSOLE;
			String msg = ex.getMessage();
			if (!(ex instanceof ErrorException)) {
				msg = "" + ex;
				StringWriter wr = new StringWriter();
				PrintWriter pwr = new PrintWriter(wr);
				ex.printStackTrace(pwr);
				pwr.close();
				try {
					BufferedReader rd = new BufferedReader(new StringReader(wr.toString()));
					rd.readLine();
					msg += rd.readLine();
				} catch (IOException ignore) {
					/* Empty */
				}
			}
			io.showErrorMessage(msg);
		}
	}

	private Program targetProgram;
	private String actionCommand;

}

/* Package class: MailStream */
/**
 * This class creates a <code>PrintStream</code> subclass that allows the client to send a
 * message using the standard RFC 2822 message format.  Access to this class is obtained
 * through the static methods in <code>JTFTools</code>.  The class is not exported
 * simply to avoid proliferation of classes in the <code>acm.util</code> package.
 */
class MailStream extends PrintStream {

/* Constructor: MailStream(smtpServer, from, to) */
/**
 * Creates a <code>PrintStream</code> subclass that allows the client to send a
 * message.
 *
 * @usage MailStream mail = new MailStream(smtpServer, from, to);
 * @param smtpServer A string indicating the host name of the SMTP server
 * @param from A string indicating the e-mail address of the sender
 * @param to A string indicating the e-mail address of this recipient
 */
	public MailStream(String smtpServer, String from, String to) {
		super(new NullOutputStream());
		try {
			sender = from;
			recipient = to;
			socket = new Socket(smtpServer, SMTP_PORT);
			out = new BufferedOutputStream(socket.getOutputStream());
			in = new BufferedInputStream(socket.getInputStream());
			verify("220");
			println("HELO " + InetAddress.getLocalHost().getHostName());
			verify("250");
			println("MAIL FROM: " + sender);
			verify("250");
			StringTokenizer tokenizer = new StringTokenizer(to, " ,");
			while (tokenizer.hasMoreTokens()) {
				println("RCPT TO: " + tokenizer.nextToken());
				verify("250");
			}
			println("DATA");
			verify("354");
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/** Prints a <code>boolean</code> value */
	public void print(boolean b) {
		print("" + b);
	}

/** Prints a <code>char</code> value */
	public void print(char c) {
		print("" + c);
	}

/** Prints a <code>int</code> value */
	public void print(int i) {
		print("" + i);
	}

/** Prints a <code>long</code> value */
	public void print(long l) {
		print("" + l);
	}

/** Prints a <code>float</code> value */
	public void print(float f) {
		print("" + f);
	}

/** Prints a <code>double</code> value */
	public void print(double d) {
		print("" + d);
	}

/** Prints the contents of a character array */
	public void print(char[] array) {
		print(new String(array));
	}

/** Prints a <code>String</code> value */
	public void print(String s) {
		write(s);
	}

/** Prints any object */
	public void print(Object obj) {
		print("" + obj);
	}

/** Terminates the current line */
	public void println() {
		try {
			out.write('\n');
			out.flush();
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/** Prints a <code>boolean</code> value and terminates the line */
	public void println(boolean b) {
		print(b);
		println();
	}

/** Prints a <code>char</code> value and terminates the line */
	public void println(char c) {
		print(c);
		println();
	}

/** Prints a <code>int</code> value and terminates the line */
	public void println(int i) {
		print(i);
		println();
	}

/** Prints a <code>long</code> value and terminates the line */
	public void println(long l) {
		print(l);
		println();
	}

/** Prints a <code>float</code> value and terminates the line */
	public void println(float f) {
		print(f);
		println();
	}

/** Prints a <code>double</code> value and terminates the line */
	public void println(double d) {
		print(d);
		println();
	}

/** Prints a character array and terminates the line */
	public void println(char[] array) {
		print(array);
		println();
	}

/** Prints a <code>String</code> value and terminates the line */
	public void println(String s) {
		print(s);
		println();
	}

/** Prints any object and terminates the line */
	public void println(Object x) {
		print(x);
		println();
	}

/** Writes a single character to the output stream */
	public void write(int c) {
		try {
			if (c > 0xFF) {
				throw new ErrorException("Illegal character in mail stream");
			}
			out.write(c);
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/** Writes a segment of a character array to the output stream */
	public void write(char[] buffer, int offset, int length) {
		try {
			for (int i = 0; i < length; i++) {
				char c = buffer[offset + i];
				if (c > 0xFF) {
					throw new ErrorException("Illegal character in mail stream");
				}
				out.write(c);
			}
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/** Writes the entire character array to the output stream */
	public void write(char[] buffer) {
		write(buffer, 0, buffer.length);
	}

/** Writes a <code>String</code> to the output stream */
	public void write(String s) {
		try {
			for (int i = 0; i < s.length(); i++) {
				char c = s.charAt(i);
				if (c > 0xFF) {
					throw new ErrorException("Illegal character in mail stream");
				}
				out.write(c);
			}
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/**
 * Checks for errors in the stream.  This method always returns <code>false</code>
 * because all errors are reported using the <code>ErrorException<code> class.
 */
	public boolean checkError() {
		return false;
	}

/**
 * Flushes the data in the output stream.
 */
	public void flush() {
		try {
			out.flush();
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/**
 * Closes the output stream and sends the message.
 */
	public void close() {
		try {
			println(".");
			verify("250");
			out.close();
			socket.close();
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/**
 * Cancels the current message without sending it.
 */
	public void cancel() {
		try {
			socket.close();
			out.close();
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/**
 * Adds the standard "To", "From", "Subject", and "Date" headers to the message.
 */
	public void sendStandardHeaders(String senderName, String subject) {
		println("To: " + recipient);
		if (senderName != null && senderName.length() != 0) {
			boolean requiresQuotes = false;
			for (int i = 0; i < senderName.length(); i++) {
				char ch = senderName.charAt(i);
				if (ch != ' ' && ch != '-' && !Character.isJavaIdentifierPart(ch)) {
					requiresQuotes = true;
					break;
				}
			}
			if (requiresQuotes) {
				sender = '"' + senderName + '"' + " <" + sender + ">";
			} else {
				sender = senderName + " <" + sender + ">";
			}
		}
		println("From: " + sender);
		if (subject != null && subject.length() != 0) {
			println("Subject: " + subject);
		}
		println("Date: " + RFC2822_DATE_FORMAT.format(new Date()));
	}

/*
 * Waits for a response from the server and makes sure it begins with the
 * specified pattern.
 */
	private void verify(String pattern) {
		try {
			String response = "";
			while (true) {
				int c = in.read();
				if (c == -1 || c == '\n') break;
				response += (char) c;
			}
			if (pattern != null && !response.startsWith(pattern)) {
				throw new ErrorException("Unexpected SMTP response: " + response);
			}
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/* Private constants */
	private static final int SMTP_PORT = 25;
	private static SimpleDateFormat RFC2822_DATE_FORMAT = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z", Locale.US);

/* Private instance variables */
	private Socket socket;
	private InputStream in;
	private OutputStream out;
	private String sender;
	private String recipient;

}

/* Package class: HexByteOutputStream */
/**
 * This class creates an <code>OutputStream</code> subclass that converts data
 * to hex bytes.  The class is not exported simply to avoid proliferation of
 * classes in the <code>acm.util</code> package.
 */
class HexByteOutputStream extends OutputStream {

/* Constructor: HexByteOutputStream(printStream) */
/**
 * Creates an <code>OutputStream</code> subclass that converts data to hex bytes.
 *
 * @usage HexByteOutputStream out = new HexByteOutputStream(printStream);
 * @param printStream The underlying <code>PrintStream</code> to which data is written
 */
	public HexByteOutputStream(PrintStream printStream) {
		out = printStream;
		out.flush();
		columnCount = 0;
	}

/** Writes a single byte to the output stream */
	public void write(int b) throws IOException {
		String hex = Integer.toHexString(0x100 + (b & 0xFF)).toUpperCase();
		out.write(hex.charAt(1));
		out.write(hex.charAt(2));
		columnCount += 2;
		if (columnCount >= COLUMNS) {
			columnCount = 0;
			out.println();
		}
	}

/** Flushes data in the output stream */
	public void flush() {
		out.flush();
	}

/** Closes the output stream */
	public void close() {
		out.close();
	}

/* Private constants */
	private static final int COLUMNS = 76;

/* Private instance variables */
	private int columnCount;
	private PrintStream out;

}

/* Package class: Base64OutputStream */
/**
 * This class creates an <code>OutputStream</code> subclass that converts data to
 * its Base64 encoding.  This encoding transmits three bytes of binary data as a
 * series of four characters, each of which supplies six bits of the value.
 * The class is not exported simply to avoid proliferation of classes in the
 * <code>acm.util</code> package.
 */
class Base64OutputStream extends OutputStream {

/* Constructor: Base64OutputStream(printStream) */
/**
 * Creates an <code>OutputStream</code> subclass that converts data to
 * its Base64 encoding.
 *
 * @usage Base64OutputStream out = new Base64OutputStream(printStream);
 * @param printStream The underlying <code>PrintStream</code> to which data is written
 */
	public Base64OutputStream(PrintStream printStream) {
		out = printStream;
		out.flush();
		byteCount = 0;
		columnCount = 0;
		buffer = 0;
	}

/** Writes a single byte to the output stream */
	public void write(int b) throws IOException {
		buffer = buffer << 8 | (b & 0xFF);
		byteCount++;
		if (byteCount == 3) {
			out.write(BASE64[(buffer >> 18) & 0x3F]);
			out.write(BASE64[(buffer >> 12) & 0x3F]);
			out.write(BASE64[(buffer >> 6) & 0x3F]);
			out.write(BASE64[buffer & 0x3F]);
			columnCount += 4;
			if (columnCount >= COLUMNS) {
				columnCount = 0;
				out.println();
			}
			byteCount = 0;
			buffer = 0;
		}
	}

/** Flushes data in the output stream */
	public void flush() {
		out.flush();
	}

/** Closes the output stream */
	public void close() {
		pad();
		out.close();
	}

/** Pads the data to ensure that there is a valid Base64 sequence */
	public void pad() {
		switch (byteCount) {
		  case 0:
			/* Empty */
			break;
		  case 1:
			out.write(BASE64[(buffer >> 2) & 0x3F]);
			out.write(BASE64[(buffer << 4) & 0x3F]);
			out.write('=');
			out.write('=');
			break;
		  case 2:
			out.write(BASE64[(buffer >> 10) & 0x3F]);
			out.write(BASE64[(buffer >> 4) & 0x3F]);
			out.write(BASE64[(buffer << 2) & 0x3F]);
			out.write('=');
			break;
		}
		out.println();
		out.println();
	}

/* Private constants */
	private static final int COLUMNS = 76;
	private static final char[] BASE64 = {
		'A','B','C','D','E','F','G','H',
		'I','J','K','L','M','N','O','P',
		'Q','R','S','T','U','V','W','X',
		'Y','Z','a','b','c','d','e','f',
		'g','h','i','j','k','l','m','n',
		'o','p','q','r','s','t','u','v',
		'w','x','y','z','0','1','2','3',
		'4','5','6','7','8','9','+','/',
	};

/* Private instance variables */
	private int buffer;
	private int columnCount;
	private int byteCount;
	private PrintStream out;

}

/* Package class: NullOutputStream */
/**
 * This class represents an <code>OutputStream</code> that ignores all data,
 * much like the device <code>/dev/null</code> on a Unix system.  The reason
 * for including this class is that some older unstuffing programs cannot
 * read ZIP files that write the sizes and checksum at the end of the entry
 * data.  By writing the entry to a <code>NullOutputStream</code> first,
 * the system can calculate the correct values before writing them to the
 * actual <code>ZipOutputStream</code>.  Note that this strategy means that
 * each file in the submission is compressed twice.  Caching this result
 * would save time at the expense of memory, but could easily fail for
 * large image files, for example.
 */
class NullOutputStream extends OutputStream {
	public void write(int b) { }
	public void write(byte[] b) { }
	public void write(byte[] b, int off, int len) { }
	public void flush() { }
	public void close() { }
}

/* Package class: SubmitOptions */
/**
 * This class keeps track of the options used in the "Submit Options" menu.
 */
class SubmitOptions implements ActionListener, DocumentListener {

/* Constructor: SubmitOptions(program) */
/**
 * Creates a <code>SubmitOptions</code> for the program, filling in
 * any fields that can be determined using reflection.
 */
	public SubmitOptions(Program program) {
		parent = program;
		authorNameField = new JTextField();
		authorEMailField = new JTextField();
		instructorEMailField = new JTextField();
		smtpServerField = new JTextField();
		cancelButton = new JButton("Cancel");
		submitButton = new JButton("Submit");
		authorEMailField.getDocument().addDocumentListener(this);
		instructorEMailField.getDocument().addDocumentListener(this);
		smtpServerField.getDocument().addDocumentListener(this);
		cancelButton.addActionListener(this);
		submitButton.addActionListener(this);
		initPreferences(program);
	}

/* Returns the author name */
	public String getAuthorName() {
		return authorNameField.getText().trim();
	}

/* Returns the author email address */
	public String getAuthorEMail() {
		return authorEMailField.getText().trim();
	}

/* Returns the submission email address */
	public String getSubmissionEMail() {
		return instructorEMailField.getText().trim();
	}

/* Returns the SMTP server */
	public String getSMTPServer() {
		return smtpServerField.getText().trim();
	}

/* Returns <code>true</code> if all the necessary fields are filled in */
	public boolean isComplete() {
		if (getAuthorEMail().indexOf('@') == -1) return false;
		if (getSubmissionEMail().indexOf('@') == -1) return false;
		if (getSMTPServer().length() == 0) return false;
		return true;
	}

/*
 * Pops up a dialog to request the fields and waits for it to complete.
 * Returns <code>true</code> if the user activates the dialog by clicking
 * <code>Submit</code>.
 */
	public boolean popup() {
		Frame frame = JTFTools.getEnclosingFrame(parent);
		if (frame == null) return false;
		dialog = new JDialog(frame, "Submit Project Options", true);
		Container contentPane = dialog.getContentPane();
		contentPane.setLayout(new TableLayout(6, 2, 0, 4));
		contentPane.add(new JLabel("Instructor email ", JLabel.RIGHT));
		contentPane.add(instructorEMailField, "width=" + FIELD_WIDTH);
		contentPane.add(new JLabel("Author name ", JLabel.RIGHT));
		contentPane.add(authorNameField, "width=" + FIELD_WIDTH);
		contentPane.add(new JLabel("Author email ", JLabel.RIGHT));
		contentPane.add(authorEMailField, "width=" + FIELD_WIDTH);
		contentPane.add(new JLabel("SMTP server ", JLabel.RIGHT));
		contentPane.add(smtpServerField, "width=" + FIELD_WIDTH);
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new FlowLayout());
		buttonPanel.add(cancelButton);
		buttonPanel.add(submitButton);
		contentPane.add(new JLabel(""));
		contentPane.add(buttonPanel, "top=10");
		dialog.setSize(DIALOG_WIDTH, DIALOG_HEIGHT);
		submitFlag = false;
		submitButton.setEnabled(isComplete());
		dialog.setVisible(true);
		return submitFlag;
	}

/* Responds to an action event */
	public void actionPerformed(ActionEvent e) {
		submitFlag = (e.getSource() == submitButton);
		dialog.setVisible(false);
	}

/* Responds to any changes in the text fields */
	public void changedUpdate(DocumentEvent e) {
		submitButton.setEnabled(isComplete());
	}

	public void removeUpdate(DocumentEvent e) {
		submitButton.setEnabled(isComplete());
	}

	public void insertUpdate(DocumentEvent e) {
		submitButton.setEnabled(isComplete());
	}

/* Tries to initialize the fields by reflection */
	private void initPreferences(Program program) {
		instructorEMailField.setText(getDefaultField("INSTRUCTOR_EMAIL", program));
		authorNameField.setText(getDefaultField("AUTHOR_NAME", program));
		String sender = getDefaultField("AUTHOR_EMAIL", program);
		if (sender.length() == 0) {
			sender = System.getProperty("user.name");
			if (sender == null) {
				sender = "";
			} else {
				String localHost = JTFTools.getLocalHostName();
				if (localHost != null) sender += "@" + localHost;
			}
		}
		authorEMailField.setText(sender);
		String smtp = getDefaultField("SMTP_SERVER", program);
		if (smtp.length() == 0) {
			smtp = System.getProperty("mail.smtp.host");
			if (smtp == null) {
				String localHost = JTFTools.getLocalHostName();
				if (localHost == null) {
					smtp = "";
				} else {
					smtp = "smtp." + localHost.substring(localHost.indexOf('.') + 1);
					try {
						new Socket(smtp, SMTP_PORT).close();
					} catch (Exception ex) {
						smtp = "";
					}
				}
			}
		}
		smtpServerField.setText(smtp);
	}

/* Looks up a field by name in the program class */
	private String getDefaultField(String name, Program program) {
		try {
			Field field = program.getClass().getField(name);
			String value = (String) field.get(program);
			if (value != null && value.trim().length() != 0) return value;
		} catch (Exception ex) {
			/* Empty */
		}
		return "";
	}

/* Private constants */
	private static final int FIELD_WIDTH = 300;
	private static final int DIALOG_WIDTH = 500;
	private static final int DIALOG_HEIGHT = 230;
	private static final int SMTP_PORT = 25;

/* Private instance variables */
	private Component parent;
	private JDialog dialog;
	private JTextField authorNameField;
	private JTextField authorEMailField;
	private JTextField instructorEMailField;
	private JTextField smtpServerField;
	private JButton cancelButton;
	private JButton submitButton;
	private boolean submitFlag;

}

/* Package class: ProgressBarDialog */
/**
 * This class implements the dialog used to track the progress of the
 * "ExportApplet" and "Submit Program" menu options.
 */
class ProgressBarDialog extends JDialog implements ActionListener {

/* Constructor: ProgressBarDialog(program) */
/**
 * Creates a dialog containing a progress bar and a <code>Cancel</code> button.
 */
	public ProgressBarDialog(Program program) {
		super(JTFTools.getEnclosingFrame(program), "Progress", false);
		Container contentPane = getContentPane();
		contentPane.setLayout(new TableLayout(2, 1));
		progressBar = new JProgressBar();
		contentPane.add(progressBar, "weightx=1 left=5 right=5 height=" + PROGRESS_BAR_HEIGHT);
		JButton cancelButton = new JButton("Cancel");
		cancelButton.addActionListener(this);
		contentPane.add(cancelButton, "top=10 fill=NONE");
		Rectangle programBounds = program.getBounds();
		int x = programBounds.x + (programBounds.width - DIALOG_WIDTH) / 2;
		int y = programBounds.y + (programBounds.height - DIALOG_HEIGHT) / 2;
		setBounds(x, y, DIALOG_WIDTH, DIALOG_HEIGHT);
	}

/* Returns the progress bar */
	public JProgressBar getProgressBar() {
		return progressBar;
	}

/* Responds to an action event */
	public void actionPerformed(ActionEvent e) {
		progressBar.setString("Cancel");
		setVisible(false);
	}

/* Checks to see if a progress bar has been cancelled */
	public static boolean hasBeenCancelled(JProgressBar bar) {
		if (bar == null) return false;
		String str = bar.getString();
		return str != null && str.equals("Cancel");
	}

/* Pops up the dialog if the progress bar is included inside one */
	public static void popup(JProgressBar bar) {
		Component comp = bar;
		while (comp != null) {
			if (comp instanceof ProgressBarDialog) {
				comp.setVisible(true);
				comp.repaint();
				return;
			}
			comp = comp.getParent();
		}
	}

/* Dismisses the dialog if the progress bar is included inside one */
	public static void dismiss(JProgressBar bar) {
		Component comp = bar;
		while (comp != null) {
			if (comp instanceof ProgressBarDialog) {
				comp.setVisible(false);
				return;
			}
			comp = comp.getParent();
		}
	}

/* Private constants */
	private static final int DIALOG_WIDTH = 250;
	private static final int DIALOG_HEIGHT = 90;
	private static final int PROGRESS_BAR_HEIGHT = 25;

/* Private instance variables */
	private JProgressBar progressBar;
}

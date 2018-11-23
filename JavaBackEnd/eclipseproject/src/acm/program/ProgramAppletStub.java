/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 * - alphabetized methods
 */

package acm.program;

import java.applet.*;
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import java.lang.reflect.Method;
import java.net.*;
import java.util.*;

import acm.util.ErrorException;

/* Package class: ProgramAppletStub */
/**
 * This class implements both the AppletStub and AppletContext interfaces and allows
 * standard applications to run with the same capabilities as an applet.
 */
@SuppressWarnings("deprecation")
class ProgramAppletStub implements AppletContext, AppletStub {
	/* Private instance variables */
	private Applet applet;
	private Frame enclosure;
	private boolean recursiveResizeCheck;

	public ProgramAppletStub(Program program) {
		applet = program;
	}

	public void appletResize(int width, int height) {
		if (enclosure == null) {
			if (!recursiveResizeCheck) {
				recursiveResizeCheck = true;
				applet.resize(width, height);
				applet.validate();
				recursiveResizeCheck = false;
			}
		} else {
			enclosure.setSize(width, height);
			enclosure.validate();
		}
	}

	public Applet getApplet(String name) {
		return null;
	}

	public AppletContext getAppletContext() {
		return this;
	}

	public Enumeration<Applet> getApplets() {
		return new Vector<Applet>().elements();
	}

	public AudioClip getAudioClip(URL url) {
		AudioClip clip = null;
		if (clip == null) clip = getNewAudioClip(url);
		return clip;
	}

	private String getCanonicalPath(String start) {
		String path = new File(start).getAbsolutePath();
		int sp;
		while ((sp = path.indexOf(' ')) != -1) {
			path = path.substring(0, sp) + "%20" + path.substring(sp + 1);
		}
		return path;
	}

	public URL getCodeBase() {
		try {
			return new URL("file:" + getCanonicalPath("."));
		} catch (MalformedURLException ex) {
			throw new ErrorException("Error: Illegal document base URL");
		}
	}

	public URL getDocumentBase() {
		return getCodeBase();
	}

	public Image getImage(URL url) {
		try {
			Object content = url.getContent();
			if (content instanceof ImageProducer) {
				return applet.createImage((ImageProducer) content);
			}
		} catch (IOException ex) {
			/* Ignore the exception and fail */
		}
		return null;
	}

	private synchronized AudioClip getNewAudioClip(URL url) {
		try {
			Class<?> type = java.applet.Applet.class;
			Class<?>[] types = { java.net.URL.class };
			Object[] args = { url };
			Method fn = type.getMethod("newAudioClip", types);
			return (AudioClip) fn.invoke(null, args);
		} catch (Exception ex) {
			return null;
		}
	}

	public String getParameter(String name) {
		return null;
	}

	public InputStream getStream(String key) {
		throw new ErrorException("getStream: unimplemented operation");
	}

	public java.util.Iterator<String> getStreamKeys() {
		throw new ErrorException("getStreamKeys: unimplemented operation");
	}

	public boolean isActive() {
		return true;
	}

	public void setFrame(Frame frame) {
		enclosure = frame;
	}

	public void setStream(String key, InputStream stream) {
		throw new ErrorException("setStream: unimplemented operation");
	}

	public void showDocument(URL url) {
		if (applet != null) applet.getAppletContext().showDocument(url);
	}

	public void showDocument(URL url, String target) {
		if (applet != null) applet.getAppletContext().showDocument(url, target);
	}

	public void showStatus(String status) {
		if (applet == null) {
			System.out.println(status);
		} else {
			applet.showStatus(status);
		}
	}
}

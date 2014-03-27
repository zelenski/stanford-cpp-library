/*
 * @(#)MovieClip.java   1.99.1 08/12/08
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

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.net.*;

/* Implementation notes */
/*
 * This class uses QuickTime for Java, which is not available on all Java platforms.
 * To ensure that this class can be compiled even on systems that lack the QuickTime
 * libraries, all calls to the library are implemented through reflection.
 */

/* Class: MovieClip */
/**
 * This class represents a video clip, which can be read from a QuickTime
 * movie file or web-based resource.
 */
public class MovieClip extends Container {

/* Constructor: MovieClip(filename) */
/**
 * Creates a <code>MovieClip</code> object from the specified movie file.
 *
 * @usage MovieClip movie = new MovieClip(filename);
 * @param filename The file from which the movie is read
 */
	public MovieClip(String filename) {
		this(filename, filename);
	}

/* Constructor: MovieClip(file) */
/**
 * Creates a <code>MovieClip</code> object from the specified movie file.
 *
 * @usage MovieClip movie = new MovieClip(file);
 * @param file A <code>File</code> object from which the movie is read
 */
	public MovieClip(File file) {
		this(file, file.getName());
	}

/* Constructor: MovieClip(url) */
/**
 * Creates a <code>MovieClip</code> object from the specified network URL.
 *
 * @usage MovieClip movie = new MovieClip(file);
 * @param url A network URL containing the movie
 */
	public MovieClip(URL url) {
		this(url, JTFTools.getURLSuffix(url.toString()));
	}

/* Method: play() */
/**
 * Starts the movie from its current position.
 *
 * @usage movie.play();
 */
	public void play() {
		setLooping(false);
		startMovie();
	}

/* Method: loop() */
/**
 * Plays the movie in a continuous audio loop.
 *
 * @usage movie.loop();
 */
	public void loop() {
		setLooping(true);
		startMovie();
	}

/* Method: stop() */
/**
 * Stops the playback of the movie.
 *
 * @usage movie.stop();
 */
	public synchronized void stop() {
		stopMovie();
		setLooping(false);
	}

/* Method: getName() */
/**
 * Returns the name of the movie, which is typically the file name from which it
 * was read.
 *
 * @usage String name = movie.getName();
 * @return The name of the movie
 */
	public String getName() {
		return movieName;
	}

/* Method: setName(name) */
/**
 * Sets a name to identify the movie.
 *
 * @usage movie.setName(name);
 * @param name The name to use for the movie
 */
	public void setName(String name) {
		movieName = name;
	}

/* Method: getFrameCount() */
/**
 * Returns the number of frames in a movie.
 *
 * @usage int nFrames = movie.getFrameCount();
 * @return The number of frames in a movie
 */
	public int getFrameCount() {
		return getQTDuration();
	}

/* Method: getFrameRate() */
/**
 * Returns the frame rate of the movie.
 *
 * @usage double frameRate = movie.getFrameRate();
 * @return The frame rate of the movie (in frames/second)
 */
	public double getFrameRate() {
		return getQTTimeScale();
	}

/* Method: getDuration() */
/**
 * Returns the duration of a movie (in seconds).
 *
 * @usage double duration = movie.getDuration();
 * @return The duration of a movie (in seconds)
 */
	public double getDuration() {
		return getFrameCount() / getFrameRate();
	}

/* Method: getFrameIndex() */
/**
 * Returns the current frame index in the movie.
 *
 * @usage int frameIndex = movie.getFrameIndex();
 * @return The current frame index in the movie
 */
	public int getFrameIndex() {
		return getCurrentTime();
	}

/* Method: setFrameIndex(frame) */
/**
 * Sets the current frame index.
 *
 * @usage movie.setFrameIndex(frameIndex);
 * @param frameIndex The current frame index in the movie
 */
	public void setFrameIndex(int frameIndex) {
		setCurrentTime(frameIndex);
	}

/* Method: rewind() */
/**
 * Rewinds the movie to the beginning.  This method is useful after you have
 * stopped a movie and want to replay it from the beginning.
 *
 * @usage movie.rewind();
 */
	public void rewind() {
		setFrameIndex(0);
	}

/* Method: getVolume() */
/**
 * Returns the playback volume setting for the movie, which is a number
 * between 0 (silent) and 1 (maximum volume).
 *
 * @usage double volume = movie.getVolume();
 * @return The playback volume setting for the movie
 */
	public double getVolume() {
		return clipVolume;
	}

/* Method: setVolume(volume) */
/**
 * Sets the playback volume setting for the movie, which is a number
 * between 0 (silent) and 1 (maximum volume).
 *
 * @usage movie.setVolume(volume);
 * @param volume The new volume setting for the movie
 */
	public void setVolume(double volume) {
		clipVolume = volume;
		setControllerVolume(volume);
	}

/* Method: getPlaybackRate() */
/**
 * Returns the playback rate, which is a floating-point number indicating
 * the speed and direction of playback.  The value 1.0 indicates normal
 * speed, 2.0 indicates a movie running at double speed, and 0.5 indicates
 * a movie running at half speed.  Negative values indicate that the movie
 * should run in reverse, so that a playback rate of -1.0 specifies that the
 * movie should run backwards at normal speed.
 *
 * @usage double rate = movie.getPlaybackRate();
 * @return The playback rate for the movie
 */
	public double getPlaybackRate() {
		return clipRate;
	}

/* Method: setPlaybackRate(rate) */
/**
 * Sets the playback rate for the movie.  The value 1.0 indicates normal
 * speed, 2.0 indicates a movie running at double speed, and 0.5 indicates
 * a movie running at half speed.  Negative values indicate that the movie
 * should run in reverse, so that a playback rate of -1.0 specifies that the
 * movie should run backwards at normal speed.
 *
 * @usage sound.setPlaybackRate(rate);
 * @param rate The new playback rate for the movie
 */
	public void setPlaybackRate(double rate) {
		clipRate = rate;
		if (!isStopped()) setRate(rate);
	}

/* Method: enableController() */
/**
 * Enables the QuickTime controller displayed at the bottom of the movie.  This
 * call has no effect if a controller is already enabled.
 *
 * @usage movie.enableController();
 */
	public void enableController() {
		if (hasQuickTime) setControllerVisible(true);
	}

/* Method: disableController() */
/**
 * Disables the QuickTime controller from the bottom of the movie, making it
 * disappear from the window.  This call has no effect if no controller is
 * already disabled.
 *
 * @usage movie.disableController();
 */
	public void disableController() {
		if (hasQuickTime) setControllerVisible(false);
	}

/* Method: isControllerEnabled() */
/**
 * Returns <code>true</code> if the QuickTime controller is enabled.
 *
 * @usage if (movie.isControllerEnabled()) . . .
 * @return <code>true</code> if the controller is enabled
 */
	public boolean isControllerEnabled() {
		return controllerVisible;
	}

/* Method: getControllerHeight() */
/**
 * Returns the height of the QuickTime controller, measured in pixels.  If
 * no controller is installed, <code>getControllerHeight</code> returns 0.
 *
 * @usage int height = movie.getControllerHeight();
 * @return The height of the QuickTime controller (in pixels)
 */
	public int getControllerHeight() {
		return (controller == null) ? 0 : CONTROLLER_HEIGHT;
	}

/**********************************************************************/
/* Overridden methods                                                 */
/**********************************************************************/

/* Method: getPreferredSize() */
/**
 * Returns the desired height of the movie panel, which is taken from the
 * underlying movie, taking account of the controller, if any.
 *
 * @usage Dimension size = movie.getPreferredSize();
 * @return The desired height of the movie panel
 * @noshow
 */
	public Dimension getPreferredSize() {
		if (movie == null) return DEFAULT_SIZE;
		try {
			Method getNaturalBoundsRect = movieClass.getMethod("getNaturalBoundsRect", new Class[0]);
			Method getWidth = qdRectClass.getMethod("getWidth", new Class[0]);
			Method getHeight = qdRectClass.getMethod("getHeight", new Class[0]);
			Object rect = getNaturalBoundsRect.invoke(movie, new Object[0]);
			int width = ((Integer) getWidth.invoke(rect, new Object[0])).intValue();
			int height = ((Integer) getHeight.invoke(rect, new Object[0])).intValue();
			Dimension size = new Dimension(width, height + getControllerHeight());
			return size;
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/**********************************************************************/
/* Private methods                                                    */
/**********************************************************************/

/* Private constructor: MovieClip(source, name) */
/**
 * Constructs a new movie from a data source and assigns the specified name.
 */
	private MovieClip(Object source, String name) {
		startQuickTime();
		movieName = name;
		clipVolume = 1.0;
		clipRate = 1.0;
		setLayout(new BorderLayout());
		if (hasQuickTime) {
			try {
				readMovie(source);
			} catch (Exception ex) {
				add(new ErrorWindow(name, "Can't read movie file"), BorderLayout.CENTER);
			}
		} else {
			add(new ErrorWindow(name, errorMessage), BorderLayout.CENTER);
		}
		addComponentListener(new MovieClipListener());
	}

/* Private method: startQuickTime() */
/**
 * Opens a new QuickTime session and tests to see whether the correct version
 * of QuickTime exists.
 */
	private void startQuickTime() {
		hasQuickTime = false;
		try {
			movieClass = Class.forName("quicktime.std.movies.Movie");
			movieControllerClass = Class.forName("quicktime.std.movies.MovieController");
			openMovieFileClass = Class.forName("quicktime.io.OpenMovieFile");
			qdRectClass = Class.forName("quicktime.qd.QDRect");
			qtDataRefClass = Class.forName("quicktime.std.movies.media.DataRef");
			qtFactoryClass = Class.forName("quicktime.app.view.QTFactory");
			qtFileClass = Class.forName("quicktime.io.QTFile");
			qtSessionClass = Class.forName("quicktime.QTSession");
			timeRecordClass = Class.forName("quicktime.std.clocks.TimeRecord");
			Method isInitialized = qtSessionClass.getMethod("isInitialized", new Class[0]);
			synchronized (lock) {
				if (isInitialized.invoke(null, new Object[0]).equals(Boolean.FALSE)) {
					Method open = qtSessionClass.getMethod("open", new Class[0]);
					open.invoke(null, new Object[0]);
				}
			}
			Method getMajorVersion = qtSessionClass.getMethod("getMajorVersion", new Class[0]);
			version = ((Integer) getMajorVersion.invoke(null, new Object[0])).intValue();
			if (version < MIN_QUICKTIME_VERSION) {
				throw new ErrorException("MovieClip requires QuickTime V7 or later");
			}
			hasQuickTime = true;
		} catch (Exception ex) {
			errorMessage = ex.getMessage();
		}
	}

/* Private method: startMovie() */
/**
 * Starts the movie from its current position.  This method is identical to
 * clicking the <code>play</code> control in the QuickTime controller.
 */
	private void startMovie() {
		if (!hasQuickTime || movie == null) return;
		try {
			Class[] types = { Float.TYPE };
			Object[] args = { new Float((float) clipRate) };
			Method play = movieControllerClass.getMethod("play", types);
			play.invoke(controller, args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: stopMovie() */
/**
 * Stops the movie.  This method is identical to clicking the
 * <code>pause</code> control in the QuickTime controller.
 */
	private void stopMovie() {
		if (!hasQuickTime || movie == null) return;
		try {
			Class[] types = { Float.TYPE };
			Object[] args = { new Float(0.0) };
			Method play = movieControllerClass.getMethod("play", types);
			play.invoke(controller, args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: setLooping(flag) */
/**
 * Sets a flag to indicate whether the movie should play in a continuous
 * loop.
 */
	private void setLooping(boolean flag) {
		if (!hasQuickTime || movie == null) return;
		try {
			Class[] types = { Boolean.TYPE };
			Object[] args = { new Boolean(flag) };
			Method setLooping = movieControllerClass.getMethod("setLooping", types);
			setLooping.invoke(controller, args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: readMovie(source) */
/**
 * Reads a QuickTime movie by calling a method appropriate to the source type.
 * If the movie is read successfully, this code adds it to the primary container.
 */
	private void readMovie(Object source) {
		try {
			if (source instanceof String) {
				String name = (String) source;
				if (name.startsWith("http:")) {
					try {
						movie = readMovieFromURL(new URL(name));
					} catch (MalformedURLException ex) {
						throw new ErrorException("MovieClip: Malformed URL");
					}
				} else {
					movie = readMovieFromFile(new File(name));
				}
			} else if (source instanceof File) {
				movie = readMovieFromFile((File) source);
			} else if (source instanceof URL) {
				movie = readMovieFromURL((URL) source);
			}
			controller = createController();
			controllerVisible = true;
			addQTComponent(controller);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: createController() */
/**
 * Creates a QuickTime controller for the current movie.
 */
	private Object createController() throws Exception {
		Class[] types = { movie.getClass() };
		Object[] args = { movie };
		Constructor<?> movieControllerConstructor = movieControllerClass.getConstructor(types);
		return movieControllerConstructor.newInstance(args);
	}

/* Private method: addQTComponent(obj) */
/**
 * Adds a component to the main container.  If the argument is a QuickTime object,
 * the code invokes the appropriate <code>asComponent</code> method to create a
 * Java component.
 */
	private void addQTComponent(Object obj) throws Exception {
		if (!(obj instanceof Component)) {
			Class[] types = { obj.getClass() };
			Object[] args = { obj };
			Method makeQTComponent = qtFactoryClass.getMethod("makeQTComponent", types);
			obj = makeQTComponent.invoke(null, args);
			if (!(obj instanceof Component)) {
				Class<?> qtClass = obj.getClass();
				Method asComponent = qtClass.getMethod("asComponent", new Class[0]);
				obj = asComponent.invoke(obj, new Object[0]);
			}
		}
		removeAll();
		add((Component) obj, BorderLayout.CENTER);
	}

/* Private method: readMovieFromFile(file) */
/**
 * Reads a movie from a file object.
 */
	private Object readMovieFromFile(File file) {
		try {
			Class[] types1 = { Class.forName("java.io.File") };
			Object[] args1 = { file };
			Constructor<?> qtFileConstructor = qtFileClass.getConstructor(types1);
			Object qtFile = qtFileConstructor.newInstance(args1);
			Class[] types2 = { qtFileClass };
			Object[] args2 = { qtFile };
			Method asRead = openMovieFileClass.getMethod("asRead", types2);
			Object openMovieFile = asRead.invoke(null, args2);
			Class[] types3 = { openMovieFileClass };
			Object[] args3 = { openMovieFile };
			Method fromFile = movieClass.getMethod("fromFile", types3);
			return fromFile.invoke(null, args3);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: readMovieFromURL(href) */
/**
 * Reads a movie from the specified URL.
 */
	private Object readMovieFromURL(URL url) {
		try {
			Class[] types1 = { Class.forName("java.lang.String") };
			Object[] args1 = { url.toString() };
			Constructor<?> qtDataRefConstructor = qtDataRefClass.getConstructor(types1);
			Object qtFile = qtDataRefConstructor.newInstance(args1);
			Class[] types2 = { qtDataRefClass, Integer.TYPE };
			Object[] args2 = { qtFile, new Integer(0) };
			Method fromDataRef = movieClass.getMethod("fromDataRef", types2);
			return fromDataRef.invoke(null, args2);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: setControllerVisible(flag) */
/**
 * Sets whether the controller is visible.
 */
	private void setControllerVisible(boolean flag) {
		if (!hasQuickTime || movie == null) return;
		if (flag != controllerVisible) {
			try {
				Class[] types = { Boolean.TYPE };
				Object[] args = { new Boolean(flag) };
				Method setVisible = movieControllerClass.getMethod("setVisible", types);
				setVisible.invoke(controller, args);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
			controllerVisible = flag;
		}
	}

/* Private method: getCurrentTime() */
/**
 * Returns the movie time as maintained by the controller.
 */
	private int getCurrentTime() {
		if (!hasQuickTime || movie == null) return 0;
		try {
			Method getCurrentTime = movieControllerClass.getMethod("getCurrentTime", new Class[0]);
			return ((Integer) getCurrentTime.invoke(controller, new Object[0])).intValue();
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: setCurrentTime(time) */
/**
 * Sets the movie time.  As with the other <code>set</code> operations, this
 * code makes the changes in the controller to ensure that its controls are
 * updated.
 */
	private void setCurrentTime(int time) {
		if (!hasQuickTime || movie == null) return;
		try {
			Class[] types = { timeRecordClass };
			Object[] args = { createTimeRecord(time) };
			Method goToTime = movieControllerClass.getMethod("goToTime", types);
			goToTime.invoke(controller, args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: setControllerVolume(volume) */
/**
 * Sets the controller volume.
 */
	private void setControllerVolume(double volume) {
		if (!hasQuickTime || movie == null) return;
		try {
			Class[] types = { Float.TYPE };
			Object[] args = { new Float((float) volume) };
			Method setVolume = movieControllerClass.getMethod("setVolume", types);
			setVolume.invoke(controller, args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: setRate(rate) */
/**
 * Sets the movie playback rate.
 */
	private void setRate(double volume) {
		if (!hasQuickTime || movie == null) return;
		try {
			Class[] types = { Float.TYPE };
			Object[] args = { new Float((float) volume) };
			Method setRate = movieClass.getMethod("setRate", types);
			setRate.invoke(movie, args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: isStopped() */
/**
 * Returns whether the controller is currently stopped.
 */
	private boolean isStopped() {
		if (!hasQuickTime || movie == null) return true;
		try {
			Method getPlayRate = movieControllerClass.getMethod("getPlayRate", new Class[0]);
			return ((Float) getPlayRate.invoke(controller, new Object[0])).floatValue() == 0.0;
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: getQTDuration() */
/**
 * Returns the duration in movie time units.
 */
	private int getQTDuration() {
		if (!hasQuickTime || movie == null) return 0;
		try {
			Method getDuration = movieClass.getMethod("getDuration", new Class[0]);
			return ((Integer) getDuration.invoke(movie, new Object[0])).intValue();
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: getQTTimeScale() */
/**
 * Returns the movie time in units per second.
 */
	private int getQTTimeScale() {
		if (!hasQuickTime || movie == null) return DEFAULT_TIME_SCALE;
		try {
			Method getTimeScale = movieClass.getMethod("getTimeScale", new Class[0]);
			return ((Integer) getTimeScale.invoke(movie, new Object[0])).intValue();
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: createTimeRecord() */
/**
 * Creates a <code>TimeRecord</code> object, which is required in the call
 * to <code>goToTime</code>.
 */
	private Object createTimeRecord(int time) {
		try {
			Class[] types = { Integer.TYPE, Long.TYPE };
			Object[] args = { new Integer(getQTTimeScale()), new Long(time) };
			Constructor<?> newTimeRecord = timeRecordClass.getConstructor(types);
			return newTimeRecord.newInstance(args);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private constants */
	private static final int MIN_QUICKTIME_VERSION = 7;
	private static final int CONTROLLER_HEIGHT = 16;
	private static final int DEFAULT_TIME_SCALE = 600;
	private static final Dimension DEFAULT_SIZE = new Dimension(300, 200);

/* Private instance variables */
	private Class<?> movieClass;
	private Class<?> movieControllerClass;
	private Class<?> openMovieFileClass;
	private Class<?> qdRectClass;
	private Class<?> qtDataRefClass;
	private Class<?> qtFactoryClass;
	private Class<?> qtFileClass;
	private Class<?> qtSessionClass;
	private Class<?> timeRecordClass;
	private Object controller;
	private Object movie;
	private String movieName;
	private String errorMessage;
	private int version;
	private boolean hasQuickTime;
	private boolean controllerVisible;
	private double clipVolume;
	private double clipRate;

/* Private class variables */
	private static Object lock = new Object();

}

/* Package class: MovieClipListener */
/**
 * This class encapsulates the listeners for the movie clip.
 */
class MovieClipListener implements ComponentListener {

	public void componentResized(ComponentEvent e) {
		((Container) e.getSource()).validate();
	}

	public void componentHidden(ComponentEvent e) { }
	public void componentMoved(ComponentEvent e) { }
	public void componentShown(ComponentEvent e) { }

}

/* Package class: ErrorWindow */
/**
 * This class is displayed in the movie container if QuickTime does not exist
 * on the system or the movie can't be found.
 */
class ErrorWindow extends Component {

/* Constructor: ErrorWindow */
/**
 * Constructs an error window, passing in the movie name and the error message string.
 */
	public ErrorWindow(String name, String msg) {
		movieName = name;
		errorMessage = msg;
	}

/* Overridden method: paint */
/**
 * Paints the name of the movie and the error message.
 */
	public void paint(Graphics g) {
		Dimension size = getSize();
		FontMetrics fm = g.getFontMetrics();
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, size.width, size.height);
		g.setColor(Color.BLACK);
		int x = (size.width - fm.stringWidth(movieName)) / 2;
		int y = size.height / 2 - fm.getHeight();
		g.drawString(movieName, x, y);
		g.setColor(Color.RED);
		x = (size.width - fm.stringWidth(errorMessage)) / 2;
		y += 2 * fm.getHeight();
		g.drawString(errorMessage, x, y);
	}

/* Private instance variables */
	private String movieName;
	private String errorMessage;
}

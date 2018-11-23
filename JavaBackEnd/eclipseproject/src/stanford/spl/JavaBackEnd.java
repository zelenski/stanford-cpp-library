/*
 * @version 2018/07/16
 * - adjustment listener for GScrollBar
 * @version 2018/07/08
 * - cleanup parameter/variable names
 * @version 2018/06/20
 * - added mouseEntered, mouseExited, and mouseWheelMoved events
 * @version 2018/06/20
 * - added mouseEntered, mouseExited, and mouseWheelMoved events
 * @version 2016/11/24
 * - separated windowClosing / windowClosed operations to enable C++ to stop windows from closing
 * @version 2016/10/22
 * - bug fix for shutting down console repeatedly (don't shut down back-end)
 */

package stanford.spl;

import acm.graphics.GObject;
import acm.io.*;
import acm.program.*;
import acm.util.*;
import stanford.cs106.gui.GuiUtils;
import stanford.cs106.util.ExceptionUtils;
import stanford.cs106.util.StringUtils;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.sound.sampled.*;
import javax.swing.*;
import javax.swing.event.*;

@SuppressWarnings("deprecation")
public class JavaBackEnd implements
		ActionListener,
		ChangeListener,
		ComponentListener,
		KeyListener,
		MouseListener,
		MouseMotionListener,
		MouseWheelListener,
		Observer,
		WindowListener {
	
	// default window geometry
	public static final int DEFAULT_CONSOLE_X      = 10;
	public static final int DEFAULT_CONSOLE_Y      = 40;
	public static final int DEFAULT_CONSOLE_WIDTH  = 500;
	public static final int DEFAULT_CONSOLE_HEIGHT = 250;
	public static final int DEFAULT_GRAPHICS_X     = 10;
	public static final int DEFAULT_GRAPHICS_Y     = 10;
	
	// event classes; these should be kept in sync with C++ constants in gevents.h
	public static final int EVENT_SUBTYPE_MASK = 15;
	public static final int ACTION_EVENT       = 0x0010;
	public static final int KEY_EVENT          = 0x0020;
	public static final int TIMER_EVENT        = 0x0040;
	public static final int WINDOW_EVENT       = 0x0080;
	public static final int MOUSE_EVENT        = 0x0100;
	public static final int CLICK_EVENT        = 0x0200;
	// public static final int TABLE_EVENT	= 0x0400;   // see GTable.java
	// public static final int SERVER_EVENT	= 0x0800;
	public static final int CHANGE_EVENT       = 0x1000;
	public static final int ANY_EVENT          = 0x03F0;
	
	// event subtypes
	public static final int ACTION_PERFORMED = ACTION_EVENT + 1;
	public static final int WINDOW_CLOSED    = WINDOW_EVENT + 1;
	public static final int WINDOW_RESIZED   = WINDOW_EVENT + 2;
	public static final int WINDOW_CLOSING   = WINDOW_EVENT + 4;
	public static final int MOUSE_CLICKED    = MOUSE_EVENT + 1;
	public static final int MOUSE_PRESSED    = MOUSE_EVENT + 2;
	public static final int MOUSE_RELEASED   = MOUSE_EVENT + 3;
	public static final int MOUSE_MOVED      = MOUSE_EVENT + 4;
	public static final int MOUSE_DRAGGED    = MOUSE_EVENT + 5;
	public static final int MOUSE_ENTERED    = MOUSE_EVENT + 6;
	public static final int MOUSE_EXITED     = MOUSE_EVENT + 7;
	public static final int MOUSE_WHEEL_DOWN = MOUSE_EVENT + 8;
	public static final int MOUSE_WHEEL_UP   = MOUSE_EVENT + 9;
	public static final int KEY_PRESSED      = KEY_EVENT + 1;
	public static final int KEY_RELEASED     = KEY_EVENT + 2;
	public static final int KEY_TYPED        = KEY_EVENT + 3;
	public static final int TIMER_TICKED     = TIMER_EVENT + 1;
	// see GTable.java for TABLE_* types
	// public static final int SERVER_REQUEST   = SERVER_EVENT + 1;
	public static final int STATE_CHANGED    = CHANGE_EVENT + 1;
	
	// modifier flags (these must match the C++ ModifierCodes type in gevent.h)
	public static final int SHIFT_DOWN     = 0x1;
	public static final int CTRL_DOWN      = 0x2;
	public static final int META_DOWN      = 0x4;
	public static final int ALT_DOWN       = 0x8;
	public static final int ALT_GRAPH_DOWN = 0x10;
	public static final int BUTTON1_DOWN   = 0x20;
	public static final int BUTTON2_DOWN   = 0x40;
	public static final int BUTTON3_DOWN   = 0x80;
	
	private static final String DEFAULT_APP_NAME = "JBE";
	private static final String DEBUG_PROPERTY = "stanfordspl.debug";
	private static final String DEBUG_LOG_FILE = "stanfordspldebug.txt";
	private static boolean DEBUG = false;
	private static final Color ERROR_COLOR = new Color(192, 0, 0);   // slightly dark red
	private static final int ERROR_STYLE = Font.BOLD;
	private static Font ourConsoleFont = null;
	
	public static Font getConsoleFont() {
		return ourConsoleFont;
	}

	public static void main(String[] args) {
		try {
			String prop = System.getProperty(DEBUG_PROPERTY);
			DEBUG = prop != null && (prop.startsWith("t") || prop.startsWith("1"));
		} catch (Exception e) {
			// empty
		}
		
		new JavaBackEnd().run(args);
	}
	
	private String appName;
	private String exec;
	private String consoleWindowTitle = "Console";
	private JBEMenuBar menuBar;
	private Program program;
	private JBEConsole console;
	private JFrame consoleFrame;
	private int consoleCloseOperation = JFrame.HIDE_ON_CLOSE;

	private int consoleX = 10;
	private int consoleY = 40;
	private int consoleWidth = 500;
	private int consoleHeight = 250;
	private HashMap<String, JBECommand> cmdTable;
	private HashMap<String, JBEWindowInterface> windowTable;
	private HashMap<String, GObject> gobjTable;
	private HashMap<String, GTimer> timerTable;
	private HashMap<String, Image> imageTable;
	private HashMap<String, Clip> clipTable;
	private HashMap<String, Clip> clipIdTable;
	private HashMap<JComponent, String> sourceTable;
	private Container empty = JTFTools.createEmptyContainer();
	private int activeWindowCount;
	private int eventMask;
	private Object eventLock = new Object();
	private boolean eventAcknowledged;
	private boolean eventPending;

	// called by AutograderInput; represents input the user wants to insert
	public void update(Observable obs, Object arg) {
		String input = (String) arg;
		console.setInputScript(new BufferedReader(new StringReader(input)));
		// console.println(text);   // WRONG WRONG WRONG *** TODO WRONG WRONG *** TODO
	}
	
	public void setExitOnConsoleClose(boolean value) {
		if (value) {
			setConsoleCloseOperation(JFrame.EXIT_ON_CLOSE);
		} else {
			setConsoleCloseOperation(JFrame.HIDE_ON_CLOSE);
		}
	}
	
	public void setCppVersion(String version) {
		Version.setCppLibraryVersion(version);
	}
	
	public String getCppVersion() {
		return Version.getCppLibraryVersion();
	}

	public String getJbeVersion() {
		return Version.getLibraryVersion();
	}
	
	public JBEConsole getJBEConsole() {
		return console;
	}
	
	public int getJBEConsoleWidth() {
		return consoleWidth;
	}
	
	public int getJBEConsoleHeight() {
		return consoleHeight;
	}
	
	public JFrame getJBEConsoleFrame() {
		return consoleFrame;
	}

	public void run(String[] args) {
		processArguments(args);
		initSystemProperties();
		this.cmdTable = JBECommand.createCommandTable();
		this.imageTable = new HashMap<String, Image>();
		this.windowTable = new HashMap<String, JBEWindowInterface>();
		this.gobjTable = new HashMap<String, GObject>();
		this.timerTable = new HashMap<String, GTimer>();
		this.clipTable = new HashMap<String, Clip>();
		this.clipIdTable = new HashMap<String, Clip>();
		this.sourceTable = new HashMap<JComponent, String>();
		this.eventMask = 0;
		this.eventAcknowledged = false;
		this.eventPending = false;
		this.activeWindowCount = 0;
		this.console = new JBEConsole();
		this.console.setErrorColor(ERROR_COLOR);
		this.console.setErrorStyle(ERROR_STYLE);
		
		if (!GraphicsEnvironment.isHeadless()) {
			AutograderInput autograderInput = AutograderInput.getInstance(this);
			autograderInput.addObserver(this);
			this.menuBar = new JBEMenuBar(this, this.console);
			this.program = menuBar.getProgram();
			this.console.setMenuBar(this.menuBar);
			program.loadConfiguration();
			new Thread(new Runnable() {
				public void run() {
					try { Thread.sleep(1000); } catch (Exception e) {}
					Window window = program.getWindow();
					if (window != null) {
						window.toFront();
					}
				}
			}).start();
		}
		
		ourConsoleFont = this.console.getFont();
		
		if (this.exec != null) {
			try {
				Process localProcess = Runtime.getRuntime().exec(this.exec);
				System.setIn(localProcess.getInputStream());
				System.setOut(new PrintStream(localProcess.getOutputStream(), /* autoflush */ true));
			} catch (IOException localIOException) {
				System.err.println("Can't exec process: " + localIOException.getMessage());
			}
		}
		commandLoop();
	}

	public void createWindow(String windowId, int width, int height,
			TopCompound topCompound) {
		createWindow(windowId, width, height, topCompound, true);
	}
	
	// JL: SwingUtilities.invokeLater
	public void createWindow(String windowId, int width, int height,
			TopCompound top, boolean visible) {
		if (GraphicsEnvironment.isHeadless()) {
			JBEHeadlessWindow headlessWindow = new JBEHeadlessWindow(this, windowId, this.appName, width, height);
			this.windowTable.put(windowId, headlessWindow);
			// headlessWindow.setLocation(10, 10);
			// headlessWindow.setResizable(false);
			headlessWindow.getCanvas().setTopCompound(top);
			this.activeWindowCount += 1;
		} else {
			JBEWindow jbeWindow = new JBEWindow(this, windowId, this.appName, width, height);
			this.windowTable.put(windowId, jbeWindow);
			
			// commented out by Marty 2014/03/05;
			// This code used to set console's size to the size of the last created window
			// for some reason.  Why??  No.  Bad.  Turning this off.
			// this.consoleWidth = width;
			// this.consoleY = (50 + height);
			jbeWindow.pack();
			jbeWindow.setLocation(10, 10);
//			jbeWindow.getCanvas().initOffscreenImage();
			jbeWindow.getCanvas().setTopCompound(top);
			jbeWindow.setResizable(false);
			this.activeWindowCount += 1;
			if (visible) {
				jbeWindow.setVisible(true);
				waitForWindowActive(jbeWindow);
			} else {
				jbeWindow.setVisible(false);
			}
		}
	}

	public void deleteWindow(String windowId) {
		this.windowTable.remove(windowId);
	}

	public void defineGObject(String id, GObject gobj) {
		this.gobjTable.put(id, gobj);
	}

	public void defineSource(JComponent comp, String id) {
		this.sourceTable.put(comp, id);
	}

	public void deleteGObject(String id) {
		this.gobjTable.remove(id);
	}

	protected String getSourceId(JComponent comp) {
		return (String) this.sourceTable.get(comp);
	}

	public GObject getGObject(String id) {
		return (GObject) this.gobjTable.get(id);
	}

	public JComponent getInteractor(GObject gobj) {
		if ((gobj instanceof GInteractor)) {
			return ((GInteractor) gobj).getInteractor();
		}
		if ((gobj instanceof JBELabel)) {
			return ((JBELabel) gobj).getInteractor();
		}
		return null;
	}

	public JBEWindow getWindow(String windowId) {
		if (this.windowTable.get(windowId) instanceof JBEWindow) {
			return (JBEWindow) this.windowTable.get(windowId);
		} else {
			return null;
		}
	}

	/*
	 * version that works in headless mode
	 */
	public JBEWindowInterface getWindowInterface(String windowId) {
		return this.windowTable.get(windowId);
	}

	public void clearConsole() {
		this.console.clear();
	}
	
	public void setConsoleCloseOperation(int op) {
		this.consoleCloseOperation = op;
	}

	public void setConsoleFont(String fontString) {
		Font font = JTFTools.decodeFont(fontString);
		ourConsoleFont = font;
		this.console.setFont(font);
	}

	public void consoleMinimize() {
		if (this.consoleFrame != null) {
			this.consoleFrame.setState(JFrame.ICONIFIED);
		}
	}

	public void consoleToFront() {
		if (this.consoleFrame != null) {
			this.consoleFrame.setState(JFrame.NORMAL);
			this.consoleFrame.toFront();
		}
	}

	public void consoleSetTitle(String title) {
		this.consoleWindowTitle = title;
		if (this.consoleFrame != null) {
			this.consoleFrame.setTitle(title);
		}
		
		// mild hack: if C++ code tells me that program is "[completed]",
		// tell Program object that it is no longer running
		// (this is helpful for me for various hooks that wait for run() to be done)
		if (title.endsWith(AbstractConsoleProgram.PROGRAM_COMPLETED_TITLE_SUFFIX)) {
			this.program.setRunning(false);
		}
	}

	// JL: SwingUtilities.invokeLater
	public void setConsoleSize(int width, int height) {
		this.consoleWidth = width;
		this.consoleHeight = height;
//		if (this.console != null) {
//			this.console.setPreferredSize(this.consoleWidth, this.consoleHeight);
//			if (this.consoleFrame != null) {
//				this.consoleFrame.pack();
//			}
//		}
	}

	// JL: SwingUtilities.invokeLater
	public void setConsoleLocation(int x, int y) {
		this.consoleX = x;
		this.consoleY = y;
		if (this.consoleFrame != null) {
			this.consoleFrame.setLocation(this.consoleX, this.consoleY);
		}
	}

	public String getConsole() {
		if (this.consoleFrame == null) {
			showConsole();
		}
		return this.console.readLine();
	}

	protected void putConsole(String text) {
		putConsole(text, false);
	}
	
	public void putConsole(String text, boolean isStderr) {
		if (this.consoleFrame == null) {
			showConsole();
		}
		if (isStderr) {
			ConsoleModel model = this.console.getConsoleModel();
			model.print(text, ConsoleModel.ERROR_STYLE);
		} else {
			this.console.print(text);
		}
	}

	protected void endLineConsole() {
		endLineConsole(false);
	}
	
	protected void endLineConsole(boolean isStderr) {
		if (this.consoleFrame == null) {
			showConsole();
		}
		this.console.println();
	}

	public double getEventTime() {
		return new Date().getTime();
	}

	public void println(String text) {
		synchronized (this.eventLock) {
			SplPipeDecoder.println(text);
		}
	}

	public void acknowledgeEvent(String eventText, Object... args) {
		acknowledgeEvent(String.format(Locale.US, eventText, args));
	}
	
	public void acknowledgeEvent(String eventText) {
		synchronized (this.eventLock) {
			SplPipeDecoder.println(eventText);
			if (!this.eventAcknowledged) {
				// strip (), "" stuff from event to shorten it / ease encoding/decoding
				eventText = eventText.replaceAll("\\(.{0,99999}", "");
				eventText = eventText.replaceAll("\".{0,99999}", "");
				SplPipeDecoder.writeAck(eventText);
				this.eventAcknowledged = true;
				this.eventPending = false;
			} else {
				this.eventPending = true;
			}
		}
	}
	
	public void acknowledgeEvent() {
		synchronized (this.eventLock) {
			if (!this.eventAcknowledged) {
				SplPipeDecoder.writeAck();
				this.eventAcknowledged = true;
				this.eventPending = false;
			} else {
				this.eventPending = true;
			}
		}
	}

	protected void getNextEvent(int eventMask) {
		synchronized (this.eventLock) {
			this.eventAcknowledged = false;
			this.eventPending = false;
			acknowledgeEvent();
			this.eventMask = eventMask;
		}
	}

	protected void waitForEvent(int eventMask) {
		synchronized (this.eventLock) {
			this.eventAcknowledged = false;
			if (this.eventPending) {
				acknowledgeEvent();
			}
			this.eventMask = eventMask;
		}
	}

	// JL: SwingUtilities.invokeLater
	private void showConsole() {
		this.console.setPreferredSize(this.consoleWidth, this.consoleHeight);
		this.consoleFrame = new JFrame(this.consoleWindowTitle);
		this.consoleFrame.setDefaultCloseOperation(consoleCloseOperation);
		this.consoleFrame.setLayout(new BorderLayout());
		this.consoleFrame.add(this.console);
		this.consoleFrame.pack();
		this.consoleFrame.setLocation(this.consoleX, this.consoleY);
		this.consoleFrame.addWindowListener(this);
		if (Platform.isMac()) {
			this.menuBar.install(this.consoleFrame);
		} else {
			this.consoleFrame.setJMenuBar(this.menuBar);
		}
		this.consoleFrame.setVisible(true);
		waitForWindowActive(this.consoleFrame);
		this.activeWindowCount += 1;
	}

	protected void createSound(String id, String filename) {
		Clip clip = getClip(filename);
		this.clipIdTable.put(id, clip);
	}

	protected void deleteSound(String id) {
		this.clipIdTable.remove(id);
	}

	protected void playSound(String id) {
		Clip clip = (Clip) this.clipIdTable.get(id);
		if (clip != null) {
			clip.stop();
			clip.setFramePosition(0);
			clip.start();
		}
	}

	protected void createTimer(String id, double ms) {
		GTimer timer = new GTimer(id, ms);
		timer.addActionListener(this);
		this.timerTable.put(id, timer);
	}

	protected void deleteTimer(String id) {
		GTimer timer = this.timerTable.get(id);
		if (timer != null) {
			timer.stop();
		}
		this.timerTable.remove(id);
	}

	protected void startTimer(String id) {
		GTimer timer = this.timerTable.get(id);
		if (timer != null) {
			timer.start();
		}
	}

	protected void stopTimer(String id) {
		GTimer timer = this.timerTable.get(id);
		if (timer != null) {
			timer.stop();
		}
	}

	// https://docs.oracle.com/javase/7/docs/api/constant-values.html#java.awt.event.InputEvent.BUTTON1_DOWN_MASK
	private int convertModifiers(int modifiers) {
		int converted = 0;
		if ((modifiers & 0x40) != 0) {
			converted |= SHIFT_DOWN;
		}
		if ((modifiers & 0x80) != 0) {
			converted |= CTRL_DOWN;
		}
		if ((modifiers & 0x100) != 0) {
			converted |= META_DOWN;
		}
		if ((modifiers & 0x200) != 0) {
			converted |= ALT_DOWN;
		}
		if ((modifiers & 0x2000) != 0) {
			converted |= ALT_GRAPH_DOWN;
		}
		if ((modifiers & 0x400) != 0) {
			converted |= BUTTON1_DOWN;
		}
		if ((modifiers & 0x800) != 0) {
			converted |= BUTTON2_DOWN;
		}
		if ((modifiers & 0x1000) != 0) {
			converted |= BUTTON3_DOWN;
		}
		return converted;
	}

	public String openFileDialog(String title, String mode, String path) {
		// BUGFIX: (2014/10/09) was crashing when null/default current dir was passed by C++ lib
		if (path == null) {
			path = "";
		}
		JBEFileFilter fileFilter = new JBEFileFilter(path);
		JFileChooser fileChooser = new JFileChooser(fileFilter.getDirectory());
		fileChooser.setFileFilter(fileFilter);
		fileChooser.setDialogTitle(title);
		int result = 0;
		Component parent = null;
		if (console != null) {
			parent = console;
		}
		if (mode.equalsIgnoreCase("load")) {
			result = fileChooser.showOpenDialog(parent);
		} else if (mode.equalsIgnoreCase("save")) {
			result = fileChooser.showSaveDialog(parent);
		} else {
			return "";
		}
		
		if (result == JFileChooser.APPROVE_OPTION) {
			return fileChooser.getSelectedFile().getAbsolutePath();
		} else {
			return "";
		}
	}

	public void actionPerformed(ActionEvent event) {
		Object source = event.getSource();
		Object sourceId;
		if ((source instanceof GTimer)) {
			if (listeningForEvent(TIMER_EVENT)) {
				sourceId = (GTimer) event.getSource();
				acknowledgeEvent("event:timerTicked(\"%s\", %d)",
						((GTimer) sourceId).getId(), (long) getEventTime());
			}
		} else if (listeningForEvent(ACTION_EVENT)) {
			sourceId = getSourceId((JComponent) source);
			GInteractor localGInteractor = (GInteractor) getGObject((String) sourceId);
			String str = localGInteractor.getActionCommand();
			// if (str != null && !str.isEmpty()) {
				acknowledgeEvent("event:actionPerformed(\"%s\", \"%s\", %d, %d)",
						(String) sourceId, str,
						(long) getEventTime(),
						event.getModifiers());
			// }
		}
	}

	public void keyPressed(KeyEvent event) {
		if (listeningForEvent(KEY_EVENT)) {
			printEvent("keyPressed", event);
		}
	}

	public void keyReleased(KeyEvent event) {
		if (listeningForEvent(KEY_EVENT)) {
			printEvent("keyReleased", event);
		}
	}

	public void keyTyped(KeyEvent event) {
		if (listeningForEvent(KEY_EVENT)) {
			printEvent("keyTyped", event);
		}
	}

	private void printEvent(String type, KeyEvent keyEvent) {
		String windowId = getWindowId(keyEvent);
		int keyCode = keyEvent.getKeyCode();
		acknowledgeEvent("event:%s(\"%s\", %d, %d, %d, %d)",
				type, windowId, (long) getEventTime(),
				convertModifiers(keyEvent.getModifiersEx()),
				(int) keyEvent.getKeyChar(),
				keyCode);
	}
	
	private void printEvent(String type, String sourceId, AdjustmentEvent docEvent) {
		acknowledgeEvent("event:%s(\"%s\", %d)",
				type, sourceId, (long) getEventTime());
	}
	
	private void printEvent(String type, String sourceId, DocumentEvent docEvent) {
		acknowledgeEvent("event:%s(\"%s\", %d)",
				type, sourceId, (long) getEventTime());
	}
	
	private void printEvent(String type, String sourceId, HyperlinkEvent linkEvent) {
		/* The underlying URL may be null if the link can't be interpreted as a URL.
		 * To fix this, we'll fall back on the link description, which the JavaDoc
		 * describes as being the proper way to handle a malformed URL.
		 */
		String url = (linkEvent.getURL() != null? linkEvent.getURL().toString() : linkEvent.getDescription());
		url = StringUtils.urlEncode(url);
		acknowledgeEvent("event:%s(\"%s\", \"%s\", %d)",
				type, sourceId, url, (long) getEventTime());
	}
	
//	private void printEvent(String type, String sourceId, AWTEvent event) {
//		acknowledgeEvent("event:%s(\"%s\", %d, %d)",
//				type, sourceId, (long) getEventTime());
//	}
	
	private String getWindowId(EventObject event) {
		Object src = event.getSource();
		if (src instanceof JBECanvas) {
			JBECanvas canvas = (JBECanvas) src;
			return canvas.getWindowId();
		} else if (src instanceof JBEWindow) {
			JBEWindow window = (JBEWindow) src;
			return window.getWindowId();
		} else if (src instanceof Component) {
			Component comp = (Component) src;
			JBEWindow window = GuiUtils.getAncestor(comp, JBEWindow.class);
			if (window != null) {
				return window.getWindowId();
			}
		}
		return "???";
	}
	
	/**
	 * Returns a DocumentListener that listens for events on the given interactor.
	 * This method is needed for DocumentListener unlike other listeners because DocumentEvent
	 * does not have a getSource method, so we need a reference to the back-end and interactor
	 * to determine the event source.
	 */
	public DocumentListener createDocumentListener(final GInteractor interactor) {
		return new DocumentListener() {
			// required method of DocumentListener interface
			public void changedUpdate(DocumentEvent event) {
				fireEvent(event);
			}
			
			// required method of DocumentListener interface
			public void removeUpdate(DocumentEvent event) {
				fireEvent(event);
			}
			
			// required method of DocumentListener interface
			public void insertUpdate(DocumentEvent event) {
				fireEvent(event);
			}
			
			// common code to actually send the event through the pipe
			private void fireEvent(DocumentEvent event) {
				String sourceId = sourceTable.get(interactor.getInteractor());
				printEvent("stateChanged", sourceId, event);
			}
		};
	};

	/**
	 * Returns an AdjustmentListener that listens for scrolling on the given scroll bar.
	 */
	public AdjustmentListener createAdjustmentListener(final GScrollBar scrollBar) {
		return new AdjustmentListener() {
			// required method of AdjustmentListener interface
			public void adjustmentValueChanged(AdjustmentEvent event) {
				String sourceId = sourceTable.get(scrollBar.getInteractor());
				printEvent("scrollPerformed", sourceId, event);
			}
		};
	}

	/**
	 * Returns a HyperlinkListener that listens for hyperlink events on the given interactor.
	 */
	public HyperlinkListener createHyperlinkListener(final GInteractor interactor) {
		return new HyperlinkListener() {
			// required method of DocumentListener interface
			public void hyperlinkUpdate(HyperlinkEvent event) {
				if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
					String sourceId = sourceTable.get(interactor.getInteractor());
					printEvent("hyperlinkClicked", sourceId, event);
				}
			}
		};
	}

	public void mouseClicked(MouseEvent event) {
		((Component) event.getSource()).requestFocus();
		if (listeningForEvent(MOUSE_EVENT | CLICK_EVENT)) {
			printEvent("mouseClicked", event);
		}
	}

	public void mouseEntered(MouseEvent event) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseEntered", event);
		}
	}

	public void mouseExited(MouseEvent event) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseExited", event);
		}
	}

	public void mousePressed(MouseEvent event) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mousePressed", event);
		}
	}

	public void mouseReleased(MouseEvent event) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseReleased", event);
		}
	}

	public void mouseMoved(MouseEvent event) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseMoved", event);
		}
	}

	public void mouseDragged(MouseEvent event) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseDragged", event);
		}
	}

	public void mouseWheelMoved(MouseWheelEvent event) {
		if (listeningForEvent(MOUSE_EVENT)) {
			int rotation = event.getWheelRotation();
			if (rotation > 0) {
				printEvent("mouseWheelDown", event);
			} else if (rotation < 0) {
				printEvent("mouseWheelUp", event);
			}
		}
	}

	private void printEvent(String type, MouseEvent mouseEvent) {
		String windowId = getWindowId(mouseEvent);
		acknowledgeEvent("event:%s(\"%s\", %d, %d, %d, %d)",
				type, windowId, (long) getEventTime(),
				convertModifiers(mouseEvent.getModifiersEx()),
				mouseEvent.getX(), mouseEvent.getY());
	}

	public void windowActivated(WindowEvent event) {
		// empty
	}

	public void windowClosed(WindowEvent event) {
		if (event.getSource() == this.consoleFrame) {
			if (consoleCloseOperation == JFrame.DO_NOTHING_ON_CLOSE) {
				return;
			} else if (consoleCloseOperation == JFrame.EXIT_ON_CLOSE) {
				shutdownBackEnd(/* sendEvent */ false);
			} else if (consoleCloseOperation == JFrame.HIDE_ON_CLOSE) {
				consoleFrame.setVisible(false);
			}
			
			acknowledgeEvent("event:consoleWindowClosed()");
		} else {
			JBEWindow window = (JBEWindow) event.getSource();
			if (this.windowTable.containsKey(window.getWindowId())) {
				acknowledgeEvent("event:windowClosed(\"%s\", %d)", window.getWindowId(), (long) getEventTime());
				this.windowTable.remove(window.getWindowId());
			}

			this.activeWindowCount -= 1;
			if (this.activeWindowCount == 0) {
				acknowledgeEvent("event:lastWindowGWindow_closed()");
				shutdownBackEnd(false);
			}
		}
	}
	
	// notify the back end that a window has been closed;
	// this is used with headless windows only
	public void notifyOfWindowClosed(JBEWindowInterface window) {
		String windowId = window.getWindowId();
		if (this.windowTable.containsKey(windowId)) {
			acknowledgeEvent("event:windowClosed(\"%s\", %d)", windowId, (long) getEventTime());
			this.windowTable.remove(windowId);
		}

		this.activeWindowCount -= 1;
		if (this.activeWindowCount == 0) {
			acknowledgeEvent("event:lastWindowGWindow_closed()");
			shutdownBackEnd(false);
		}
		
	}

	public void shutdownBackEnd(boolean sendEvent) {
		new Thread(new Runnable() {
			public void run() {
				try {
					Thread.sleep(200);
				} catch (InterruptedException ie) {}
				try {
					System.out.close();
				} catch (Exception e) {
					// empty
				}
				try {
					System.in.close();
				} catch (Exception e) {
					// empty
				}
				System.exit(0);
			}
		}).start();
		
		if (sendEvent) {
			if (consoleFrame != null) {
				consoleFrame.setVisible(false);
				acknowledgeEvent("event:consoleWindowClosed()");
			}
			acknowledgeEvent("event:lastWindowGWindow_closed()");
		}
	}
	
	public void windowClosing(WindowEvent event) {
		if (event.getSource() == this.consoleFrame) {
			if (consoleCloseOperation == JFrame.DO_NOTHING_ON_CLOSE) {
				return;
			}
		} else {
			JBEWindow window = (JBEWindow) event.getSource();
			if (this.windowTable.containsKey(window.getWindowId())) {
				acknowledgeEvent("event:windowClosing(\"%s\", %d)", window.getWindowId(), (long) getEventTime());
			}
		}
	}

	public void windowDeactivated(WindowEvent event) {
		// empty
	}

	public void windowDeiconified(WindowEvent event) {
		// empty
	}

	public void windowIconified(WindowEvent event) {
		// empty
	}

	public void windowOpened(WindowEvent event) {
		JFrame frame = (JFrame) event.getSource();
		synchronized (frame) {
			frame.notifyAll();
		}
	}

	private void waitForWindowActive(JFrame frame) {
		synchronized (frame) {
			while (!frame.isShowing()) {
				try {
					frame.wait();
					Thread.sleep(100);
				} catch (InterruptedException ie) {
					// empty
				}
			}
		}
	}

	// required method of ChangeListener interface
	public void stateChanged(ChangeEvent event) {
		Object source = event.getSource();
		if (listeningForEvent(CHANGE_EVENT)) {
			String sourceId = getSourceId((JComponent) source);
			acknowledgeEvent("event:stateChanged(\"%s\", %d)", sourceId, (long) getEventTime());
		} else if (listeningForEvent(ACTION_EVENT)) {
			String sourceId = getSourceId((JComponent) source);
			GInteractor interactor = (GInteractor) getGObject(sourceId);
			String actionCommand = interactor.getActionCommand();
			if (!actionCommand.isEmpty()) {
				acknowledgeEvent("event:actionPerformed(\"%s\", \"%s\", %d)", sourceId, actionCommand, (long) getEventTime());
			}
		}
	}

	public void componentHidden(ComponentEvent event) {
		// empty
	}

	public void componentMoved(ComponentEvent event) {
		// empty
	}

	public void componentResized(ComponentEvent event) {
		if (listeningForEvent(WINDOW_EVENT)) {
			String windowId = getWindowId(event);
			acknowledgeEvent("event:windowResized(\"%s\", %d)", windowId, (long) getEventTime());
		}
	}

	public void componentShown(ComponentEvent event) {
		// empty
	}
	
	// returns true if the given event type (such as MOUSE_EVENT) is currently part
	// of JBE's event mask, meaning that we are listening for that type of event
	private boolean listeningForEvent(int type) {
		return (this.eventMask & type) != 0;
	}
	
	private void printLog(String text) {
		try {
			PrintStream out = new PrintStream(new FileOutputStream(DEBUG_LOG_FILE, /* append */ true));
			out.println(text);
			out.flush();
			out.close();
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	private void commandLoop() {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		final TokenScanner scanner = new TokenScanner();
		scanner.ignoreWhitespace();
		scanner.scanNumbers();
		scanner.scanStrings();
		scanner.addWordCharacters(".");
		try {
			for (;;) {
				String command = reader.readLine();
				if (command == null) {
					break;
				}
				if (DEBUG) {
					printLog(command);
				}
				
				if (command.equals("LongCommand.begin()")) {
					command = readLongCommand(reader);
				}
				
				scanner.setInput(command);
				String commandName = scanner.nextToken();
				final JBECommand jbeCommand = this.cmdTable.get(commandName);
				if (jbeCommand == null) {
					System.err.println("Unexpected error: unknown command \"" + commandName + "\"");
				} else {
					// execute JBE command; some commands must be run on Swing event thread
					if (jbeCommand.shouldRunOnSwingEventThread()) {
						Runnable run = new Runnable() {
							public void run() {
								jbeCommand.execute(scanner, JavaBackEnd.this);
							}
						};
						if (jbeCommand.shouldInvokeAndWait()) {
							try {
								SwingUtilities.invokeAndWait(run);
							} catch (InvocationTargetException ite) {
								// TODO: is this the right way to handle such an error?  :-/ not sure
								// println("error:" + ite.getMessage());
								Throwable cause = ExceptionUtils.getUnderlyingCause(ite);
								System.err.println("Unexpected error: " + cause.getMessage());
								cause.printStackTrace(System.err);
							} catch (InterruptedException ie) {
								// okay; do nothing
							}
						} else {
							SwingUtilities.invokeLater(run);
						}
					} else {
						// don't run on Swing event thread; just run on this current thread
						jbeCommand.execute(scanner, this);
					}
				}
			}
		} catch (Exception ex) {
			System.err.println("Unexpected error: " + ex.getMessage());
			ex.printStackTrace(System.err);
			if (DEBUG) {
				printLog("Unexpected error: " + ex.getMessage());
			}
		}
	}
	
	/*
	 * Handle very long command strings in special way.
	 */
	private String readLongCommand(BufferedReader systemInReader) throws IOException {
		// repeatedly read lines from System.in until long command is done
		StringBuilder sb = new StringBuilder(256000);
		while (true) {
			String line = systemInReader.readLine();
			if (DEBUG) {
				printLog(line);
			}
			if (line == null || line.equals("LongCommand.end()")) {
				break;
			}
			sb.append(line);
		}
		
		String cmd = sb.toString();
		return cmd;
	}

	private void processArguments(String[] args) {
		this.appName = DEFAULT_APP_NAME;
		this.exec = null;
		for (int i = 0; i < args.length; i++) {
			String str = args[i];
			if (str.startsWith("-")) {
				if (str.equals("-exec")) {
					this.exec = args[(++i)];
				} else {
					System.err.println("Error: Unrecognized option " + str);
				}
			} else {
				this.appName = str;
			}
		}
	}

	private void initSystemProperties() {
		if (!GraphicsEnvironment.isHeadless()) {
			System.setProperty("com.apple.mrj.application.apple.menu.about.name", this.appName);
			GuiUtils.setSystemLookAndFeel();
		}
	}

	protected Image getImage(String filename) {
		Image image = this.imageTable.get(filename);
		if (image != null) {
			return image;
		}
		Toolkit tk = Toolkit.getDefaultToolkit();
		if (filename.startsWith("http:")) {
			try {
				image = tk.getImage(new URL(filename));
			} catch (MalformedURLException localMalformedURLException) {
				throw new ErrorException("loadImage: Malformed URL");
			}
		} else {
			File file = new File(filename);
			if (!file.exists() && !filename.startsWith("/")
					&& !filename.startsWith(".")) {
				file = new File("images/" + filename);
			}
			if (!file.exists()) {
				throw new ErrorException("loadImage: File not found");
			}
			image = tk.getImage(file.getAbsolutePath());
		}
		if (image == null) {
			throw new ErrorException("loadImage: File not found");
		}
		MediaTracker mt = new MediaTracker(this.empty);
		mt.addImage(image, 0);
		try {
			mt.waitForID(0);
		} catch (InterruptedException ie) {
			throw new ErrorException("loadImage: Loading interrupted");
		}
		this.imageTable.put(filename, image);
		return image;
	}

	protected Clip getClip(String filename) {
		Clip clip = (Clip) this.clipTable.get(filename);
		if (clip != null) {
			return clip;
		}
		try {
			clip = AudioSystem.getClip();
			File file = new File(filename);
			if (!file.exists() && (!filename.startsWith("/"))
					&& (!filename.startsWith("."))) {
				file = new File("sounds/" + filename);
			}
			if (!file.exists()) {
				throw new ErrorException("createClip: File not found");
			}
			InputStream in = new BufferedInputStream(new FileInputStream(file));
			AudioInputStream audioStream = AudioSystem
					.getAudioInputStream(in);
			clip.open(audioStream);
		} catch (IOException ioe) {
			throw new ErrorException("getClip: I/O error (" + new File(filename).getAbsolutePath() + ")");
		} catch (Exception ex) {
			throw new ErrorException("getClip: " + ex);
		}
		this.clipTable.put(filename, clip);
		return clip;
	}
	
	public KeyListener getConsoleKeyListener() {
		ConsoleModel consoleModel = this.console.getConsoleModel();
		if (consoleModel != null) {
			return (KeyListener) consoleModel;   // StandardConsoleModel implements KeyListener
		}
		return null;
	}
}

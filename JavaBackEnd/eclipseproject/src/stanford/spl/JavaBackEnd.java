/*
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

	public static void main(String[] paramArrayOfString) {
		try {
			String prop = System.getProperty(DEBUG_PROPERTY);
			DEBUG = prop != null && (prop.startsWith("t") || prop.startsWith("1"));
		} catch (Exception e) {
			// empty
		}
		
		new JavaBackEnd().run(paramArrayOfString);
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

	public void run(String[] paramArrayOfString) {
		processArguments(paramArrayOfString);
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

	public void createWindow(String paramString, int paramInt1, int paramInt2,
			TopCompound paramTopCompound) {
		createWindow(paramString, paramInt1, paramInt2, paramTopCompound, true);
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
			// this.consoleWidth = paramInt1;
			// this.consoleY = (50 + paramInt2);
			jbeWindow.pack();
			jbeWindow.setLocation(10, 10);
//			localJBEWindow.getCanvas().initOffscreenImage();
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

	public void deleteWindow(String paramString) {
		this.windowTable.remove(paramString);
	}

	public void defineGObject(String paramString, GObject paramGObject) {
		this.gobjTable.put(paramString, paramGObject);
	}

	public void defineSource(JComponent paramJComponent, String paramString) {
		this.sourceTable.put(paramJComponent, paramString);
	}

	public void deleteGObject(String paramString) {
		this.gobjTable.remove(paramString);
	}

	protected String getSourceId(JComponent paramJComponent) {
		return (String) this.sourceTable.get(paramJComponent);
	}

	public GObject getGObject(String paramString) {
		return (GObject) this.gobjTable.get(paramString);
	}

	public JComponent getInteractor(GObject paramGObject) {
		if ((paramGObject instanceof GInteractor)) {
			return ((GInteractor) paramGObject).getInteractor();
		}
		if ((paramGObject instanceof JBELabel)) {
			return ((JBELabel) paramGObject).getInteractor();
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

	public void setConsoleFont(String paramString) {
		Font font = JTFTools.decodeFont(paramString);
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
	public void setConsoleSize(int paramInt1, int paramInt2) {
		this.consoleWidth = paramInt1;
		this.consoleHeight = paramInt2;
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

	protected void putConsole(String paramString) {
		putConsole(paramString, false);
	}
	
	public void putConsole(String paramString, boolean isStderr) {
		if (this.consoleFrame == null) {
			showConsole();
		}
		if (isStderr) {
			ConsoleModel model = this.console.getConsoleModel();
			model.print(paramString, ConsoleModel.ERROR_STYLE);
		} else {
			this.console.print(paramString);
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

	public void println(String paramString) {
		synchronized (this.eventLock) {
			SplPipeDecoder.println(paramString);
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

	protected void getNextEvent(int paramInt) {
		synchronized (this.eventLock) {
			this.eventAcknowledged = false;
			this.eventPending = false;
			acknowledgeEvent();
			this.eventMask = paramInt;
		}
	}

	protected void waitForEvent(int paramInt) {
		synchronized (this.eventLock) {
			this.eventAcknowledged = false;
			if (this.eventPending) {
				acknowledgeEvent();
			}
			this.eventMask = paramInt;
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

	protected void createSound(String paramString1, String paramString2) {
		Clip localClip = getClip(paramString2);
		this.clipIdTable.put(paramString1, localClip);
	}

	protected void deleteSound(String paramString) {
		this.clipIdTable.remove(paramString);
	}

	protected void playSound(String paramString) {
		Clip localClip = (Clip) this.clipIdTable.get(paramString);
		if (localClip != null) {
			localClip.stop();
			localClip.setFramePosition(0);
			localClip.start();
		}
	}

	protected void createTimer(String paramString, double paramDouble) {
		GTimer localGTimer = new GTimer(paramString, paramDouble);
		localGTimer.addActionListener(this);
		this.timerTable.put(paramString, localGTimer);
	}

	protected void deleteTimer(String paramString) {
		GTimer localGTimer = (GTimer) this.timerTable.get(paramString);
		if (localGTimer != null) {
			localGTimer.stop();
		}
		this.timerTable.remove(paramString);
	}

	protected void startTimer(String paramString) {
		GTimer localGTimer = (GTimer) this.timerTable.get(paramString);
		if (localGTimer != null) {
			localGTimer.start();
		}
	}

	protected void stopTimer(String paramString) {
		GTimer localGTimer = (GTimer) this.timerTable.get(paramString);
		if (localGTimer != null) {
			localGTimer.stop();
		}
	}

	// https://docs.oracle.com/javase/7/docs/api/constant-values.html#java.awt.event.InputEvent.BUTTON1_DOWN_MASK
	private int convertModifiers(int paramInt) {
		int i = 0;
		if ((paramInt & 0x40) != 0) {
			i |= SHIFT_DOWN;
		}
		if ((paramInt & 0x80) != 0) {
			i |= CTRL_DOWN;
		}
		if ((paramInt & 0x100) != 0) {
			i |= META_DOWN;
		}
		if ((paramInt & 0x200) != 0) {
			i |= ALT_DOWN;
		}
		if ((paramInt & 0x2000) != 0) {
			i |= ALT_GRAPH_DOWN;
		}
		if ((paramInt & 0x400) != 0) {
			i |= BUTTON1_DOWN;
		}
		if ((paramInt & 0x800) != 0) {
			i |= BUTTON2_DOWN;
		}
		if ((paramInt & 0x1000) != 0) {
			i |= BUTTON3_DOWN;
		}
		return i;
	}

	public String openFileDialog(String title, String mode, String path) {
		// BUGFIX: (2014/10/09) was crashing when null/default current dir was passed by C++ lib
		if (path == null) {
			path = "";
		}
		JBEFileFilter localJBEFileFilter = new JBEFileFilter(path);
		JFileChooser localJFileChooser = new JFileChooser(localJBEFileFilter.getDirectory());
		localJFileChooser.setFileFilter(localJBEFileFilter);
		localJFileChooser.setDialogTitle(title);
		int result = 0;
		Component parent = null;
		if (console != null) {
			parent = console;
		}
		if (mode.equalsIgnoreCase("load")) {
			result = localJFileChooser.showOpenDialog(parent);
		} else if (mode.equalsIgnoreCase("save")) {
			result = localJFileChooser.showSaveDialog(parent);
		} else {
			return "";
		}
		
		if (result == JFileChooser.APPROVE_OPTION) {
			return localJFileChooser.getSelectedFile().getAbsolutePath();
		} else {
			return "";
		}
	}

	public void actionPerformed(ActionEvent paramActionEvent) {
		Object localObject1 = paramActionEvent.getSource();
		Object localObject2;
		if ((localObject1 instanceof GTimer)) {
			if (listeningForEvent(TIMER_EVENT)) {
				localObject2 = (GTimer) paramActionEvent.getSource();
				acknowledgeEvent("event:timerTicked(\"%s\", %d)",
						((GTimer) localObject2).getId(), (long) getEventTime());
			}
		} else if (listeningForEvent(ACTION_EVENT)) {
			localObject2 = getSourceId((JComponent) localObject1);
			GInteractor localGInteractor = (GInteractor) getGObject((String) localObject2);
			String str = localGInteractor.getActionCommand();
			// if (str != null && !str.isEmpty()) {
				acknowledgeEvent("event:actionPerformed(\"%s\", \"%s\", %d, %d)",
						(String) localObject2, str,
						(long) getEventTime(),
						paramActionEvent.getModifiers());
			// }
		}
	}

	public void keyPressed(KeyEvent paramKeyEvent) {
		if (listeningForEvent(KEY_EVENT)) {
			printEvent("keyPressed", paramKeyEvent);
		}
	}

	public void keyReleased(KeyEvent paramKeyEvent) {
		if (listeningForEvent(KEY_EVENT)) {
			printEvent("keyReleased", paramKeyEvent);
		}
	}

	public void keyTyped(KeyEvent paramKeyEvent) {
		if (listeningForEvent(KEY_EVENT)) {
			printEvent("keyTyped", paramKeyEvent);
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
	
	private void printEvent(String type, String sourceId, DocumentEvent docEvent) {
		acknowledgeEvent("event:%s(\"%s\", %d)",
				type, sourceId, (long) getEventTime());
	}
	
	private void printEvent(String type, String sourceId, HyperlinkEvent linkEvent) {
		String url = linkEvent.getURL().toString();
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

	public void mouseClicked(MouseEvent paramMouseEvent) {
		((Component) paramMouseEvent.getSource()).requestFocus();
		if (listeningForEvent(MOUSE_EVENT | CLICK_EVENT)) {
			printEvent("mouseClicked", paramMouseEvent);
		}
	}

	public void mouseEntered(MouseEvent paramMouseEvent) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseEntered", paramMouseEvent);
		}
	}

	public void mouseExited(MouseEvent paramMouseEvent) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseExited", paramMouseEvent);
		}
	}

	public void mousePressed(MouseEvent paramMouseEvent) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mousePressed", paramMouseEvent);
		}
	}

	public void mouseReleased(MouseEvent paramMouseEvent) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseReleased", paramMouseEvent);
		}
	}

	public void mouseMoved(MouseEvent paramMouseEvent) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseMoved", paramMouseEvent);
		}
	}

	public void mouseDragged(MouseEvent paramMouseEvent) {
		if (listeningForEvent(MOUSE_EVENT)) {
			printEvent("mouseDragged", paramMouseEvent);
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

	public void windowActivated(WindowEvent paramWindowEvent) {
		// empty
	}

	public void windowClosed(WindowEvent paramWindowEvent) {
		if (paramWindowEvent.getSource() == this.consoleFrame) {
			if (consoleCloseOperation == JFrame.DO_NOTHING_ON_CLOSE) {
				return;
			} else if (consoleCloseOperation == JFrame.EXIT_ON_CLOSE) {
				shutdownBackEnd(/* sendEvent */ false);
			} else if (consoleCloseOperation == JFrame.HIDE_ON_CLOSE) {
				consoleFrame.setVisible(false);
			}
			
			acknowledgeEvent("event:consoleWindowClosed()");
		} else {
			JBEWindow localJBEWindow = (JBEWindow) paramWindowEvent.getSource();
			if (this.windowTable.containsKey(localJBEWindow.getWindowId())) {
				acknowledgeEvent("event:windowClosed(\"%s\", %d)", localJBEWindow.getWindowId(), (long) getEventTime());
				this.windowTable.remove(localJBEWindow.getWindowId());
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
	
	public void windowClosing(WindowEvent paramWindowEvent) {
		if (paramWindowEvent.getSource() == this.consoleFrame) {
			if (consoleCloseOperation == JFrame.DO_NOTHING_ON_CLOSE) {
				return;
			}
		} else {
			JBEWindow localJBEWindow = (JBEWindow) paramWindowEvent.getSource();
			if (this.windowTable.containsKey(localJBEWindow.getWindowId())) {
				acknowledgeEvent("event:windowClosing(\"%s\", %d)", localJBEWindow.getWindowId(), (long) getEventTime());
			}
		}
	}

	public void windowDeactivated(WindowEvent paramWindowEvent) {
		// empty
	}

	public void windowDeiconified(WindowEvent paramWindowEvent) {
		// empty
	}

	public void windowIconified(WindowEvent paramWindowEvent) {
		// empty
	}

	public void windowOpened(WindowEvent paramWindowEvent) {
		JFrame localJFrame = (JFrame) paramWindowEvent.getSource();
		synchronized (localJFrame) {
			localJFrame.notifyAll();
		}
	}

	private void waitForWindowActive(JFrame paramJFrame) {
		synchronized (paramJFrame) {
			while (!paramJFrame.isShowing()) {
				try {
					paramJFrame.wait();
					Thread.sleep(100L);
				} catch (InterruptedException localInterruptedException) {
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

	public void componentHidden(ComponentEvent paramComponentEvent) {
		// empty
	}

	public void componentMoved(ComponentEvent paramComponentEvent) {
		// empty
	}

	public void componentResized(ComponentEvent componentEvent) {
		if (listeningForEvent(WINDOW_EVENT)) {
			String windowId = getWindowId(componentEvent);
			acknowledgeEvent("event:windowResized(\"%s\", %d)", windowId, (long) getEventTime());
		}
	}

	public void componentShown(ComponentEvent paramComponentEvent) {
	}
	
	// returns true if the given event type (such as MOUSE_EVENT) is currently part
	// of JBE's event mask, meaning that we are listening for that type of event
	private boolean listeningForEvent(int type) {
		return (this.eventMask & type) != 0;
	}
	
	private void printLog(String s) {
		try {
			PrintStream out = new PrintStream(new FileOutputStream(DEBUG_LOG_FILE, /* append */ true));
			out.println(s);
			out.flush();
			out.close();
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	private void commandLoop() {
		BufferedReader localBufferedReader = new BufferedReader(new InputStreamReader(System.in));
		final TokenScanner localTokenScanner = new TokenScanner();
		localTokenScanner.ignoreWhitespace();
		localTokenScanner.scanNumbers();
		localTokenScanner.scanStrings();
		localTokenScanner.addWordCharacters(".");
		try {
			for (;;) {
				String str1 = localBufferedReader.readLine();
				if (str1 == null) {
					break;
				}
				if (DEBUG) {
					printLog(str1);
				}
				
				if (str1.equals("LongCommand.begin()")) {
					str1 = readLongCommand(localBufferedReader);
				}
				
				localTokenScanner.setInput(str1);
				String str2 = localTokenScanner.nextToken();
				final JBECommand localJBECommand = (JBECommand) this.cmdTable.get(str2);
				if (localJBECommand == null) {
					System.err.println("Unexpected error: unknown command \"" + str2 + "\"");
				} else {
					// execute JBE command; some commands must be run on Swing event thread
					if (localJBECommand.shouldRunOnSwingEventThread()) {
						Runnable run = new Runnable() {
							public void run() {
								localJBECommand.execute(localTokenScanner, JavaBackEnd.this);
							}
						};
						if (localJBECommand.shouldInvokeAndWait()) {
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
						localJBECommand.execute(localTokenScanner, this);
					}
				}
			}
		} catch (Exception localException) {
			System.err.println("Unexpected error: " + localException.getMessage());
			localException.printStackTrace(System.err);
			if (DEBUG) {
				printLog("Unexpected error: " + localException.getMessage());
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

	private void processArguments(String[] paramArrayOfString) {
		this.appName = DEFAULT_APP_NAME;
		this.exec = null;
		for (int i = 0; i < paramArrayOfString.length; i++) {
			String str = paramArrayOfString[i];
			if (str.startsWith("-")) {
				if (str.equals("-exec")) {
					this.exec = paramArrayOfString[(++i)];
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

	protected Image getImage(String paramString) {
		Image localImage = (Image) this.imageTable.get(paramString);
		if (localImage != null) {
			return localImage;
		}
		Toolkit localToolkit = Toolkit.getDefaultToolkit();
		if (paramString.startsWith("http:")) {
			try {
				localImage = localToolkit.getImage(new URL(paramString));
			} catch (MalformedURLException localMalformedURLException) {
				throw new ErrorException("loadImage: Malformed URL");
			}
		} else {
			File localObject = new File(paramString);
			if (!localObject.exists() && !paramString.startsWith("/")
					&& !paramString.startsWith(".")) {
				localObject = new File("images/" + paramString);
			}
			if (!((File) localObject).exists()) {
				throw new ErrorException("loadImage: File not found");
			}
			localImage = localToolkit.getImage(((File) localObject).getAbsolutePath());
		}
		if (localImage == null) {
			throw new ErrorException("loadImage: File not found");
		}
		Object localObject = new MediaTracker(this.empty);
		((MediaTracker) localObject).addImage(localImage, 0);
		try {
			((MediaTracker) localObject).waitForID(0);
		} catch (InterruptedException localInterruptedException) {
			throw new ErrorException("loadImage: Loading interrupted");
		}
		this.imageTable.put(paramString, localImage);
		return localImage;
	}

	protected Clip getClip(String paramString) {
		Clip localClip = (Clip) this.clipTable.get(paramString);
		if (localClip != null) {
			return localClip;
		}
		try {
			localClip = AudioSystem.getClip();
			File localFile = new File(paramString);
			if ((!localFile.exists()) && (!paramString.startsWith("/"))
					&& (!paramString.startsWith("."))) {
				localFile = new File("sounds/" + paramString);
			}
			if (!localFile.exists()) {
				throw new ErrorException("createClip: File not found");
			}
			InputStream localFileInputStream = new BufferedInputStream(new FileInputStream(localFile));
			AudioInputStream localAudioInputStream = AudioSystem
					.getAudioInputStream(localFileInputStream);
			localClip.open(localAudioInputStream);
		} catch (IOException localIOException) {
			throw new ErrorException("getClip: I/O error (" + new File(paramString).getAbsolutePath() + ")");
		} catch (Exception localException) {
			throw new ErrorException("getClip: " + localException);
		}
		this.clipTable.put(paramString, localClip);
		return localClip;
	}
	
	public KeyListener getConsoleKeyListener() {
		ConsoleModel consoleModel = this.console.getConsoleModel();
		if (consoleModel != null) {
			return (KeyListener) consoleModel;   // StandardConsoleModel implements KeyListener
		}
		return null;
	}
}

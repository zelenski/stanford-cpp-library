package stanford.spl;

import acm.graphics.GObject;
import acm.io.*;
import acm.util.ErrorException;
import acm.util.JTFTools;
import acm.util.Platform;
import acm.util.TokenScanner;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;

import javax.sound.sampled.*;
import javax.swing.*;
import javax.swing.event.*;

public class JavaBackEnd implements WindowListener, MouseListener, MouseMotionListener,
		KeyListener, ActionListener, ComponentListener, ChangeListener, Observer {
	public static final int DEFAULT_CONSOLE_X = 10;
	public static final int DEFAULT_CONSOLE_Y = 40;
	public static final int DEFAULT_CONSOLE_WIDTH = 500;
	public static final int DEFAULT_CONSOLE_HEIGHT = 250;
	public static final int DEFAULT_GRAPHICS_X = 10;
	public static final int DEFAULT_GRAPHICS_Y = 10;
	public static final int EVENT_SUBTYPE_MASK = 15;
	public static final int ACTION_EVENT = 16;
	public static final int KEY_EVENT = 32;
	public static final int TIMER_EVENT = 64;
	public static final int WINDOW_EVENT = 128;
	public static final int MOUSE_EVENT = 256;
	public static final int CLICK_EVENT = 512;
	public static final int ANY_EVENT = 1008;
	public static final int WINDOW_CLOSED = 129;
	public static final int WINDOW_RESIZED = 130;
	public static final int ACTION_PERFORMED = 17;
	public static final int MOUSE_CLICKED = 257;
	public static final int MOUSE_PRESSED = 258;
	public static final int MOUSE_RELEASED = 259;
	public static final int MOUSE_MOVED = 260;
	public static final int MOUSE_DRAGGED = 261;
	public static final int KEY_PRESSED = 33;
	public static final int KEY_RELEASED = 34;
	public static final int KEY_TYPED = 35;
	public static final int TIMER_TICKED = 65;
	public static final int SHIFT_DOWN = 1;
	public static final int CTRL_DOWN = 2;
	public static final int META_DOWN = 4;
	public static final int ALT_DOWN = 8;
	public static final int ALT_GRAPH_DOWN = 16;
	public static final int BUTTON1_DOWN = 32;
	public static final int BUTTON2_DOWN = 64;
	public static final int BUTTON3_DOWN = 128;
	private static final String DEBUG_PROPERTY = "stanfordspl.debug";
	private static final String DEBUG_LOG_FILE = "stanfordspldebug.txt";
	private static boolean DEBUG = false;
	private String appName;
	private String exec;
	private String cppVersion = "(unknown)";
	private String consoleWindowTitle = "Console";
	private JBEMenuBar menuBar;
	private JBEConsole console;
	private JFrame consoleFrame;
	private int consoleCloseOperation = JFrame.HIDE_ON_CLOSE;
	private static final Color ERROR_COLOR = new Color(192, 0, 0);   // slightly dark red
	private static final int ERROR_STYLE = Font.BOLD;

	public static void main(String[] paramArrayOfString) {
		try {
			String prop = System.getProperty(DEBUG_PROPERTY);
			DEBUG = prop != null && (prop.startsWith("t") || prop.startsWith("1"));
		} catch (Exception e) {}
		
		new JavaBackEnd().run(paramArrayOfString);
	}
	
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
		this.cppVersion = version;
	}
	
	public String getCppVersion() {
		return this.cppVersion;
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
		this.windowTable = new HashMap<String, JBEWindow>();
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
		
		AutograderInput autograderInput = AutograderInput.getInstance(this);
		autograderInput.addObserver(this);
		
		this.menuBar = new JBEMenuBar(this, this.console);
		this.console.setMenuBar(this.menuBar);
		if (this.exec != null) {
			try {
				Process localProcess = Runtime.getRuntime().exec(this.exec);
				System.setIn(localProcess.getInputStream());
				System.setOut(new PrintStream(localProcess.getOutputStream(), true));
			} catch (IOException localIOException) {
				System.err.println("Can't exec process");
			}
		}
		commandLoop();
	}

	protected void createWindow(String paramString, int paramInt1, int paramInt2,
			TopCompound paramTopCompound) {
		createWindow(paramString, paramInt1, paramInt2, paramTopCompound, true);
	}
	
	protected void createWindow(String paramString, int paramInt1, int paramInt2,
			TopCompound paramTopCompound, boolean visible) {
		JBEWindow localJBEWindow = new JBEWindow(this, paramString, this.appName, paramInt1,
				paramInt2);
		this.windowTable.put(paramString, localJBEWindow);
		
		// commented out by Marty 2014/03/05;
		// This code used to set console's size to the size of the last created window
		// for some reason.  Why??  No.  Bad.  Turning this off.
		// this.consoleWidth = paramInt1;
		// this.consoleY = (50 + paramInt2);
		localJBEWindow.pack();
		localJBEWindow.setLocation(10, 10);
		localJBEWindow.getCanvas().initOffscreenImage();
		localJBEWindow.getCanvas().setTopCompound(paramTopCompound);
		this.activeWindowCount += 1;
		localJBEWindow.setResizable(false);
		localJBEWindow.setVisible(true);
		waitForWindowActive(localJBEWindow);
		if (!visible) {
			localJBEWindow.setVisible(false);
		}
	}

	public void deleteWindow(String paramString) {
		this.windowTable.remove(paramString);
	}

	protected void defineGObject(String paramString, GObject paramGObject) {
		this.gobjTable.put(paramString, paramGObject);
	}

	protected void defineSource(JComponent paramJComponent, String paramString) {
		this.sourceTable.put(paramJComponent, paramString);
	}

	protected void deleteGObject(String paramString) {
		this.gobjTable.remove(paramString);
	}

	protected String getSourceId(JComponent paramJComponent) {
		return (String) this.sourceTable.get(paramJComponent);
	}

	protected GObject getGObject(String paramString) {
		return (GObject) this.gobjTable.get(paramString);
	}

	protected JComponent getInteractor(GObject paramGObject) {
		if ((paramGObject instanceof GInteractor)) {
			return ((GInteractor) paramGObject).getInteractor();
		}
		if ((paramGObject instanceof JBELabel)) {
			return ((JBELabel) paramGObject).getInteractor();
		}
		return null;
	}

	protected JBEWindow getWindow(String paramString) {
		return (JBEWindow) this.windowTable.get(paramString);
	}

	protected void clearConsole() {
		this.console.clear();
	}
	
	protected void setConsoleCloseOperation(int op) {
		this.consoleCloseOperation = op;
	}

	protected void setConsoleFont(String paramString) {
		this.console.setFont(JTFTools.decodeFont(paramString));
	}

	protected void consoleMinimize() {
		if (this.consoleFrame != null) {
			this.consoleFrame.setState(JFrame.ICONIFIED);
		}
	}

	protected void consoleToFront() {
		if (this.consoleFrame != null) {
			this.consoleFrame.setState(JFrame.NORMAL);
			this.consoleFrame.toFront();
		}
	}

	protected void consoleSetTitle(String title) {
		this.consoleWindowTitle = title;
		if (this.consoleFrame != null) {
			this.consoleFrame.setTitle(title);
		}
	}

	protected void setConsoleSize(int paramInt1, int paramInt2) {
		this.consoleWidth = paramInt1;
		this.consoleHeight = paramInt2;
//		if (this.console != null) {
//			this.console.setPreferredSize(this.consoleWidth, this.consoleHeight);
//			if (this.consoleFrame != null) {
//				this.consoleFrame.pack();
//			}
//		}
	}

	protected void setConsoleLocation(int x, int y) {
		this.consoleX = x;
		this.consoleY = y;
		if (this.consoleFrame != null) {
			this.consoleFrame.setLocation(this.consoleX, this.consoleY);
		}
	}

	protected String getConsole() {
		if (this.consoleFrame == null) {
			showConsole();
		}
		return this.console.readLine();
	}

	protected void putConsole(String paramString) {
		putConsole(paramString, false);
	}
	
	protected void putConsole(String paramString, boolean isStderr) {
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

	protected double getEventTime() {
		return new Date().getTime();
	}

	protected void println(String paramString) {
		synchronized (this.eventLock) {
			System.out.println(paramString);
			System.out.flush();
		}
	}

	protected void acknowledgeEvent(String eventText, Object... args) {
		acknowledgeEvent(String.format(Locale.US, eventText, args));
	}
	
	protected void acknowledgeEvent(String eventText) {
		synchronized (this.eventLock) {
			System.out.println(eventText);
			System.out.flush();
			if (!this.eventAcknowledged) {
				System.out.println("result:___jbe___ack___ " + eventText);
				System.out.flush();
				this.eventAcknowledged = true;
				this.eventPending = false;
			} else {
				this.eventPending = true;
			}
		}
	}
	
	protected void acknowledgeEvent() {
		synchronized (this.eventLock) {
			if (!this.eventAcknowledged) {
				System.out.println("result:___jbe___ack___");
				System.out.flush();
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

	private void showConsole() {
		this.console.setPreferredSize(this.consoleWidth, this.consoleHeight);
		this.consoleFrame = new JFrame(this.consoleWindowTitle);
		this.consoleFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
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

	private int convertModifiers(int paramInt) {
		int i = 0;
		if ((paramInt & 0x40) != 0) {
			i |= 0x1;
		}
		if ((paramInt & 0x80) != 0) {
			i |= 0x2;
		}
		if ((paramInt & 0x100) != 0) {
			i |= 0x4;
		}
		if ((paramInt & 0x200) != 0) {
			i |= 0x8;
		}
		if ((paramInt & 0x2000) != 0) {
			i |= 0x10;
		}
		if ((paramInt & 0x400) != 0) {
			i |= 0x20;
		}
		if ((paramInt & 0x800) != 0) {
			i |= 0x40;
		}
		if ((paramInt & 0x1000) != 0) {
			i |= 0x80;
		}
		return i;
	}

	protected String openFileDialog(String title, String mode, String path) {
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
			if ((this.eventMask & 0x40) != 0) {
				localObject2 = (GTimer) paramActionEvent.getSource();
				acknowledgeEvent("event:timerTicked(\"%s\", %d)",
						((GTimer) localObject2).getId(), (long) getEventTime());
			}
		} else if ((this.eventMask & 0x10) != 0) {
			localObject2 = getSourceId((JComponent) localObject1);
			GInteractor localGInteractor = (GInteractor) getGObject((String) localObject2);
			String str = localGInteractor.getActionCommand();
			if (!str.isEmpty()) {
				acknowledgeEvent("event:actionPerformed(\"%s\", \"%s\", %d)",
						(String) localObject2, str,
						(long) getEventTime());
			}
		}
	}

	public void keyPressed(KeyEvent paramKeyEvent) {
		if ((this.eventMask & 0x20) != 0) {
			printEvent("keyPressed", paramKeyEvent);
		}
	}

	public void keyReleased(KeyEvent paramKeyEvent) {
		if ((this.eventMask & 0x20) != 0) {
			printEvent("keyReleased", paramKeyEvent);
		}
	}

	public void keyTyped(KeyEvent paramKeyEvent) {
		if ((this.eventMask & 0x20) != 0) {
			printEvent("keyTyped", paramKeyEvent);
		}
	}

	private void printEvent(String type, KeyEvent paramKeyEvent) {
		JBECanvas localJBECanvas = (JBECanvas) paramKeyEvent.getSource();
		acknowledgeEvent("event:%s(\"%s\", %d, %d, %s, %d)",
				type, localJBECanvas.getWindowId(), (long) getEventTime(),
				convertModifiers(paramKeyEvent.getModifiersEx()),
				paramKeyEvent.getKeyChar(), paramKeyEvent.getKeyCode());
	}

	public void mouseClicked(MouseEvent paramMouseEvent) {
		((Component) paramMouseEvent.getSource()).requestFocus();
		if ((this.eventMask & 0x300) != 0) {
			printEvent("mouseClicked", paramMouseEvent);
		}
	}

	public void mouseEntered(MouseEvent paramMouseEvent) {
	}

	public void mouseExited(MouseEvent paramMouseEvent) {
	}

	public void mousePressed(MouseEvent paramMouseEvent) {
		if ((this.eventMask & 0x100) != 0) {
			printEvent("mousePressed", paramMouseEvent);
		}
	}

	public void mouseReleased(MouseEvent paramMouseEvent) {
		if ((this.eventMask & 0x100) != 0) {
			printEvent("mouseReleased", paramMouseEvent);
		}
	}

	public void mouseMoved(MouseEvent paramMouseEvent) {
		if ((this.eventMask & 0x100) != 0) {
			printEvent("mouseMoved", paramMouseEvent);
		}
	}

	public void mouseDragged(MouseEvent paramMouseEvent) {
		if ((this.eventMask & 0x100) != 0) {
			printEvent("mouseDragged", paramMouseEvent);
		}
	}

	private void printEvent(String type, MouseEvent paramMouseEvent) {
		JBECanvas localJBECanvas = (JBECanvas) paramMouseEvent.getSource();
		acknowledgeEvent("event:%s(\"%s\", %d, %d, %d, %d)",
				type, localJBECanvas.getWindowId(), (long) getEventTime(),
				convertModifiers(paramMouseEvent.getModifiersEx()),
				paramMouseEvent.getX(), paramMouseEvent.getY());
	}

	public void windowActivated(WindowEvent paramWindowEvent) {
	}

	public void windowClosed(WindowEvent paramWindowEvent) {
		windowClosing(paramWindowEvent);
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
			}
			acknowledgeEvent("event:lastWindowGWindow_closed()");
			acknowledgeEvent("event:consoleWindowClosed()");
		}
	}
	
	public void windowClosing(WindowEvent paramWindowEvent) {
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
		}
		this.activeWindowCount -= 1;
		if (this.activeWindowCount == 0) {
			acknowledgeEvent("event:lastWindowGWindow_closed()");
			System.exit(0);
		}
	}

	public void windowDeactivated(WindowEvent paramWindowEvent) {
	}

	public void windowDeiconified(WindowEvent paramWindowEvent) {
	}

	public void windowIconified(WindowEvent paramWindowEvent) {
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

	public void stateChanged(ChangeEvent paramChangeEvent) {
		Object localObject1 = paramChangeEvent.getSource();
		if ((this.eventMask & 0x10) != 0) {
			String str1 = getSourceId((JComponent) localObject1);
			GInteractor localGInteractor = (GInteractor) getGObject(str1);
			String str2 = localGInteractor.getActionCommand();
			if (!str2.isEmpty()) {
				acknowledgeEvent("event:actionPerformed(\"%s\", \"%s\", %d)", str1, str2, (long) getEventTime());
			}
		}
	}

	public void componentHidden(ComponentEvent paramComponentEvent) {
	}

	public void componentMoved(ComponentEvent paramComponentEvent) {
	}

	public void componentResized(ComponentEvent paramComponentEvent) {
		if ((this.eventMask & 0x80) != 0) {
			JBECanvas localJBECanvas = (JBECanvas) paramComponentEvent.getSource();
			acknowledgeEvent("event:windowResized(\"%s\", %d)", localJBECanvas.getWindowId(), (long) getEventTime());
		}
	}

	public void componentShown(ComponentEvent paramComponentEvent) {
	}
	
	private void printLog(String s) {
		try {
			PrintStream out = new PrintStream(new FileOutputStream(DEBUG_LOG_FILE, /* append */ true));
			out.println(s);
			out.flush();
			out.close();
		} catch (IOException ioe) {
			
		}
	}

	private void commandLoop() {
		BufferedReader localBufferedReader = new BufferedReader(new InputStreamReader(System.in));
		TokenScanner localTokenScanner = new TokenScanner();
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
				JBECommand localJBECommand = (JBECommand) this.cmdTable.get(str2);
				if (localJBECommand == null) {
					System.err.println("Unexpected error: unknown command \"" + str2 + "\"");
				} else {
					localJBECommand.execute(localTokenScanner, this);
				}
			}
		} catch (Exception localException) {
			localException.printStackTrace(System.err);
			System.err.println("Unexpected error: " + localException.getMessage());
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
		StringBuilder sb = new StringBuilder(65536);
		while (true) {
			String line = systemInReader.readLine();
			if (line == null || line.equals("LongCommand.end()")) {
				break;
			}
			sb.append(line);
		}
		
		String cmd = sb.toString();
		return cmd;
	}

	private void processArguments(String[] paramArrayOfString) {
		this.appName = "JBE";
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
		System.setProperty("com.apple.mrj.application.apple.menu.about.name", this.appName);

		System.setProperty("apple.laf.useScreenMenuBar", "true");
		try {
			UIManager.put("Slider.paintValue", false);
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
			UIManager.put("Slider.paintValue", false);
		} catch (Exception localException) {
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

	private int consoleX = 10;
	private int consoleY = 40;
	private int consoleWidth = 500;
	private int consoleHeight = 250;
	private HashMap<String, JBECommand> cmdTable;
	private HashMap<String, JBEWindow> windowTable;
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
}

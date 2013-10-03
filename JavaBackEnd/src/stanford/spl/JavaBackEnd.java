/*
 * File: JavaBackEnd.java
 * ----------------------
 * This program implements a Java-based graphics back end for the
 * StanfordCPPLib package.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (C) 2013 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

package stanford.spl;

import acm.graphics.GObject;
import acm.util.ErrorException;
import acm.util.JTFTools;
import acm.util.TokenScanner;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Image;
import java.awt.MediaTracker;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Date;
import java.util.HashMap;

import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.UIManager;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class JavaBackEnd implements
              WindowListener, MouseListener, MouseMotionListener,
              KeyListener, ActionListener, ComponentListener, ChangeListener {

   public static final int DEFAULT_CONSOLE_X = 10;
   public static final int DEFAULT_CONSOLE_Y = 40;
   public static final int DEFAULT_CONSOLE_WIDTH = 500;
   public static final int DEFAULT_CONSOLE_HEIGHT = 250;
   public static final int DEFAULT_GRAPHICS_X = 10;
   public static final int DEFAULT_GRAPHICS_Y = 10;

/*
 * Constants: Event types
 * ----------------------
 * These constants are shared with clients.  Any changes to these values
 * must be reflected in the various front-end implementations.
 */

   public static final int EVENT_SUBTYPE_MASK = 0xF;

   public static final int ACTION_EVENT = 0x010;
   public static final int KEY_EVENT = 0x020;
   public static final int TIMER_EVENT = 0x040;
   public static final int WINDOW_EVENT = 0x080;
   public static final int MOUSE_EVENT = 0x100;
   public static final int CLICK_EVENT = 0x200;
   public static final int ANY_EVENT = 0x3F0;

   public static final int WINDOW_CLOSED = WINDOW_EVENT + 1;
   public static final int WINDOW_RESIZED = WINDOW_EVENT + 2;

   public static final int ACTION_PERFORMED = ACTION_EVENT + 1;

   public static final int MOUSE_CLICKED = MOUSE_EVENT + 1;
   public static final int MOUSE_PRESSED = MOUSE_EVENT + 2;
   public static final int MOUSE_RELEASED = MOUSE_EVENT + 3;
   public static final int MOUSE_MOVED = MOUSE_EVENT + 4;
   public static final int MOUSE_DRAGGED = MOUSE_EVENT + 5;

   public static final int KEY_PRESSED = KEY_EVENT + 1;
   public static final int KEY_RELEASED = KEY_EVENT + 2;
   public static final int KEY_TYPED = KEY_EVENT + 3;

   public static final int TIMER_TICKED = TIMER_EVENT + 1;

/*
 * Constants: Modifiers
 * --------------------
 * These modifiers are shared with the clients.  Any changes to these
 * values must be reflected in the various front-end implementations.
 */

   public static final int SHIFT_DOWN = 1 << 0;
   public static final int CTRL_DOWN = 1 << 1;
   public static final int META_DOWN = 1 << 2;
   public static final int ALT_DOWN = 1 << 3;
   public static final int ALT_GRAPH_DOWN = 1 << 4;
   public static final int BUTTON1_DOWN = 1 << 5;
   public static final int BUTTON2_DOWN = 1 << 6;
   public static final int BUTTON3_DOWN = 1 << 7;

/* Entry point */

   public static void main(String[] args) {
      new JavaBackEnd().run(args);
   }

   public void run(String[] args) {
      processArguments(args);
      initSystemProperties();
      cmdTable = JBECommand.createCommandTable();
      imageTable = new HashMap<String,Image>();
      windowTable = new HashMap<String,JBEWindow>();
      gobjTable = new HashMap<String,GObject>();
      timerTable = new HashMap<String,GTimer>();
      clipTable = new HashMap<String,Clip>();
      clipIdTable = new HashMap<String,Clip>();
      sourceTable = new HashMap<JComponent,String>();
      eventMask = 0;
      eventAcknowledged = false;
      eventPending = false;
      activeWindowCount = 0;
      console = new JBEConsole();
      if (exec != null) {
         try {
            Process proc = Runtime.getRuntime().exec(exec);
            System.setIn(proc.getInputStream());
            System.setOut(new PrintStream(proc.getOutputStream(), true));
         } catch (IOException ex) {
            System.err.println("Can't exec process");
         }
      }
      commandLoop();
   }

   protected void createWindow(String id, int width, int height,
                               TopCompound top) {
      JBEWindow window = new JBEWindow(this, id, appName, width, height);
      windowTable.put(id, window);
      consoleWidth = width;
      consoleY = 50 + height;
      window.pack();
      window.setLocation(DEFAULT_GRAPHICS_X, DEFAULT_GRAPHICS_Y);
      window.getCanvas().initOffscreenImage();
      window.getCanvas().setTopCompound(top);
      activeWindowCount++;
      window.setResizable(false);
      window.setVisible(true);
      waitForWindowActive(window);
   }

   protected void deleteWindow(String id) {
      windowTable.remove(id);
   }

   protected void defineGObject(String id, GObject gobj) {
      gobjTable.put(id, gobj);
   }

   protected void defineSource(JComponent comp, String id) {
      sourceTable.put(comp, id);
   }

   protected void deleteGObject(String id) {
      gobjTable.remove(id);
   }

   protected String getSourceId(JComponent comp) {
      return sourceTable.get(comp);
   }

   protected GObject getGObject(String id) {
      return gobjTable.get(id);
   }

   protected JComponent getInteractor(GObject gobj) {
      if (gobj instanceof GInteractor) {
         return ((GInteractor) gobj).getInteractor();
      } else if (gobj instanceof JBELabel) {
         return ((JBELabel) gobj).getInteractor();
      }
      return null;
   }

   protected JBEWindow getWindow(String id) {
      return windowTable.get(id);
   }

   protected void clearConsole() {
      console.clear();
   }

   protected void setConsoleFont(String font) {
      console.setFont(JTFTools.decodeFont(font));
   }

   protected void setConsoleSize(int width, int height) {
      consoleWidth = width;
      consoleHeight = height;
      // Make this work after creation as well
   }

   protected String getConsole() {
      if (consoleFrame == null) showConsole();
      return console.readLine();
   }

   protected void putConsole(String str) {
      if (consoleFrame == null) showConsole();
      console.print(str);
   }

   protected void endLineConsole() {
      if (consoleFrame == null) showConsole();
      console.println();
   }

   protected double getEventTime() {
      return new Date().getTime();
   }

   protected void println(String line) {
      synchronized (eventLock) {
         System.out.println(line);
         System.out.flush();
      }
   }

   protected void acknowledgeEvent() {
      synchronized (eventLock) {
         if (!eventAcknowledged) {
            System.out.println("result:ack");
            System.out.flush();
            eventAcknowledged = true;
            eventPending = false;
         } else {
            eventPending = true;
         }
      }
   }

   protected void getNextEvent(int mask) {
      synchronized (eventLock) {
         eventAcknowledged = false;
         eventPending = false;
         acknowledgeEvent();
         eventMask = mask;
      }
   }

   protected void waitForEvent(int mask) {
      synchronized (eventLock) {
         eventAcknowledged = false;
         if (eventPending) acknowledgeEvent();
         eventMask = mask;
      }
   }

   private void showConsole() {
      console.setPreferredSize(consoleWidth, consoleHeight);
      consoleFrame = new JFrame("Console");
      consoleFrame.setLayout(new BorderLayout());
      consoleFrame.add(console);
      consoleFrame.pack();
      consoleFrame.setLocation(consoleX, consoleY);
      consoleFrame.addWindowListener(this);
      consoleFrame.setVisible(true);
      waitForWindowActive(consoleFrame);
      activeWindowCount++;
   }

   protected void createSound(String id, String filename) {
      Clip clip = getClip(filename);
      clipIdTable.put(id, clip);
   }

   protected void deleteSound(String id) {
      clipIdTable.remove(id);
   }

   protected void playSound(String id) {
      Clip clip = clipIdTable.get(id);
      if (clip != null) {
         clip.stop();
         clip.setFramePosition(0);
         clip.start();
      }
   }

   protected void createTimer(String id, double msec) {
      GTimer timer = new GTimer(id, msec);
      timer.addActionListener(this);
      timerTable.put(id, timer);
   }

   protected void deleteTimer(String id) {
      GTimer timer = timerTable.get(id);
      if (timer != null) timer.stop();
      timerTable.remove(id);
   }

   protected void startTimer(String id) {
      GTimer timer = timerTable.get(id);
      if (timer != null) timer.start();
   }

   protected void stopTimer(String id) {
      GTimer timer = timerTable.get(id);
      if (timer != null) timer.stop();
   }

   private int convertModifiers(int javaModifiers) {
      int cppModifiers = 0;
      if ((javaModifiers & InputEvent.SHIFT_DOWN_MASK) != 0) {
         cppModifiers |= SHIFT_DOWN;
      }
      if ((javaModifiers & InputEvent.CTRL_DOWN_MASK) != 0) {
         cppModifiers |= CTRL_DOWN;
      }
      if ((javaModifiers & InputEvent.META_DOWN_MASK) != 0) {
         cppModifiers |= META_DOWN;
      }
      if ((javaModifiers & InputEvent.ALT_DOWN_MASK) != 0) {
         cppModifiers |= ALT_DOWN;
      }
      if ((javaModifiers & InputEvent.ALT_GRAPH_DOWN_MASK) != 0) {
         cppModifiers |= ALT_GRAPH_DOWN;
      }
      if ((javaModifiers & InputEvent.BUTTON1_DOWN_MASK) != 0) {
         cppModifiers |= BUTTON1_DOWN;
      }
      if ((javaModifiers & InputEvent.BUTTON2_DOWN_MASK) != 0) {
         cppModifiers |= BUTTON2_DOWN;
      }
      if ((javaModifiers & InputEvent.BUTTON3_DOWN_MASK) != 0) {
         cppModifiers |= BUTTON3_DOWN;
      }
      return cppModifiers;
   }

/* File dialogs */

   protected String openFileDialog(String title, String mode, String path) {
      JBEFileFilter filter = new JBEFileFilter(path);
      JFileChooser chooser = new JFileChooser(filter.getDirectory());
      chooser.setFileFilter(filter);
      chooser.setDialogTitle(title);
      int response = 0;
      if (mode.equalsIgnoreCase("load")) {
         response = chooser.showOpenDialog(null);
      } else if (mode.equalsIgnoreCase("save")) {
         response = chooser.showSaveDialog(null);
      } else {
         return "";
      }
      if (response == JFileChooser.APPROVE_OPTION) {
         return chooser.getSelectedFile().getAbsolutePath();
      } else {
         return "";
      }
   }

/* ActionListener */

   public void actionPerformed(ActionEvent e) {
      Object source = e.getSource();
      synchronized (eventLock) {
         if (source instanceof GTimer) {
            if ((eventMask & TIMER_EVENT) != 0) {
               GTimer timer = (GTimer) e.getSource();
               System.out.print("event:timerTicked");
               System.out.print("(\"" + timer.getId() + "\", "
                                      + getEventTime());
               System.out.println(")");
               System.out.flush();
               acknowledgeEvent();
            }
         } else {
            if ((eventMask & ACTION_EVENT) != 0) {
               String id = getSourceId((JComponent) source);
               GInteractor interactor = (GInteractor) getGObject(id);
               String cmd = interactor.getActionCommand();
               if (!cmd.isEmpty()) {
                  System.out.print("event:actionPerformed");
                  System.out.print("(\"" + id + "\", \"" + cmd + "\", "
                                         + getEventTime());
                  System.out.println(")");
                  System.out.flush();
                  acknowledgeEvent();
               }
            }
         }
      }
   }

/* KeyListener */

   public void keyPressed(KeyEvent e) {
      if ((eventMask & KEY_EVENT) != 0) {
         printEvent("keyPressed", e);
         acknowledgeEvent();
      }
   }

   public void keyReleased(KeyEvent e) {
      if ((eventMask & KEY_EVENT) != 0) {
         printEvent("keyReleased", e);
         acknowledgeEvent();
      }
   }

   public void keyTyped(KeyEvent e) {
      if ((eventMask & KEY_EVENT) != 0) {
         printEvent("keyTyped", e);
         acknowledgeEvent();
      }
   }

   private void printEvent(String type, KeyEvent e) {
      JBECanvas jc = (JBECanvas) e.getSource();
      synchronized (eventLock) {
         System.out.print("event:" + type);
         System.out.print("(\"" + jc.getWindowId() + "\", " + getEventTime());
         System.out.print(", " + convertModifiers(e.getModifiersEx()));
         System.out.print(", " + (int) e.getKeyChar());
         System.out.println(", " + e.getKeyCode() + ")");
         System.out.flush();
      }
   }

/* MouseListener */

   public void mouseClicked(MouseEvent e) {
      ((Component) e.getSource()).requestFocus();
      if ((eventMask & (MOUSE_EVENT | CLICK_EVENT)) != 0) {
         printEvent("mouseClicked", e);
         acknowledgeEvent();
      }
   }

   public void mouseEntered(MouseEvent e) {
      /* Empty */
   }

   public void mouseExited(MouseEvent e) {
      /* Empty */
   }

   public void mousePressed(MouseEvent e) {
      if ((eventMask & MOUSE_EVENT) != 0) {
         printEvent("mousePressed", e);
         acknowledgeEvent();
      }
   }

   public void mouseReleased(MouseEvent e) {
      if ((eventMask & MOUSE_EVENT) != 0) {
         printEvent("mouseReleased", e);
         acknowledgeEvent();
      }
   }

/* MouseMotionListener */

   public void mouseMoved(MouseEvent e) {
      if ((eventMask & MOUSE_EVENT) != 0) {
         printEvent("mouseMoved", e);
         acknowledgeEvent();
      }
   }

   public void mouseDragged(MouseEvent e) {
      if ((eventMask & MOUSE_EVENT) != 0) {
         printEvent("mouseDragged", e);
         acknowledgeEvent();
      }
   }

   private void printEvent(String type, MouseEvent e) {
      JBECanvas jc = (JBECanvas) e.getSource();
      synchronized (eventLock) {
         System.out.print("event:" + type);
         System.out.print("(\"" + jc.getWindowId() + "\", " + getEventTime());
         System.out.print(", " + convertModifiers(e.getModifiersEx()));
         System.out.println(", " + e.getX() + ", " + e.getY() + ")");
         System.out.flush();
      }
   }

/* WindowListener */

   public void windowActivated(WindowEvent e) { }
   public void windowClosed(WindowEvent e) { windowClosing(e); }
   public void windowClosing(WindowEvent e) {
      if (e.getSource() != consoleFrame) {
         JBEWindow jw = (JBEWindow) e.getSource();
         synchronized (eventLock) {
            System.out.print("event:windowClosed");
            System.out.print("(\"" + jw.getWindowId());
            System.out.print("\", " + getEventTime());
            System.out.println(")");
            System.out.flush();
            acknowledgeEvent();
         }
         windowTable.remove(jw.getWindowId());
      }
      activeWindowCount--;
      if (activeWindowCount == 0) {
         synchronized (eventLock) {
            System.out.println("event:lastWindowGWindow_closed()");
            System.out.flush();
            acknowledgeEvent();
         }
         System.exit(0);
      }
   }
   public void windowDeactivated(WindowEvent e) { }
   public void windowDeiconified(WindowEvent e) { }
   public void windowIconified(WindowEvent e) { }
   public void windowOpened(WindowEvent e) {
      JFrame window = (JFrame) e.getSource();
      synchronized (window) {
         window.notifyAll();
      }
   }

   private void waitForWindowActive(JFrame window) {
      synchronized (window) {
         while (!window.isShowing()) {
            try {
               window.wait();
               Thread.sleep(100);
            } catch (InterruptedException ex) {
               /* Empty */
            }
         }
      }
   }

/* ChangeListener */

   public void stateChanged(ChangeEvent e) {
      Object source = e.getSource();
      synchronized (eventLock) {
         if ((eventMask & ACTION_EVENT) != 0) {
            String id = getSourceId((JComponent) source);
            GInteractor interactor = (GInteractor) getGObject(id);
            String cmd = interactor.getActionCommand();
            if (!cmd.isEmpty()) {
               System.out.print("event:actionPerformed");
               System.out.print("(\"" + id + "\", \"" + cmd + "\", "
                                      + getEventTime());
               System.out.println(")");
               System.out.flush();
               acknowledgeEvent();
            }
         }
      }
   }

/* ComponentListener */

   public void componentHidden(ComponentEvent e) { }
   public void componentMoved(ComponentEvent e) { }
   public void componentResized(ComponentEvent e) {
      if ((eventMask & WINDOW_EVENT) != 0) {
         JBECanvas jc = (JBECanvas) e.getSource();
         synchronized (eventLock) {
            System.out.print("event:windowResized");
            System.out.print("(\"" + jc.getWindowId());
            System.out.print("\", " + getEventTime());
            System.out.println(")");
            System.out.flush();
            acknowledgeEvent();
         }
      }
   }
   public void componentShown(ComponentEvent e) { }

   private void commandLoop() {
      BufferedReader rd = new BufferedReader(new InputStreamReader(System.in));
      TokenScanner scanner = new TokenScanner();
      scanner.ignoreWhitespace();
      scanner.scanNumbers();
      scanner.scanStrings();
      scanner.addWordCharacters(".");
      while (true) {
         try {
            String line = rd.readLine();
            if (line == null) break;
            scanner.setInput(line);
            String fn = scanner.nextToken();
            JBECommand cmd = cmdTable.get(fn);
            if (cmd != null) cmd.execute(scanner, this);
         } catch (Exception ex) {
            ex.printStackTrace(System.err);
            System.err.println("Unexpected error: " + ex.getMessage());
         }
      }
   }

   private void processArguments(String[] args) {
      appName = "JBE";
      exec = null;
      for (int i = 0; i < args.length; i++) {
         String arg = args[i];
         if (arg.startsWith("-")) {
            if (arg.equals("-exec")) {
               exec = args[++i];
            } else {
               System.err.println("Error: Unrecognized option " + arg);
            }
         } else {
            appName = arg;
         }
      }
   }

   private void initSystemProperties() {
      System.setProperty("com.apple.mrj.application.apple.menu.about.name",
                         appName);
      System.setProperty("apple.laf.useScreenMenuBar", "true");
      try {
         UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
      } catch (Exception ex) {
         /* Empty */
      }
   }

   protected Image getImage(String name) {
      Image image = imageTable.get(name);
      if (image != null) return image;
      Toolkit toolkit = Toolkit.getDefaultToolkit();
      if (name.startsWith("http:")) {
         try {
            image = toolkit.getImage(new URL(name));
         } catch (MalformedURLException ex) {
            throw new ErrorException("loadImage: Malformed URL");
         }
      } else {
         File file = new File(name);
         if (!file.exists()) {
            if (!name.startsWith("/") && !name.startsWith(".")) {
               file = new File("images/" + name);
            }
         }
         if (!file.exists()) {
            throw new ErrorException("loadImage: File not found");
         }
         image = toolkit.getImage(file.getAbsolutePath());
      }
      if (image == null) {
         throw new ErrorException("loadImage: File not found");
      }
      MediaTracker tracker = new MediaTracker(empty);
      tracker.addImage(image, 0);
      try {
         tracker.waitForID(0);
      } catch (InterruptedException ex) {
         throw new ErrorException("loadImage: Loading interrupted");
      }
      imageTable.put(name, image);
      return image;
   }

   protected Clip getClip(String name) {
      Clip clip = clipTable.get(name);
      if (clip != null) return clip;
      try {
         clip = AudioSystem.getClip();
         File file = new File(name);
         if (!file.exists()) {
            if (!name.startsWith("/") && !name.startsWith(".")) {
               file = new File("sounds/" + name);
            }
         }
         if (!file.exists()) {
            throw new ErrorException("createClip: File not found");
         }
         FileInputStream is = new FileInputStream(file);
         AudioInputStream ais = AudioSystem.getAudioInputStream(is);
         clip.open(ais);
      } catch (IOException ex) {
         throw new ErrorException("getClip: File not found");
      } catch (Exception ex) {
         throw new ErrorException("getClip: " + ex);
      }
      clipTable.put(name, clip);
      return clip;
   }

/* Instance variables */

   private String appName;
   private String exec;
   private JBEConsole console;
   private JFrame consoleFrame;
   private int consoleX = DEFAULT_CONSOLE_X;
   private int consoleY = DEFAULT_CONSOLE_Y;
   private int consoleWidth = DEFAULT_CONSOLE_WIDTH;
   private int consoleHeight = DEFAULT_CONSOLE_HEIGHT;
   private HashMap<String,JBECommand> cmdTable;
   private HashMap<String,JBEWindow> windowTable;
   private HashMap<String,GObject> gobjTable;
   private HashMap<String,GTimer> timerTable;
   private HashMap<String,Image> imageTable;
   private HashMap<String,Clip> clipTable;
   private HashMap<String,Clip> clipIdTable;
   private HashMap<JComponent,String> sourceTable;
   private Container empty = JTFTools.createEmptyContainer();
   private int activeWindowCount;
   private int eventMask;
   private Object eventLock = new Object();
   private boolean eventAcknowledged;
   private boolean eventPending;

}

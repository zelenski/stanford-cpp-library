/*
 * File: JBECommand.java
 * ---------------------
 * This file implements the entire command mechanism for the JBE.
 * Note that this file includes many package classes that are invoked
 * from the command table.
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

import acm.graphics.G3DRect;
import acm.graphics.GArc;
import acm.graphics.GCompound;
import acm.graphics.GDimension;
import acm.graphics.GFillable;
import acm.graphics.GImage;
import acm.graphics.GLabel;
import acm.graphics.GLine;
import acm.graphics.GObject;
import acm.graphics.GOval;
import acm.graphics.GPolygon;
import acm.graphics.GRect;
import acm.graphics.GRectangle;
import acm.graphics.GResizable;
import acm.graphics.GRoundRect;
import acm.util.JTFTools;
import acm.util.TokenScanner;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Toolkit;
import java.lang.reflect.Method;
import java.util.HashMap;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.Timer;

public abstract class JBECommand {
   public abstract void execute(TokenScanner scanner, JavaBackEnd jbe);

   public static HashMap<String,JBECommand> createCommandTable() {
      HashMap<String,JBECommand> cmdTable = new HashMap<String,JBECommand>();
      cmdTable.put("File.openFileDialog", new File_openFileDialog());
      cmdTable.put("G3DRect.create", new G3DRect_create());
      cmdTable.put("G3DRect.setRaised", new G3DRect_setRaised());
      cmdTable.put("GArc.create", new GArc_create());
      cmdTable.put("GArc.setFrameRectangle", new GArc_setFrameRectangle());
      cmdTable.put("GArc.setStartAngle", new GArc_setStartAngle());
      cmdTable.put("GArc.setSweepAngle", new GArc_setSweepAngle());
      cmdTable.put("GButton.create", new GButton_create());
      cmdTable.put("GCheckBox.create", new GCheckBox_create());
      cmdTable.put("GCheckBox.isSelected", new GCheckBox_isSelected());
      cmdTable.put("GCheckBox.setSelected", new GCheckBox_setSelected());
      cmdTable.put("GCompound.add", new GCompound_add());
      cmdTable.put("GCompound.create", new GCompound_create());
      cmdTable.put("GEvent.getNextEvent", new GEvent_getNextEvent());
      cmdTable.put("GEvent.waitForEvent", new GEvent_waitForEvent());
      cmdTable.put("GImage.create", new GImage_create());
      cmdTable.put("GInteractor.setActionCommand",
                   new GInteractor_setActionCommand());
      cmdTable.put("GInteractor.getSize", new GInteractor_getSize());
      cmdTable.put("GLabel.create", new GLabel_create());
      cmdTable.put("GLabel.getFontAscent", new GLabel_getFontAscent());
      cmdTable.put("GLabel.getFontDescent", new GLabel_getFontDescent());
      cmdTable.put("GLabel.getGLabelSize", new GLabel_getGLabelSize());
      cmdTable.put("GLabel.setFont", new GLabel_setFont());
      cmdTable.put("GLabel.setLabel", new GLabel_setLabel());
      cmdTable.put("GLine.create", new GLine_create());
      cmdTable.put("GLine.setEndPoint", new GLine_setEndPoint());
      cmdTable.put("GLine.setStartPoint", new GLine_setStartPoint());
      cmdTable.put("GObject.contains", new GObject_contains());
      cmdTable.put("GObject.delete", new GObject_delete());
      cmdTable.put("GObject.getBounds", new GObject_getBounds());
      cmdTable.put("GObject.remove", new GObject_remove());
      cmdTable.put("GObject.rotate", new GObject_rotate());
      cmdTable.put("GObject.scale", new GObject_scale());
      cmdTable.put("GObject.sendBackward", new GObject_sendBackward());
      cmdTable.put("GObject.sendForward", new GObject_sendForward());
      cmdTable.put("GObject.sendToBack", new GObject_sendToBack());
      cmdTable.put("GObject.sendToFront", new GObject_sendToFront());
      cmdTable.put("GObject.setColor", new GObject_setColor());
      cmdTable.put("GObject.setFillColor", new GObject_setFillColor());
      cmdTable.put("GObject.setFilled", new GObject_setFilled());
      cmdTable.put("GObject.setLineWidth", new GObject_setLineWidth());
      cmdTable.put("GObject.setLocation", new GObject_setLocation());
      cmdTable.put("GObject.setSize", new GObject_setSize());
      cmdTable.put("GObject.setVisible", new GObject_setVisible());
      cmdTable.put("GOval.create", new GOval_create());
      cmdTable.put("GPolygon.addVertex", new GPolygon_addVertex());
      cmdTable.put("GPolygon.create", new GPolygon_create());
      cmdTable.put("GRect.create", new GRect_create());
      cmdTable.put("GRoundRect.create", new GRoundRect_create());
      cmdTable.put("GSlider.create", new GSlider_create());
      cmdTable.put("GSlider.getValue", new GSlider_getValue());
      cmdTable.put("GSlider.setValue", new GSlider_setValue());
      cmdTable.put("GTextField.create", new GTextField_create());
      cmdTable.put("GTextField.getText", new GTextField_getText());
      cmdTable.put("GTextField.setText", new GTextField_setText());
      cmdTable.put("GChooser.create", new GChooser_create());
      cmdTable.put("GChooser.addItem", new GChooser_addItem());
      cmdTable.put("GChooser.getSelectedItem", new GChooser_getSelectedItem());
      cmdTable.put("GChooser.setSelectedItem", new GChooser_setSelectedItem());
      cmdTable.put("GTimer.create", new GTimer_create());
      cmdTable.put("GTimer.deleteTimer", new GTimer_deleteTimer());
      cmdTable.put("GTimer.pause", new GTimer_pause());
      cmdTable.put("GTimer.startTimer", new GTimer_startTimer());
      cmdTable.put("GTimer.stopTimer", new GTimer_stopTimer());
      cmdTable.put("GWindow.addToRegion", new GWindow_addToRegion());
      cmdTable.put("GWindow.setRegionAlignment",
                   new GWindow_setRegionAlignment());
      cmdTable.put("GWindow.clear", new GWindow_clear());
      cmdTable.put("GWindow.close", new GWindow_close());
      cmdTable.put("GWindow.create", new GWindow_create());
      cmdTable.put("GWindow.delete", new GWindow_delete());
      cmdTable.put("GWindow.draw", new GWindow_draw());
      cmdTable.put("GWindow.exitGraphics", new GWindow_exitGraphics());
      cmdTable.put("GWindow.getScreenHeight", new GWindow_getScreenHeight());
      cmdTable.put("GWindow.getScreenWidth", new GWindow_getScreenWidth());
      cmdTable.put("GWindow.repaint", new GWindow_repaint());
      cmdTable.put("GWindow.requestFocus", new GWindow_requestFocus());
      cmdTable.put("GWindow.setResizable", new GWindow_setResizable());
      cmdTable.put("GWindow.setTitle", new GWindow_setTitle());
      cmdTable.put("GWindow.setVisible", new GWindow_setVisible());
      cmdTable.put("TopCompound.create", new TopCompound_create());
      cmdTable.put("JBEConsole.clear", new JBEConsole_clear());
      cmdTable.put("JBEConsole.getLine", new JBEConsole_getLine());
      cmdTable.put("JBEConsole.print", new JBEConsole_print());
      cmdTable.put("JBEConsole.println", new JBEConsole_println());
      cmdTable.put("JBEConsole.setFont", new JBEConsole_setFont());
      cmdTable.put("JBEConsole.setSize", new JBEConsole_setSize());
      cmdTable.put("Sound.create", new Sound_create());
      cmdTable.put("Sound.delete", new Sound_delete());
      cmdTable.put("Sound.play", new Sound_play());
      return cmdTable;
   }

   public int nextInt(TokenScanner scanner) {
      return Integer.parseInt(scanner.nextToken());
   }

   public double nextDouble(TokenScanner scanner) {
      String token = scanner.nextToken();
      if (token.equals("-")) token += scanner.nextToken();
      return Double.parseDouble(token);
   }

   public String nextString(TokenScanner scanner) {
      return scanner.getStringValue(scanner.nextToken());
   }

   public boolean nextBoolean(TokenScanner scanner) {
      return scanner.nextToken().startsWith("t");
   }
}

class GWindow_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int width = nextInt(scanner);
      scanner.verifyToken(",");
      int height = nextInt(scanner);
      scanner.verifyToken(",");
      String top = nextString(scanner);
      scanner.verifyToken(")");
      jbe.createWindow(id, width, height, (TopCompound) jbe.getGObject(top));
      jbe.println("result:ok");
   }
}

class GWindow_delete extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteWindow(id);
   }
}

class GWindow_close extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw != null) jw.close();
   }
}

class GWindow_addToRegion extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id1 = nextString(scanner);
      scanner.verifyToken(",");
      String id2 = nextString(scanner);
      scanner.verifyToken(",");
      String region = nextString(scanner);
      scanner.verifyToken(")");
      JBEWindow jw = jbe.getWindow(id1);
      GObject gobj = jbe.getGObject(id2);
      if (jw != null && gobj != null) {
         jw.addToRegion(jbe.getInteractor(gobj), region);
      }
   }
}

class GWindow_clear extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw != null) jw.clear();
   }
}

class GWindow_repaint extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw != null) jw.getCanvas().repaint();
   }
}

class GWindow_requestFocus extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(")");
      if (jw != null) {
         jw.toFront();
         jw.getCanvas().requestFocus();
      }
   }
}

class GWindow_setResizable extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(",");
      boolean flag = scanner.nextToken().equals("true");
      scanner.verifyToken(")");
      if (jw != null) jw.setResizable(flag);
   }
}

class GWindow_setTitle extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(",");
      String title = nextString(scanner);
      scanner.verifyToken(")");
      if (jw != null) jw.setTitle(title);
   }
}

class GWindow_setVisible extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(",");
      boolean flag = scanner.nextToken().equals("true");
      scanner.verifyToken(")");
      if (jw != null) jw.setVisible(flag);
   }
}

class GTimer_pause extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      double milliseconds = nextDouble(scanner);
      scanner.verifyToken(")");
      try {
         Thread.sleep((int) Math.round(milliseconds));
      } catch (Exception ex) {
         /* Empty */
      }
      jbe.println("result:ok");
   }
}

class GWindow_getScreenWidth extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
      jbe.println("result:" + size.width);
   }
}

class GWindow_getScreenHeight extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
      jbe.println("result:" + size.height);
   }
}

class JBEConsole_clear extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      jbe.clearConsole();
   }
}

class JBEConsole_setFont extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String font = nextString(scanner);
      scanner.verifyToken(")");
      jbe.setConsoleFont(font);
   }
}

class JBEConsole_setSize extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      int width = nextInt(scanner);
      scanner.verifyToken(",");
      int height = nextInt(scanner);
      scanner.verifyToken(")");
      jbe.setConsoleSize(width, height);
   }
}

class JBEConsole_getLine extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      jbe.println("result:" + jbe.getConsole());
   }
}

class JBEConsole_print extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String str = nextString(scanner);
      scanner.verifyToken(")");
      jbe.putConsole(str);
   }
}

class JBEConsole_println extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      jbe.endLineConsole();
   }
}

class GEvent_getNextEvent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      int mask = nextInt(scanner);
      scanner.verifyToken(")");
      jbe.getNextEvent(mask);
   }
}

class GEvent_waitForEvent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      int mask = nextInt(scanner);
      scanner.verifyToken(")");
      jbe.waitForEvent(mask);
   }
}

class Sound_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String filename = nextString(scanner);
      scanner.verifyToken(")");
      String status = "ok";
      try {
         jbe.createSound(id, filename);
      } catch (Exception ex) {
         status = ex.getMessage();
      }
      jbe.println("result:" + status);
   }
}

class Sound_delete extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteSound(id);
   }
}

class Sound_play extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.playSound(id);
   }
}

class GTimer_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double msec = nextDouble(scanner);
      scanner.verifyToken(")");
      jbe.createTimer(id, msec);
   }
}

class GTimer_deleteTimer extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteTimer(id);
   }
}

class GTimer_startTimer extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.startTimer(id);
   }
}

class GTimer_stopTimer extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.stopTimer(id);
   }
}

class GTimer extends Timer {
   public GTimer(String id, double delay) {
      super((int) Math.round(delay), null);
      this.id = id;
   }

   public String getId() {
      return id;
   }

   private String id;
}

class GRect_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GRect gobj = new GRect(width, height);
      jbe.defineGObject(id, gobj);
   }
}

class GRoundRect_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(",");
      double arc = nextDouble(scanner);
      scanner.verifyToken(")");
      GRoundRect gobj = new GRoundRect(0, 0, width, height, arc);
      jbe.defineGObject(id, gobj);
   }
}

class G3DRect_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(",");
      boolean raised = scanner.nextToken().equals("true");
      scanner.verifyToken(")");
      G3DRect gobj = new G3DRect(0, 0, width, height, raised);
      jbe.defineGObject(id, gobj);
   }
}

class G3DRect_setRaised extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      boolean raised = nextBoolean(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((G3DRect) gobj).setRaised(raised);
   }
}

class GLine_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x1 = nextDouble(scanner);
      scanner.verifyToken(",");
      double y1 = nextDouble(scanner);
      scanner.verifyToken(",");
      double x2 = nextDouble(scanner);
      scanner.verifyToken(",");
      double y2 = nextDouble(scanner);
      scanner.verifyToken(")");
      GLine gobj = new GLine(x1, y1, x2, y2);
      jbe.defineGObject(id, gobj);
   }
}

class GLine_setStartPoint extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GLine) gobj).setStartPoint(x, y);
   }
}

class GLine_setEndPoint extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GLine) gobj).setEndPoint(x, y);
   }
}

class GPolygon_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GPolygon gobj = new GPolygon();
      jbe.defineGObject(id, gobj);
   }
}

class GPolygon_addVertex extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GPolygon) gobj).addVertex(x, y);
   }
}

class GImage_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String filename = nextString(scanner);
      scanner.verifyToken(")");
      try {
         GImage gobj = new GImage(filename);
         jbe.defineGObject(id, gobj);
         System.out.println("result:GDimension(" + gobj.getWidth() +
                            ", " + gobj.getHeight() + ")");
         System.out.flush();
      } catch (Exception ex) {
         System.out.println("result:" + ex.getMessage());
         System.out.flush();
      }
   }
}

class GOval_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GOval gobj = new GOval(width, height);
      jbe.defineGObject(id, gobj);
   }
}

class GLabel_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String str = nextString(scanner);
      scanner.verifyToken(")");
      JBELabel gobj = new JBELabel(str);
      jbe.defineGObject(id, gobj);
   }
}

class GArc_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(",");
      double start = nextDouble(scanner);
      scanner.verifyToken(",");
      double sweep = nextDouble(scanner);
      scanner.verifyToken(")");
      GArc gobj = new GArc(width, height, start, sweep);
      jbe.defineGObject(id, gobj);
   }
}

class GArc_setStartAngle extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double angle = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GArc) gobj).setStartAngle(angle);
   }
}

class GArc_setSweepAngle extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double angle = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GArc) gobj).setSweepAngle(angle);
   }
}

class GArc_setFrameRectangle extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         ((GArc) gobj).setFrameRectangle(x, y, width, height);
      }
   }
}

class GCompound_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      TopCompound gobj = new TopCompound();
      jbe.defineGObject(id, gobj);
   }
}

class GCompound_add extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj1 = jbe.getGObject(id);
      scanner.verifyToken(",");
      id = nextString(scanner);
      GObject gobj2 = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj1 != null && gobj2 != null) {
         ((TopCompound) gobj1).add(gobj2);
      }
   }
}

class TopCompound_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      TopCompound gobj = new TopCompound();
      jbe.defineGObject(id, gobj);
   }
}

class GObject_delete extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      jbe.deleteGObject(id);
   }
}

// Move the computation here into the client side

class GObject_contains extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      System.out.println("result:" + (gobj != null && gobj.contains(x, y)));
      System.out.flush();
   }
}

// Move the computation here into the client side

class GObject_getBounds extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj == null) {
         System.out.println("error: NULL object");
      } else {
         GRectangle bounds = gobj.getBounds();
         System.out.println("result:GRectangle(" + bounds.getX() + ", "
                            + bounds.getY() + ", " + bounds.getWidth() + ", "
                            + bounds.getHeight() + ")");
      }
      System.out.flush();
   }
}

class GObject_remove extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) {
         GCompound parent = (GCompound) gobj.getParent();
         if (parent != null) parent.remove(gobj);
      }
   }
}

class GObject_rotate extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      double theta = nextDouble(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.rotate(theta);
   }
}

class GObject_scale extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      double sx = nextDouble(scanner);
      scanner.verifyToken(",");
      double sy = nextDouble(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.scale(sx, sy);
   }
}

class GObject_sendForward extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendForward();
   }
}

class GObject_sendToFront extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendToFront();
   }
}

class GObject_sendBackward extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendBackward();
   }
}

class GObject_sendToBack extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (gobj != null) gobj.sendToBack();
   }
}

class GObject_setLineWidth extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      double lineWidth = nextDouble(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.setLineWidth(lineWidth);
   }
}

class GObject_setColor extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      String color = nextString(scanner);
      scanner.verifyToken(")");
      if (gobj != null) {
         gobj.setColor(color.equals("") ? null : JTFTools.decodeColor(color));
      }
   }
}

class GObject_setLocation extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double x = nextDouble(scanner);
      scanner.verifyToken(",");
      double y = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) gobj.setLocation(x, y);
   }
}

class GObject_setVisible extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(",");
      boolean flag = nextBoolean(scanner);
      scanner.verifyToken(")");
      if (gobj != null) gobj.setVisible(flag);
   }
}

class GWindow_draw extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      JBEWindow jw = jbe.getWindow(id);
      scanner.verifyToken(",");
      id = nextString(scanner);
      GObject gobj = jbe.getGObject(id);
      scanner.verifyToken(")");
      if (jw != null && gobj != null && gobj.isVisible()) {
         JBECanvas jc = jw.getCanvas();
         Graphics2D osg = jc.getOSG();
         osg.setColor(gobj.getColor());
         gobj.paint(osg);
         jc.repaint();
      }
   }
}

class GObject_setSize extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      double width = nextDouble(scanner);
      scanner.verifyToken(",");
      double height = nextDouble(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GResizable) gobj).setSize(width, height);
   }
}

class GObject_setFilled extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      boolean flag = scanner.nextToken().equals("true");
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) ((GFillable) gobj).setFilled(flag);
   }
}

class GObject_setFillColor extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String color = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         Color c = (color.equals("")) ? null : JTFTools.decodeColor(color);
         ((GFillable) gobj).setFillColor(c);
      }
   }
}

class GLabel_setFont extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String font = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      if (label != null) label.setFont(font);
   }
}

class GLabel_setLabel extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String str = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      if (label != null) label.setLabel(str);
   }
}

class GLabel_getFontAscent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      System.out.println("result:" + label.getAscent());
      System.out.flush();
   }
}

class GLabel_getFontDescent extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      System.out.println("result:" + label.getDescent());
      System.out.flush();
   }
}

class GLabel_getGLabelSize extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GLabel label = (GLabel) jbe.getGObject(id);
      GDimension size = label.getSize();
      System.out.println("result:GDimension(" + size.getWidth()
                       + ", " + size.getHeight() + ")");
      System.out.flush();
   }
}

class GInteractor_setActionCommand extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String cmd = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         ((GInteractor) gobj).setActionCommand(cmd);
      }
   }
}

class GInteractor_getSize extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComponent jcomp = ((GInteractor) gobj).getInteractor();
         Dimension size = (jcomp.isShowing()) ? jcomp.getSize()
                                              : jcomp.getPreferredSize();
         System.out.println("result:GDimension(" + size.width + ", "
                                                 + size.height + ")");
         System.out.flush();
      } else {
         System.out.println("result:GDimension(0, 0)");
         System.out.flush();
      }
   }
}

class GButton_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String label = nextString(scanner);
      scanner.verifyToken(")");
      GButton gobj = new GButton(label, jbe);
      gobj.setActionCommand(label);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GCheckBox_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String label = nextString(scanner);
      scanner.verifyToken(")");
      GCheckBox gobj = new GCheckBox(label, jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GCheckBox_isSelected extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JCheckBox chkbox = (JCheckBox) ((GCheckBox) gobj).getInteractor();
         System.out.println("result:" + chkbox.isSelected());
         System.out.flush();
      } else {
         System.out.println("result:false");
         System.out.flush();
      }
   }
}

class GCheckBox_setSelected extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      boolean state = nextBoolean(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JCheckBox chkbox = (JCheckBox) ((GCheckBox) gobj).getInteractor();
         chkbox.setSelected(state);
      }
   }
}

class GSlider_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int min = nextInt(scanner);
      scanner.verifyToken(",");
      int max = nextInt(scanner);
      scanner.verifyToken(",");
      int value = nextInt(scanner);
      scanner.verifyToken(")");
      GSlider gobj = new GSlider(min, max, value, jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GSlider_getValue extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JSlider slider = (JSlider) ((GSlider) gobj).getInteractor();
         System.out.println("result:" + slider.getValue());
         System.out.flush();
      } else {
         System.out.println("result:0");
         System.out.flush();
      }
   }
}

class GSlider_setValue extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int value = nextInt(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JSlider slider = (JSlider) ((GSlider) gobj).getInteractor();
         slider.setValue(value);
      }
   }
}

class GTextField_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      int nChars = nextInt(scanner);
      scanner.verifyToken(")");
      GTextField gobj = new GTextField(nChars, jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GTextField_getText extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JTextField field = (JTextField) ((GTextField) gobj).getInteractor();
         System.out.println("result:" + field.getText());
         System.out.flush();
      } else {
         System.out.println("result:");
         System.out.flush();
      }
   }
}

class GTextField_setText extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String str = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JTextField field = (JTextField) ((GTextField) gobj).getInteractor();
         field.setText(str);
      }
   }
}

class GChooser_create extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GChooser gobj = new GChooser(jbe);
      jbe.defineGObject(id, gobj);
      jbe.defineSource(gobj.getInteractor(), id);
   }
}

class GChooser_addItem extends JBECommand {
/*
 * Implementation note
 * -------------------
 * This implementation uses reflection to call the addItem method to
 * avoid generating warnings in Java 7 (which added a type parameter
 * to the JComboBox class) while still allowing compilation in Java 6.
 */
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String item = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComboBox chooser = (JComboBox) ((GChooser) gobj).getInteractor();
         Class<?> c = chooser.getClass();
         Class[] types = { Object.class };
         Object[] args = { item };
         try {
            Method addItem = c.getMethod("addItem", types);
            addItem.invoke(chooser, args);
         } catch (Exception ex) {
            System.out.println("error: " + ex);
         }
      }
   }
}

class GChooser_getSelectedItem extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComboBox chooser = (JComboBox) ((GChooser) gobj).getInteractor();
         System.out.println("result:" + chooser.getSelectedItem());
         System.out.flush();
      } else {
         System.out.println("result:");
         System.out.flush();
      }
   }
}

class GChooser_setSelectedItem extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String item = nextString(scanner);
      scanner.verifyToken(")");
      GObject gobj = jbe.getGObject(id);
      if (gobj != null) {
         JComboBox chooser = (JComboBox) ((GChooser) gobj).getInteractor();
         chooser.setSelectedItem(item);
      }
   }
}

class GWindow_setRegionAlignment extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String id = nextString(scanner);
      scanner.verifyToken(",");
      String region = nextString(scanner);
      scanner.verifyToken(",");
      String align = nextString(scanner);
      scanner.verifyToken(")");
      JBEWindow jw = jbe.getWindow(id);
      if (jw != null) jw.setRegionAlignment(region, align);
   }
}

class GWindow_exitGraphics extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      scanner.verifyToken(")");
      System.exit(0);
   }
}

class File_openFileDialog extends JBECommand {
   public void execute(TokenScanner scanner, JavaBackEnd jbe) {
      scanner.verifyToken("(");
      String title = nextString(scanner);
      scanner.verifyToken(",");
      String mode = nextString(scanner);
      scanner.verifyToken(",");
      String path = nextString(scanner);
      scanner.verifyToken(")");
      System.out.println("result:" + jbe.openFileDialog(title, mode, path));
      System.out.flush();
   }
}

/*
 * File: JBEWindow.java
 * --------------------
 * This file implements a graphics window for the JBE.
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

import acm.gui.TableLayout;

import java.awt.BorderLayout;
import java.awt.event.WindowEvent;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class JBEWindow extends JFrame {

   public JBEWindow(JavaBackEnd parent, String id, String appName,
                                        int width, int height) {
      super(appName);
      jbe = parent;
      windowId = id;
      setLayout(new BorderLayout());
      canvas = new JBECanvas(id, width, height);
      addWindowListener(jbe);
      canvas.addComponentListener(jbe);
      canvas.addMouseListener(jbe);
      canvas.addMouseMotionListener(jbe);
      canvas.addKeyListener(jbe);
      add(canvas, BorderLayout.CENTER);
      createSidePanels();
   }

   public JBECanvas getCanvas() {
      return canvas;
   }

   public String getWindowId() {
      return windowId;
   }

   public void close() {
      WindowEvent e = new WindowEvent(this, WindowEvent.WINDOW_CLOSING);
      processWindowEvent(e);
   }

   public void clear() {
      canvas.clear();
      boolean needsValidating = false;
      if (northPanel != null) {
         northPanel.removeAll();
         needsValidating = true;
      }
      if (eastPanel != null) {
         eastPanel.removeAll();
         needsValidating = true;
      }
      if (southPanel != null) {
         southPanel.removeAll();
         needsValidating = true;
      }
      if (westPanel != null) {
         westPanel.removeAll();
         needsValidating = true;
      }
      if (needsValidating) validate();
   }

   public void addToRegion(JComponent comp, String region) {
      JPanel panel = null;
      if (region.equalsIgnoreCase("NORTH")) {
         panel = northPanel;
      } else if (region.equalsIgnoreCase("EAST")) {
         panel = eastPanel;
      } else if (region.equalsIgnoreCase("SOUTH")) {
         panel = southPanel;
      } else if (region.equalsIgnoreCase("WEST")) {
         panel = westPanel;
      }
      if (panel != null) {
         panel.add(comp);
         validate();
      }
   }

   public void setRegionAlignment(String region, String align) {
      JPanel panel = null;
      if (region.equalsIgnoreCase("NORTH")) {
         panel = northPanel;
      } else if (region.equalsIgnoreCase("EAST")) {
         panel = eastPanel;
      } else if (region.equalsIgnoreCase("SOUTH")) {
         panel = southPanel;
      } else if (region.equalsIgnoreCase("WEST")) {
         panel = westPanel;
      }
      int constraint = TableLayout.CENTER;
      if (align.equalsIgnoreCase("LEFT")) {
         constraint = TableLayout.LEFT;
      } else if (align.equalsIgnoreCase("RIGHT")) {
         constraint = TableLayout.RIGHT;
      }
      if (panel != null) {
         ((TableLayout) panel.getLayout()).setHorizontalAlignment(constraint);
      }
   }

   private void createSidePanels() {
      northPanel = new JPanel();
      southPanel = new JPanel();
      eastPanel = new JPanel();
      westPanel = new JPanel();
      northPanel.setLayout(new TableLayout(1, 0, 5, 5));
      southPanel.setLayout(new TableLayout(1, 0, 5, 5));
      westPanel.setLayout(new TableLayout(0, 1, 5, 5));
      eastPanel.setLayout(new TableLayout(0, 1, 5, 5));
      add(northPanel, BorderLayout.NORTH);
      add(southPanel, BorderLayout.SOUTH);
      add(eastPanel, BorderLayout.EAST);
      add(westPanel, BorderLayout.WEST);
   }

   private JavaBackEnd jbe;
   private JBECanvas canvas;
   private String windowId;
   private JPanel northPanel;
   private JPanel eastPanel;
   private JPanel southPanel;
   private JPanel westPanel;
}

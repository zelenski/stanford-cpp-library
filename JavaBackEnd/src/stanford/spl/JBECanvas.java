/*
 * File: JBECanvas.java
 * --------------------
 * This file implements the drawing surface for graphics windows.
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

import acm.graphics.GCanvas;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Shape;
import java.awt.Toolkit;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;

public class JBECanvas extends GCanvas {

   public JBECanvas(String id, int width, int height) {
      windowId = id;
      setBackground(Color.WHITE);
      setForeground(Color.BLACK);
      setOpaque(false);
      setLayout(null);
      preferredWidth = width;
      preferredHeight = height;
   }

   protected Graphics2D getOSG() {
      return osg;
   }

   protected void setTopCompound(TopCompound top) {
      topCompound = top;
      top.setCanvas(this);
      top.setParent(this);
   }

   protected TopCompound getTopCompound() {
      return topCompound;
   }

   protected void initOffscreenImage() {
      Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
      offscreenImage = createImage(size.width, size.height);
      osg = (Graphics2D) offscreenImage.getGraphics();
      osg.setColor(getBackground());
      osg.fillRect(0, 0, size.width, size.height);
      osg.setColor(getForeground());
   }

   public String getWindowId() {
      return windowId;
   }

   public Dimension getPreferredSize() {
      return new Dimension(preferredWidth, preferredHeight);
   }

   public void clear() {
      Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
      osg.setColor(Color.WHITE);
      osg.fillRect(0, 0, size.width, size.height);
      osg.setColor(Color.BLACK);
      repaint();
   }

   public void draw(Shape shape) {
      osg.draw(shape);
      repaint();
   }

   public void fill(Shape shape) {
      osg.fill(shape);
      repaint();
   }

   public void drawRect(double x, double y, double width, double height) {
      Rectangle2D r = new Rectangle2D.Double(x, y, width, height);
      osg.draw(r);
      repaint();
   }

   public void fillRect(double x, double y, double width, double height) {
      Rectangle2D r = new Rectangle2D.Double(x, y, width, height);
      osg.fill(r);
      osg.draw(r);
      repaint();
   }

   public void drawOval(double x, double y, double width, double height) {
      Ellipse2D e = new Ellipse2D.Double(x, y, width, height);
      osg.draw(e);
      repaint();
   }

   public void fillOval(double x, double y, double width, double height) {
      Ellipse2D e = new Ellipse2D.Double(x, y, width, height);
      osg.fill(e);
      osg.draw(e);
      repaint();
   }

   public void drawArc(double x, double y, double width, double height,
                                           double start, double sweep) {
      Arc2D arc = new Arc2D.Double(x, y, width, height,
                                   start, sweep, Arc2D.OPEN);
      osg.draw(arc);
      repaint();
   }

   public void fillArc(double x, double y, double width, double height,
                                           double start, double sweep) {
      Arc2D arc = new Arc2D.Double(x, y, width, height,
                                   start, sweep, Arc2D.PIE);
      osg.fill(arc);
      osg.draw(arc);
      repaint();
   }

   public void drawLine(double x0, double y0, double x1, double y1) {
      Line2D line = new Line2D.Double(x0, y0, x1, y1);
      osg.draw(line);
      repaint();
   }

   public void setColor(int rgb) {
      osg.setColor(new Color(rgb));
   }

   public void paint(Graphics g) {
      g.drawImage(offscreenImage, 0, 0, this);
      topCompound.paint(g);
      super.paint(g);
   }

/* Instance variables */

   private String windowId;
   private TopCompound topCompound;
   private Graphics2D osg;
   private Image offscreenImage;
   private int preferredWidth;
   private int preferredHeight;

}

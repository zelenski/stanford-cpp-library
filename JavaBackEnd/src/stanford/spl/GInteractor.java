/*
 * File: GInteractor.java
 * ----------------------
 * This file implements the abstract class <code>GInteractor</code>,
 * which forms the root of the interactor hierarchy.
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

import acm.graphics.GContainer;
import acm.graphics.GDimension;
import acm.graphics.GMath;
import acm.graphics.GObject;
import acm.graphics.GRectangle;
import acm.graphics.GResizable;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;

import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;

public abstract class GInteractor extends GObject implements GResizable {
   public GInteractor(JComponent comp) {
      interactor = comp;
      actionCommand = "";
      Dimension size = comp.getPreferredSize();
      comp.setSize(size.width, size.height);
   }

   public JComponent getInteractor() {
      return interactor;
   }

   public GRectangle getBounds() {
      Point pt = interactor.getLocation();
      Dimension size = interactor.getPreferredSize();
      return new GRectangle(pt.x, pt.y, size.width, size.height);
   }

   public void setLocation(double x, double y) {
      super.setLocation(x, y);
      interactor.setLocation(GMath.round(x), GMath.round(y));
      interactor.repaint();
   }

   public void setVisible(boolean visible) {
      super.setVisible(visible);
      interactor.setVisible(visible);
      interactor.repaint();
   }

   public void setParent(GContainer parent) {
      if (parent == null) {
         interactor.getParent().remove(interactor);
      } else if (parent instanceof TopCompound) {
         TopCompound top = (TopCompound) parent;
         JBECanvas jc = top.getCanvas();
         if (jc != null) {
            jc.add(interactor);
            jc.validate();
         }
      }
   }

   public void setSize(double width, double height) {
      int iw = GMath.round(width);
      int ih = GMath.round(height);
      interactor.setPreferredSize(new Dimension(iw, ih));
      interactor.setSize(iw, ih);
      interactor.repaint();
   }

   public void setSize(GDimension size) {
      setSize(size.getWidth(), size.getHeight());
   }

   public void setBounds(double x, double y, double width, double height) {
      setLocation(x, y);
      setSize(width, height);
   }

   public void setBounds(GRectangle bounds) {
      setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(),
                                              bounds.getHeight());
   }

   public void setActionCommand(String cmd) {
      actionCommand = cmd;
   }

   public String getActionCommand() {
      return actionCommand;
   }

   public void repaint() {
      /* Empty */
   }

   public void paint(Graphics g) {
      /* Empty */
   }

   protected void paintObject(Graphics g) {
      /* Empty */
   }

   private JComponent interactor;
   private String actionCommand;
}

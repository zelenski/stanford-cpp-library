/*
 * File: JBEConsole.java
 * ---------------------
 * This file implements the console for the JBE.  Most of the code is
 * inherited from <code>IOConsole</code>.
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

import acm.io.IOConsole;

import java.awt.Dimension;

public class JBEConsole extends IOConsole {
   public void setPreferredSize(int width, int height) {
      preferredWidth = width;
      preferredHeight = height;
   }

   public Dimension getPreferredSize() {
      return new Dimension(preferredWidth, preferredHeight);
   }

   private int preferredWidth;
   private int preferredHeight;

}

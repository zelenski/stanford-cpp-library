/*
 * File: GChooser.java
 * -------------------
 * This file implements the <code>GChooser</code> interactor class.
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

import java.awt.event.ActionListener;

import javax.swing.JComboBox;

public class GChooser extends GInteractor {
   public GChooser(ActionListener listener) {
      super(new JComboBox());
      JComboBox chooser = (JComboBox) getInteractor();
      if (listener != null) {
         chooser.setEditable(false);
         chooser.addActionListener(listener);
      }
   }
}

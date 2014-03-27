/*
 * File: JBEFileFilter.java
 * ------------------------
 * This file implements the file filter for the file dialogs.
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

import acm.util.FileChooserFilter;

public class JBEFileFilter extends FileChooserFilter {

   public JBEFileFilter(String path) {
      super(getPattern(path));
      int sp = Math.max(path.lastIndexOf("/"), path.lastIndexOf('\\'));
      dir = (sp == -1) ? "" : path.substring(0, sp);
      String last = path.substring(sp + 1);
      if (!isPattern(last)) {
         if (dir.isEmpty()) dir += "/";
         dir += last;
      }
      if (dir.isEmpty()) {
         dir = System.getProperty("user.dir");
      } else if (!dir.startsWith("/")) {
         dir = System.getProperty("user.dir") + "/" + dir;
      }
   }

   public String getDirectory() {
      return dir;
   }

   private static String getPattern(String path) {
      int sp = Math.max(path.lastIndexOf("/"), path.lastIndexOf('\\'));
      String last = path.substring(sp + 1);
      return (isPattern(last)) ? last : null;
   }

   private static boolean isPattern(String str) {
      for (int i = 0; i < str.length(); i++) {
         switch (str.charAt(i)) {
          case ';': case '*': case '?': case '[': case ']': return true;
         }
      }
      return false;
   }

   private String dir;

}

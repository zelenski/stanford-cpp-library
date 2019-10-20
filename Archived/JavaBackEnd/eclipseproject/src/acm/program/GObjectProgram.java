/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/25
 * - initial version; moved out of GraphicsProgram.java into its own file
 */

package acm.program;

import java.lang.reflect.Method;
import acm.graphics.*;
import acm.util.*;

/* Package class: GObjectProgram */
/**
 * This class is used to launch a program containing a single
 * <code>GObject</code> instance at its center.
 */
class GObjectProgram extends GraphicsProgram {

	/* Hook method: runHook() */
	/**
	 * Calls the run method in the graphical object.
	 */
	protected void runHook() {
		GObject gobj = (GObject) getStartupObject();
		GDimension size = gobj.getSize();
		add(gobj, (getWidth() - size.getWidth()) / 2,
				(getHeight() - size.getHeight()) / 2);
		try {
			Class<?> gobjClass = gobj.getClass();
			String className = gobjClass.getName();
			className = className.substring(className.lastIndexOf(".") + 1);
			setTitle(className);
			Method run = gobjClass.getMethod("run", new Class[0]);
			if (run == null)
				throw new ErrorException(className + " has no run method");
			run.invoke(gobj, new Object[0]);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}
}

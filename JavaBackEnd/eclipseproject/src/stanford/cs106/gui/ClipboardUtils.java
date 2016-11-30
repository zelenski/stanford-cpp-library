/*
 * @author Marty Stepp
 * @version 2016/11/24
 * - initial version
 */

package stanford.cs106.gui;

import java.awt.*;
import java.awt.datatransfer.*;
import java.io.*;

/**
 * Utility methods to set and get the state of the current OS clipboard.
 */
public final class ClipboardUtils {
	private ClipboardUtils() {
		// empty
	}

	/**
	 * Sets the clipboard to store the given text.
	 * Equivalent to a Ctrl+C "copy" operation.
	 */
	public static void copy(String text) {
		if (text == null) {
			text = "";
		}
		StringSelection selection = new StringSelection(text);
		Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
		clipboard.setContents(selection, selection);
	}

	/**
	 * Returns the current contents of the clipboard.
	 * If the clipboard contains no contents, returns an empty string.
	 */
	public static String get() {
		Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
		Transferable content = clipboard.getContents(/* requestor; not used */ ClipboardUtils.class);
		String result = "";
		if (content != null) {
			try {
				result = content.getTransferData(DataFlavor.stringFlavor).toString();
				if (result == null) {
					result = "";
				}
			} catch (UnsupportedFlavorException ufe) {
				// empty
			} catch (IOException ioe) {
				// empty
			}
		}
		return result;
	}
	
	/**
	 * Returns true if the clipboard currently does not contain any text.
	 */
	public static boolean isEmpty() {
		String clip = get();
		return clip == null || clip.isEmpty();
	}
}

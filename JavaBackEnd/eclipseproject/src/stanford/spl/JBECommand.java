/*
 * @version 2016/12/01
 * - added AutograderUnitTest_catchExceptions
 * @version 2016/11/26
 * - added Clipboard_get/set
 * - added GTable_set Cell/Column/Row Background, Font, Foreground
 * - added GWindow_setCloseOperation
 * @version 2016/11/18
 * - added GTable_setEditorValue, _setRowColumnHeadersVisible, etc.
 * @version 2016/10/25
 * - added GInteractor_set/getFont, Mnemonic, Accelerator
 * @version 2016/10/23
 * - added GInteractor_add/removeActionListener
 * @version 2016/10/16
 * - added GWindow pixel methods; added GInteractor_setTooltip
 * @version 2016/10/12
 * - added shouldRunOnSwingEventThread
 * @version 2016/10/08
 * - added GWindow_toBack
 * @version 2016/09/26
 * - added Note_play
 * @version 2016/07/06
 * - added DiffImage_show
 */

package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.reflect.ReflectionRuntimeException;

import java.io.BufferedReader;
import java.io.Reader;
import java.lang.reflect.Field;
import java.util.*;

public abstract class JBECommand {
	@SuppressWarnings("unchecked")
	private static final Set<Class<? extends JBECommand>> KNOWN_COMMANDS = new LinkedHashSet<Class<? extends JBECommand>>(Arrays.asList(
			AutograderInput_addButton.class,
			AutograderInput_addCategory.class,
			AutograderInput_removeButton.class,
			AutograderInput_removeCategory.class,
			AutograderInput_setColumns.class,
			AutograderInput_setVisible.class,
			AutograderUnitTest_addTest.class,
			AutograderUnitTest_catchExceptions.class,
			AutograderUnitTest_clearTests.class,
			AutograderUnitTest_clearTestResults.class,
			AutograderUnitTest_isChecked.class,
			AutograderUnitTest_setChecked.class,
			AutograderUnitTest_setTestCounts.class,
			AutograderUnitTest_setTestDetails.class,
			AutograderUnitTest_setTestingCompleted.class,
			AutograderUnitTest_setTestResult.class,
			AutograderUnitTest_setTestRuntime.class,
			AutograderUnitTest_setVisible.class,
			AutograderUnitTest_setWindowDescriptionText.class,
			Clipboard_get.class,
			Clipboard_set.class,
			DiffImage_compareWindowToImage.class,
			DiffImage_show.class,
			DiffImage_compareImages.class,
			File_openFileDialog.class,
			File_getTempDirectory.class,
			G3DRect_create.class,
			G3DRect_setRaised.class,
			GArc_create.class,
			GArc_setFrameRectangle.class,
			GArc_setStartAngle.class,
			GArc_setSweepAngle.class,
			GBufferedImage_create.class,
			GBufferedImage_fill.class,
			GBufferedImage_fillRegion.class,
			GBufferedImage_load.class,
			GBufferedImage_resize.class,
			GBufferedImage_save.class,
			GBufferedImage_setRGB.class,
			GBufferedImage_updateAllPixels.class,
			GButton_create.class,
			GCheckBox_create.class,
			GCheckBox_isSelected.class,
			GCheckBox_setSelected.class,
			GChooser_addItem.class,
			GChooser_create.class,
			GChooser_getSelectedItem.class,
			GChooser_setSelectedItem.class,
			GCompound_add.class,
			GCompound_create.class,
			GEvent_getNextEvent.class,
			GEvent_waitForEvent.class,
			GFileChooser_showOpenDialog.class,
			GFileChooser_showSaveDialog.class,
			GImage_create.class,
			GInteractor_addActionListener.class,
			GInteractor_getAccelerator.class,
			GInteractor_getFont.class,
			GInteractor_getMnemonic.class,
			GInteractor_getSize.class,
			GInteractor_isEnabled.class,
			GInteractor_removeActionListener.class,
			GInteractor_requestFocus.class,
			GInteractor_setAccelerator.class,
			GInteractor_setActionCommand.class,
			GInteractor_setBackground.class,
			GInteractor_setEnabled.class,
			GInteractor_setFont.class,
			GInteractor_setIcon.class,
			GInteractor_setMnemonic.class,
			GInteractor_setText.class,
			GInteractor_setTextPosition.class,
			GInteractor_setTooltip.class,
			GLabel_create.class,
			GLabel_getFontAscent.class,
			GLabel_getFontDescent.class,
			GLabel_getGLabelSize.class,
			GLabel_setFont.class,
			GLabel_setLabel.class,
			GLine_create.class,
			GLine_setEndPoint.class,
			GLine_setStartPoint.class,
			GObject_contains.class,
			GObject_delete.class,
			GObject_getBounds.class,
			GObject_remove.class,
			GObject_rotate.class,
			GObject_scale.class,
			GObject_sendBackward.class,
			GObject_sendForward.class,
			GObject_sendToBack.class,
			GObject_sendToFront.class,
			GObject_setAntialiasing.class,
			GObject_setColor.class,
			GObject_setFillColor.class,
			GObject_setFilled.class,
			GObject_setLineWidth.class,
			GObject_setLocation.class,
			GObject_setSize.class,
			GObject_setVisible.class,
			GOptionPane_showConfirmDialog.class,
			GOptionPane_showInputDialog.class,
			GOptionPane_showMessageDialog.class,
			GOptionPane_showOptionDialog.class,
			GOptionPane_showTextFileDialog.class,
			GOval_create.class,
			GPolygon_addVertex.class,
			GPolygon_create.class,
			GRadioButton_create.class,
			GRadioButton_isSelected.class,
			GRadioButton_setSelected.class,
			GRect_create.class,
			GRoundRect_create.class,
			GSlider_create.class,
			GSlider_getMajorTickSpacing.class,
			GSlider_getMinorTickSpacing.class,
			GSlider_getPaintLabels.class,
			GSlider_getPaintTicks.class,
			GSlider_getSnapToTicks.class,
			GSlider_getValue.class,
			GSlider_setMajorTickSpacing.class,
			GSlider_setMinorTickSpacing.class,
			GSlider_setPaintLabels.class,
			GSlider_setPaintTicks.class,
			GSlider_setSnapToTicks.class,
			GSlider_setValue.class,
			GTable_autofitColumnWidths.class,
			GTable_clear.class,
			GTable_clearFormatting.class,
			GTable_create.class,
			GTable_get.class,
			GTable_getColumnWidth.class,
			GTable_getSelection.class,
			GTable_resize.class,
			GTable_select.class,
			GTable_set.class,
			GTable_setCellAlignment.class,
			GTable_setCellBackground.class,
			GTable_setCellFont.class,
			GTable_setCellForeground.class,
			GTable_setColumnAlignment.class,
			GTable_setColumnBackground.class,
			GTable_setColumnFont.class,
			GTable_setColumnForeground.class,
			GTable_setColumnHeaderStyle.class,
			GTable_setColumnWidth.class,
			GTable_setEditable.class,
			GTable_setEditorValue.class,
			GTable_setEventEnabled.class,
			GTable_setFont.class,
			GTable_setHorizontalAlignment.class,
			GTable_setRowAlignment.class,
			GTable_setRowBackground.class,
			GTable_setRowColumnHeadersVisible.class,
			GTable_setRowFont.class,
			GTable_setRowForeground.class,
			GTextArea_create.class,
			GTextArea_getText.class,
			GTextArea_setEditable.class,
			GTextArea_setFont.class,
			GTextArea_setText.class,
			GTextField_create.class,
			GTextField_getText.class,
			GTextField_isEditable.class,
			GTextField_setEditable.class,
			GTextField_setPlaceholder.class,
			GTextField_setText.class,
			GTimer_create.class,
			GTimer_deleteTimer.class,
			GTimer_pause.class,
			GTimer_startTimer.class,
			GTimer_stopTimer.class,
			GWindow_addToRegion.class,
			GWindow_clear.class,
			GWindow_clearCanvas.class,
			GWindow_close.class,
			GWindow_create.class,
			GWindow_delete.class,
			GWindow_drawInBackground.class,
			GWindow_draw.class,
			GWindow_exitGraphics.class,
			GWindow_getCanvasSize.class,
			GWindow_getContentPaneSize.class,
			GWindow_getLocation.class,
			GWindow_getPixel.class,
			GWindow_getPixels.class,
			GWindow_getRegionSize.class,
			GWindow_getScreenHeight.class,
			GWindow_getScreenSize.class,
			GWindow_getScreenWidth.class,
			GWindow_getSize.class,
			GWindow_minimize.class,
			GWindow_pack.class,
			GWindow_removeFromRegion.class,
			GWindow_repaint.class,
			GWindow_requestFocus.class,
			GWindow_saveCanvasPixels.class,
			GWindow_setCanvasSize.class,
			GWindow_setCloseOperation.class,
			GWindow_setExitOnClose.class,
			GWindow_setLocation.class,
			GWindow_setLocationSaved.class,
			GWindow_setPixel.class,
			GWindow_setPixels.class,
			GWindow_setRegionAlignment.class,
			GWindow_setRegionSize.class,
			GWindow_setResizable.class,
			GWindow_setSize.class,
			GWindow_setTitle.class,
			GWindow_setVisible.class,
			GWindow_toBack.class,
			GWindow_toFront.class,
			HttpServer_start.class,
			HttpServer_stop.class,
			HttpServer_sendResponse.class,
			HttpServer_sendResponseFile.class,
			JBEConsole_clear.class,
			JBEConsole_getLine.class,
			JBEConsole_minimize.class,
			JBEConsole_println.class,
			JBEConsole_print.class,
			JBEConsole_setCloseOperation.class,
			JBEConsole_setErrorColor.class,
			JBEConsole_setExitOnClose.class,
			JBEConsole_setFont.class,
			JBEConsole_setLocation.class,
			JBEConsole_setLocationSaved.class,
			JBEConsole_setOutputColor.class,
			JBEConsole_setSize.class,
			JBEConsole_setTitle.class,
			JBEConsole_setVisible.class,
			JBEConsole_toFront.class,
			Note_play.class,
			Regex_matchCount.class,
			Regex_matchCountWithLines.class,
			Regex_match.class,
			Regex_replace.class,
			Sound_create.class,
			Sound_delete.class,
			Sound_play.class,
			StanfordCppLib_getJbeVersion.class,
			StanfordCppLib_setCppVersion.class,
			TopCompound_create.class,
			URL_download.class
	));
	
	public abstract void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd);
	
	public static HashMap<String, JBECommand> createCommandTable() {
		HashMap<String, JBECommand> commandMap = new HashMap<String, JBECommand>();
		for (Class<? extends JBECommand> commandClass : KNOWN_COMMANDS) {
			addCommand(commandMap, commandClass);
		}
		return commandMap;
	}
	
	private static void addCommand(HashMap<String, JBECommand> commandMap, Class<? extends JBECommand> commandClass) {
		String className = commandClass.getSimpleName();
		className = className.replace("_", ".");
		try {
			JBECommand command = commandClass.newInstance();
			commandMap.put(className, command);
		} catch (IllegalAccessException iae) {
			throw new ReflectionRuntimeException(iae);
		} catch (InstantiationException ie) {
			throw new ReflectionRuntimeException(ie);
		}
	}

	public int nextInt(TokenScanner paramTokenScanner) {
		String token = paramTokenScanner.nextToken();
		if (token.equals("-")) {
			// BUGBUG: argh geez, doesn't handle negative numbers? really? cmon
			token += paramTokenScanner.nextToken();
		}
		return Integer.parseInt(token);
	}

	public double nextDouble(TokenScanner paramTokenScanner) {
		String str = paramTokenScanner.nextToken();
		if (str.equals("-")) {
			// BUGBUG: argh geez, doesn't handle negative numbers? really? cmon
			str += paramTokenScanner.nextToken();
		}
		return Double.parseDouble(str);
	}

	public String nextString(TokenScanner paramTokenScanner) {
		return paramTokenScanner.getStringValue(paramTokenScanner.nextToken());
	}
	
	public String nextBase64(TokenScanner paramTokenScanner) {
		String base64 = "";
		try {
			BufferedReader reader = new BufferedReader(getTokenScannerReader(paramTokenScanner));
			// throw away ", \"" char
			while (reader.read() != '"') {
				// empty
			}
			base64 = reader.readLine();
			
			// trim off "" from start/end and ) from end
			if (base64.endsWith("\")")) {
				base64 = base64.substring(0, base64.length() - 2);
				
				// put ) char back into token scanner to read 
				paramTokenScanner.ungetChar(')');
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return base64;
	}

	public boolean nextBoolean(TokenScanner paramTokenScanner) {
		return paramTokenScanner.nextToken().startsWith("t");
	}

	/**
	 * Returns whether this command should wait for itself to finish running on the Swing GUI event thread.
	 * This is true only for graphical/GUI related commands that return important results that must be
	 * processed in an exact order.
	 */
	public boolean shouldInvokeAndWait() {
		// TODO: implement
		return true;
	}

	/**
	 * Returns whether this command should run on the Swing GUI event thread.
	 * This is typically true for graphical/GUI related commands.
	 */
	public boolean shouldRunOnSwingEventThread() {
		String className = getClass().getSimpleName();
		return className.startsWith("G")
				// || className.startsWith("JBEConsole")
				|| className.startsWith("TopCompound");
	}

	private Reader getTokenScannerReader(TokenScanner scanner) {
		try {
			Field inputField = TokenScanner.class.getDeclaredField("input");
			inputField.setAccessible(true);
			return (Reader) inputField.get(scanner);
		} catch (Exception e) {
			System.err.println(e);
			return null;
		}
	}
}

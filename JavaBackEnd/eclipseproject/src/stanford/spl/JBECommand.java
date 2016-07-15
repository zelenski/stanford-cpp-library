/*
 * @version 2016/07/06
 * - added DiffImage_show
 */

package stanford.spl;

import acm.util.TokenScanner;

import java.util.HashMap;

public abstract class JBECommand {
	public abstract void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd);

	public static HashMap<String, JBECommand> createCommandTable() {
		HashMap<String, JBECommand> localHashMap = new HashMap<String, JBECommand>();
		localHashMap.put("AutograderInput.addButton", new AutograderInput_addButton());
		localHashMap.put("AutograderInput.addCategory", new AutograderInput_addCategory());
		localHashMap.put("AutograderInput.removeButton", new AutograderInput_removeButton());
		localHashMap.put("AutograderInput.removeCategory", new AutograderInput_removeCategory());
		localHashMap.put("AutograderInput.setColumns", new AutograderInput_setColumns());
		localHashMap.put("AutograderInput.setVisible", new AutograderInput_setVisible());
		localHashMap.put("AutograderUnitTest.addTest", new AutograderUnitTest_addTest());
		localHashMap.put("AutograderUnitTest.clearTests", new AutograderUnitTest_clearTests());
		localHashMap.put("AutograderUnitTest.clearTestResults", new AutograderUnitTest_clearTestResults());
		localHashMap.put("AutograderUnitTest.isChecked", new AutograderUnitTest_isChecked());
		localHashMap.put("AutograderUnitTest.setChecked", new AutograderUnitTest_setChecked());
		localHashMap.put("AutograderUnitTest.setTestCounts", new AutograderUnitTest_setTestCounts());
		localHashMap.put("AutograderUnitTest.setTestDetails", new AutograderUnitTest_setTestDetails());
		localHashMap.put("AutograderUnitTest.setTestingCompleted", new AutograderUnitTest_setTestingCompleted());
		localHashMap.put("AutograderUnitTest.setTestResult", new AutograderUnitTest_setTestResult());
		localHashMap.put("AutograderUnitTest.setTestRuntime", new AutograderUnitTest_setTestRuntime());
		localHashMap.put("AutograderUnitTest.setVisible", new AutograderUnitTest_setVisible());
		localHashMap.put("AutograderUnitTest.setWindowDescriptionText", new AutograderUnitTest_setWindowDescriptionText());
		localHashMap.put("DiffImage.compareWindowToImage", new DiffImage_compareWindowToImage());
		localHashMap.put("DiffImage.show", new DiffImage_show());
		localHashMap.put("File.openFileDialog", new File_openFileDialog());
		localHashMap.put("File.getTempFolder", new File_getTempDirectory());
		localHashMap.put("G3DRect.create", new G3DRect_create());
		localHashMap.put("G3DRect.setRaised", new G3DRect_setRaised());
		localHashMap.put("GArc.create", new GArc_create());
		localHashMap.put("GArc.setFrameRectangle", new GArc_setFrameRectangle());
		localHashMap.put("GArc.setStartAngle", new GArc_setStartAngle());
		localHashMap.put("GArc.setSweepAngle", new GArc_setSweepAngle());
		localHashMap.put("GBufferedImage.create", new GBufferedImage_create());
		localHashMap.put("GBufferedImage.fill", new GBufferedImage_fill());
		localHashMap.put("GBufferedImage.fillRegion", new GBufferedImage_fillRegion());
		localHashMap.put("GBufferedImage.load", new GBufferedImage_load());
		localHashMap.put("GBufferedImage.resize", new GBufferedImage_resize());
		localHashMap.put("GBufferedImage.save", new GBufferedImage_save());
		localHashMap.put("GBufferedImage.setRGB", new GBufferedImage_setRGB());
		localHashMap.put("GBufferedImage.updateAllPixels", new GBufferedImage_updateAllPixels());
		localHashMap.put("GButton.create", new GButton_create());
		localHashMap.put("GCheckBox.create", new GCheckBox_create());
		localHashMap.put("GCheckBox.isSelected", new GCheckBox_isSelected());
		localHashMap.put("GCheckBox.setSelected", new GCheckBox_setSelected());
		localHashMap.put("GChooser.addItem", new GChooser_addItem());
		localHashMap.put("GChooser.create", new GChooser_create());
		localHashMap.put("GChooser.getSelectedItem", new GChooser_getSelectedItem());
		localHashMap.put("GChooser.setSelectedItem", new GChooser_setSelectedItem());
		localHashMap.put("GCompound.add", new GCompound_add());
		localHashMap.put("GCompound.create", new GCompound_create());
		localHashMap.put("GEvent.getNextEvent", new GEvent_getNextEvent());
		localHashMap.put("GEvent.waitForEvent", new GEvent_waitForEvent());
		localHashMap.put("GFileChooser.showOpenDialog", new GFileChooser_showOpenDialog());
		localHashMap.put("GFileChooser.showSaveDialog", new GFileChooser_showSaveDialog());
		localHashMap.put("GImage.create", new GImage_create());
		localHashMap.put("GInteractor.getSize", new GInteractor_getSize());
		localHashMap.put("GInteractor.isEnabled", new GInteractor_isEnabled());
		localHashMap.put("GInteractor.setActionCommand", new GInteractor_setActionCommand());
		localHashMap.put("GInteractor.setBackground", new GInteractor_setBackground());
		localHashMap.put("GInteractor.setEnabled", new GInteractor_setEnabled());
		localHashMap.put("GInteractor.setIcon", new GInteractor_setIcon());
		localHashMap.put("GInteractor.setTextPosition", new GInteractor_setTextPosition());
		localHashMap.put("GLabel.create", new GLabel_create());
		localHashMap.put("GLabel.getFontAscent", new GLabel_getFontAscent());
		localHashMap.put("GLabel.getFontDescent", new GLabel_getFontDescent());
		localHashMap.put("GLabel.getGLabelSize", new GLabel_getGLabelSize());
		localHashMap.put("GLabel.setFont", new GLabel_setFont());
		localHashMap.put("GLabel.setLabel", new GLabel_setLabel());
		localHashMap.put("GLine.create", new GLine_create());
		localHashMap.put("GLine.setEndPoint", new GLine_setEndPoint());
		localHashMap.put("GLine.setStartPoint", new GLine_setStartPoint());
		localHashMap.put("GObject.contains", new GObject_contains());
		localHashMap.put("GObject.delete", new GObject_delete());
		localHashMap.put("GObject.getBounds", new GObject_getBounds());
		localHashMap.put("GObject.remove", new GObject_remove());
		localHashMap.put("GObject.rotate", new GObject_rotate());
		localHashMap.put("GObject.scale", new GObject_scale());
		localHashMap.put("GObject.sendBackward", new GObject_sendBackward());
		localHashMap.put("GObject.sendForward", new GObject_sendForward());
		localHashMap.put("GObject.sendToBack", new GObject_sendToBack());
		localHashMap.put("GObject.sendToFront", new GObject_sendToFront());
		localHashMap.put("GObject.setAntialiasing", new GObject_setAntialiasing());
		localHashMap.put("GObject.setColor", new GObject_setColor());
		localHashMap.put("GObject.setFillColor", new GObject_setFillColor());
		localHashMap.put("GObject.setFilled", new GObject_setFilled());
		localHashMap.put("GObject.setLineWidth", new GObject_setLineWidth());
		localHashMap.put("GObject.setLocation", new GObject_setLocation());
		localHashMap.put("GObject.setSize", new GObject_setSize());
		localHashMap.put("GObject.setVisible", new GObject_setVisible());
		localHashMap.put("GOptionPane.showConfirmDialog", new GOptionPane_showConfirmDialog());
		localHashMap.put("GOptionPane.showInputDialog", new GOptionPane_showInputDialog());
		localHashMap.put("GOptionPane.showMessageDialog", new GOptionPane_showMessageDialog());
		localHashMap.put("GOptionPane.showOptionDialog", new GOptionPane_showOptionDialog());
		localHashMap.put("GOptionPane.showTextFileDialog", new GOptionPane_showTextFileDialog());
		localHashMap.put("GOval.create", new GOval_create());
		localHashMap.put("GPolygon.addVertex", new GPolygon_addVertex());
		localHashMap.put("GPolygon.create", new GPolygon_create());
		localHashMap.put("GRadioButton.create", new GRadioButton_create());
		localHashMap.put("GRadioButton.isSelected", new GRadioButton_isSelected());
		localHashMap.put("GRadioButton.setSelected", new GRadioButton_setSelected());
		localHashMap.put("GRect.create", new GRect_create());
		localHashMap.put("GRoundRect.create", new GRoundRect_create());
		localHashMap.put("GSlider.create", new GSlider_create());
		localHashMap.put("GSlider.getMajorTickSpacing", new GSlider_getMajorTickSpacing());
		localHashMap.put("GSlider.getMinorTickSpacing", new GSlider_getMinorTickSpacing());
		localHashMap.put("GSlider.getPaintLabels", new GSlider_getPaintLabels());
		localHashMap.put("GSlider.getPaintTicks", new GSlider_getPaintTicks());
		localHashMap.put("GSlider.getSnapToTicks", new GSlider_getSnapToTicks());
		localHashMap.put("GSlider.getValue", new GSlider_getValue());
		localHashMap.put("GSlider.setMajorTickSpacing", new GSlider_setMajorTickSpacing());
		localHashMap.put("GSlider.setMinorTickSpacing", new GSlider_setMinorTickSpacing());
		localHashMap.put("GSlider.setPaintLabels", new GSlider_setPaintLabels());
		localHashMap.put("GSlider.setPaintTicks", new GSlider_setPaintTicks());
		localHashMap.put("GSlider.setSnapToTicks", new GSlider_setSnapToTicks());
		localHashMap.put("GSlider.setValue", new GSlider_setValue());
		localHashMap.put("GTable.clear", new GTable_clear());
		localHashMap.put("GTable.create", new GTable_create());
		localHashMap.put("GTable.get", new GTable_get());
		localHashMap.put("GTable.getColumnWidth", new GTable_getColumnWidth());
		localHashMap.put("GTable.getSelection", new GTable_getSelection());
		localHashMap.put("GTable.resize", new GTable_resize());
		localHashMap.put("GTable.select", new GTable_select());
		localHashMap.put("GTable.set", new GTable_set());
		localHashMap.put("GTable.setColumnWidth", new GTable_setColumnWidth());
		localHashMap.put("GTable.setEditable", new GTable_setEditable());
		localHashMap.put("GTable.setEventEnabled", new GTable_setEventEnabled());
		localHashMap.put("GTable.setFont", new GTable_setFont());
		localHashMap.put("GTable.setHorizontalAlignment", new GTable_setHorizontalAlignment());
		localHashMap.put("GTextField.create", new GTextField_create());
		localHashMap.put("GTextField.getText", new GTextField_getText());
		localHashMap.put("GTextField.isEditable", new GTextField_isEditable());
		localHashMap.put("GTextField.setEditable", new GTextField_setEditable());
		localHashMap.put("GTextField.setText", new GTextField_setText());
		localHashMap.put("GTimer.create", new GTimer_create());
		localHashMap.put("GTimer.deleteTimer", new GTimer_deleteTimer());
		localHashMap.put("GTimer.pause", new GTimer_pause());
		localHashMap.put("GTimer.startTimer", new GTimer_startTimer());
		localHashMap.put("GTimer.stopTimer", new GTimer_stopTimer());
		localHashMap.put("GWindow.addToRegion", new GWindow_addToRegion());
		localHashMap.put("GWindow.clear", new GWindow_clear());
		localHashMap.put("GWindow.clearCanvas", new GWindow_clearCanvas());
		localHashMap.put("GWindow.close", new GWindow_close());
		localHashMap.put("GWindow.create", new GWindow_create());
		localHashMap.put("GWindow.delete", new GWindow_delete());
		localHashMap.put("GWindow.drawInBackground", new GWindow_drawInBackground());
		localHashMap.put("GWindow.draw", new GWindow_draw());
		localHashMap.put("GWindow.exitGraphics", new GWindow_exitGraphics());
		localHashMap.put("GWindow.getCanvasSize", new GWindow_getCanvasSize());
		localHashMap.put("GWindow.getLocation", new GWindow_getLocation());
		localHashMap.put("GWindow.getRegionSize", new GWindow_getRegionSize());
		localHashMap.put("GWindow.getScreenHeight", new GWindow_getScreenHeight());
		localHashMap.put("GWindow.getScreenSize", new GWindow_getScreenSize());
		localHashMap.put("GWindow.getScreenWidth", new GWindow_getScreenWidth());
		localHashMap.put("GWindow.getSize", new GWindow_getSize());
		localHashMap.put("GWindow.minimize", new GWindow_minimize());
		localHashMap.put("GWindow.pack", new GWindow_pack());
		localHashMap.put("GWindow.removeFromRegion", new GWindow_removeFromRegion());
		localHashMap.put("GWindow.repaint", new GWindow_repaint());
		localHashMap.put("GWindow.requestFocus", new GWindow_requestFocus());
		localHashMap.put("GWindow.setCanvasSize", new GWindow_setCanvasSize());
		localHashMap.put("GWindow.setExitOnClose", new GWindow_setExitOnClose());
		localHashMap.put("GWindow.setLocation", new GWindow_setLocation());
		localHashMap.put("GWindow.setLocationSaved", new GWindow_setLocationSaved());
		localHashMap.put("GWindow.setRegionAlignment", new GWindow_setRegionAlignment());
		localHashMap.put("GWindow.setRegionSize", new GWindow_setRegionSize());
		localHashMap.put("GWindow.setResizable", new GWindow_setResizable());
		localHashMap.put("GWindow.setSize", new GWindow_setSize());
		localHashMap.put("GWindow.setTitle", new GWindow_setTitle());
		localHashMap.put("GWindow.setVisible", new GWindow_setVisible());
		localHashMap.put("GWindow.toFront", new GWindow_toFront());
		localHashMap.put("HttpServer.start", new HttpServer_start());
		localHashMap.put("HttpServer.stop", new HttpServer_stop());
		localHashMap.put("HttpServer.sendResponse", new HttpServer_sendResponse());
		localHashMap.put("HttpServer.sendResponseFile", new HttpServer_sendResponseFile());
		localHashMap.put("JBEConsole.clear", new JBEConsole_clear());
		localHashMap.put("JBEConsole.getLine", new JBEConsole_getLine());
		localHashMap.put("JBEConsole.minimize", new JBEConsole_minimize());
		localHashMap.put("JBEConsole.println", new JBEConsole_println());
		localHashMap.put("JBEConsole.print", new JBEConsole_print());
		localHashMap.put("JBEConsole.setCloseOperation", new JBEConsole_setCloseOperation());
		localHashMap.put("JBEConsole.setErrorColor", new JBEConsole_setErrorColor());
		localHashMap.put("JBEConsole.setExitOnClose", new JBEConsole_setExitOnClose());
		localHashMap.put("JBEConsole.setFont", new JBEConsole_setFont());
		localHashMap.put("JBEConsole.setLocation", new JBEConsole_setLocation());
		localHashMap.put("JBEConsole.setLocationSaved", new JBEConsole_setLocationSaved());
		localHashMap.put("JBEConsole.setOutputColor", new JBEConsole_setOutputColor());
		localHashMap.put("JBEConsole.setSize", new JBEConsole_setSize());
		localHashMap.put("JBEConsole.setTitle", new JBEConsole_setTitle());
		localHashMap.put("JBEConsole.setVisible", new JBEConsole_setVisible());
		localHashMap.put("JBEConsole.toFront", new JBEConsole_toFront());
		localHashMap.put("Regex.matchCount", new Regex_matchCount());
		localHashMap.put("Regex.matchCountWithLines", new Regex_matchCountWithLines());
		localHashMap.put("Regex.match", new Regex_match());
		localHashMap.put("Regex.replace", new Regex_replace());
		localHashMap.put("Sound.create", new Sound_create());
		localHashMap.put("Sound.delete", new Sound_delete());
		localHashMap.put("Sound.play", new Sound_play());
		localHashMap.put("StanfordCppLib.getJbeVersion", new StanfordCppLib_getJbeVersion());
		localHashMap.put("StanfordCppLib.setCppVersion", new StanfordCppLib_setCppVersion());
		localHashMap.put("TopCompound.create", new TopCompound_create());
		localHashMap.put("URL.download", new URL_download());
		return localHashMap;
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

	public boolean nextBoolean(TokenScanner paramTokenScanner) {
		return paramTokenScanner.nextToken().startsWith("t");
	}
}

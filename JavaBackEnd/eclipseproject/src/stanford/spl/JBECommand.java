package stanford.spl;

import acm.util.TokenScanner;

import java.util.HashMap;

public abstract class JBECommand {
	public abstract void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd);

	public static HashMap<String, JBECommand> createCommandTable() {
		HashMap<String, JBECommand> localHashMap = new HashMap<String, JBECommand>();
		localHashMap.put("File.openFileDialog", new File_openFileDialog());
		localHashMap.put("G3DRect.create", new G3DRect_create());
		localHashMap.put("G3DRect.setRaised", new G3DRect_setRaised());
		localHashMap.put("GArc.create", new GArc_create());
		localHashMap.put("GArc.setFrameRectangle", new GArc_setFrameRectangle());
		localHashMap.put("GArc.setStartAngle", new GArc_setStartAngle());
		localHashMap.put("GArc.setSweepAngle", new GArc_setSweepAngle());
		localHashMap.put("GButton.create", new GButton_create());
		localHashMap.put("GCheckBox.create", new GCheckBox_create());
		localHashMap.put("GCheckBox.isSelected", new GCheckBox_isSelected());
		localHashMap.put("GCheckBox.setSelected", new GCheckBox_setSelected());
		localHashMap.put("GCompound.add", new GCompound_add());
		localHashMap.put("GCompound.create", new GCompound_create());
		localHashMap.put("GEvent.getNextEvent", new GEvent_getNextEvent());
		localHashMap.put("GEvent.waitForEvent", new GEvent_waitForEvent());
		localHashMap.put("GImage.create", new GImage_create());
		localHashMap.put("GInteractor.setActionCommand", new GInteractor_setActionCommand());
		localHashMap.put("GInteractor.isEnabled", new GInteractor_isEnabled());
		localHashMap.put("GInteractor.setEnabled", new GInteractor_setEnabled());
		localHashMap.put("GInteractor.getSize", new GInteractor_getSize());
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
		localHashMap.put("GObject.setColor", new GObject_setColor());
		localHashMap.put("GObject.setFillColor", new GObject_setFillColor());
		localHashMap.put("GObject.setFilled", new GObject_setFilled());
		localHashMap.put("GObject.setLineWidth", new GObject_setLineWidth());
		localHashMap.put("GObject.setLocation", new GObject_setLocation());
		localHashMap.put("GObject.setSize", new GObject_setSize());
		localHashMap.put("GObject.setVisible", new GObject_setVisible());
		localHashMap.put("GObject.setAntialiasing", new GObject_setAntialiasing());
		localHashMap.put("GOval.create", new GOval_create());
		localHashMap.put("GPolygon.addVertex", new GPolygon_addVertex());
		localHashMap.put("GPolygon.create", new GPolygon_create());
		localHashMap.put("GRect.create", new GRect_create());
		localHashMap.put("GRoundRect.create", new GRoundRect_create());
		localHashMap.put("GSlider.create", new GSlider_create());
		localHashMap.put("GSlider.getValue", new GSlider_getValue());
		localHashMap.put("GSlider.setValue", new GSlider_setValue());
		localHashMap.put("GTextField.create", new GTextField_create());
		localHashMap.put("GTextField.getText", new GTextField_getText());
		localHashMap.put("GTextField.setText", new GTextField_setText());
		localHashMap.put("GChooser.create", new GChooser_create());
		localHashMap.put("GChooser.addItem", new GChooser_addItem());
		localHashMap.put("GChooser.getSelectedItem", new GChooser_getSelectedItem());
		localHashMap.put("GChooser.setSelectedItem", new GChooser_setSelectedItem());
		localHashMap.put("GTimer.create", new GTimer_create());
		localHashMap.put("GTimer.deleteTimer", new GTimer_deleteTimer());
		localHashMap.put("GTimer.pause", new GTimer_pause());
		localHashMap.put("GTimer.startTimer", new GTimer_startTimer());
		localHashMap.put("GTimer.stopTimer", new GTimer_stopTimer());
		localHashMap.put("GWindow.addToRegion", new GWindow_addToRegion());
		localHashMap.put("GWindow.setRegionAlignment", new GWindow_setRegionAlignment());
		localHashMap.put("GWindow.clear", new GWindow_clear());
		localHashMap.put("GWindow.close", new GWindow_close());
		localHashMap.put("GWindow.create", new GWindow_create());
		localHashMap.put("GWindow.delete", new GWindow_delete());
		localHashMap.put("GWindow.draw", new GWindow_draw());
		localHashMap.put("GWindow.drawInBackground", new GWindow_drawInBackground());
		localHashMap.put("GWindow.exitGraphics", new GWindow_exitGraphics());
		localHashMap.put("GWindow.getScreenHeight", new GWindow_getScreenHeight());
		localHashMap.put("GWindow.getScreenWidth", new GWindow_getScreenWidth());
		localHashMap.put("GWindow.repaint", new GWindow_repaint());
		localHashMap.put("GWindow.requestFocus", new GWindow_requestFocus());
		localHashMap.put("GWindow.setResizable", new GWindow_setResizable());
		localHashMap.put("GWindow.setTitle", new GWindow_setTitle());
		localHashMap.put("GWindow.setVisible", new GWindow_setVisible());
		localHashMap.put("GWindow.setLocation", new GWindow_setLocation());
		localHashMap.put("GWindow.setExitOnClose", new GWindow_setExitOnClose());
		localHashMap.put("TopCompound.create", new TopCompound_create());
		localHashMap.put("JBEConsole.clear", new JBEConsole_clear());
		localHashMap.put("JBEConsole.getLine", new JBEConsole_getLine());
		localHashMap.put("JBEConsole.print", new JBEConsole_print());
		localHashMap.put("JBEConsole.println", new JBEConsole_println());
		localHashMap.put("JBEConsole.setFont", new JBEConsole_setFont());
		localHashMap.put("JBEConsole.setSize", new JBEConsole_setSize());
		localHashMap.put("JBEConsole.setLocation", new JBEConsole_setLocation());
		localHashMap.put("JBEConsole.setExitOnClose", new JBEConsole_setExitOnClose());
		localHashMap.put("Sound.create", new Sound_create());
		localHashMap.put("Sound.delete", new Sound_delete());
		localHashMap.put("Sound.play", new Sound_play());
		localHashMap.put("Regex.match", new Regex_match());
		localHashMap.put("Regex.matchCount", new Regex_matchCount());
		localHashMap.put("Regex.matchCountWithLines", new Regex_matchCountWithLines());
		localHashMap.put("Regex.replace", new Regex_replace());
		localHashMap.put("StanfordCppLib.getJbeVersion", new StanfordCppLib_getJbeVersion());
		localHashMap.put("StanfordCppLib.setCppVersion", new StanfordCppLib_setCppVersion());
		return localHashMap;
	}

	public int nextInt(TokenScanner paramTokenScanner) {
		return Integer.parseInt(paramTokenScanner.nextToken());
	}

	public double nextDouble(TokenScanner paramTokenScanner) {
		String str = paramTokenScanner.nextToken();
		if (str.equals("-")) {
			str = str + paramTokenScanner.nextToken();
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

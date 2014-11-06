/*
 * @author Marty Stepp
 * @version 2014/11/04
 * 
 * icons for buttons to match JOptionPane:
 * http://www.java2s.com/Tutorial/Java/0240__Swing/CustomizingaJOptionPaneLookandFeel.htm
 * http://docs.oracle.com/javase/7/docs/api/javax/swing/plaf/synth/doc-files/componentProperties.html
 */

package stanford.spl;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

import stanford.cs106.gui.GuiUtils;
import stanford.cs106.gui.WindowCloseKeyListener;
import stanford.cs106.util.StringUtils;

public class TextFileDialog {
	public static final int DEFAULT_ROWS    = 20;
	public static final int DEFAULT_COLUMNS = 80;
	
	public static final int MIN_ROWS    = 5;
	public static final int MIN_COLUMNS = 40;
	
	public static final int MAX_ROWS    = 50;
	public static final int MAX_COLUMNS = 100;
	
	private TextFileDialog() {
		// empty
	}
	
	public static void showDialog(Window parent, String title, String text) {
		showDialog(parent, title, text, DEFAULT_ROWS, DEFAULT_COLUMNS);
	}
	
	public static void showDialog(Window parent, String title, String text, int rows, int cols) {
		if (rows <= 0) {
			rows = DEFAULT_ROWS;
		}
		if (cols <= 0) {
			cols = DEFAULT_COLUMNS;
		}
		
		int height = StringUtils.getHeight(text);
		int width = StringUtils.getWidth(text);
		
		height = Math.max(MIN_ROWS, Math.min(MAX_ROWS, height));
		width  = Math.max(MIN_COLUMNS, Math.min(MAX_COLUMNS, width));
		
		final JDialog dialog = new JDialog(parent);
		dialog.setTitle(title);
		dialog.setModal(true);
		dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		
		JTextArea textArea = new JTextArea(text, height, width);
		textArea.setEditable(false);
		Font oldFont = textArea.getFont();
		textArea.setFont(new Font("Monospaced", Font.PLAIN, oldFont.getSize()));
		
		JScrollPane center = new JScrollPane(textArea);
		dialog.add(center, BorderLayout.CENTER);
		
		JPanel south = new JPanel();
		JButton ok = new JButton("OK");
		ok.setIcon(GuiUtils.extractOptionPaneIcon("OK"));
		ok.setMnemonic('O');
		// growFont(ok, 2);
		// widen(ok, 50);
		
		ok.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				dialog.setVisible(false);
				dialog.dispose();
			}
		});
		
		south.add(ok);
		dialog.add(south, BorderLayout.SOUTH);
		
		new WindowCloseKeyListener(dialog);
		dialog.pack();
		
		SPLWindowSettings.loadWindowLocation(dialog);
		SPLWindowSettings.saveWindowLocation(dialog);
		
		ok.requestFocus();
		dialog.setVisible(true);
	}
	
	static void growFont(JComponent button) {
		growFont(button, 1);
	}
	
	static void growFont(JComponent button, int amount) {
		Font font = button.getFont();
		font = font.deriveFont((float) (font.getSize() + amount));
		button.setFont(font);
	}
	
	static void widen(JComponent component, int px) {
		Dimension size = component.getPreferredSize();
		size.width += px;
		component.setPreferredSize(size);
	}
}

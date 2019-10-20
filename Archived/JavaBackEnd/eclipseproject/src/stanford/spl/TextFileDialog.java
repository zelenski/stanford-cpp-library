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

public class TextFileDialog extends JDialog {
	public static final int DEFAULT_ROWS    = 20;
	public static final int DEFAULT_COLUMNS = 80;
	
	public static final int MIN_ROWS    = 5;
	public static final int MIN_COLUMNS = 40;
	
	public static final int MAX_ROWS    = 50;
	public static final int MAX_COLUMNS = 100;
	
	private JTextArea textarea;
	private JScrollPane scrollPane;
	
	public TextFileDialog(Window parent) {
		super(parent);
	}
	
	public void addText(String text) {
		textarea.setText(textarea.getText() + text);
		JScrollBar vertical = scrollPane.getVerticalScrollBar();
		vertical.setValue(vertical.getMaximum());
	}
	
	public void clearText() {
		textarea.setText("");
	}
	
	public void print(String text) {
		addText(text);
	}
	
	public void println(String text) {
		addText(text + "\n");
	}
	
	public void println(Object o) {
		println(String.valueOf(o));
	}
	
	public static TextFileDialog showDialog(Component parent, String title) {
		return showDialog(parent, title, /* text */ "");
	}
	
	public static TextFileDialog showDialog(Component parent, String title, boolean modal) {
		return showDialog(parent, title, /* text */ "", DEFAULT_ROWS, DEFAULT_COLUMNS, modal);
	}
	
	public static TextFileDialog showDialog(Component parent, String title, String text) {
		return showDialog(parent, title, text, DEFAULT_ROWS, DEFAULT_COLUMNS, /* modal */ true);
	}
	
	public static TextFileDialog showDialog(Component parent, String title, String text, boolean modal) {
		return showDialog(parent, title, text, DEFAULT_ROWS, DEFAULT_COLUMNS, modal);
	}
	
	public static TextFileDialog showDialog(Component parent, String title, String text, int rows, int cols) {
		return showDialog(parent, title, text, rows, cols, /* modal */ true);
	}
	
	public static TextFileDialog showDialog(Component parent, String title, String text, int rows, int cols, boolean modal) {
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
		
		final TextFileDialog dialog = new TextFileDialog((parent instanceof Window) ? (Window) parent : null);
		dialog.setTitle(title);
		dialog.setModal(modal);
		dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		
		dialog.textarea = new JTextArea(text, height, width);
		dialog.textarea.setEditable(false);
		Font oldFont = dialog.textarea.getFont();
		dialog.textarea.setFont(new Font("Monospaced", Font.PLAIN, oldFont.getSize()));
		
		dialog.scrollPane = new JScrollPane(dialog.textarea);
		dialog.add(dialog.scrollPane, BorderLayout.CENTER);
		
		JPanel south = new JPanel();
		JButton ok = new JButton("OK");
		ok.setIcon(GuiUtils.extractOptionPaneIcon("OK"));
		ok.setMnemonic('O');
		// GuiUtils.growFont(ok, 2);
		// GuiUtils.widen(ok, 50);
		
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
		
		return dialog;
	}
}

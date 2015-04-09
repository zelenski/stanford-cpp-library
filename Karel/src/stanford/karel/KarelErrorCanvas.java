package stanford.karel;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

class KarelErrorCanvas extends JPanel {

	private JLabel errorLabel;
	private JTextArea stackTraceArea;
	
	public KarelErrorCanvas(KarelErrorDialog dialog) {
		setLayout(new BorderLayout(10, 10));
		setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		setOpaque(false);
		
		errorLabel = new JLabel("");
		Font oldFont = errorLabel.getFont();
		errorLabel.setFont(oldFont.deriveFont(Font.BOLD, oldFont.getSize() + 2));
		errorLabel.addKeyListener(dialog);
		
		stackTraceArea = new JTextArea(8, 50);
		stackTraceArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
		stackTraceArea.setBorder(null);
		stackTraceArea.setEditable(false);
		// stackTraceArea.setFocusable(false);
		stackTraceArea.setBackground(KarelErrorDialog.DIALOG_BGCOLOR);
		stackTraceArea.addKeyListener(dialog);
		
		// layout
		add(errorLabel, BorderLayout.NORTH);
		JScrollPane scroll = new JScrollPane(stackTraceArea);
		scroll.setBorder(null);
		scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		add(scroll, BorderLayout.CENTER);
		setPreferredSize(new Dimension(getPreferredSize().width + 5, getPreferredSize().height + 20));
	}

	public void setText(String msg) {
		errorLabel.setText("Error: " + msg);
	}
	
	public void setStackTrace(String stackTrace) {
		String[] lines = stackTrace.split("\n");
		String stackTraceOut = "";
		for (String line : lines) {
			// skip some lines
			if (line.contains("at stanford.")) {
				continue;
			}
			if (line.contains("at acm.")) {
				continue;
			}
			line = line.replaceAll("\\([A-Za-z0-9_$]+.java:([0-9]+)\\)", "(), line $1");
			line = line.replaceAll("[ \t]+at ", "");
			if (line.contains("Exception: ")) {
				continue;
			}
			stackTraceOut += line + "\n";
		}
		
		if (!stackTraceOut.isEmpty()) {
			stackTraceOut = stackTraceOut.trim();
		}
		stackTraceArea.setText(stackTraceOut);
	}
}

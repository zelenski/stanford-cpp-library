/*
 * @version 2018/07/08
 * - bug fix for split pane divider location (wasn't waiting for window to show)
 * @version 2018/01/23
 * - modified diff panes to use same font as JBE console if present
 */

package stanford.cs106.diff;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

import stanford.cs106.gui.*;
import stanford.cs106.util.*;
import stanford.spl.*;

public class DiffGui implements ActionListener, AdjustmentListener {
	public static boolean SIDE_BY_SIDE_ENABLED = false;
	private static final int SPLIT = JSplitPane.VERTICAL_SPLIT;
	private static final int MAX_WIDTH = 1000;
	private static final int MAX_HEIGHT = 700;
	public static final String EXPECTED_COLOR = "#00aa00";
	public static final String STUDENT_COLOR = "#aa0000";
	public static final String LINE_NUMBER_BG_COLOR = "#ffffff";
	public static final String LINE_NUMBER_FG_COLOR = "#777777";

//	public static final int IGNORE_LEADING      = 0x1;
//	public static final int IGNORE_TRAILING     = 0x2;
//	public static final int IGNORE_WHITESPACE   = 0x4;
//	public static final int IGNORE_BLANKLINES   = 0x8;
//	public static final int IGNORE_CASE         = 0x10;
//	public static final int IGNORE_NUMBERS      = 0x20;
//	public static final int IGNORE_NONNUMBERS   = 0x40;
//	public static final int IGNORE_AFTERDECIMAL = 0x80;
//	public static final int IGNORE_CHARORDER    = 0x100;
//	public static final int IGNORE_LINEORDER    = 0x200;
//	public static final int IGNORE_EVERYTHING   = 0x1000;
	
	private String s1;
	private String s2;
	private JFrame frame;
	private JTextPane diffsArea;
	private JTextPane sbsDiffsArea;
	private JScrollPane scroll1;
	private JScrollPane scroll2;
	private int oldScrollValue1 = 0;
	private int oldScrollValue2 = 0;
	private JCheckBox syncBox;
	private JCheckBox ignoreLeading;
	private JCheckBox ignoreTrailing;
	private JCheckBox ignoreWhitespace;
	private JCheckBox ignoreBlankLines;
	private JCheckBox ignoreCase;
	private JCheckBox ignorePunctuation;
	private JCheckBox ignoreNumbers;
	private JCheckBox ignoreNonNumbers;
	
	public DiffGui(String s1, String s2) {
		this(null, s1, null, s2);
	}
	
	public DiffGui(String name1, String s1, String name2, String s2) {
		this(name1, s1, name2, s2, Diff.FLAGS_DEFAULT_LENIENT, /* checkboxes */ true);
	}
	
	public DiffGui(String name1, String s1, String name2, String s2, boolean checkboxes) {
		this(name1, s1, name2, s2, Diff.FLAGS_DEFAULT_LENIENT, checkboxes);
	}
	
	public DiffGui(String name1, String s1, String name2, String s2, int flags) {
		this(name1, s1, name2, s2, flags, /* checkboxes */ true);
	}
	
	public DiffGui(String name1, String s1, String name2, String s2, int flags, boolean checkboxes) {
		this.s1 = s1;
		this.s2 = s2;
		
		JTextPane area1 = makeHtmlPane(colorOutput(StringUtils.numberLines(s1)));
		JTextPane area2 = makeHtmlPane(colorOutput(StringUtils.numberLines(s2)));
		diffsArea = makeHtmlPane(colorDiffs(Diff.diff(s1, s2, flags)));
		sbsDiffsArea = makeHtmlPane(colorSideBySideDiffs(Diff.diffSideBySide(s1, s2, StringUtils.getWidth(s1))));
		
		syncBox = new JCheckBox("Sync scrolling", true);
		syncBox.setHorizontalAlignment(JCheckBox.CENTER);
		syncBox.setMnemonic('S');
		
		ignoreLeading = new JCheckBox("Start", (flags & Diff.IGNORE_LEADING) != 0);
		ignoreTrailing = new JCheckBox("End", (flags & Diff.IGNORE_TRAILING) != 0);
		ignoreWhitespace = new JCheckBox("Spaces", (flags & Diff.IGNORE_WHITESPACE) != 0);
		ignoreBlankLines = new JCheckBox("Blank", (flags & Diff.IGNORE_BLANK_LINES) != 0);
		ignoreCase = new JCheckBox("CaSe", (flags & Diff.IGNORE_CASE) != 0);
		ignorePunctuation = new JCheckBox("Punct", (flags & Diff.IGNORE_PUNCTUATION) != 0);
		ignoreNumbers = new JCheckBox("[0-9]", (flags & Diff.IGNORE_NUMBERS) != 0);
		ignoreNonNumbers = new JCheckBox("![0-9]", (flags & Diff.IGNORE_NONNUMBERS) != 0);
		
		// do layout
		Container center = new JPanel(new GridLayout(1, 2));
		scroll1 = new JScrollPane(area1);
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		GuiUtils.ensureMinimumPreferredSize(scroll1, (int) (screen.width * .45), (int) (screen.height * .7));
		scroll1.getVerticalScrollBar().addAdjustmentListener(this);
		scroll2 = new JScrollPane(area2);
		scroll2.setPreferredSize(scroll1.getPreferredSize());
		scroll2.getVerticalScrollBar().addAdjustmentListener(this);
		center.add(scroll1);
		center.add(scroll2);
		
		JTabbedPane diffsSouth = new JTabbedPane();
		String tab1text = "Diffs";
		String tab2text = "Side-by-side";
		if (checkboxes) {
			tab1text += " (use checkboxes below)";
			tab2text += " (checkboxes have no effect)";
		}
		diffsSouth.addTab(tab1text, new JScrollPane(diffsArea));
		if (SIDE_BY_SIDE_ENABLED) {
			diffsSouth.addTab(tab2text, new JScrollPane(sbsDiffsArea));
		}
		GuiUtils.setPreferredHeight(diffsSouth, 100);
		
		Container buttonPane = null;
		if (checkboxes) {
			buttonPane = GuiUtils.createPanel(
					new FlowLayout(FlowLayout.CENTER),
					new JLabel("Ignore: "),
					ignoreLeading,
					ignoreTrailing,
					ignoreWhitespace,
					ignoreBlankLines,
					ignoreCase,
					ignorePunctuation,
					ignoreNumbers,
					ignoreNonNumbers,
					GuiUtils.createButton("Update", 'U', this),
					Box.createHorizontalStrut(20),
					GuiUtils.createButton("Close", 'C', this)
			);
		} else {
			buttonPane = GuiUtils.createPanel(
					new FlowLayout(FlowLayout.CENTER),
					GuiUtils.createButton("Close", 'C', this)
			);
		}
		
		Container overall = new JPanel(new BorderLayout());
		final JSplitPane splitPane = new JSplitPane(SPLIT, center, diffsSouth);
		overall.add(splitPane, BorderLayout.CENTER);
		overall.add(buttonPane, BorderLayout.SOUTH);
		
		Container north = new JPanel(new GridLayout(1, 3));
		if (name1 == null) {
			north.add(new JLabel());
		} else {
			String htmlLabelText = GuiUtils.htmlLabelText(name1, CollectionUtils.asMap(
					"font-color", EXPECTED_COLOR
			));
			JLabel label1 = new JLabel(htmlLabelText);
			label1.setHorizontalAlignment(JLabel.LEFT);
			north.add(label1);
		}
		north.add(syncBox);
		if (name2 == null) {
			north.add(new JLabel());
		} else {
			String htmlLabelText = GuiUtils.htmlLabelText(name1, CollectionUtils.asMap(
					"font-color", STUDENT_COLOR
			));
			JLabel label2 = new JLabel(htmlLabelText);
			label2.setHorizontalAlignment(JLabel.RIGHT);
			north.add(label2);
		}
		overall.add(north, BorderLayout.NORTH);
		
		frame = new JFrame();
		frame.setTitle("Diff");
		frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		frame.setContentPane(overall);
		WindowCloseKeyListener.add(frame);
		frame.pack();
		if (frame.getWidth() > MAX_WIDTH) {
			frame.setSize(MAX_WIDTH, frame.getHeight());
		}
		if (frame.getHeight() > MAX_HEIGHT) {
			frame.setSize(frame.getWidth(), MAX_HEIGHT);
		}
		// splitPane.setDividerLocation(0.6);   // 60/40 split
		GuiUtils.centerWindow(frame);
		
		// set split pane's split divider location;
		// doesn't really work unless you give a delay after show()
		new Thread(new Runnable() {
			public void run() {
				// wait for up to 5 sec
				long ms = 0;
				while (ms < 5000 && splitPane.getHeight() <= 1) {
					try {
						Thread.sleep(50);
						ms += 50;
					} catch (InterruptedException ie) {
						// empty
					}
				}
				splitPane.setDividerLocation(0.6);   // 60/40 split top/bottom
			}
		}).start();
	}
	
	public void adjustmentValueChanged(AdjustmentEvent event) {
		int value = event.getValue();
		JScrollBar bar1 = scroll1.getVerticalScrollBar();
		JScrollBar bar2 = scroll2.getVerticalScrollBar();
		if (syncBox.isSelected()) {
			synchronized (bar1) {
				bar1.removeAdjustmentListener(this);
				bar2.removeAdjustmentListener(this);
				if (event.getSource() == bar1) {
					int diff = value - oldScrollValue1;
					bar2.setValue(bar2.getValue() + diff);
				} else {
					// came from scroll2's scrollbar
					int diff = value - oldScrollValue2;
					bar1.setValue(bar1.getValue() + diff);
				}
				bar1.addAdjustmentListener(this);
				bar2.addAdjustmentListener(this);
			}
		}
		oldScrollValue1 = bar1.getValue();
		oldScrollValue2 = bar2.getValue();
	}

	public void actionPerformed(ActionEvent event) {
		String cmd = event.getActionCommand().intern();
		if (cmd == "Update") {
			int newFlags = 0;
			if (ignoreLeading.isSelected()) {
				newFlags |= Diff.IGNORE_LEADING;
			}
			if (ignoreTrailing.isSelected()) {
				newFlags |= Diff.IGNORE_TRAILING;
			}
			if (ignoreWhitespace.isSelected()) {
				newFlags |= Diff.IGNORE_WHITESPACE;
			}
			if (ignoreBlankLines.isSelected()) {
				newFlags |= Diff.IGNORE_BLANK_LINES;
			}
			if (ignoreCase.isSelected()) {
				newFlags |= Diff.IGNORE_CASE;
			}
			if (ignorePunctuation.isSelected()) {
				newFlags |= Diff.IGNORE_PUNCTUATION;
			}
			if (ignoreNumbers.isSelected()) {
				newFlags |= Diff.IGNORE_NUMBERS;
			}
			if (ignoreNonNumbers.isSelected()) {
				newFlags |= Diff.IGNORE_NONNUMBERS;
			}
			diffsArea.setText(colorDiffs(Diff.diff(s1, s2, newFlags)));
			diffsArea.setCaretPosition(0);
		}
		if (cmd == "Close") {
			frame.setVisible(false);
			frame.dispose();
		}
	}
	
	public void show() {
		frame.setVisible(true);
	}
	
	private String htmlBodyWrap(String text) {
		Font monospacedFont = JavaBackEnd.getConsoleFont();
		if (monospacedFont == null) {
			Font defaultLabelFont = new JLabel().getFont();
			monospacedFont = new Font("monospaced", Font.PLAIN, defaultLabelFont.getSize());
		}
		return GuiUtils.htmlLabelText(text, CollectionUtils.asMap(
				"font", monospacedFont
		));
	}
	
	private String colorOutput(String output) {
		String[] lines = output.split("\r?\n");
		for (int i = 0; i < lines.length; i++) {
			lines[i] = lines[i].replaceAll("[&]", "&amp;");
			lines[i] = lines[i].replaceAll("[ ]", "&nbsp;");
			lines[i] = lines[i].replaceAll("[<]", "&lt;");
			lines[i] = lines[i].replaceAll("[>]", "&gt;");
			lines[i] = lines[i].replaceAll("^([0-9]{1,4}[|])", "<font color='" + LINE_NUMBER_FG_COLOR + "' bgcolor='" + LINE_NUMBER_BG_COLOR + "'>$1</font>");
		}
		return htmlBodyWrap(StringUtils.join(lines, "<br>"));
	}
	
	private String colorDiffs(String diffs) {
		diffs = StringUtils.htmlEncode(diffs, /* replaceSpecialChars */ true, /* nbsp */ true);
		diffs = diffs.replaceAll("(EXPECTED&nbsp;&lt;.{0,999})", "<font color='" + EXPECTED_COLOR + "'>$1</font>");
		diffs = diffs.replaceAll("(STUDENT&nbsp;&nbsp;&gt;.{0,999})", "<font color='" + STUDENT_COLOR + "'>$1</font>");
		return htmlBodyWrap(diffs);
	}
	
	private String colorSideBySideDiffs(String diffs) {
		diffs = StringUtils.htmlEncode(diffs, /* replaceSpecialChars */ true, /* nbsp */ true);
		return htmlBodyWrap(diffs);
	}

	private JTextPane makeHtmlPane(String htmlText) {
		JTextPane pane = new JTextPane();
		
		Font font = pane.getFont();
		font = new Font(Font.MONOSPACED, Font.PLAIN, font.getSize());

		// BUGFIX: set diff font size to match console font size
		Font consoleFont = JavaBackEnd.getConsoleFont();
		if (consoleFont != null) {
			font = consoleFont;
		}
		
		pane.setFont(font);
		pane.setContentType("text/html");
		pane.setText(htmlText);
		pane.setCaretPosition(0);
		pane.setEditable(false);
		return pane;
	}
	
	public void setDiffCheckboxes(boolean value) {
		
	}
}

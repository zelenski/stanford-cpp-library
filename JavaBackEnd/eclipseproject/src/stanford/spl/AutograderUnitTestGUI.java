/*
 * @author Marty Stepp
 * @version 2017/05/02
 * - fixed minor bugs with test output html/coloring, diff flags missing
 * @version 2016/12/01
 * - fixed bugs with badly displayed / wrong test results
 * - clarified assertion output for ASSERT_TRUE and _FALSE
 * - checkbox to catch exceptions in tests
 * @version 2016/10/30
 * - made window close when you press Escape
 * - added ignore-punctuation diff flag
 * @version 2016/10/22
 * - added support for diff flags
 * @version 2016/10/12
 * - fixed bug with double-HTML-encoding diff expected/actual output
 * @version 2016/09/22
 * - fixed bugs with forgetting to HTML-encode test results message/expected/actual
 * @version 2016/04/28
 * - max window size so a program with many tests doesn't fall off screen
 * @version 2015/05/28
 * - slight tweaks for ASSERT_NULL and ASSERT_NOT_NULL test failure display
 * @version 2015/05/02
 * - added JUnitListener functionality; use internal image icons from JAR
 * @version 2015/04/21
 * - made it work even if Java back-end is null
 * @version 2014/11/15
 * - spinner GIF while tests are in progress
 * - shut down JBE if window closed while tests are in progress
 */

package stanford.spl;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;
import acm.graphics.GImage;
import stanford.cs106.diff.*;
import stanford.cs106.gui.*;
import stanford.cs106.io.*;
import stanford.cs106.junit.*;
import stanford.cs106.util.*;

public class AutograderUnitTestGUI extends Observable
		implements ActionListener, JUnitListener, MouseListener {
	private static final int DIALOG_WIDTH = 500;   // px
	private static final Color ZEBRA_STRIPE_COLOR_1 = new Color(250, 250, 250);
	private static final Color ZEBRA_STRIPE_COLOR_2 = new Color(235, 235, 235);
	
	// should possibly keep in sync with colors in DiffGui.java
	private static final Color PASS_COLOR = new Color(0, 96, 0);
	private static final String PASS_COLOR_HEX = GImage.colorName(PASS_COLOR.getRGB());
	private static final Color FAIL_COLOR = new Color(96, 0, 0);
	private static final String FAIL_COLOR_HEX = GImage.colorName(FAIL_COLOR.getRGB());
	private static final Color WARN_COLOR = new Color(112, 112, 0);
	private static final int MAX_VALUE_DISPLAY_LENGTH = 120;
	private static final int MAX_WINDOW_WIDTH = 1000;   // px
	private static final int MAX_WINDOW_HEIGHT = 700;   // px
	private static Color NORMAL_COLOR = null;
	
	//private static final int MIN_WIDTH = 75;
	private static AutograderUnitTestGUI instance;             // singleton
	private static AutograderUnitTestGUI styleCheckInstance;   // singleton
	
	private static final String TESTS_TITLE = "Autograder Tests";
	private static final String STYLE_CHECK_TITLE = "Style Checker";
	private static final String ICONS_FOLDER = "res/icons/";
	
	public static synchronized AutograderUnitTestGUI getInstance(JavaBackEnd javaBackEnd) {
		if (instance == null) {
			instance = new AutograderUnitTestGUI(javaBackEnd, TESTS_TITLE);
		}
		return instance;
	}
	
	public static synchronized AutograderUnitTestGUI getInstance(JavaBackEnd javaBackEnd, boolean isStyleCheck) {
		if (isStyleCheck) {
			return getStyleCheckInstance(javaBackEnd);
		} else {
			return getInstance(javaBackEnd);
		}
	}
	
	public static synchronized AutograderUnitTestGUI getStyleCheckInstance(JavaBackEnd javaBackEnd) {
		if (styleCheckInstance == null) {
			styleCheckInstance = new AutograderUnitTestGUI(javaBackEnd, STYLE_CHECK_TITLE);
		}
		return styleCheckInstance;
	}
	
	private class TestInfo {
		private String name;
		private String category;
		private JCheckBox checked;
		private JLabel description;
		private JLabel result;
		private Map<String, String> details = new LinkedHashMap<String, String>();
		
		public String fullName() {
			return (category == null || category.isEmpty()) ? name : (category + "_" + name);
		}
	}
	
	private JavaBackEnd javaBackEnd;
	private JFrame frame = null;
	private JScrollPane scroll = null;
	private JCheckBox catchErrorsBox = null;
	private Box contentPaneBox = null;
	private JLabel descriptionLabel = null;
	private JLabel southLabel = null;
	private AutograderUnitTestGUIMouseAdapter mouseListener;
	
	private Map<String, Container> allCategories = new LinkedHashMap<String, Container>();
	private Container currentCategory = null;
	private Map<Object, TestInfo> allTests = new LinkedHashMap<Object, TestInfo>();
	
	private int passCount = 0;
	private int testCount = 0;
	private boolean testingIsInProgress = false;
	private int testPanelHeight = -1;
	private boolean allCategoriesHidden = false;
	private boolean checkboxesShown = true;
	
	public AutograderUnitTestGUI(JavaBackEnd javaBackEnd, String title) {
		this.javaBackEnd = javaBackEnd;
		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
		frame.setTitle(title);
		frame.setVisible(false);
		frame.addWindowListener(new AutograderUnitTestGUIWindowAdapter());
		WindowCloseKeyListener.add(frame);
		
		descriptionLabel = new JLabel(TESTS_TITLE);
		if (NORMAL_COLOR == null) {
			NORMAL_COLOR= descriptionLabel.getForeground();
		}
		descriptionLabel.setHorizontalAlignment(JLabel.CENTER);
		descriptionLabel.setAlignmentX(0.5f);
		GuiUtils.shrinkFont(descriptionLabel);
		
		Box north = Box.createVerticalBox();
		north.setAlignmentX(0.0f);
		north.setBorder(BorderFactory.createEmptyBorder(/* TLBR */ 2, 5, 2, 2));
		// north.add(descriptionLabel);
		catchErrorsBox = new JCheckBox("Catch exceptions in tests", /* selected */ true);
		catchErrorsBox.setMnemonic('C');
		north.add(catchErrorsBox);
		frame.add(north, BorderLayout.NORTH);
		
		contentPaneBox = Box.createVerticalBox();
		scroll = new JScrollPane(contentPaneBox);
		scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		scroll.getVerticalScrollBar().setUnitIncrement(32);
		contentPaneBox.add(Box.createGlue());   // to eat up excess vertical space
		frame.add(scroll, BorderLayout.CENTER);
		
		southLabel = new JLabel(" ");
		southLabel.setIcon(new ImageIcon(ResourceUtils.filenameToURL(ICONS_FOLDER + "progress.gif")));
		southLabel.setHorizontalTextPosition(SwingConstants.LEFT);
		southLabel.setHorizontalAlignment(JLabel.CENTER);
		southLabel.setAlignmentX(0.5f);
		frame.add(southLabel, BorderLayout.SOUTH);
		updateSouthText();
		
		new WindowCloseKeyListener(frame);
		mouseListener = new AutograderUnitTestGUIMouseAdapter();
		
		SPLWindowSettings.loadWindowLocation(frame);
		SPLWindowSettings.saveWindowLocation(frame);
	}
	
	public void actionPerformed(ActionEvent event) {
		showTestDetails(event.getActionCommand());
	}
	
	public Container addCategory(String name) {
		if (!allCategories.containsKey(name)) {
			final Container category = Box.createVerticalBox();
			currentCategory = category;
			category.setName("category");
			allCategories.put(name, currentCategory);
			if (!name.isEmpty()) {
				if (isStyleCheck()) {
					TitledBorder border = BorderFactory.createTitledBorder(name);
					border.setTitleJustification(SwingConstants.CENTER);
					((Box) currentCategory).setBorder(border);
				} else {
					Border border = BorderFactory.createLineBorder(Color.DARK_GRAY);
					((Box) currentCategory).setBorder(border);
				}
				currentCategory.setBackground(ZEBRA_STRIPE_COLOR_2);
			}
			
			if (!isStyleCheck()) {
				// top row of 'select/deselect all' buttons
				JPanel top = new JPanel(new FlowLayout(FlowLayout.LEFT));
				JButton selectAll = new JButton("All");
				selectAll.setIcon(new ImageIcon(ResourceUtils.filenameToURL(ICONS_FOLDER + "checkbox-checked.gif")));
				GuiUtils.shrinkFont(selectAll, 2);
				selectAll.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent event) {
						selectAll(category, true);
					}
				});
				selectAll.addMouseListener(mouseListener);
				selectAll.setToolTipText("Double-click to select all tests from all categories.");
				
				JButton deselectAll = new JButton("None");
				deselectAll.setIcon(new ImageIcon(ResourceUtils.filenameToURL(ICONS_FOLDER + "checkbox-unchecked.gif")));
				GuiUtils.shrinkFont(deselectAll, 2);
				deselectAll.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent event) {
						selectAll(category, false);
					}
				});
				deselectAll.addMouseListener(mouseListener);
				deselectAll.setToolTipText("Double-click to deselect all tests from all categories.");
				
				JButton minimize = new JButton("Hide");
				// minimize.setIcon(new ImageIcon(ResourceUtils.filenameToURL(ICONS_FOLDER + "minus.gif"));
				GuiUtils.shrinkFont(minimize, 2);
				minimize.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent event) {
						minimize(category);
					}
				});
				minimize.addMouseListener(mouseListener);
				minimize.setToolTipText("Double-click to minimize all categories.");
				
				if (checkboxesShown) {
					top.add(selectAll);
					top.add(deselectAll);
				}
				top.add(minimize);
				
				if (!name.isEmpty()) {
					JLabel nameLabel = new JLabel(name);
					GuiUtils.shrinkFont(nameLabel, 1);
					top.add(nameLabel);
				}
				
				category.add(top);
			}
			
			// add just before last index; last index is occupied by invisible 'glue' component
			// that stretches to eat up excess vertical space
			// (without this glue, the panels awkwardly vertically stretch)
			contentPaneBox.add(currentCategory, contentPaneBox.getComponentCount() - 1);
			
			checkVisibility();
		}
		return allCategories.get(name);
	}
	
	public void addTest(String testName, String categoryName) {
		testCount++;
		currentCategory = addCategory(categoryName);
		
		// add a panel about this test
		JPanel testPanel = new JPanel(new BorderLayout());
		testPanel.setName("testPanel");
		JPanel testWestPanel = new JPanel();
		Color bgColor = (testCount % 2 == 0) ? ZEBRA_STRIPE_COLOR_1 : ZEBRA_STRIPE_COLOR_2;
		testPanel.setBackground(bgColor);
		testWestPanel.setBackground(bgColor);
		
		TestInfo testInfo = new TestInfo();
		testInfo.name = testName;
		testInfo.category = categoryName;
		allTests.put(testInfo.fullName(), testInfo);
		
		testInfo.checked = new JCheckBox();
		
		// testInfo.checked.setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY, 1));
		// testInfo.checked.setPreferredSize(new Dimension(10, 10));
		
		testInfo.checked.setSelected(true);
		if (!isStyleCheck() && checkboxesShown) {
			testWestPanel.add(testInfo.checked);
		}
		
		JLabel testCountLabel = new JLabel(String.format("%3d. ", testCount));
		allTests.put(testCountLabel, testInfo);
		testCountLabel.setHorizontalAlignment(SwingConstants.RIGHT);
		testCountLabel.setAlignmentX(Component.RIGHT_ALIGNMENT);
		Font oldFont = testCountLabel.getFont();
		testCountLabel.setFont(new Font("Monospaced", Font.BOLD, oldFont.getSize()));
		// boldFont(testCountLabel);
		testWestPanel.add(testCountLabel);
		testPanel.add(testWestPanel, BorderLayout.WEST);
		
		String testNameShort = testName.replaceAll("Test_[0-9]{1,5}_", "");
		testNameShort = testNameShort.replaceAll("^Test_", "");
		testInfo.description = new JLabel(testNameShort);
		testInfo.description.setToolTipText("Click to see detailed results from this test.");
		GuiUtils.shrinkFont(testInfo.description);
		testInfo.description.setFont(testInfo.description.getFont().deriveFont(Font.BOLD));
		testPanel.add(testInfo.description, BorderLayout.CENTER);
		testInfo.description.addMouseListener(this);
		allTests.put(testInfo.description, testInfo);
		
		testInfo.result = new JLabel(new ImageIcon(ResourceUtils.filenameToURL(ICONS_FOLDER + "running.gif")));
		testInfo.result.setText("        ");
		GuiUtils.shrinkFont(testInfo.result, 2);
		testInfo.result.setHorizontalTextPosition(SwingConstants.LEFT);
		testInfo.result.setToolTipText("Click to see detailed results from this test.");
		testInfo.result.addMouseListener(this);
		testPanel.add(testInfo.result, BorderLayout.EAST);
		allTests.put(testInfo.result, testInfo);
		
		currentCategory.add(testPanel);

		// BUGFIX: bounding the max height prevents vertical stretching on window resize
		Dimension prefSize = currentCategory.getPreferredSize();
		prefSize.width = Integer.MAX_VALUE;
		currentCategory.setMaximumSize(prefSize);
		
		checkVisibility();
	}
	
	public boolean catchExceptions() {
		return catchErrorsBox.isSelected();
	}
	
	public void clearTests() {
		currentCategory = null;
		allCategories.clear();
		allTests.clear();
		passCount = 0;
		testCount = 0;
		testingIsInProgress = true;
		contentPaneBox.removeAll();
		contentPaneBox.add(Box.createGlue());   // to eat up excess vertical space
		contentPaneBox.validate();
		scroll.validate();
		updateSouthText();
		checkVisibility();
	}
	
	public void clearTestResults() {
		passCount = 0;
		for (TestInfo testInfo : allTests.values()) {
			testInfo.details.clear();
			testInfo.description.setForeground(NORMAL_COLOR);
			testInfo.result.setIcon(new ImageIcon(ResourceUtils.filenameToURL(ICONS_FOLDER + "running.gif")));
			testInfo.result.setText("");
		}
		updateSouthText();
	}
	
	private String getTestResult(String testFullName) {
		TestInfo testInfo = allTests.get(testFullName);
		if (testInfo == null) {
			return "no such test";
		} else if (testInfo.description.getForeground().equals(FAIL_COLOR)) {
			return "fail";
		} else if (testInfo.description.getForeground().equals(WARN_COLOR)) {
			return "warn";
		} else if (testInfo.description.getForeground().equals(PASS_COLOR)) {
			return "pass";
		} else {
			if (testInfo.details.containsKey("passed")) {
				String passed = testInfo.details.get("passed");
				return passed.equalsIgnoreCase("true") ? "pass" : "fail";
			} else {
				return "unknown";
			}
		}
	}
	
	public int getFailedCount() {
		return getTestCount() - getPassedCount();
	}
	
	public int getPassedCount() {
		return passCount;
	}
	
	public int getTestCount() {
		return getCheckedTestCount();
	}
	
	public boolean isChecked(String testFullName) {
		TestInfo testInfo = allTests.get(testFullName);
		if (testInfo == null) {
			return false;
		} else {
			return testInfo.checked.isSelected();
		}
	}
	
	public boolean isEmpty() {
		return allCategories.isEmpty();
	}
	
	public boolean isStyleCheck() {
		return this == styleCheckInstance || frame.getTitle().equals(STYLE_CHECK_TITLE);
	}
	
	public boolean isVisible() {
		return frame != null && frame.isVisible();
	}
	
	public void mouseClicked(MouseEvent event) {
		JLabel label = (JLabel) event.getSource();
		TestInfo testInfo = allTests.get(label);
		String testName = testInfo.fullName();
		if (testName != null) {
			showTestDetails(testName);
		}
	}
	
	public void mouseEntered(MouseEvent event) {
		// empty
	}
	
	public void mouseExited(MouseEvent event) {
		// empty
	}
	
	public void mousePressed(MouseEvent event) {
		// empty
	}
	
	public void mouseReleased(MouseEvent event) {
		// empty
	}
	
	public void setChecked(String testFullName, boolean checked) {
		TestInfo testInfo = allTests.get(testFullName);
		if (testInfo != null) {
			testInfo.checked.setSelected(checked);
		}
	}
	
	public void setDescription(String text) {
		text = text.replaceAll("[ \t\r\n\f]+", " ");
		text = text.replaceAll("Note:", "<b>Note:</b>");
		text = text.replaceAll("Warning:", "<b>Warning:</b>");
		text = text.replaceAll("Error:", "<b>Error:</b>");
		descriptionLabel.setText("<html><body style='width: " + DIALOG_WIDTH + "; max-width: " + DIALOG_WIDTH + "'><center>" + text + "</center></body></html>");
		descriptionLabel.validate();
		checkVisibility();
	}
	
	public void setTestCounts(int passCount, int testCount) {
		this.passCount = passCount;
		this.testCount = testCount;
		updateSouthText();
	}
	
	// looking for a map with these keys:
	// testType, message, ...
	public void setTestDetails(String testFullName, Map<String, String> details) {
		TestInfo testInfo = allTests.get(testFullName);
		if (testInfo == null) {
			return;
		}

		// BUGFIX: don't replace test details if a test already failed here
		String existingResult = getTestResult(testFullName).intern();
		if (existingResult == "fail" || existingResult == "warn") {
			if (!testInfo.details.isEmpty()) {
				return;
			}
		}
		
		testInfo.details = details;
	}
	
	public void setTestDetailsMessage(String testName, String detailsMessage) {
		Map<String, String> details = new TreeMap<String, String>();
		details.put("testType", "MANUAL");
		details.put("message", detailsMessage);
		setTestDetails(testName, details);
	}
	
	public void setTestingCompleted(boolean completed) {
		testingIsInProgress = !completed;
		updateSouthText();
	}
	
	public boolean setTestResult(String testFullName, String result) {
		result = result.toLowerCase().intern();
		if (result == "error") {
			result = "fail";  // synonyms
		}
		
		TestInfo testInfo = allTests.get(testFullName);
		if (testInfo == null) {
			// test case not found!
			return false;
		}
		
		// BUGFIX: if test already failed previously, don't set back to passed
		String existingResult = getTestResult(testFullName).intern();
		if ((existingResult == "fail" || existingResult == "warn") && result != "fail") {
			return true;
		}
		
		String iconFile = ICONS_FOLDER + result + ".gif";
		testInfo.result.setIcon(new ImageIcon(ResourceUtils.filenameToURL(iconFile)));   // pass, fail, running, warn
		if (result == "pass") {
			passCount++;
			testInfo.description.setForeground(PASS_COLOR);
		} else if (result == "fail") {
			testInfo.description.setForeground(FAIL_COLOR);
		} else if (result == "warn") {
			testInfo.description.setForeground(WARN_COLOR);
		} else if (result == "warn") {
			testInfo.description.setForeground(NORMAL_COLOR);
		}
		updateSouthText();
		return true;
	}
	
	public boolean setTestRuntime(String testFullName, int runtimeMS) {
		TestInfo testInfo = allTests.get(testFullName);
		if (testInfo == null) {
			return false;
		} else {
			String text = " (" + runtimeMS + "ms)";
			testInfo.result.setText(text);
			return true;
		}
	}
	
	public void setCheckboxesShown(boolean shown) {
		checkboxesShown = shown;
	}
	
	public void setVisible(boolean visible) {
		frame.setVisible(visible);
	}
	
	private void checkVisibility() {
		contentPaneBox.revalidate();
		scroll.revalidate();
		// frame.revalidate();
		frame.validate();
		frame.pack();
		int width = Math.min(MAX_WINDOW_WIDTH, frame.getWidth() + 32);
		int height = Math.min(MAX_WINDOW_HEIGHT, frame.getHeight() + 10);
		frame.setSize(width, height);  // a bit of buffer for scrollbar
		
//		if (frame.isVisible() != !isEmpty()) {
//			frame.setVisible(!isEmpty());
//		}
		
		// scroll to bottom as new tests appear
		// scroll.getVerticalScrollBar().setValue(scroll.getVerticalScrollBar().getMaximum());
	}
	
	/*
	 * toggles minimized state
	 */
	private void minimize(Container category) {
		minimize(category, !category.getName().contains("hidden"));
	}
	
	private void minimize(Container category, boolean minimized) {
		if (minimized) {
			category.setName("category_hidden");
		} else {
			category.setName("category");
		}
		for (Component comp : category.getComponents()) {
			if (comp instanceof JPanel) {
				JPanel panel = (JPanel) comp;
				String name = panel.getName();
				if (name == null || !name.equals("testPanel")) {
					continue;
				}
				Dimension size = panel.getPreferredSize();
				if (testPanelHeight < 0) {
					testPanelHeight = size.height;
				}
				
				size.height = minimized ? 0 : testPanelHeight;
				panel.setPreferredSize(size);
				if (minimized) {
					panel.setMaximumSize(size);
				} else {
					panel.setMaximumSize(null);
				}
				panel.invalidate();
				category.invalidate();
			}
		}
		category.validate();
		contentPaneBox.validate();
		scroll.validate();
		frame.validate();
	}
	
	private void selectAll(Container category, boolean selected) {
		for (Component comp : category.getComponents()) {
			if (comp instanceof JCheckBox) {
				JCheckBox checkBox = (JCheckBox) comp;
				checkBox.setSelected(selected);
			} else if (comp instanceof Container) {
				Container container = (Container) comp;
				selectAll(container, selected);
			}
		}
	}
	
	/*
		enum UnitTestType {
		    ASSERT_EQUALS = 0,
		    ASSERT_NOT_EQUALS,
		    ASSERT_NEAR,
		    ASSERT_DIFF,
		    ASSERT_TRUE,
		    ASSERT_FALSE,
		    ASSERT_NOT_NULL,
		    ASSERT_NULL,
		    EXCEPTION,
		    NOT_EXCEPTION,
		    PASS,
		    FAIL,
		};
	 */
	private void showTestDetails(String testFullName) {
		// {testType=TEST_ASSERT_EQUALS,
		//  message="......",
		//  expected=foobarbaz,
		//  student=foobarbaz,
		//  valueType=string,
		//  passed=true}
		TestInfo testInfo = allTests.get(testFullName);
		if (testInfo == null) {
			return;
		}
		Map<String, String> deets = testInfo.details;
		if (deets.isEmpty()) {
			return;
		}
		
		boolean passed = deets.containsKey("passed") && deets.get("passed").equalsIgnoreCase("true");
		String type = deets.containsKey("testType") ? deets.get("testType").toUpperCase().intern() : "";
		String message = StringUtils.htmlEncode(deets.get("message"));
		if (type == "ASSERT_EQUALS") {
			message += " (must be equal)";
		} else if (type == "ASSERT_NOT_EQUALS") {
			message += " (must be non-equal)";
		} else if (type == "ASSERT_NEAR") {
			message += " (must be nearly equal)";
		} else if (type == "ASSERT_DIFF") {
			message += " (program output must match)";
//		} else if (type == "ASSERT_TRUE") {
//			message += " (must be true)";
//		} else if (type == "ASSERT_FALSE") {
//			message += " (must be false)";
//		} else if (type == "ASSERT_NULL") {
//			message += " (must be null)";
//		} else if (type == "ASSERT_NOT_NULL") {
//			message += " (must not be null)";
		} else if (type == "EXCEPTION") {
			// message += " (threw exception)";
		} else if (type == "NOT_EXCEPTION") {
			message += " (didn't throw expected exception)";
		} else if (type == "PASS") {
			message += " (passed)";
		} else if (type == "FAIL") {
			message += " (failed)";
		} else if (type == "STYLE_CHECK") {
			// message += " (style checker warning)";
		}
		
		// simple expected/actual tests (show both as bullets)
		String expected = String.valueOf(deets.get("expected"));
		String student  = String.valueOf(deets.get("student"));
		if (type != "ASSERT_DIFF") {
			// BUGFIX: don't HTML-encode Diff outputs here; they are encoded by DiffGui, so don't double-HTML-encode
			expected = StringUtils.htmlEncode(expected);
			student  = StringUtils.htmlEncode(student);
		}
		
		String valueType = deets.containsKey("valueType") ? deets.get("valueType").toLowerCase().intern() : "";
		if (valueType == "string") {
			expected = "\"" + expected + "\"";
			student  = "\"" + student  + "\"";
		} else if (valueType == "char") {
			expected = "'" + expected + "'";
			student  = "'" + student  + "'";
		}
		
		String stack = "";
		if (deets.containsKey("stackTrace")) {
			stack = StringUtils.htmlEncode(deets.get("stackTrace")).replace("\n", "<br>").replace("\t", "  ");
		}
		
		boolean shouldShowJOptionPane = true;
		if (type == "ASSERT_EQUALS" || type == "ASSERT_NOT_EQUALS"
				|| type == "ASSERT_NEAR" || type == "STYLE_CHECK"
				|| type == "ASSERT_NULL" || type == "ASSERT_NOT_NULL"
				|| type == "ASSERT_TRUE" || type == "ASSERT_FALSE") {
			String htmlMessage = "";
			String expectedTruncated = StringUtils.truncate(expected, MAX_VALUE_DISPLAY_LENGTH, " ...");
			String studentTruncated  = StringUtils.truncate(student, MAX_VALUE_DISPLAY_LENGTH, " ...");
			htmlMessage += "<html><body style='max-width: " + DIALOG_WIDTH + "px;'>";
			htmlMessage += "<p>" + message + "</p>";
			htmlMessage += "<ul>";
			htmlMessage += "<li><font style='font-family: monospaced' color='" + DiffGui.EXPECTED_COLOR + "'>expected:</font> <font style='font-family: monospaced'>" + expectedTruncated + "</font></li>";
			htmlMessage += "<li><font style='font-family: monospaced' color='" + DiffGui.STUDENT_COLOR  + "'>student :</font> <font style='font-family: monospaced'>" + studentTruncated  + "</font></li>";
			htmlMessage += "</ul>";
			
			htmlMessage += "<p>result: "
					+ "<font color='" + (passed ? PASS_COLOR_HEX : FAIL_COLOR_HEX).replace("0x", "#") + "'><b>"
					+ (passed ? "PASS" : "FAIL")
					+ "</b></font></p>";
			
			if (!stack.isEmpty()) {
				htmlMessage += "<div style='margin-top: 5px;'><b>Stack trace:</b></div><pre>" + stack + "</pre>";
			}
			
			htmlMessage += "</body></html>";
			htmlMessage = htmlMessage.replace("\n", "\\n");
			htmlMessage = htmlMessage.replace("\r", "\\r");
			htmlMessage = htmlMessage.replace("\t", "\\t");
			message = htmlMessage;
		} else if (type == "ASSERT_DIFF") {
			shouldShowJOptionPane = false;
			int flags = Diff.FLAGS_DEFAULT_LENIENT;
			if (deets.containsKey("diffFlags")) {
				flags = Integer.parseInt(deets.get("diffFlags"));
			}
			new DiffGui("expected output", expected, "student output", student, flags).show();
		} else if (type == "ASSERT_DIFF_IMAGE") {
			shouldShowJOptionPane = false;
			DiffImage diff = new DiffImage(expected, student);
			diff.setVisible(true);
		} else if (type == "MANUAL") {
			shouldShowJOptionPane = true;
			String htmlMessage = "";
			htmlMessage += "<html><body style='max-width: " + DIALOG_WIDTH + "px;'>";
			htmlMessage += "<pre>" + message + "</pre>";
			htmlMessage += "</body></html>";
			htmlMessage = htmlMessage.replace("\n", "<br>");
			htmlMessage = htmlMessage.replace("\r", "");
			htmlMessage = htmlMessage.replace("\t", "    ");
			message = htmlMessage;
		}
		
		if (shouldShowJOptionPane) {
			JOptionPane.showMessageDialog(
					/* parent */ frame,
					message,
					/* title */ testFullName,
					/* type */ passed ? JOptionPane.INFORMATION_MESSAGE : JOptionPane.ERROR_MESSAGE);
		}
		
		setChanged();
		notifyObservers(testFullName);
	}
	
	private int getCheckedTestCount() {
		int count = 0;
		Set<TestInfo> tests = new HashSet<TestInfo>(allTests.values());
		for (TestInfo info : tests) {
			if (isChecked(info.fullName())) {
				count++;
			}
		}
		return count;
	}
	
	private void updateSouthText() {
		String text = "passed " + passCount + " / " + getCheckedTestCount() + " tests";
		if (testingIsInProgress) {
			text += " (running ...)";
			if (southLabel.getIcon() == null) {
				southLabel.setIcon(new ImageIcon(ResourceUtils.filenameToURL(ICONS_FOLDER + "progress.gif")));
			}
		} else {
			text += " (complete)";
			southLabel.setIcon(null);
		}
		southLabel.setText(text);
	}
	
	private class AutograderUnitTestGUIWindowAdapter extends WindowAdapter {
		public void windowClosing(WindowEvent event) {
			if (testingIsInProgress) {
				// probably a hung student test case; kill back-end
				if (javaBackEnd != null) {
					javaBackEnd.shutdownBackEnd(/* sendEvent */ true);
				}
			}
		}
	}
	
	/**
	 * Select/Deselect all on double-click.
	 */
	private class AutograderUnitTestGUIMouseAdapter extends MouseAdapter {
		public void mouseClicked(MouseEvent event) {
			if (event.getClickCount() == 2) {
				JButton button = (JButton) event.getSource();
				if (button.getText().contains("All")) {
					selectAll(contentPaneBox, true);
				} else if (button.getText().contains("None")) {
					selectAll(contentPaneBox, false);
				} else if (button.getText().contains("Hide")) {
					allCategoriesHidden = !allCategoriesHidden;
					for (Container category : allCategories.values()) {
						minimize(category, allCategoriesHidden);
					}
				}
			}
		}
	}
}

package stanford.spl;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;
import stanford.cs106.diff.DiffGui;
import stanford.cs106.gui.*;

public class AutograderUnitTestGUI extends Observable implements ActionListener, MouseListener {
	private static final int DIALOG_WIDTH = 500;   // px
	private static final Color ZEBRA_STRIPE_COLOR_1 = new Color(250, 250, 250);
	private static final Color ZEBRA_STRIPE_COLOR_2 = new Color(235, 235, 235);
	
	// should possibly keep in sync with colors in DiffGui.java
	private static final Color PASS_COLOR = new Color(0, 96, 0);
	private static final Color FAIL_COLOR = new Color(96, 0, 0);
	private static final Color WARN_COLOR = new Color(112, 112, 0);
	
	//private static final int MIN_WIDTH = 75;
	private static AutograderUnitTestGUI instance;             // singleton
	private static AutograderUnitTestGUI styleCheckInstance;   // singleton
	
	public static synchronized AutograderUnitTestGUI getInstance(JavaBackEnd javaBackEnd) {
		if (instance == null) {
			instance = new AutograderUnitTestGUI(javaBackEnd);
			instance.frame.setTitle("Autograder Tests");
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
			styleCheckInstance = new AutograderUnitTestGUI(javaBackEnd);
			styleCheckInstance.frame.setTitle("Style Checker");
		}
		return styleCheckInstance;
	}
	
	private JavaBackEnd javaBackEnd;
	private JFrame frame = null;
	private JScrollPane scroll = null;
	private Box contentPaneBox = null;
	private Container currentCategory = null;
	private JLabel descriptionLabel = null;
	private JLabel southLabel = null;
	private Map<String, Container> allCategories = new LinkedHashMap<String, Container>();
	private Map<String, JLabel> allTestDescriptions = new LinkedHashMap<String, JLabel>();
	private Map<String, JLabel> allTestResultLabels = new LinkedHashMap<String, JLabel>();
	private Map<JLabel, String> labelToTestName = new LinkedHashMap<JLabel, String>();
	private Map<String, Map<String, String>> allTestDetails = new LinkedHashMap<String, Map<String, String>>();
	private int passCount = 0;
	private int testCount = 0;
	private boolean testingIsInProgress = true;
	
	public AutograderUnitTestGUI(JavaBackEnd javaBackEnd) {
		this.javaBackEnd = javaBackEnd;
		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
		frame.setVisible(false);
		frame.addWindowListener(new AutograderUnitTestGUIWindowAdapter());
		
		descriptionLabel = new JLabel("Autograder Tests");
		descriptionLabel.setHorizontalAlignment(JLabel.CENTER);
		descriptionLabel.setAlignmentX(0.5f);
		GuiUtils.shrinkFont(descriptionLabel);
		frame.add(descriptionLabel, BorderLayout.NORTH);
		
		contentPaneBox = Box.createVerticalBox();
		scroll = new JScrollPane(contentPaneBox);
		scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		scroll.getVerticalScrollBar().setUnitIncrement(32);
		frame.add(scroll, BorderLayout.CENTER);
		
		southLabel = new JLabel(" ");
		southLabel.setIcon(new ImageIcon("progress.gif"));
		southLabel.setHorizontalTextPosition(SwingConstants.LEFT);
		southLabel.setHorizontalAlignment(JLabel.CENTER);
		southLabel.setAlignmentX(0.5f);
		frame.add(southLabel, BorderLayout.SOUTH);
		updateSouthText();
		
		new WindowCloseKeyListener(frame);
		
		SPLWindowSettings.loadWindowLocation(frame);
		SPLWindowSettings.saveWindowLocation(frame);
	}
	
	public void actionPerformed(ActionEvent event) {
		showTestDetails(event.getActionCommand());
	}
	
	public Container addCategory(String name) {
		if (!allCategories.containsKey(name)) {
			currentCategory = Box.createVerticalBox();
			allCategories.put(name, currentCategory);
			if (!name.isEmpty()) {
				TitledBorder border = BorderFactory.createTitledBorder(name);
				border.setTitleJustification(SwingConstants.CENTER);
				((Box) currentCategory).setBorder(border);
				currentCategory.setBackground(ZEBRA_STRIPE_COLOR_2);
			}
			contentPaneBox.add(currentCategory);
			checkVisibility();
		}
		return allCategories.get(name);
	}
	
	public void addTest(String testName, String categoryName) {
		currentCategory = addCategory(categoryName);
		
		// add a panel about this test
		testCount++;
		JPanel testPanel = new JPanel(new BorderLayout());
		if (testCount % 2 == 0) {
			testPanel.setBackground(ZEBRA_STRIPE_COLOR_1);
		} else {
			testPanel.setBackground(ZEBRA_STRIPE_COLOR_2);
		}
		
		JLabel testCountLabel = new JLabel(String.format("%3d. ", testCount));
		labelToTestName.put(testCountLabel, testName);
		testCountLabel.setHorizontalAlignment(SwingConstants.RIGHT);
		testCountLabel.setAlignmentX(Component.RIGHT_ALIGNMENT);
		Font oldFont = testCountLabel.getFont();
		testCountLabel.setFont(new Font("Monospaced", Font.BOLD, oldFont.getSize()));
		// boldFont(testCountLabel);
		testPanel.add(testCountLabel, BorderLayout.WEST);
		
		String testNameShort = testName.replaceAll("Test_[0-9]{1,5}_", "");
		JLabel testNameLabel = new JLabel(testNameShort);
		labelToTestName.put(testNameLabel, testName);
		allTestDescriptions.put(testName, testNameLabel);
		testNameLabel.setToolTipText("Click to see detailed results from this test.");
		GuiUtils.shrinkFont(testNameLabel);
		testNameLabel.setFont(testNameLabel.getFont().deriveFont(Font.BOLD));
		testPanel.add(testNameLabel, BorderLayout.CENTER);
		testNameLabel.addMouseListener(this);
		
		JLabel resultIconLabel = new JLabel(new ImageIcon("running.gif"));
		GuiUtils.shrinkFont(resultIconLabel, 2);
		resultIconLabel.setHorizontalTextPosition(SwingConstants.LEFT);
		
		resultIconLabel.setToolTipText("Click to see detailed results from this test.");
		labelToTestName.put(resultIconLabel, testName);
//		resultIconLabel.setActionCommand(testName);
//		resultIconLabel.addActionListener(this);
//		resultIconLabel.setBorderPainted(false);
//		resultIconLabel.setMargin(new Insets(0, 0, 0, 0));
		resultIconLabel.addMouseListener(this);
		allTestResultLabels.put(testName, resultIconLabel);
		testPanel.add(resultIconLabel, BorderLayout.EAST);
		
		currentCategory.add(testPanel);
		
		checkVisibility();
	}
	
	public void clearTests() {
		currentCategory = null;
		allCategories.clear();
		allTestDetails.clear();
		allTestResultLabels.clear();
		labelToTestName.clear();
		passCount = 0;
		testCount = 0;
		testingIsInProgress = true;
		contentPaneBox.removeAll();
		contentPaneBox.validate();
		scroll.validate();
		updateSouthText();
		checkVisibility();
	}
	
	public boolean isEmpty() {
		return allCategories.isEmpty();
	}
	
	public void mouseClicked(MouseEvent event) {
		JLabel label = (JLabel) event.getSource();
		String testName = labelToTestName.get(label);
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
	
	public void setDescription(String text) {
		text = text.replaceAll("[ \t\r\n\f]+", " ");
		text = text.replaceAll("Note:", "<b>Note:</b>");
		text = text.replaceAll("Warning:", "<b>Warning:</b>");
		text = text.replaceAll("Error:", "<b>Error:</b>");
		descriptionLabel.setText("<html><body style='width: " + DIALOG_WIDTH + "; max-width: " + DIALOG_WIDTH + "'><center>" + text + "</center></body></html>");
		descriptionLabel.validate();
		checkVisibility();
	}
	
	private String getTestResult(String testName) {
		JLabel testNameLabel = allTestDescriptions.get(testName);
		if (testNameLabel == null) {
			return "no such test";
		} else if (testNameLabel.getForeground().equals(FAIL_COLOR)) {
			return "fail";
		} else if (testNameLabel.getForeground().equals(WARN_COLOR)) {
			return "warn";
		} else if (testNameLabel.getForeground().equals(PASS_COLOR)) {
			return "pass";
		} else {
			if (allTestDetails.containsKey(testName)
					&& allTestDetails.get(testName).containsKey("passed")) {
				String passed = allTestDetails.get(testName).get("passed");
				return passed.equalsIgnoreCase("true") ? "pass" : "fail";
			} else {
				return "unknown";
			}
		}
	}
	
	public boolean setTestResult(String testName, String result) {
		result = result.toLowerCase().intern();
		if (result == "error") {
			result = "fail";  // synonyms
		}
		
		JLabel testNameLabel = allTestDescriptions.get(testName);
		JLabel resultIconLabel = allTestResultLabels.get(testName);
		if (testNameLabel == null || resultIconLabel == null) {
			return false;
		}
		
		// BUGFIX: if test already failed previously, don't set back to passed
		String existingResult = getTestResult(testName).intern();
		if ((existingResult == "fail" || existingResult == "warn") && result != "fail") {
			return true;
		}
		
		resultIconLabel.setIcon(new ImageIcon(result + ".gif"));   // pass, fail, running, warn
		if (result == "pass") {
			passCount++;
			testNameLabel.setForeground(PASS_COLOR);
		} else if (result == "fail") {
			testNameLabel.setForeground(FAIL_COLOR);
		} else if (result == "warn") {
			testNameLabel.setForeground(WARN_COLOR);
		}
		updateSouthText();
		return true;
	}
	
	public void setTestDetails(String testName, Map<String, String> details) {
		// BUGFIX: don't replace test details if a test already failed here
		String existingResult = getTestResult(testName).intern();
		if (existingResult == "fail" || existingResult == "warn") {
			return;
		}
		allTestDetails.put(testName, details);
	}
	
	public void setTestingCompleted(boolean completed) {
		testingIsInProgress = !completed;
		updateSouthText();
	}
	
	public boolean setTestRuntime(String testName, int runtimeMS) {
		JLabel resultIconLabel = allTestResultLabels.get(testName);
		if (resultIconLabel == null) {
			return false;
		} else {
			String text = " (" + runtimeMS + "ms)";
			resultIconLabel.setText(text);
			return true;
		}
	}
	
	public void setVisible(boolean visible) {
		frame.setVisible(visible);
	}
	
	private void checkVisibility() {
		contentPaneBox.revalidate();
		scroll.revalidate();
		frame.revalidate();
		frame.validate();
		frame.pack();
		frame.setSize(frame.getWidth() + 32, frame.getHeight());  // a bit of buffer for scrollbar
		if (frame.isVisible() != !isEmpty()) {
			frame.setVisible(!isEmpty());
		}
		
		// scroll to bottom as new tests appear
		scroll.getVerticalScrollBar().setValue(scroll.getVerticalScrollBar().getMaximum());
	}
	
	/*
		enum UnitTestType {
		    ASSERT_EQUALS = 0,
		    ASSERT_NOT_EQUALS,
		    ASSERT_NEAR,
		    ASSERT_DIFF,
		    ASSERT_TRUE,
		    ASSERT_FALSE,
		    EXCEPTION,
		    NOT_EXCEPTION,
		    PASS,
		    FAIL,
		};
	 */
	private void showTestDetails(String testName) {
		// TODO: better
		// JOptionPane.showMessageDialog(null, "DETAILS: \"" + allTestDetails.get(testName) + "\"");
		
		// {testType=TEST_ASSERT_EQUALS,
		//  message="......",
		//  expected=foobarbaz,
		//  actual=foobarbaz,
		//  valueType=string,
		//  passed=true}
		Map<String, String> deets = allTestDetails.get(testName);
		if (deets == null) {
			return;
		}
		
		boolean passed = deets.containsKey("passed") && deets.get("passed").equalsIgnoreCase("true");
		String type = deets.containsKey("testType") ? deets.get("testType").toUpperCase().intern() : "";
		String message = deets.get("message");
		if (type == "ASSERT_EQUALS") {
			message += " (must be equal)";
		} else if (type == "ASSERT_NOT_EQUALS") {
			message += " (must be non-equal)";
		} else if (type == "ASSERT_NEAR") {
			message += " (must be nearly equal)";
		} else if (type == "ASSERT_DIFF") {
			message += " (program output must match)";
		} else if (type == "ASSERT_TRUE") {
			message += " (must be true)";
		} else if (type == "ASSERT_FALSE") {
			message += " (must be false)";
		} else if (type == "EXCEPTION") {
			message += " (threw exception)";
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
		String expected = deets.get("expected");
		String student  = deets.get("student");
		String valueType = deets.containsKey("valueType") ? deets.get("valueType").toLowerCase().intern() : "";
		if (valueType == "string") {
			expected = "\"" + expected + "\"";
			student  = "\"" + student  + "\"";
		} else if (valueType == "char") {
			expected = "'" + expected + "'";
			student  = "'" + student  + "'";
		}
		
		boolean shouldShowJOptionPane = true;
		if (type == "ASSERT_EQUALS" || type == "ASSERT_NOT_EQUALS" || type == "ASSERT_NEAR" || type == "STYLE_CHECK") {
			String htmlMessage = "";
			htmlMessage += "<html><body style='max-width: " + DIALOG_WIDTH + "px;'>";
			htmlMessage += "<p>" + message + "</p>";
			htmlMessage += "<ul>";
			htmlMessage += "<li><font style='font-family: monospaced' color='" + DiffGui.EXPECTED_COLOR + "'>expected:</font>" + expected + "</li>";
			htmlMessage += "<li><font style='font-family: monospaced' color='" + DiffGui.STUDENT_COLOR  + "'>student :</font>" + student  + "</li>";
			htmlMessage += "</ul>";
			htmlMessage += "</body></html>";
			htmlMessage = htmlMessage.replace("\n", "\\n");
			htmlMessage = htmlMessage.replace("\r", "\\r");
			htmlMessage = htmlMessage.replace("\t", "\\t");
			message = htmlMessage;
		} else if (type == "ASSERT_DIFF") {
			shouldShowJOptionPane = false;
			new DiffGui("expected output", expected, "student output", student).show();
		}
		
		if (shouldShowJOptionPane) {
			JOptionPane.showMessageDialog(
					/* parent */ frame,
					message,
					/* title */ testName,
					/* type */ passed ? JOptionPane.INFORMATION_MESSAGE : JOptionPane.ERROR_MESSAGE);
		}
		
		setChanged();
		notifyObservers(testName);
	}
	
	private void updateSouthText() {
		String text = "passed " + passCount + " / " + testCount + " tests";
		if (testingIsInProgress) {
			text += " (running ...)";
			if (southLabel.getIcon() == null) {
				southLabel.setIcon(new ImageIcon("progress.gif"));
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
				javaBackEnd.shutdownBackEnd(/* sendEvent */ true);
			}
		}
	}
}

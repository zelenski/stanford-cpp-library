/*
 * @author Marty Stepp
 * @version 2017/05/24
 * - bug fix for running classes that are not Programs (launch main())
 * @version 2017/05/07
 * - small bug fix for RandomGenerator label width
 * - added constGet
 * @version 2016/04/28
 * - made static fields/methods show "static" in drop-down menu
 * @version 2016/04/08
 * - fixed bug with user input table layout
 * @version 2015/05/28
 * - slight improvements in assert* output
 * @version 2015/05/19
 * - added support for 'main' classes in addition to 'Program' classes
 * @version 2015/05/03
 * - added JUnit testing functionality
 */

package stanford.cs106.autograder;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.util.*;
import java.util.List;
import javax.swing.*;
import javax.swing.event.*;
import stanford.cs106.diff.*;
import stanford.cs106.gui.*;
import stanford.cs106.io.*;
import stanford.cs106.junit.*;
import stanford.cs106.program.*;
import stanford.cs106.reflect.*;
import stanford.cs106.util.*;
import stanford.spl.AutograderUnitTestGUI;
import acm.graphics.*;
import acm.gui.*;
import acm.program.*;

public abstract class GuidedAutograder implements ActionListener, ChangeListener {
	// should be set by subclass
	protected static Class<? extends Program> STUDENT_CLASS = Program.class;
	protected static Class<?>[] REFLECTION_PANEL_CLASSES = new Class<?>[] {STUDENT_CLASS};
	protected static Class<?>[] LAUNCH_CLASSES = new Class<?>[] {STUDENT_CLASS};
	protected static Class<?>[] JUNIT_CLASSES = new Class<?>[] {};
	protected static List<String> USER_INPUTS_ADDITIONAL = new ArrayList<String>();

	// autograder version number/date and author's name
	protected static String VERSION = "1.0";
	protected static String AUTHOR = "Marty Stepp";
	protected static String TITLE = "";

	// color of onscreen grid lines
	protected static Color GRID_LINE_COLOR = new Color(24, 157, 135);

	protected static String INPUTS_DIR = "inputs/";
	protected static String LATE_DAYS_FILE = "lateDays.txt";
	
	protected static String ASSERT_OUTPUT_PREFIX = "  * ";
	
	protected static boolean SHOW_WINDOW_PANEL = true;
	protected static boolean SHOW_FONT_PANEL = true;
	protected static boolean SHOW_DELAY_PANEL = true;
	protected static boolean SHOW_REFLECTION_PANELS = true;
	protected static boolean SHOW_REFLECTION_FIELDS = true;
	protected static boolean SHOW_REFLECTION_METHODS = true;
	protected static boolean SHOW_MESSAGELOG_PANEL = true;
	protected static boolean SHOW_INPUT_INJECT_PANEL = true;
	protected static boolean SHOW_USER_INPUT_PANEL = true;
	protected static boolean SHOW_RANDOM_PANEL = true;
	protected static boolean SHOW_JUNIT_PANEL = false;
	protected static boolean SHOW_INIT_RUN_BOXES = true;

	// these constants come from acm.program.Program class
	private static final int DEFAULT_WIDTH = 754;
	private static final int DEFAULT_HEIGHT = 492;
	
	// dimensions of the message log area; can be changed by subclass
	protected static int MESSAGE_LOG_ROWS = 5;
	protected static int MESSAGE_LOG_COLS = 40;
	protected static Font MESSAGE_LOG_FONT = new Font("Monospaced", Font.PLAIN, 12);
	
	// whether to print 'assert' statements that pass in the message log
	protected static boolean PRINT_PASSING_ASSERTS = true;
	
	protected static boolean SET_LOOK_AND_FEEL = true;
	
	// types of constants/fields that autograder knows how to set/change safely
	protected static final Set<Class<?>> SUPPORTED_FIELD_TYPES = new HashSet<Class<?>>();
	static {
		SUPPORTED_FIELD_TYPES.add(Integer.TYPE);
		SUPPORTED_FIELD_TYPES.add(Integer.class);
		SUPPORTED_FIELD_TYPES.add(Double.TYPE);
		SUPPORTED_FIELD_TYPES.add(Double.class);
		SUPPORTED_FIELD_TYPES.add(Boolean.TYPE);
		SUPPORTED_FIELD_TYPES.add(Boolean.class);
		SUPPORTED_FIELD_TYPES.add(Character.TYPE);
		SUPPORTED_FIELD_TYPES.add(Character.class);
		SUPPORTED_FIELD_TYPES.add(Long.TYPE);
		SUPPORTED_FIELD_TYPES.add(Long.class);
		SUPPORTED_FIELD_TYPES.add(Short.TYPE);
		SUPPORTED_FIELD_TYPES.add(Short.class);
		SUPPORTED_FIELD_TYPES.add(Byte.TYPE);
		SUPPORTED_FIELD_TYPES.add(Byte.class);
		SUPPORTED_FIELD_TYPES.add(Float.TYPE);
		SUPPORTED_FIELD_TYPES.add(Float.class);
		SUPPORTED_FIELD_TYPES.add(String.class);
//		SUPPORTED_FIELD_TYPES.add(List.class);
//		SUPPORTED_FIELD_TYPES.add(ArrayList.class);
//		SUPPORTED_FIELD_TYPES.add(Map.class);
//		SUPPORTED_FIELD_TYPES.add(HashMap.class);
		SUPPORTED_FIELD_TYPES.add(new int[0].getClass());
		SUPPORTED_FIELD_TYPES.add(new double[0].getClass());
		SUPPORTED_FIELD_TYPES.add(new char[0].getClass());
		SUPPORTED_FIELD_TYPES.add(new long[0].getClass());
		SUPPORTED_FIELD_TYPES.add(new boolean[0].getClass());
		SUPPORTED_FIELD_TYPES.add(new String[0].getClass());
//		SUPPORTED_FIELD_TYPES.add(GObject.class);
//		SUPPORTED_FIELD_TYPES.add(GLine.class);
//		SUPPORTED_FIELD_TYPES.add(GOval.class);
//		SUPPORTED_FIELD_TYPES.add(GRect.class);
//		SUPPORTED_FIELD_TYPES.add(GLabel.class);
//		SUPPORTED_FIELD_TYPES.add(GPolygon.class);
	}
	
	public static Color getGridLineColor() {
		return GRID_LINE_COLOR;
	}

	public static void setGridLineColor(Color color) {
		GRID_LINE_COLOR = color;
	}

	// fields
	protected Program studentProgram = null;
	protected StudentProgramRunnerThread runnerThread = null;
	protected Container overallPanel;
	protected JCheckBox initRunBox;
	protected JCheckBox restartBox;
	
	@SuppressWarnings("rawtypes")
	protected JComboBox inputsBox;
	@SuppressWarnings("rawtypes")
	protected JComboBox outputsBox;
	
	protected JFrame autograderFrame;
	protected JScrollPane messageLogScrollPane;
	protected JSlider timeSlider;
	protected IntField randomSeedField;
	protected IntField randomIntField;
	protected JCheckBox uppercaseBox;
	protected JTextArea messageLog;
	protected Map<String, Container> panelTable = new HashMap<String, Container>();
	protected Map<Class<?>, ReflectionPanel> reflectionPanels = new HashMap<Class<?>, ReflectionPanel>();
	protected int labelWidth;
	protected int windowWidth = DEFAULT_WIDTH;
	protected int windowHeight = DEFAULT_HEIGHT;
	
	// assert counts
	protected int passCount = 0;
	protected int failCount = 0;
	
	protected boolean assertUsingGui = false;
	
	protected LookAndFeel lookAndFeel = UIManager.getLookAndFeel();

	// runs the overall autograder program
	public void start() {
		// helps with loading internal resources from a JAR
		ResourceUtils.setResourceLoaderClass(getClass());

		createComponents();
		doLayout();
		
		// show the window!
		autograderFrame.add(overallPanel);
		autograderFrame.pack();
		autograderFrame.setVisible(true);
		
		if (!SET_LOOK_AND_FEEL) {
			// put look 'n' feel back
			try {
				UIManager.setLookAndFeel(lookAndFeel);
			} catch (Exception e) {
				// empty
			}
		}
		
		// make it so that the field value display updates as the game goes on
		if (SHOW_REFLECTION_PANELS) {
			new Thread(new FieldValueUpdateRunnable()).start();
		}
	}

	// constructs all the various components and adds event listeners to them
	@SuppressWarnings({ "unchecked", "rawtypes" })
	protected void createComponents() {
		//if (SET_LOOK_AND_FEEL) {
			GuiUtils.setSystemLookAndFeel();
		//}

		String title = STUDENT_CLASS.getName();
		if (TITLE != null && !TITLE.isEmpty()) {
			title = TITLE;
		}
		autograderFrame = new JFrame(title + " Autograder v" + VERSION + " by " + AUTHOR);
		GuiUtils.rememberWindowLocation(autograderFrame);
		autograderFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		overallPanel = Box.createVerticalBox();

		initRunBox = new JCheckBox("Call init()/run()", true);
		initRunBox.setMnemonic('C');
		restartBox = new JCheckBox("Restart on constant change", true);
		restartBox.setMnemonic('R');

		timeSlider = new JSlider(1, 7);
		timeSlider.setValue(4);
		timeSlider.setMajorTickSpacing(1);
		timeSlider.setPaintLabels(false);
		timeSlider.setPaintTicks(true);
		timeSlider.setSnapToTicks(true);
		timeSlider.addChangeListener(this);
		timeSlider.setPreferredSize(new Dimension(100, timeSlider.getPreferredSize().height));
		
		randomSeedField = new IntField();
		randomSeedField.setActionCommand("Random seed");
		randomIntField = new IntField(0);
		randomIntField.setActionCommand("Random int");

		inputsBox = new JComboBox();
		inputsBox.setEditable(false);
		inputsBox.addItem("(select a script)");
		File[] files = new File(INPUTS_DIR).listFiles();
		if (files != null && files.length > 0) {
			Arrays.sort(files);
			for (File file : files) {
				if (file.getName().startsWith("input")) {
					inputsBox.addItem(file.getName().replace("input-", "").replace(".txt", ""));
				}
			}
		}

		outputsBox = new JComboBox();
		outputsBox.setEditable(false);
		outputsBox.addItem("(select an output)");
		if (files != null && files.length > 0) {
			for (File file : files) {
				if (file.getName().startsWith("expected")) {
					outputsBox.addItem(file.getName().replace("expected-", "").replace(".txt", ""));
				}
			}
		}
		uppercaseBox = new JCheckBox("Uppercase?", true);
		
		messageLog = new JTextArea(MESSAGE_LOG_ROWS, MESSAGE_LOG_COLS);
		messageLog.setFont(MESSAGE_LOG_FONT);
		messageLog.setEditable(false);
		messageLogScrollPane = new JScrollPane(messageLog);
		messageLogScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		messageLogScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);

		// just for kicks, put their lateDays.txt into message log
		displayLateness();
	}

	// adds all of the various components to the screen layout
	protected Container doLayout() {
		final int GAP = 15;
		
		// program launcher panel
		List<Component> launchButtons = new ArrayList<Component>();
		for (Class<?> clazz : LAUNCH_CLASSES) {
			JButton launchButton = GuiUtils.createButton("Run " + clazz.getName(), 'B', this);
			launchButton.setActionCommand("Launch " + clazz.getName());
			Font buttonFont = launchButton.getFont();
			launchButton.setFont(buttonFont.deriveFont(Font.BOLD, buttonFont.getSize() + 2f));
			int wider = 10;  // 50
			int taller = 16;
			Dimension launchSize = new Dimension(launchButton.getPreferredSize().width + wider, launchButton.getPreferredSize().height + taller);
			launchButton.setPreferredSize(launchSize);
			launchButtons.add(launchButton);
		}
		if (SHOW_INIT_RUN_BOXES) {
			launchButtons.add(Box.createHorizontalStrut(10));
			launchButtons.add(initRunBox);
			launchButtons.add(restartBox);
			addPanel("Launch", launchButtons);
		} else {
			addPanel("Launch", new FlowLayout(FlowLayout.CENTER), launchButtons);
		}

		labelWidth = new JLabel("JUnit automated tests: ").getPreferredSize().width;
		if (SHOW_JUNIT_PANEL) {
			JPanel junitPanel = new JPanel();
			for (Class<?> clazz : JUNIT_CLASSES) {
				String className = clazz.getName();
				JButton junitClassButton = GuiUtils.createButton(className, "Run JUnit " + className, ' ', this);
				junitClassButton.setFont(junitClassButton.getFont().deriveFont(Font.BOLD));
				junitPanel.add(junitClassButton);
			}
			
			addPanel("JUnit tests",
					GuiUtils.createLabel("JUnit automated tests: ", labelWidth),
					junitPanel
			);
		}
		
		// window settings panel
		if (SHOW_WINDOW_PANEL) {
			addPanel("Window",
					GuiUtils.createLabel("Window: ", labelWidth),
					GuiUtils.createButton("420x600px (Default)", "Default window size", 'D', this),
					GuiUtils.createButton("700x500px", "Wide window size", ' ', this),
					GuiUtils.createButton("300x700px", "Tall window size", ' ', this));
		}
	
		if (SHOW_DELAY_PANEL) {
			// delay MS panel
			addPanel("Delay",
					GuiUtils.createLabel("DELAY: ", labelWidth, /* rightAligned */true),
					GuiUtils.createButton("Default", "Default DELAY", 'u', this),
					GuiUtils.createButton("1ms", "1ms DELAY", ' ', this),
					GuiUtils.createButton("5ms", "5ms DELAY", ' ', this),
					GuiUtils.createButton("100ms", "100ms DELAY", ' ', this),
					GuiUtils.createButton("1s", "1000ms DELAY", ' ', this),
					GuiUtils.createButton("1000s", "1000000ms DELAY", ' ', this),
					timeSlider,
					GuiUtils.createButton("Tick", "Tick", 'T', this));
		}
		
		doExtraLayout();

		if (SHOW_INPUT_INJECT_PANEL) {
			addPanel("Inputs",
					GuiUtils.createLabel("Input scripts: ", labelWidth),
					inputsBox,
					GuiUtils.createButton("Inject", "Inject input", 'I', this),
					Box.createHorizontalStrut(GAP),
					new JLabel("outputs: "),
					outputsBox,
					GuiUtils.createButton("Diff", "Diff output", 'D', this),
					Box.createHorizontalStrut(GAP),
					GuiUtils.createButton("Clear console", ' ', this));
		}
		
		if (SHOW_USER_INPUT_PANEL) {
			String inputs = "Yes A B C D E F G H I J K L M ? Enter " +
		                    "No  N O P Q R S T U V W X Y Z . Space";
			int additionalGridWidth = 2;   // 2 columns per cell for user inputs additional
			int cols = 16;
			int rows = 2;

			// add another row of inputs for every <= 16 additional inputs you have
			if (!USER_INPUTS_ADDITIONAL.isEmpty()) { 
				rows += (int) Math.ceil((double) USER_INPUTS_ADDITIONAL.size() / cols / additionalGridWidth);
			}
			JPanel userInputPanel = new JPanel(new TableLayout(rows, cols));
			// add
			for (String input : inputs.split("[ \t]+")) {
				JButton inputButton = GuiUtils.createButton(input, "User input " + input, ' ', this);
				Dimension size = inputButton.getPreferredSize();
				if (size.width < size.height) {
					size.width = size.height;
					inputButton.setPreferredSize(size);
				}
				userInputPanel.add(inputButton);
			}
			for (String input : USER_INPUTS_ADDITIONAL) {
				JButton inputButton = GuiUtils.createButton(input, "User input " + input, ' ', this);
				userInputPanel.add(inputButton, "gridwidth=" + additionalGridWidth);
			}
			
			// fill rest with empty panels
			while (userInputPanel.getComponentCount() < rows * cols) {
				userInputPanel.add(new JPanel());   // empty square
			}
			
			addPanel("User input",
					GuiUtils.createLabel("User input: ", labelWidth),
					userInputPanel,
					uppercaseBox
			);
		}
		
		if (SHOW_RANDOM_PANEL) {
			addPanel("RandomGenerator",
					GuiUtils.createLabel("RandomGenerator: ", labelWidth),
					new JLabel("Seed: "),
					randomSeedField,
					GuiUtils.createButton("Set", "Random seed", 'R', this),
					GuiUtils.createButton("Unset", "Random seed unset", 'u', this),
					Box.createHorizontalStrut(GAP),
					new JLabel("Fixed int: "),
					randomIntField,
					GuiUtils.createButton("Set", "Random int", ' ', this),
					GuiUtils.createButton("Unset", "Random int unset", 'n', this)
					);
		}
		
		if (SHOW_REFLECTION_PANELS) {
			for (Class<?> clazz : REFLECTION_PANEL_CLASSES) {
				ReflectionPanel panel = new ReflectionPanel(clazz);
				panel.doLayout();
				reflectionPanels.put(clazz, panel);
			}
		}

		if (SHOW_MESSAGELOG_PANEL) {
			// message log
			addPanel("Message Log",
					new JLabel("Message Log:"));
			overallPanel.add(messageLogScrollPane);
		}
		return overallPanel;
	}
	
	protected void doExtraLayout() {
		// does nothing; override me
	}

	// required by ActionListener interface
	// listens to events on controls in the autograder GUI
	public void actionPerformed(ActionEvent event) {
		String cmd = event.getActionCommand().intern();
		if (cmd.startsWith("Launch ")) {
			String className = cmd.replace("Launch ", "");
			studentProgramThreadLaunch(className);
		} else if (cmd.startsWith("Run JUnit ")) {
			String className = cmd.replace("Run JUnit ", "");
			junitLaunch(className);
		} else if (cmd == "Default window size") {
			// empty
		} else if (cmd == "Wide window size") {
			constantsSetAllWindow(700, 500);
		} else if (cmd == "Tall window size") {
			constantsSetAllWindow(300, 700);
		} else if (cmd == "Tick") {
//			if (hangman == null) {
//				return;
//			}
//			hangman.pauseTick();
		} else if (cmd == "Inject input") {
			injectInput();
		} else if (cmd == "Diff output") {
			diffOutput();
		} else if (cmd == "Clear console") {
			if (JOptionPane.showConfirmDialog(autograderFrame, "Are you sure you want to clear the console?", "Are you sure?", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
				if (studentProgram != null) {
					((ConsoleProgram) studentProgram).getConsole().clear();
				}
			}
		} else if (cmd == "Random seed") {
			int seed = randomSeedField.getValue();
			ReflectionUtils.staticMethodInvoke("RandomGenerator", "setAllSeed", seed);
			printlnLog("set RandomGenerator seed to " + seed);
		} else if (cmd == "Random seed unset") {
			randomSeedField.setText("");
			ReflectionUtils.staticMethodInvoke("RandomGenerator", "setAllSeed", new Object[] {null});
			printlnLog("unset RandomGenerator seed");
		} else if (cmd == "Random int") {
			int rig = randomIntField.getValue();
			ReflectionUtils.staticMethodInvoke("RandomGenerator", "rigInt", rig);
			ReflectionUtils.staticMethodInvoke("RandomGenerator", "rigDouble", (double) rig);
			printlnLog("set RandomGenerator so all nextInt/Double calls return " + rig);
		} else if (cmd == "Random int unset") {
			randomIntField.setText("");
			ReflectionUtils.staticMethodInvoke("RandomGenerator", "rigInt", new Object[] {null});
			ReflectionUtils.staticMethodInvoke("RandomGenerator", "rigDouble", new Object[] {null});
			printlnLog("unset RandomGenerator nextInt/Double fixed value");
		} else if (cmd.startsWith("User input ")) {
			userInput(cmd, event);
		}
	}

	// required by ChangeListener interface
	public void stateChanged(ChangeEvent event) {
		// scaleTimeBySlider(timeSlider.getValue());
	}

	// very limited subset of JUnit-style methods with pass/fail counting and 
	// test result printing to message log
	protected void assertTrue(String msg, boolean b) {
		if (b) {
			if (PRINT_PASSING_ASSERTS) {
				printlnLog(ASSERT_OUTPUT_PREFIX + msg + ": pass");
			}
			passCount++;
		} else {
			printlnLog(ASSERT_OUTPUT_PREFIX + msg + ": FAIL!");
			failCount++;
		}
	}
	
	protected void assertFalse(String msg, boolean b) {
		assertTrue(msg, !b);
	}
	
	protected void assertEquals(String msg, Object expected, Object actual) {
		if (expected == actual || (expected != null && actual != null && expected.equals(actual))) {
			if (PRINT_PASSING_ASSERTS) {
				printlnLog(ASSERT_OUTPUT_PREFIX + msg + ": pass");
			}
			passCount++;
		} else {
			printlnLog(ASSERT_OUTPUT_PREFIX + msg + ": FAIL! (expected <" + expected + ">, got <" + actual + ">)");
			failCount++;
		}
	}
	
	protected void assertPrintCounts() {
		assertPrintCounts(true);
	}
	
	protected void assertPrintCounts(boolean resetAfter) {
		int total = (passCount + failCount);
		printlnLog("Passed " + passCount + " out of " + total + " tests."
				+ (total == passCount ? " (all tests passed!)" : ""));
		if (resetAfter) {
			assertResetCounts();
		}
	}
	
	protected void assertResetCounts() {
		passCount = 0;
		failCount = 0;
	}
	
	// adds a JPanel to the autograder's layout
	protected JPanel addPanel(String name, Collection<Component> components) {
		return addPanel(name, components.toArray(new Component[0]));
	}
	
	protected JPanel addPanel(String name, Component... components) {
		return addPanel(name, new FlowLayout(FlowLayout.LEFT), components);
	}

	protected JPanel addPanel(String name, LayoutManager layout, Collection<Component> components) {
		return addPanel(name, layout, components.toArray(new Component[0]));
	}
	
	protected JPanel addPanel(String name, LayoutManager layout, Component... components) {
		JPanel panel = new JPanel(layout);
		for (Component comp : components) {
			if (comp != null) {
				panel.add(comp);
			}
		}
		panelTable.put(name, panel);
		overallPanel.add(panel);
		return panel;
	}
	
	protected void diffOutput() {
		if (studentProgram == null) {
			return;
		}
		
		String selected = String.valueOf(outputsBox.getSelectedItem());
		if (selected == null || selected.isEmpty() || selected.startsWith("(")) {
			return;
		}
		String expected = IOUtils.readEntireFile(INPUTS_DIR + "expected-" + selected + ".txt");
		String output = ProgramUtils.getConsoleOutput((ConsoleProgram) studentProgram);
		new DiffGui("Expected output", expected, "Student output", output).show();
	}
	
	protected void injectInput() {
		if (studentProgram == null) {
			return;
		}
		String selected = String.valueOf(inputsBox.getSelectedItem());
		if (selected == null || selected.isEmpty() || selected.startsWith("(")) {
			return;
		}
		outputsBox.setSelectedItem(selected);
		String fileText = IOUtils.readEntireFile(INPUTS_DIR + "input-" + selected + ".txt");
		ProgramUtils.injectConsoleInput((ConsoleProgram) studentProgram, fileText);
		printlnLog("loaded input script from " + selected);
	}
	
	protected void userInput(String cmd, ActionEvent event) {
		String userInput = cmd.replace("User input ", "").intern();
		if (userInput == "Enter") {
			userInput = "\n";
		} else if (userInput == "Space") {
			userInput = " ";
		}
		if ((userInput.length() == 1 && uppercaseBox.isSelected())
				|| (event.getModifiers() & KeyEvent.SHIFT_DOWN_MASK) != 0) {
			userInput = userInput.toUpperCase();
		} else {
			userInput = userInput.toLowerCase();
		}
		if (!userInput.endsWith("\n")) {
			userInput += "\n";
		}
		ProgramUtils.injectConsoleInput((ConsoleProgram) studentProgram, userInput);
	}
	
	// stops the current student program thread, if any, and closes its JFrame
	protected void studentProgramThreadKill() {
		if (runnerThread == null) {
			return;
		}
		if (runnerThread.started && runnerThread.isAlive()) {
			runnerThread.interrupt();
		}
		runnerThread.killMe();
		studentProgram = null;
		runnerThread = null;
	}

	// (re)starts student program in its own JFrame and Thread,
	// if the "restart on constant changed" box is checked
	protected void studentProgramThreadLaunchIfBoxChecked() {
		if (restartBox.isSelected()) {
			studentProgramThreadLaunch();
		}
	}

	// (re)starts program in its own JFrame and Thread
	protected void studentProgramThreadLaunch() {
		studentProgramThreadLaunch(STUDENT_CLASS);
	}
	
	@SuppressWarnings("unchecked")
	protected void studentProgramThreadLaunch(String className) {
		try {
			studentProgramThreadLaunch((Class<? extends Program>) Class.forName(className));
		} catch (ClassNotFoundException e) {
			throw new ReflectionRuntimeException(e);
		}
	}
	
	protected void junitLaunch(String className) {
		try {
			junitLaunch(Class.forName(className));
		} catch (ClassNotFoundException e) {
			throw new ReflectionRuntimeException(e);
		}
	}
	
	protected void junitLaunch(final Class<?> clazz) {
		studentProgramThreadKill();
		printlnLog("running JUnit tests from " + clazz.getName());
		
		final AutograderUnitTestGUI testGUI = AutograderUnitTestGUI.getInstance(/* JavaBackEnd */ null);
		testGUI.clearTestResults();
		testGUI.clearTests();
		testGUI.setCheckboxesShown(false);
		testGUI.setDescription("Click a test to see error details.");
		
		// look for JUnit test methods and add them to autograder test runner GUI
		Method[] methods = clazz.getMethods();
		Arrays.sort(methods, new ReflectionUtils.MethodNameComparator());
		final List<Method> testMethods = new ArrayList<Method>();
		for (Method method : methods) {
			if (JUnitUtils.isJUnitMethod(clazz, method)) {
				String categoryName = JUnitUtils.getTestCategory(clazz, method);
				testGUI.addTest(method.getName(), categoryName);
				testMethods.add(method);
			}
		}
		testGUI.setVisible(true);
		
		Runnable junitRunnable = new Runnable() {
			public void run() {
				// look for JUnit test methods and run them
				for (Method method : testMethods) {
					JUnitUtils.runJUnitMethod(clazz, method, testGUI);
				}
				testGUI.setTestingCompleted(true);
				printlnLog("Passed " + testGUI.getPassedCount() + " of "
						+ testGUI.getTestCount() + " tests.");
			}
		};
		Thread thread = new Thread(junitRunnable);
		thread.start();
	}

	protected void studentProgramThreadLaunch(Class<? extends Program> clazz) {
		studentProgramThreadKill();
		
		printlnLog("launching " + clazz.getName());
		runnerThread = new StudentProgramRunnerThread(clazz, windowWidth, windowHeight, initRunBox.isSelected());
		studentProgram = runnerThread.getProgram();
		// scaleTimeBySlider(timeSlider.getValue());
		runnerThread.start();
		try {
			runnerThread.join(50);
		} catch (InterruptedException ie) {
			// empty
		}
	}

	// sets all constants related to the game window to the given values
	protected void constantsSetAllWindow(int width, int height) {
		printlnLog("setting window size to " + width + "x" + height + "px (-1 = defaults)");
		studentProgramThreadLaunchIfBoxChecked();
	}

	@SuppressWarnings("unchecked")
	protected <T> T constGet(String name) {
		return (T) ReflectionUtils.getConstantValue(STUDENT_CLASS, studentProgram, name);
	}

	protected int constGetInt(String name) {
		return ReflectionUtils.getConstantValueInt(STUDENT_CLASS, studentProgram, name);
	}

	// sets the given constant in the student's class
	// to the given value;
	// if no such constant is found, prints an error to the message log
	protected void constSet(String name, Object value) {
		name = name.intern();
		if (ReflectionUtils.constantExists(STUDENT_CLASS, name)) {
			ReflectionUtils.setConstantValue(STUDENT_CLASS, studentProgram, name, value);
		} else if (ReflectionUtils.constantExists(GraphicsProgram.class, name)) {
			ReflectionUtils.setConstantValue(GraphicsProgram.class, studentProgram, name, value);
		} else {
			printlnLog("ERROR: No constant named '" + name
					+ "' exists in the student's classes.");
		}
	}

	// sets the given constant in the student's classes
	// to the given value, if that value is positive, else sets to the given
	// default value;
	// if no such constant is found, prints an error to the message log
	protected void constSetMaybe(String name, int value, Object defaultValue) {
		if (value >= 0) {
			constSet(name, value);
		} else {
			constSet(name, defaultValue);
		}
	}

	// sets the given constant in the student's classes
	// to the given value, if that value is positive, else sets to the given
	// default value;
	// if no such constant is found, prints an error to the message log
	protected void constSetMaybe(String name, double value, Object defaultValue) {
		if (value > 0) {
			constSet(name, value);
		} else {
			constSet(name, defaultValue);
		}
	}
	
	protected int getWindowWidth() {
		return windowWidth;
	}

	protected int getWindowHeight() {
		return windowHeight;
	}
	
	protected void setWindowWidth(int width) {
		this.windowWidth = width;
	}
	
	protected void setWindowHeight(int height) {
		this.windowHeight = height;
	}

	protected void setWindowSize(int width, int height) {
		this.windowWidth = width;
		this.windowHeight = height;
	}

	// shows lateDays.txt information in message log
	protected void displayLateness() {
		File lateDaysFile = new File(LATE_DAYS_FILE);
		if (lateDaysFile.exists()) {
			try {
				Scanner input = new Scanner(lateDaysFile);
				StringBuilder sb = new StringBuilder();
				while (input.hasNextLine()) {
					String line = input.nextLine();
					line = line.replace("assignment_due_time: ", "assignment_due_time:     ");
					sb.append(line);
					sb.append('\n');
				}
				input.close();
				sb.append("=============================================");
				printlnLog(sb.toString());
			} catch (FileNotFoundException fnfe) {
				printlnLog("Unable to read " + LATE_DAYS_FILE + ": " + fnfe);
			}
		}
	}

	// adds one grid line at the given points
	protected void gridLineAdd(GContainer container, double x1, double y1, double x2, double y2) {
		GLine line = new GLine(x1, y1, x2, y2);
		line.setColor(GRID_LINE_COLOR);
		container.add(line);
	}
	
	protected void gridLineAdd(double x1, double y1, double x2, double y2) {
		GLine line = new GLine(x1, y1, x2, y2);
		line.setColor(GRID_LINE_COLOR);
		if (studentProgram instanceof GraphicsProgram) {
			GraphicsProgram graphicsProgram = (GraphicsProgram) studentProgram;
			graphicsProgram.add(line);
			// graphicsProgram.setInvisible(line, true); // so getElementAt won't return it
		}
	}

	protected boolean gridLinesExist(GContainer container) {
		if (container == null) {
			return false;
		}
		for (int i = 0; i < container.getElementCount(); i++) {
			GObject gobj = container.getElement(i);
			if (
					// gobj instanceof GLine && 
					gobj.getColor().equals(GRID_LINE_COLOR)) {
				return true;
			}
		}
		return false;
	}
	
	// returns true if any grid lines have been previously added and are on
	// screen
	protected boolean gridLinesExist() {
		if (studentProgram == null) {
			return false;
		}
		if (studentProgram instanceof GraphicsProgram) {
			GraphicsProgram graphicsProgram = (GraphicsProgram) studentProgram;
			for (Iterator<?> itr = graphicsProgram.iterator(); itr.hasNext();) {
				Object obj = itr.next();
				if (
						// obj instanceof GLine &&
						obj instanceof GObject &&
						GRID_LINE_COLOR.equals(((GObject) obj).getColor())) {
					return true;
				}
			}
		}
		return false;
	}

	// gets rid of all grid lines on screen
	protected void gridLineRemoveAll(GContainer container) {
		if (container == null) {
			return;
		}
		
		Set<GObject> toRemove = new HashSet<GObject>();
		for (int i = 0; i < container.getElementCount(); i++) {
			GObject gobj = container.getElement(i);
			if (
					// gobj instanceof GLine && 
					GRID_LINE_COLOR.equals(gobj.getColor())) {
				toRemove.add(gobj);
			}
		}
		for (GObject gobj : toRemove) {
			container.remove(gobj);
		}
	}

	// gets rid of all grid lines on screen
	protected void gridLineRemoveAll() {
		if (studentProgram == null) {
			return;
		}
		
		if (studentProgram instanceof GraphicsProgram) {
			GraphicsProgram graphicsProgram = (GraphicsProgram) studentProgram;
			Set<GObject> toRemove = new HashSet<GObject>();
			for (Iterator<?> itr = graphicsProgram.iterator(); itr.hasNext();) {
				Object obj = itr.next();
				if (obj instanceof GObject && ((GObject) obj).getColor().equals(GRID_LINE_COLOR)) {
					toRemove.add((GObject) obj);
				}
			}
			for (GObject gobj : toRemove) {
				graphicsProgram.remove(gobj);
			}
		}
	}

	// adds the given string message to the bottom of the autograder's message
	// log
	protected void printlnLog(String msg) {
		String text = messageLog.getText();
		if (text.length() > 0) {
			text += "\n";
		}
		text += msg;
		messageLog.setText(text);
		messageLog.setCaretPosition(text.length());
		System.out.println(msg);
	}

	// tells student's pause() method to scale its actual pause time by
	// various amounts depending on the slider's current value
//	protected void scaleTimeBySlider(int value) {
//		if (studentProgram == null) {
//			return;
//		}
//		if (value == 1) {
//			hangman.setPauseScaleFactor(0.0); // stop time
//		} else if (value == 2) {
//			hangman.setPauseScaleFactor(4.0);
//		} else if (value == 3) {
//			hangman.setPauseScaleFactor(2.0);
//		} else if (value == 4) {
//			hangman.setPauseScaleFactor(1.0);
//		} else if (value == 5) {
//			hangman.setPauseScaleFactor(0.5);
//		} else if (value == 6) {
//			hangman.setPauseScaleFactor(0.25);
//		} else if (value == 7) {
//			hangman.setPauseScaleFactor(0.05);
//		}
//	}
	
	public void setAssertUsingGui(boolean gui) {
		this.assertUsingGui = gui;
	}
	
	protected class ReflectionPanel implements ActionListener {
		protected Class<?> clazz;
		protected Object object;
		
		@SuppressWarnings("rawtypes")
		protected JComboBox fieldBox;
		@SuppressWarnings("rawtypes")
		protected JComboBox constantBox;
		@SuppressWarnings("rawtypes")
		protected JComboBox methodBox;
		
		protected JTextField fieldValueFromBox;
		protected JTextField fieldValueToBox;
		protected JTextField constantValueFromBox;
		protected JTextField constantValueToBox;
		protected Map<String, Field> fieldTable = new TreeMap<String, Field>();
		protected Map<String, Field> constantTable = new TreeMap<String, Field>();
		protected Map<String, Method> methodTable = new TreeMap<String, Method>();
		
		public ReflectionPanel(Class<?> clazz) {
			this(clazz, null);
		}
		
		@SuppressWarnings({ "unchecked", "rawtypes" })
		public ReflectionPanel(Class<?> clazz, Object object) {
			this.clazz = clazz;
			this.object = object;
			
			fieldBox = new JComboBox();
			fieldBox.setEditable(false);
			fieldBox.setActionCommand(clazz.getName() + " Get field value");
			fieldBox.addActionListener(this);
			fieldBox.addItem("(select a field)");
			Field[] fields = clazz.getDeclaredFields();
			Arrays.sort(fields, new ReflectionUtils.FieldNameComparator());
			for (Field field : fields) {
				if (SUPPORTED_FIELD_TYPES.contains(field.getType())) {
					String fieldString = (Modifier.isStatic(field.getModifiers()) ? "static " : "")
							+ ReflectionUtils.getClassNameWithoutPackage(field.getType(), field.getGenericType())
							+ " " + field.getName();
					fieldTable.put(fieldString, field);
					fieldBox.addItem(fieldString);
				}
			}
			if (fieldBox.getItemCount() <= 1) {
				fieldBox.setEnabled(false);
			}
			fieldValueFromBox = new JTextField(8);
			fieldValueFromBox.setEditable(false);
			fieldValueToBox = new JTextField(8);
			fieldValueToBox.addActionListener(this);
			fieldValueToBox.setActionCommand(clazz.getName() + " Set field value");

			constantBox = new JComboBox();
			constantBox.setEditable(false);
			constantBox.setActionCommand(clazz.getName() + " Get constant value");
			constantBox.addActionListener(this);
			constantBox.addItem("(select a constant)");
			Field[] constants = clazz.getDeclaredFields();
			Arrays.sort(constants, new ReflectionUtils.FieldNameComparator());
			for (Field constant : constants) {
				if (Modifier.isStatic(constant.getModifiers()) && SUPPORTED_FIELD_TYPES.contains(constant.getType())) {
					String constantString = (Modifier.isStatic(constant.getModifiers()) ? "static " : "")
							+ ReflectionUtils.getClassNameWithoutPackage(constant
							.getType(), constant.getGenericType()) + " " + constant.getName();
					constantTable.put(constantString, constant);
					constantBox.addItem(constantString);
				}
			}
			if (constantBox.getItemCount() <= 1) {
				constantBox.setEnabled(false);
			}
			constantValueFromBox = new JTextField(8);
			constantValueFromBox.setEditable(false);
			constantValueToBox = new JTextField(8);
			constantValueToBox.addActionListener(this);
			constantValueToBox.setActionCommand(clazz.getName() + " Set constant value");

			methodBox = new JComboBox();
			methodBox.setEditable(false);
			methodBox.setActionCommand(clazz.getName() + " Get method value");
			methodBox.addActionListener(this);
			methodBox.addItem("(select a method)");
			Method[] methods = clazz.getDeclaredMethods();
			Arrays.sort(methods, new ReflectionUtils.MethodNameComparator());
			for (Method method : methods) {
				String methodString = (Modifier.isStatic(method.getModifiers()) ? "static " : "")
						+ ReflectionUtils.getClassNameWithoutPackage(method.getReturnType())
						+ " "
						+ method.getName()
						+ "()";
				methodTable.put(methodString, method);
				methodBox.addItem(methodString);
			}
			// methodBox.setPreferredSize(new Dimension(400,
			// methodBox.getPreferredSize().height));

		}
		
		public void doLayout() {
			// field get/set panel
			if (SHOW_REFLECTION_FIELDS) {
				addPanel(clazz.getName() + " fields",
						GuiUtils.createLabel(clazz.getName() + " fields: ", labelWidth), fieldBox,
						new JLabel(" from "), fieldValueFromBox, new JLabel(" to "), fieldValueToBox,
						GuiUtils.createButton("Set", clazz.getName() + " Set field value", 'S', this));
			}
	
			// constant get/set panel
//			addPanel("Change Constant",
//					GuiUtils.createLabel("change constant: ", labelWidth), constantBox, new JLabel(
//					" from "), constantValueFromBox, new JLabel(" to "), constantValueToBox,
//					GuiUtils.createButton("Set", "Set constant value", ' ', this));
	
			// method call panel
			if (SHOW_REFLECTION_METHODS) {
				addPanel(clazz.getName() + " methods",
						GuiUtils.createLabel(clazz.getName() + " methods: ", labelWidth), methodBox,
						GuiUtils.createButton("Call", clazz.getName() + " Call method", 'C', this));
			}
		}
		
		public void setObject(Object object) {
			this.object = object;
		}
		
		public void actionPerformed(ActionEvent event) {
			String cmd = event.getActionCommand().intern();
			if (cmd.equals(clazz.getName() + " Get field value")) {
				currentFieldValueGet();
			} else if (cmd.equals(clazz.getName() + " Set field value")) {
				currentFieldValueSet();
			} else if (cmd.equals(clazz.getName() + " Get constant value")) {
				currentConstantValueGet();
			} else if (cmd.equals(clazz.getName() + " Set constant value")) {
				currentConstantValueSet();
			} else if (cmd.equals(clazz.getName() + " Call method")) {
				currentMethodCall();
			}
		}
	
		// updates the constant value field to show the currently selected
		// constant's value
		protected void currentFieldValueGet() {
			String fieldString = String.valueOf(fieldBox.getItemAt(fieldBox.getSelectedIndex()));
			if (fieldString == null || fieldString.equals("(select a field)")) {
				return;
			}
			Field field = fieldTable.get(fieldString);
			if (object == null && !Modifier.isStatic(field.getModifiers())) {
				return;
			}
			String fieldValueText = StringUtils.fitToWidth(String.valueOf(ReflectionUtils.getFieldValue(object, field)), 80);
			String currentText = fieldValueFromBox.getText();
			if (currentText == null || !currentText.equals(fieldValueText)) {
				fieldValueFromBox.setText(fieldValueText);
				fieldValueFromBox.setCaretPosition(0);
			}
		}

		// sets the currently selected constant's value to match the value typed
		// into the constant value text field
		protected void currentFieldValueSet() {
			String fieldString = roundDouble(String.valueOf(fieldBox.getItemAt(fieldBox
					.getSelectedIndex())));
			if (fieldString == null || fieldString.equals("(select a field)")) {
				return;
			}
			Field field = fieldTable.get(fieldString);
			if (object == null && !Modifier.isStatic(field.getModifiers())) {
				return;
			}
			String valueStr = fieldValueToBox.getText();
			try {
				Object value = ReflectionUtils.parseValue(field.getType(), field.getGenericType(), valueStr);
				ReflectionUtils.setFieldValue(object, field, value);
			} catch (NumberFormatException nfe) {
				JOptionPane.showMessageDialog(autograderFrame, "Error trying to read value:\n\n" + nfe, "Parse error", JOptionPane.ERROR_MESSAGE);
			} catch (IllegalArgumentException nfe) {
				JOptionPane.showMessageDialog(autograderFrame, "Error trying to read value:\n\n" + nfe, "Parse error", JOptionPane.ERROR_MESSAGE);
			}
		}

		private String roundDouble(String s) {
			try {
				double d = Double.parseDouble(s);
				d = Math.round(d * 10000.0) / 10000.0;
				return String.valueOf(d);
			} catch (NumberFormatException e) {
				return s;
			}
		}

		// updates the constant value field to show the currently selected
		// constant's value
		protected void currentConstantValueGet() {
			String constantString = String
					.valueOf(constantBox.getItemAt(constantBox.getSelectedIndex()));
			if (constantString == null || constantString.equals("(select a constant)")) {
				return;
			}
			Field constant = constantTable.get(constantString);
			String constantValueText = String.valueOf(ReflectionUtils.getConstantValue(
					STUDENT_CLASS, constant.getName()));
			String currentText = constantValueFromBox.getText();
			if (currentText == null || !currentText.equals(constantValueText)) {
				constantValueFromBox.setText(constantValueText);
			}
		}

		// sets the currently selected constant's value to match the value typed
		// into the constant value text field
		protected void currentConstantValueSet() {
			String constantString = String
					.valueOf(constantBox.getItemAt(constantBox.getSelectedIndex()));
			if (constantString == null || constantString.equals("(select a constant)")) {
				return;
			}
			Field constant = constantTable.get(constantString);
			String valueStr = constantValueToBox.getText();
			Object value = ReflectionUtils.parseValue(constant.getType(), constant.getGenericType(), valueStr);
			ReflectionUtils.setConstantValue(STUDENT_CLASS, constant.getName(), value);
		}

		protected void currentMethodCall() {
			String methodString = String.valueOf(methodBox.getItemAt(methodBox.getSelectedIndex()));
			if (methodString == null || methodString.equals("(select a method)")) {
				return;
			}
			Method method = methodTable.get(methodString);
			if (object == null && !Modifier.isStatic(method.getModifiers())) {
				return;
			}
			Object[] paramValues = ReflectionUtils.getDefaultArgs(method);
			try {
				method.setAccessible(true);
				method.invoke(object, paramValues);
			} catch (Throwable t) {
				printlnLog("Error running method " + method.getName() + ": " + t);
				t.printStackTrace();
			}
		}
	}
	
	protected void updateAllReflectionPanels() {
		for (ReflectionPanel panel : reflectionPanels.values()) {
			panel.currentFieldValueGet();
			panel.currentConstantValueGet();
		}
	}
	
	
	private class FieldValueUpdateRunnable implements Runnable {
		public void run() {
			while (true) {
				try {
					Thread.sleep(500);
				} catch (InterruptedException ie) {
					// empty
				}
				updateAllReflectionPanels();
			}
		}
	}

	protected class StudentProgramRunnerThread extends Thread {
		private boolean callInitAndRun;
		public boolean started = false;
		private Program program;
		private Class<?> programClass;

		public StudentProgramRunnerThread(Class<?> clazz, boolean callInitAndRun) {
			this(clazz, DEFAULT_WIDTH, DEFAULT_HEIGHT, callInitAndRun);
		}
		
		public StudentProgramRunnerThread(Class<?> clazz, int width, int height, boolean callInitAndRun) {
			this.setName(getClass().getName() + "-" + clazz.getName() + "@" + hashCode());
			this.programClass = clazz;
			
			this.callInitAndRun = callInitAndRun;
			try {
				if (Program.class.isAssignableFrom(clazz)) {
					this.program = (Program) clazz.getDeclaredConstructor().newInstance();
					this.program.setExitOnClose(false);
					ReflectionPanel panel = reflectionPanels.get(STUDENT_CLASS);
					if (panel != null) {
						panel.setObject(this.program);
					}
				}
			} catch (InstantiationException ie) {
				throw new ReflectionRuntimeException(ie);
			} catch (IllegalAccessException iae) {
				throw new ReflectionRuntimeException(iae);
			} catch (InvocationTargetException ite) {
				throw new ReflectionRuntimeException(ExceptionUtils.getUnderlyingCause(ite));
			} catch (NoSuchMethodException nsme) {
				throw new ReflectionRuntimeException(nsme);
			}
		}

		public Program getProgram() {
			return program;
		}
		
		public JFrame getFrame() {
			return program.getJFrame();
		}

		@SuppressWarnings("deprecation")
		public void killMe() {
			if (program != null) {
				program.stop();
				if (program.getWindow() != null) {
					program.getWindow().setVisible(false);
					program.getWindow().dispose();
					program.setVisible(false);
				}
			}
		}

		public void run() {
			if (callInitAndRun) {
				if (program != null && Program.class.isAssignableFrom(programClass)) {
					// run as a spl Program
					started = true;
					program.start();
				} else {
					// run as a standard Java main method
					try {
						String[] args = new String[0];
						Method mainMethod = programClass.getMethod("main", args.getClass());
						if (mainMethod != null) {
							mainMethod.invoke(null, (Object) args);
							started = true;
						}
					} catch (IllegalAccessException e) {
						e.printStackTrace();
					} catch (IllegalArgumentException e) {
						e.printStackTrace();
					} catch (InvocationTargetException e) {
						Throwable cause = ExceptionUtils.getUnderlyingCause(e);
						throw new RuntimeException(cause);
					} catch (NoSuchMethodException e) {
						// empty
					}
				}
				
				if (!started) {
					throw new IllegalStateException("class " + programClass.getName()
							+ " does not extend Program and does not seem to have a suitable main() method. Cannot run.");
				}
				// GuiUtils.rememberWindowLocation(program.getWindow());
			}
		}
	}
}

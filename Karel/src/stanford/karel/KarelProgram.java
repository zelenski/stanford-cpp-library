/*
 * File: KarelProgram.java
 * -----------------------
 * This file implements the KarelProgram class.
 * - 2015/03/31: Commented out unused field 'program'.
 * - 2015/03/31: Changed to use Swing graphical components.
 * - 2015/03/31: Improved error dialog to display program stack trace.
 * - 2015/03/31: Added setStatus method to display program state.
 */

package stanford.karel;

import acm.program.*;
import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;

/**
 * This class is a subclass of <code><a href="Program.html">Program</a></code>
 * that runs a Karel program.
 */
public class KarelProgram extends Program {

	public static final int NORTH = KarelWorld.NORTH;
	public static final int EAST = KarelWorld.EAST;
	public static final int SOUTH = KarelWorld.SOUTH;
	public static final int WEST = KarelWorld.WEST;

	public static final int INFINITE = KarelWorld.INFINITE;

	public static final int SIMPLE = KarelWorld.SIMPLE;
	public static final int FANCY = KarelWorld.FANCY;

	public static final Color BLACK = Color.BLACK;
	public static final Color BLUE = Color.BLUE;
	public static final Color CYAN = Color.CYAN;
	public static final Color DARK_GRAY = Color.DARK_GRAY;
	public static final Color GRAY = Color.GRAY;
	public static final Color GREEN = Color.GREEN;
	public static final Color LIGHT_GRAY = Color.LIGHT_GRAY;
	public static final Color MAGENTA = Color.MAGENTA;
	public static final Color ORANGE = Color.ORANGE;
	public static final Color PINK = Color.PINK;
	public static final Color RED = Color.RED;
	public static final Color WHITE = Color.WHITE;
	public static final Color YELLOW = Color.YELLOW;

	/* Constructor: KarelProgram() */
	/**
	 * Creates a new Karel program.
	 */

	public KarelProgram() {
		world = createWorld();
		world.setRepaintFlag(false);
		world.setDisplayFlag(false);
		world.init(10, 10);
		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout());
		panel.add(BorderLayout.CENTER, world);
		controlPanel = new KarelControlPanel(this);
		world.setMonitor(controlPanel);
		panel.add(BorderLayout.WEST, controlPanel);
		add(panel);
		
		// set up status label
		statusLabel = new JLabel("Welcome to Karel!");
		add(statusLabel, BorderLayout.SOUTH);
		
		validate();
	}

	/* Method: main() */
	/**
	 * Contains the code to be executed for each specific program subclass. If
	 * you are defining your own <code>KarelProgram</code> class, you need to
	 * override the definition of <code>main</code> so that it contains the code
	 * for your application.
	 */

	public void main() {
		/* Empty */
	}
	
	public void setStatus(String status) {
		statusLabel.setText(status);
	}

	/* Method: getWorld() */
	/**
	 * Returns the <code>KarelWorld</code> object in which Karel lives.
	 * 
	 * @return The <code>KarelWorld</code> object in which Karel lives
	 */

	public KarelWorld getWorld() {
		return world;
	}

	/* Static method: getWorldDirectory() */
	/**
	 * Returns the default directory in which Karel's worlds live.
	 * 
	 * @return The directory in which Karel's worlds lives
	 */

	public static String getWorldDirectory() {
		String dir = System.getProperty("user.dir");
		if (new File(dir, "worlds").isDirectory()) {
			dir += "/worlds";
		}
		return dir;
	}

	/* Factory method: createWorld() */
	/**
	 * Creates the <code>KarelWorld</code> in which Karel lives. Subclasses can
	 * override this method to create their own <code>KarelWorld</code> types.
	 * 
	 * @return The <code>World</code> object in which Karel lives
	 * @noshow student
	 */

	protected KarelWorld createWorld() {
		return new KarelWorld();
	}

	/* Protected method: isStarted() */
	/**
	 * Checks to see whether this program has started.
	 * 
	 * @noshow student
	 */

	protected boolean isStarted() {
		if (world == null || !super.isStarted())
			return false;
		Dimension size = world.getSize();
		return (size != null) && (size.width != 0) && (size.height != 0);
	}

	protected Karel getKarel() {
		return world.getKarel();
	}

	public void add(Karel karel) {
		add(karel, 1, 1, KarelWorld.EAST, KarelWorld.INFINITE);
	}

	public void add(Karel karel, int x, int y, int dir, int nBeepers) {
		karel.setLocation(x, y);
		karel.setDirection(dir);
		karel.setBeepersInBag(nBeepers);
		world.add(karel);
	}

	public void start(String[] args) {
		super.start(args);
	}

	protected void setStartupObject(Object obj) {
		super.setStartupObject(obj);
	}

	protected void startRun() {
		Karel karel = null;
		String karelClass = getParameter("karel");
		if (karelClass == null) {
			karel = (Karel) getStartupObject();
			karelClass = karel.getClass().getName();
			karelClass = karelClass.substring(karelClass.lastIndexOf(".") + 1);
		} else {
			try {
				karel = (Karel) Class.forName(karelClass).newInstance();
			} catch (Exception ex) {
				System.out.println("Exception: " + ex);
			}
		}
		if (karel != null) {
			world.add(karel);
			setTitle(karelClass);
			String worldName = getParameter("world");
			if (worldName == null)
				worldName = karelClass;
			try {
				URL url = new URL(getCodeBase(), "worlds/" + worldName + ".w");
				URLConnection connection = url.openConnection();
				world.load(new InputStreamReader(connection.getInputStream()));
			} catch (Exception ex) {
				/* Ignore this error */
			}
		}
		world.setRepaintFlag(true);
		world.setDisplayFlag(true);
		world.repaint();
		while (true) {
			started = false;
			synchronized (this) {
				while (!started) {
					try {
						wait();
					} catch (InterruptedException ex) {
						/* Empty */
					}
				}
			}
			try {
				if (karel == null) {
					main();
				} else {
					karel.run();
					setStatus("Finished running.");
				}
				world.setRepaintFlag(true);
				world.repaint();
			} catch (Exception ex) {
				setStatus("Halted due to an error.");
				if (errorDialog == null) {
					errorDialog = new KarelErrorDialog(this);
				}
				String errorMessage = ex.getMessage();
				while (!errorMessage.endsWith("\n\n")) {
					errorMessage += "\n";
				}
				ByteArrayOutputStream bytes = new ByteArrayOutputStream();
				PrintStream out = new PrintStream(bytes);
				ex.printStackTrace(out);
				String stackTrace = bytes.toString();
				errorDialog.error(errorMessage, stackTrace);
			}
		}
	}

	void signalStarted() {
		synchronized (this) {
			started = true;
			notifyAll();
		}
	}

	/* Private state */

	private KarelWorld world;
	private KarelControlPanel controlPanel;
	private KarelErrorDialog errorDialog;
	private JLabel statusLabel;
	private boolean started;
}

/* Private class: KarelErrorDialog */
/**
 * The <code>KarelErrorDialog</code> class is used to display error messages in
 * Karel's world.
 */
class KarelErrorDialog extends JDialog implements WindowListener,
		ActionListener {

	private Frame parent;

	public KarelErrorDialog(KarelProgram program) {
		super(JTFTools.getEnclosingFrame(program.getWorld()), true);
		parent = JTFTools.getEnclosingFrame(program.getWorld());
		setLayout(new BorderLayout(10, 10));
		// this.program = program;
		init();
	}

	private void init() {
		setSize(DIALOG_WIDTH, DIALOG_HEIGHT);
		setFont(DIALOG_FONT);
		setBackground(DIALOG_BGCOLOR);
		setResizable(false);
		addWindowListener(this);
		setLayout(new BorderLayout());
		HPanel hbox = new HPanel();
		hbox.setBackground(DIALOG_BGCOLOR);
		VPanel vbox = new VPanel();
		vbox.setBackground(DIALOG_BGCOLOR);
		bugIcon = new KarelBugIcon();
		okButton = new JButton("OK");
		okButton.addActionListener(this);
		okButton.setMnemonic('O');
		okButton.requestFocus();
		errorDisplay = new KarelErrorCanvas();
		errorDisplay.setFont(DIALOG_FONT);
		hbox.add("/width:" + LOGO_WIDTH + "/height:" + LOGO_HEIGHT, bugIcon);
		hbox.add("/stretch:both", errorDisplay);
		vbox.add("/stretch:both", hbox);
		vbox.add("/top:3/bottom:3/width:" + BUTTON_WIDTH + "/center", okButton);
		add(BorderLayout.CENTER, vbox);
		validate();
		pack();
		
		// center with respect to parent
		int centerX = (int) ((parent.getX() + parent.getWidth()  / 2) - DIALOG_WIDTH  / 2);
		int centerY = (int) ((parent.getY() + parent.getHeight() / 2) - DIALOG_HEIGHT / 2);
		setLocation(centerX, centerY);
	}

	public void error(String msg) {
		error(msg, "");
	}
	
	public void error(String msg, String stackTrace) {
		errorDisplay.setText(msg);
		errorDisplay.setStackTrace(stackTrace);
		setVisible(true);
	}

	/* WindowListener interface */

	public void windowClosing(WindowEvent e) {
		setVisible(false);
	}

	public void windowOpened(WindowEvent e) {
	}

	public void windowClosed(WindowEvent e) {
	}

	public void windowIconified(WindowEvent e) {
	}

	public void windowDeiconified(WindowEvent e) {
	}

	public void windowActivated(WindowEvent e) {
	}

	public void windowDeactivated(WindowEvent e) {
	}

	/* ActionListener interface */

	public void actionPerformed(ActionEvent e) {
		Component source = (Component) e.getSource();
		if (source == okButton)
			windowClosing(null);
	}

	/* Private constants */

	private static final int DIALOG_WIDTH = 630;
	private static final int DIALOG_HEIGHT = 370;
	private static final int LOGO_WIDTH = 100;
	private static final int LOGO_HEIGHT = 100;
	private static final int BUTTON_WIDTH = 60;
	private static final Font DIALOG_FONT = new Font("SansSerif", Font.PLAIN, 12);
	// private static final Font BOLD_FONT = new Font("SansSerif", Font.BOLD, 12);
	public static final Color DIALOG_BGCOLOR = new Color(255, 96, 96);   // Color.RED;

	/* Private state */

	// private KarelProgram program;
	private KarelBugIcon bugIcon;
	private JButton okButton;
	private KarelErrorCanvas errorDisplay;

}

class KarelErrorCanvas extends JPanel {

	private JLabel errorLabel;
	private JTextArea stackTraceArea;
	
	public KarelErrorCanvas() {
		/* Empty */
		setLayout(new BorderLayout(10, 10));
		setOpaque(false);
		errorLabel = new JLabel("");
		stackTraceArea = new JTextArea(8, 50);
		stackTraceArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
		stackTraceArea.setBorder(null);
		stackTraceArea.setEditable(false);
		stackTraceArea.setFocusable(false);
		stackTraceArea.setBackground(KarelErrorDialog.DIALOG_BGCOLOR);
		add(errorLabel, BorderLayout.NORTH);
		add(new JScrollPane(stackTraceArea), BorderLayout.CENTER);
		setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
	}

	public void setText(String msg) {
		// errorText = msg;
		errorLabel.setText("Error: " + msg);
		// repaint();
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
			line = line.replaceAll("\\([A-Za-z0-9_$]+.java:([0-9]+)\\)", ", line $1");
			line = line.replaceAll("[ \t]+at ", "");
			if (line.contains("Exception: ")) {
				continue;
			}
			stackTraceOut += line + "\n";
		}
		
		if (!stackTraceOut.isEmpty()) {
			stackTraceOut = stackTraceOut.trim();
		}
		
		this.stackTrace = stackTraceOut;
		stackTraceArea.setText(this.stackTrace);
	}

	/*
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		if (g instanceof Graphics2D) {
			Graphics2D g2 = (Graphics2D) g;
			g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		}
		FontMetrics fm = g.getFontMetrics();
		int x = LEFT_MARGIN;
		int y = TOP_MARGIN + fm.getAscent();
		int height = fm.getHeight();
		int limit = getSize().width - RIGHT_MARGIN;
		
		if (!this.stackTrace.isEmpty()) {
			String[] lines = (errorText + this.stackTrace).split("\n");
			int lineCount = 0;
			for (String line : lines) {
				StringTokenizer tokenizer = new StringTokenizer(line, " ", true);
				while (tokenizer.hasMoreTokens()) {
					String word = tokenizer.nextToken();
					int width = fm.stringWidth(word);
					if (x + width > limit) {
						if (x > LEFT_MARGIN && !word.equals(" ")) {
							x = LEFT_MARGIN;
							y += height;
						}
					}
					g.drawString(word, x, y);
					x += width;
				}
				x = LEFT_MARGIN;
				if (lineCount > 1) {
					x += LEFT_MARGIN;
				}
				y += height;
			}
		}
	}
	*/

	/* Private constants */

	// private static final int LEFT_MARGIN = 5;
	// private static final int RIGHT_MARGIN = 5;
	// private static final int TOP_MARGIN = 40;

	/* Private state */

	// private String errorText;
	private String stackTrace;

}

class KarelBugIcon extends JComponent {
	public KarelBugIcon() {
		image = MediaTools.createImage(KAREL_BUG_IMAGE);
		image = MediaTools.loadImage(image);
		setOpaque(false);
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(image, 0, 0, this);
	}

	private String[] KAREL_BUG_IMAGE = {
			"47494638396146005400910000FFFFFFFFCBFF77777700000021F90401000001",
			"002C00000000460054004102FF8C8F19C3ED0FA374AADA3B80DE5BF82F70E238",
			"8220305CEA9195E62782C8DB76E198AE4ACD79839778093DB61B6993D32178C5",
			"A19308781D4549A5A106D3540DCEE8B07522552790690FDA0C6BBA6BE838B233",
			"4F6F1010230B3369AD15A6FC0F88C337B82241788868416698D817F808B8D5E8",
			"67E302A422544975888527C0259546D366244848E975E2F904846A3AE8C7CA4A",
			"9A97257668CB41F671F7F5C386B4B4D810588A1663B4EA89220C1107F9578A5C",
			"14DCC8028D7D24696D35ECCCBD942DCE0C7E3D7EBE7D3A67FCC1A7A749CEB9FE",
			"4E6A31045F0D4BED951A9A0A6AA293B47C7C6285F8055096AD5FC75E153CE224",
			"80427A72D2A9A03451E18240FF163140CCA8A65FAE5BEACEF850304BE1948E8A",
			"526D4249EF1EC39004BD35FC085254A63D2CBCB93A63925D4652F116C049F0B3",
			"075012B6D07479F32DDC39A254F1A181FAC0D154683759EA48BAD58CD772488F",
			"923DDBC72CDAB5577C36606BCDEDDBB360C3F2A46BF7D1D86E7921ED2D6486A2",
			"383D7F5B7EECC54BEECC778591AE43766985BF5C8DAFCC53D3CEDE4E7E2FF531",
			"DDDC0FE6CE9145F91255594FE2A87D9D1FD26AA3DA9FC8D52E497AD67409E428",
			"D2BAE44D43981075BF8ABE21BF08CE86B743D3963281AC5B5A09C677BA3F6D8C",
			"543C68F5D91CB36BCF49EBE672E99FAB8798B5D2BB09D1F7D2E06BCD7CB8A427",
			"E17D88D4E69B7285D1E0E14B8F279D4E185D00D35B4F8B28579E3298D157934F",
			"80C4D41E836160B5C883C7C8321B757434334C31262588213F14CE2555344B59",
			"4295730771488163D888D7063CED8DC8C0336135E5143B0D4665CE39170EC459",
			"8C7719C5A3657D05C65A74F11DE9826D8840C7E49089407964658A2886252370",
			"61A0D6966469E9E5975D86C90D96649EA2D899B0A4A966376CAA50000021FF0B",
			"4D414347436F6E2004031039000000015772697474656E20627920474946436F",
			"6E76657274657220322E342E33206F66204D6F6E6461792C204D61792032352C",
			"2031393938003B" };

	private Image image;

}

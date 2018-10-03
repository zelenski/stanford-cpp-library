/*
 * This class implements Karel's world.
 * 
 * @author Marty Stepp (based on Eric Roberts version)
 * @version 2017/04/04
 * - added toImage, saveToImage methods
 * @version 2016/04/16
 * - added interactive keyboard-controlled mode
 * @version 2015/04/05
 * - added Ms. Karel mode
 * @version 2015/03/31
 * - commented out unused method updateCorner
 * - commented out unused fields and local variables
 * - changed to use Swing graphical components
 */

package stanford.karel;

import acm.graphics.GraphicsUtils;
import acm.program.CommandLineProgram;
import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.text.*;
import java.util.*;
import javax.swing.*;

public class KarelWorld extends JComponent {
	// see also: KarelDirection enum
	public static final int NORTH = 0;
	public static final int EAST = 1;
	public static final int SOUTH = 2;
	public static final int WEST = 3;
	public static final int NORTHEAST = 10;
	public static final int NORTHWEST = 11;
	public static final int SOUTHEAST = 12;
	public static final int SOUTHWEST = 13;
	public static final int CENTER = 14;

	public static final int INFINITE = 99999999;
	public static final int PLUS1 = -1;
	public static final int MINUS1 = -2;
	public static final int BLANKB = -3;

	public static final int SIMPLE = 0;
	public static final int FANCY = 1;

	public static final int LEFT_NUMBER_MARGIN = 16;
	public static final int BOTTOM_NUMBER_MARGIN = 15;
	public static final int DOUBLE_WALL_THRESHOLD = 24;
	public static final int CROSS_THRESHOLD = 11;
	public static final int NUMBER_THRESHOLD = 15;
	public static final Font NUMBER_FONT = new Font("SansSerif", Font.PLAIN, 13);
	public static final Font BEEPER_NUMBER_FONT = new Font("SansSerif",
			Font.PLAIN, 11);
	public static final double WALL_FRACTION = 0.30;
	public static final double WALL_TOLERANCE = 0.15;

	public static final int MAX_WIDTH = 50;
	public static final int MAX_HEIGHT = 50;

	public static final int MAX_DISPLAY_WIDTH = (6 * MAX_WIDTH)
			+ LEFT_NUMBER_MARGIN;
	public static final int MAX_DISPLAY_HEIGHT = (6 * MAX_HEIGHT)
			+ BOTTOM_NUMBER_MARGIN;

	public static final boolean TOKEN_TRACE = false;

	public static final Color BEEPER_COLOR = Color.LIGHT_GRAY;
	public static final Color MARKED_COLOR = Color.DARK_GRAY;
	public static final int BEEPER_BORDER = 1;
	public static final String BEEPER_FONT_FAMILY = "SansSerif";
	public static final int MIN_FANCY = 20;
	public static final int MIN_BEEPER = 4;
	public static final int MIN_LABEL = 15;
	public static final double BEEPER_FRACTION = 0.70;
	public static final double SIMPLE_FRACTION = 0.70;

	/* Private constants */
	private static final String INFINITY[] = {
			"47494638396109000600F70000FFFFFF98009833999998980011111122222200",
			"0054CBFFCB0032980033660033CC0033FE003232663300666600006598009898",
			"00CC9900FE99329800659800CC0099FE0098659898999999CC9900FE98009800",
			"329800659900CC9800FE3399CB3399FF99993398986598320098650099339998",
			"659833CB9833FF9999CC0099FE00336699656698CC9898FF9999323200336600",
			"32003233006632009833339965009866339900663300983200666600986500CC",
			"3300FE3200CC6600FE65CCCC98CCFF99FFCC99FFFF993300CC3200FE6600CC65",
			"00FECC0033CC0066FE0032FE0065339933339966669933669865CC00CCCB00FE",
			"FE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966FF9933FF98653333",
			"33326532323265326565660033653232660066653265CC3300CC6600FE3200FE",
			"65000066CC0099CC0066FE0098FE00CCCC00FECB00CCFE00FEFE33CC0033FE00",
			"66CC0066FE00CB3398CC6699FF3399FF659866CC9965FF9898CC9899FF99CCCC",
			"00CCFE00FECB00FEFE009933339966339933669865659833CB9966CC9933FF98",
			"65FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698FF65CC98CCCCCCCC",
			"CC99FFCBCBFFFF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533",
			"CB6666CC6633FF6565FFCB3333CB6533CB3365CC6666FF3333FF6633FF3366FF",
			"656533CB3333FF3333CB6633FF6666CB3366FF3366CC6665FF65CB33CBCC66CC",
			"CC33FFCC65FFFF33CCFF65CCFF33FFFF65FF66CCCC65FFCC65CCFF65FFFF98CC",
			"CC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC33FFFF33FFCC65FF",
			"FF65444444656532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000",
			"000098000066777777888888AAAAAABBBBBB5555556666660000100000224400",
			"005400000000CC0000DC0000EE0000FE00003200004400880000980000AA0000",
			"BA0000CC0000DC0000EE0000FE00CC0000DC0000EE0000FE0000004400005400",
			"006600007600220000320000AA0000BA00000022000032007600008800000000",
			"AA0000BA00007600008800000021F90401000090002C0000000009000600C7FF",
			"FFFF980098339999989800111111222222000054CBFFCB0032980033660033CC",
			"0033FE00323266330066660000659800989800CC9900FE99329800659800CC00",
			"99FE0098659898999999CC9900FE98009800329800659900CC9800FE3399CB33",
			"99FF99993398986598320098650099339998659833CB9833FF9999CC0099FE00",
			"336699656698CC9898FF99993232003366003200323300663200983333996500",
			"9866339900663300983200666600986500CC3300FE3200CC6600FE65CCCC98CC",
			"FF99FFCC99FFFF993300CC3200FE6600CC6500FECC0033CC0066FE0032FE0065",
			"339933339966669933669865CC00CCCB00FEFE00CBFE00FE6699CC6598FF9898",
			"CC9999FFCB9833CC9966FF9933FF986533333332653232326532656566003365",
			"3232660066653265CC3300CC6600FE3200FE65000066CC0099CC0066FE0098FE",
			"00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB3398CC6699FF33",
			"99FF659866CC9965FF9898CC9899FF99CCCC00CCFE00FECB00FEFE0099333399",
			"66339933669865659833CB9966CC9933FF9865FF33CBCB33FFCC33CCFF33FFFF",
			"99CB3399FF3399CC6698FF65CC98CCCCCCCCCC99FFCBCBFFFF99CCFFCBCBFF99",
			"FFFFCBFF3333CB3366CB3333FF3366FF6533CB6666CC6633FF6565FFCB3333CB",
			"6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF3333CB6633FF66",
			"66CB3366FF3366CC6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33",
			"FFFF65FF66CCCC65FFCC65CCFF65FFFF98CCCC99FFCC99CCFF99FFFFCBCB33CC",
			"FF33CCCC66CCFF65FFCC33FFFF33FFCC65FFFF65444444656532DDDDDDCBFFFF",
			"FFFFCBEEEEEE100000980000001000660000000098000066777777888888AAAA",
			"AABBBBBB5555556666660000100000224400005400000000CC0000DC0000EE00",
			"00FE00003200004400880000980000AA0000BA0000CC0000DC0000EE0000FE00",
			"CC0000DC0000EE0000FE0000004400005400006600007600220000320000AA00",
			"00BA00000022000032007600008800000000AA0000BA00007600008800000008",
			"190021091C4810D2BF7F06110A54C870E0C1840E174A2C4830200021FF0B4D41",
			"4347436F6E2004031039000000015772697474656E20627920474946436F6E76",
			"657274657220322E342E33206F66204D6F6E6461792C204D61792032352C2031",
			"393938003B" };

	private static final int KAREL_INSET = 6;             // px to shrink Karel size relative to his corner size 
	private static final double BODY_OFFSET_X = -0.20;
	private static final double BODY_OFFSET_Y = -0.33;
	private static final double BODY_WIDTH = 0.60;        // fraction of corner width occupied by Karel's body outline
	private static final double BODY_HEIGHT = 0.80;       // fraction of corner height occupied by Karel's body outline
	private static final double UPPER_NOTCH = 0.15;       // what fraction of Karel's body is beveled on U/R edge?
	private static final double LOWER_NOTCH = 0.10;       // what fraction of Karel's body is beveled on B/L edge?
	private static final double SCREEN_OFFSET_X = -0.07;
	private static final double SCREEN_OFFSET_Y = -0.05;
	private static final double SCREEN_WIDTH = 0.30;
	private static final double SCREEN_HEIGHT = 0.40;
	private static final double SLOT_WIDTH = 0.15;        // fraction of body width used by Karel's "disk" slot line
	private static final double FOOT_WIDTH = 0.08;        // size of Karel's feet relative to body 
	private static final double FOOT_LENGTH = 0.20;
	private static final double UPPER_ANKLE = 0.08;
	private static final double LOWER_ANKLE = 0.08;

	private static Image infinityImage;

	private StreamTokenizer tokenizer;
	private KarelWorldMonitor monitor;
	private KarelInterface activeKarel;
	private KarelInterface lastKarel;
	private boolean repaintFlag;
	private boolean displayFlag;
	private boolean editMode;
	private boolean numberSquaresFlag;
	private boolean displayOneFlag;
	private boolean msKarel;            // default false
	private boolean interactiveMode;    // default false
	private int cols;
	private int rows;
	private int sqSize;
	private int forcedSize;
	private int alignment;
	private int width;
	private int height;
	private int widthPx;
	private int heightPx;
	private int leftMargin;
	private int bottomMargin;
	private String lastClick;
	private String pathname;
	private String title;
	private Corner[][] map;
	private int look;
	private int lastBeeperCount;
	private NumberFormat speedFormat;
	private ArrayList<KarelInterface> karels;
	private Object sizeLock;
	private Image offscreen;

	/* Constructor */
	public KarelWorld() {
		sizeLock = new Object();
		setTitle("Karel World");
		setBackground(Color.WHITE);
		KarelWorldListener listener = new KarelWorldListener();
		addMouseListener(listener);
		addMouseMotionListener(listener);
		addComponentListener(listener);
		addKeyListener(listener);
		setFocusable(true);
		speedFormat = NumberFormat.getInstance();
		speedFormat.setMinimumIntegerDigits(1);
		speedFormat.setMaximumIntegerDigits(1);
		speedFormat.setMinimumFractionDigits(2);
		speedFormat.setMaximumFractionDigits(2);
		forcedSize = 0;
		numberSquaresFlag = true;
		displayOneFlag = false;
		msKarel = false;
		look = FANCY;
		alignment = CENTER;
		karels = new ArrayList<KarelInterface>();
		setDisplayFlag(true);
		setRepaintFlag(true);
	}

	@Override
	public Image createImage(int width, int height) {
		if (CommandLineProgram.isHeadless()) {
			BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
			return img;
		} else {
			return super.createImage(width, height);
		}
	}

	public void init(int cols, int rows) {
		synchronized (sizeLock) {
			this.cols = cols;
			this.rows = rows;
			setDisplayParameters(cols, rows);
			editMode = false;
			map = new Corner[cols + 2][rows + 2];
			for (int x = 1; x <= cols + 1; x++) {
				for (int y = 1; y <= rows + 1; y++) {
					map[x][y] = new Corner();
					map[x][y].wallSouth = (y == 1) || (y == rows + 1);
					map[x][y].wallWest = (x == 1) || (x == cols + 1);
					map[x][y].color = null;
				}
			}
			repaint();
		}
	}

	public void add(KarelInterface karel) {
		if (karels.indexOf(karel) == -1) {
			karel.setWorld(this);
			karels.add(karel);
		}
		repaint();
	}

	public void remove(KarelInterface karel) {
		karels.remove(karel);
		karel.setWorld(null);
		repaint();
	}

	@SuppressWarnings("unchecked")
	public <K extends KarelInterface> K getKarel() {
		return (K) getKarel(0);
	}

	@SuppressWarnings("unchecked")
	public <K extends KarelInterface> K getKarel(int k) {
		if (k < 0 || k >= karels.size()) {
			throw new ErrorException("Illegal Karel index");
		}
		return (K) karels.get(k);
	}

	public int getKarelCount() {
		return karels.size();
	}

	@SuppressWarnings("unchecked")
	public <K extends KarelInterface> K getKarelOnSquare(int x, int y) {
		Iterator<KarelInterface> iterator = karels.iterator();
		while (iterator.hasNext()) {
			KarelInterface karel = iterator.next();
			Point pt = karel.getLocation();
			if (pt.x == x && pt.y == y)
				return (K) karel;
		}
		return null;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getTitle() {
		return title;
	}

	public void setPathname(String pathname) {
		this.pathname = pathname;
	}

	public String getPathname() {
		return pathname;
	}

	public void setDisplayFlag(boolean flag) {
		displayFlag = flag;
	}

	public void setRepaintFlag(boolean flag) {
		repaintFlag = flag;
	}

	public boolean getRepaintFlag() {
		return repaintFlag;
	}

	public boolean getNumberSquaresFlag() {
		return numberSquaresFlag;
	}

	public void setNumberSquaresFlag(boolean flag) {
		numberSquaresFlag = flag;
	}

	public int getAlignment() {
		return alignment;
	}

	public void setAlignment(int alignment) {
		this.alignment = alignment;
	}

	public int getLook() {
		return look;
	}

	public void setLook(int look) {
		this.look = look;
	}

	public void setDisplayOneFlag(boolean flag) {
		displayOneFlag = flag;
	}

	public String getPathName() {
		return pathname;
	}

	public void setPathName(String pathname) {
		this.pathname = pathname;
	}

	public boolean getEditMode() {
		return editMode;
	}

	public void setEditMode(boolean flag) {
		editMode = flag;
	}

	public void updateEditMode(boolean flag) {
		if (monitor == null)
			throw new ErrorException("No map editor defined");
		setEditMode(flag);
		repaint();
	}

	public void forceSquareSize(int size) {
		forcedSize = size;
	}

	public void reset() {
		/* Empty */
	}

	public int getSquareSize() {
		return sqSize;
	}

	public int getColumns() {
		return (cols);
	}

	public int getRows() {
		return (rows);
	}

	public boolean outOfBounds(Point pt) {
		return outOfBounds(pt.x, pt.y);
	}

	public boolean outOfBounds(int x, int y) {
		return (x < 1 || x > cols || y < 1 || y > rows);
	}

	public int getBeepersOnCorner(Point pt) {
		return getBeepersOnCorner(pt.x, pt.y);
	}

	public int getBeepersOnCorner(int x, int y) {
		return ((map[x][y]).nBeepers);
	}

	public void setBeepersOnCorner(Point pt, int nBeepers) {
		map[pt.x][pt.y].nBeepers = nBeepers;
		updateCorner(pt);
	}

	public void setBeepersOnCorner(int x, int y, int nBeepers) {
		setBeepersOnCorner(new Point(x, y), nBeepers);
	}

	public static int adjustBeepers(int nBeepers, int delta) {
		if (nBeepers == INFINITE)
			return INFINITE;
		return nBeepers + delta;
	}

	public static int setBeepers(int nBeepers, int delta) {
		if (delta == INFINITE)
			return INFINITE;
		if (delta == PLUS1) {
			return (nBeepers == INFINITE) ? INFINITE : nBeepers + 1;
		}
		if (delta == MINUS1) {
			return (nBeepers == INFINITE) ? INFINITE : Math
					.max(0, nBeepers - 1);
		}
		return delta;
	}

	public Color getCornerColor(Point pt) {
		return getCornerColor(pt.x, pt.y);
	}

	public Color getCornerColor(int x, int y) {
		return (map[x][y].color);
	}

	public void setCornerColor(Point pt, Color color) {
		map[pt.x][pt.y].color = color;
		updateCorner(pt);
	}

	public void setCornerColor(int x, int y, Color color) {
		setCornerColor(new Point(x, y), color);
	}

	public boolean checkWall(Point pt, int dir) {
		return checkWall(pt.x, pt.y, dir);
	}

	public boolean checkWall(int x, int y, int dir) {
		switch (dir) {
		case SOUTH:
			return (map[x][y].wallSouth);
		case WEST:
			return (map[x][y].wallWest);
		case NORTH:
			return (map[x][y + 1].wallSouth);
		case EAST:
			return (map[x + 1][y].wallWest);
		}
		return (false);
	}

	public void setWall(Point pt, int dir) {
		switch (dir) {
		case SOUTH:
			map[pt.x][pt.y].wallSouth = true;
			break;
		case WEST:
			map[pt.x][pt.y].wallWest = true;
			break;
		case NORTH:
			map[pt.x][pt.y + 1].wallSouth = true;
			break;
		case EAST:
			map[pt.x + 1][pt.y].wallWest = true;
			break;
		}
		updateCorner(pt);
	}

	public void setWall(int x, int y, int dir) {
		setWall(new Point(x, y), dir);
	}

	public void clearWall(Point pt, int dir) {
		switch (dir) {
		case SOUTH:
			map[pt.x][pt.y].wallSouth = false;
			break;
		case WEST:
			map[pt.x][pt.y].wallWest = false;
			break;
		case NORTH:
			map[pt.x][pt.y + 1].wallSouth = false;
			break;
		case EAST:
			map[pt.x + 1][pt.y].wallWest = false;
			break;
		}
		updateCorner(pt);
		if (sqSize >= DOUBLE_WALL_THRESHOLD) {
			Point left = adjacentPoint(pt, leftFrom(dir));
			Point right = adjacentPoint(pt, rightFrom(dir));
			updateCorner(left);
			updateCorner(right);
			updateCorner(adjacentPoint(left, dir));
			updateCorner(adjacentPoint(pt, dir));
			updateCorner(adjacentPoint(right, dir));
		}
	}

	public void clearWall(int x, int y, int dir) {
		clearWall(new Point(x, y), dir);
	}

	public void updateCorner(int x, int y) {
		updateCorner(new Point(x, y));
	}

	public void updateCorner(Point pt) {
		Rectangle r = getCornerRect(pt);
		if (repaintFlag)
			repaint(r.x, r.y, r.width, r.height);
	}

	public static String directionName(int dir) {
		switch (dir) {
		case SOUTH:
			return "SOUTH";
		case WEST:
			return "WEST";
		case NORTH:
			return "NORTH";
		case EAST:
			return "EAST";
		}
		return null;
	}

	public static int leftFrom(int dir) {
		switch (dir) {
		case SOUTH:
			return EAST;
		case WEST:
			return SOUTH;
		case NORTH:
			return WEST;
		case EAST:
			return NORTH;
		}
		return -1;
	}

	public static int rightFrom(int dir) {
		switch (dir) {
		case SOUTH:
			return WEST;
		case WEST:
			return NORTH;
		case NORTH:
			return EAST;
		case EAST:
			return SOUTH;
		}
		return -1;
	}

	public static int oppositeDirection(int dir) {
		switch (dir) {
		case SOUTH:
			return NORTH;
		case WEST:
			return EAST;
		case NORTH:
			return SOUTH;
		case EAST:
			return WEST;
		}
		return -1;
	}

	public static Point adjacentPoint(Point pt, int dir) {
		return adjacentPoint(pt.x, pt.y, dir);
	}

	public static Point adjacentPoint(int x, int y, int dir) {
		switch (dir) {
		case SOUTH:
			return new Point(x, y - 1);
		case WEST:
			return new Point(x - 1, y);
		case NORTH:
			return new Point(x, y + 1);
		case EAST:
			return new Point(x + 1, y);
		}
		return null;
	}

	public void repaint() {
		if (repaintFlag)
			super.repaint();
	}
	
	/**
	 * Saves an image of this Karel world to the given image file.
	 */
	public void saveToImage(File file) {
		BufferedImage img = toImage();
		MediaTools.saveImage(img, file);
	}
	
	/**
	 * Saves an image of this Karel world to the given image file.
	 */
	public void saveToImage(String filename) {
		saveToImage(new File(filename));
	}
	
	@Override
	public Dimension getSize() {
		if (CommandLineProgram.isHeadless()) {
			return new Dimension(widthPx, heightPx);
		} else {
			return super.getSize();
		}
	}
	
	@Override
	public int getWidth() {
		if (CommandLineProgram.isHeadless()) {
			return widthPx;
		} else {
			return super.getWidth();
		}
	}
	
	@Override
	public int getHeight() {
		if (CommandLineProgram.isHeadless()) {
			return heightPx;
		} else {
			return super.getHeight();
		}
	}
	
	@Override
	public void setSize(int width, int height) {
		if (CommandLineProgram.isHeadless()) {
			widthPx = width;
			heightPx = height;
			setDisplayParameters(this.cols, this.rows);
		} else {
			super.setSize(width, height);
		}
	}
	
	public void setToDefaultSize() {
		setSize(MAX_WIDTH * this.cols + LEFT_NUMBER_MARGIN*2,
				MAX_HEIGHT * this.rows + BOTTOM_NUMBER_MARGIN*2);
	}
	
	/**
	 * Converts this Karel world into an image that is returned.
	 */
	public BufferedImage toImage() {
		int width = this.getWidth();
		int height = this.getHeight();
		
		// make an all-white image
		BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		Graphics g = img.getGraphics();
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, width, height);
		
		// draw Karel world onto the image
		this.paintComponent(g);
		g.dispose();
		
		return img;
	}

	public void update(Graphics g) {
		paintComponent(g);
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		if (map == null || !displayFlag)
			return;
		GraphicsUtils.setAntialiasing(g, true);
		synchronized (sizeLock) {
			if (offscreen == null) {
				Dimension size = getSize();
				offscreen = createImage(size.width, size.height);
			}
			Graphics osg = offscreen.getGraphics();
			GraphicsUtils.setAntialiasing(osg, true);
			drawEmptyWorld(osg);
			for (int pass = 0; pass < 2; pass++) {
				for (int x = 1; x <= cols + 1; x++) {
					for (int y = 1; y <= rows + 1; y++) {
						boolean mustPaint = false;
						if (getKarelOnSquare(x, y) != null) {
							mustPaint = true;
						} else if (map[x][y].color != null) {
							mustPaint = true;
						} else if (map[x][y].nBeepers != 0) {
							mustPaint = true;
						} else if (x > 1 && map[x][y].wallWest) {
							mustPaint = true;
						} else if (y > 1 && map[x][y].wallSouth) {
							mustPaint = true;
						} else if (x < cols && map[x + 1][y].wallWest) {
							mustPaint = true;
						} else if (y < rows && map[x][y + 1].wallSouth) {
							mustPaint = true;
						}
						if (mustPaint) {
							if (pass == 0) {
								updateContents(osg, new Point(x, y));
							} else {
								updateWalls(osg, new Point(x, y));
							}
						}
					}
				}
			}
			drawWorldFrame(osg);
		}
		g.drawImage(offscreen, 0, 0, this);
	}

	public void trace() {
		if (monitor != null)
			monitor.trace();
	}

	protected void setMonitor(KarelWorldMonitor monitor) {
		this.monitor = monitor;
	}

	protected KarelWorldMonitor getMonitor() {
		return monitor;
	}

	/* Protected hooks */

	protected void componentResizedHook() {
		setDisplayParameters(cols, rows);
		repaint();
	}

	protected void mousePressedHook(MouseEvent e) {
		if (editMode) {
			lastClick = "";
			Point pt = getClickCorner(e.getX(), e.getY());
			if (pt == null) {
				activeKarel = null;
				checkForWallClick(e.getX(), e.getY());
			} else {
				activeKarel = getKarelOnSquare(pt.x, pt.y);
				if (activeKarel == null) {
					checkForCornerClick(pt);
				} else {
					// added by Marty to enable placing beepers on Karel's location
					checkForCornerClick(pt);
				}
			}
		}
	}

	protected void mouseDraggedHook(MouseEvent e) {
		if (!editMode)
			return;
		if (activeKarel != null) {
			Point pt = getClickCorner(e.getX(), e.getY());
			if (pt != null && !pt.equals(activeKarel.getLocation())) {
				activeKarel.setLocation(pt);
				repaint();
			}
		} else {
			if (!checkForWallClick(e.getX(), e.getY())) {
				checkForCornerClick(e.getX(), e.getY());
			}
		}
	}

	/* Click handling methods */

	private boolean checkForWallClick(int mx, int my) {
		double sx = (double) (mx - leftMargin + sqSize / 2) / sqSize;
		double sy = (double) (getSize().height - my - bottomMargin - 1 + sqSize / 2)
				/ sqSize;
		int tx = (int) (sx + 0.5);
		int ty = (int) (sy + 0.5);
		int dir;
		if (Math.abs(Math.abs(sx - tx) - 0.5) <= WALL_TOLERANCE
				&& Math.abs(sy - ty) < WALL_FRACTION) {
			if (tx > sx)
				tx--;
			if (tx < 0 || tx > cols || ty < 1 || ty > rows)
				return false;
			dir = EAST;
		} else if (Math.abs(Math.abs(sy - ty) - 0.5) <= WALL_TOLERANCE
				&& Math.abs(sx - tx) < WALL_FRACTION) {
			if (ty > sy)
				ty--;
			if (tx < 1 || tx > cols || ty < 0 || ty > rows)
				return false;
			dir = NORTH;
		} else {
			return false;
		}
		String click = tx + "/" + ty + "/" + dir;
		if (!click.equals(lastClick)) {
			if (monitor != null)
				monitor.wallAction(new Point(tx, ty), dir);
			lastClick = click;
		}
		return true;
	}

	private boolean checkForCornerClick(int mx, int my) {
		return checkForCornerClick(getClickCorner(mx, my));
	}

	private boolean checkForCornerClick(Point pt) {
		if (pt == null)
			return false;
		String click = pt.x + "/" + pt.y;
		if (!click.equals(lastClick)) {
			if (monitor != null)
				monitor.cornerAction(pt);
			lastClick = click;
		}
		return true;
	}

	private Point getClickCorner(int mx, int my) {
		double sx = (double) (mx - leftMargin + sqSize / 2) / sqSize;
		double sy = (double) (getSize().height - my - bottomMargin - 1 + sqSize / 2)
				/ sqSize;
		int tx = (int) (sx + 0.5);
		int ty = (int) (sy + 0.5);
		if (tx < 1 || tx > cols || ty < 1 || ty > rows)
			return null;
		if (Math.abs(Math.abs(sx - tx) - 0.5) * sqSize <= 1)
			return null;
		if (Math.abs(Math.abs(sy - ty) - 0.5) * sqSize <= 1)
			return null;
		return new Point(tx, ty);
	}

	private static String encodeColor(Color color) {
		if (color.equals(Color.BLACK))
			return "BLACK";
		if (color.equals(Color.BLUE))
			return "BLUE";
		if (color.equals(Color.CYAN))
			return "CYAN";
		if (color.equals(Color.DARK_GRAY))
			return "DARK_GRAY";
		if (color.equals(Color.GRAY))
			return "GRAY";
		if (color.equals(Color.GREEN))
			return "GREEN";
		if (color.equals(Color.LIGHT_GRAY))
			return "LIGHT_GRAY";
		if (color.equals(Color.MAGENTA))
			return "MAGENTA";
		if (color.equals(Color.ORANGE))
			return "ORANGE";
		if (color.equals(Color.PINK))
			return "PINK";
		if (color.equals(Color.RED))
			return "RED";
		if (color.equals(Color.WHITE))
			return "WHITE";
		if (color.equals(Color.YELLOW))
			return "YELLOW";
		return "0x" + Integer.toString(color.getRGB() & 0xFFFFFF).toUpperCase();
	}

	private static Color decodeColor(String name) {
		if (name.equalsIgnoreCase("black"))
			return Color.BLACK;
		if (name.equalsIgnoreCase("blue"))
			return Color.BLUE;
		if (name.equalsIgnoreCase("cyan"))
			return Color.CYAN;
		if (name.equalsIgnoreCase("darkgray"))
			return Color.DARK_GRAY;
		if (name.equalsIgnoreCase("dark_gray"))
			return Color.DARK_GRAY;
		if (name.equalsIgnoreCase("gray"))
			return Color.GRAY;
		if (name.equalsIgnoreCase("green"))
			return Color.GREEN;
		if (name.equalsIgnoreCase("lightgray"))
			return Color.LIGHT_GRAY;
		if (name.equalsIgnoreCase("light_gray"))
			return Color.LIGHT_GRAY;
		if (name.equalsIgnoreCase("magenta"))
			return Color.MAGENTA;
		if (name.equalsIgnoreCase("orange"))
			return Color.ORANGE;
		if (name.equalsIgnoreCase("pink"))
			return Color.PINK;
		if (name.equalsIgnoreCase("red"))
			return Color.RED;
		if (name.equalsIgnoreCase("white"))
			return Color.WHITE;
		if (name.equalsIgnoreCase("yellow"))
			return Color.YELLOW;
		return Color.decode(name);
	}

	private void setDisplayParameters(int cols, int rows) {
		offscreen = null;
		int usableWidth = getSize().width
				- ((numberSquaresFlag) ? LEFT_NUMBER_MARGIN : 2);
		int usableHeight = getSize().height
				- ((numberSquaresFlag) ? BOTTOM_NUMBER_MARGIN : 0) - 2;
		this.width = cols;
		this.height = rows;
		if (forcedSize == 0) {
			sqSize = Math.min((int) (usableWidth / cols),
					(int) (usableHeight / rows));
		} else {
			sqSize = forcedSize;
		}
		width = cols * sqSize;
		height = rows * sqSize;
		switch (alignment) {
		case NORTHWEST:
		case WEST:
		case SOUTHWEST:
			leftMargin = (numberSquaresFlag) ? LEFT_NUMBER_MARGIN : 2;
			break;
		case NORTH:
		case CENTER:
		case SOUTH:
			leftMargin = ((numberSquaresFlag) ? LEFT_NUMBER_MARGIN : 2)
					+ (usableWidth - width) / 2;
			break;
		case NORTHEAST:
		case EAST:
		case SOUTHEAST:
			leftMargin = getSize().width - width - 1;
			break;
		}
		switch (alignment) {
		case NORTHWEST:
		case NORTH:
		case NORTHEAST:
			bottomMargin = getSize().height - height - 2;
			break;
		case WEST:
		case CENTER:
		case EAST:
			bottomMargin = ((numberSquaresFlag) ? BOTTOM_NUMBER_MARGIN : 0)
					+ (usableHeight - height) / 2;
			break;
		case SOUTHWEST:
		case SOUTH:
		case SOUTHEAST:
			bottomMargin = (numberSquaresFlag) ? BOTTOM_NUMBER_MARGIN : 0;
			break;
		}
	}

	private void drawEmptyWorld(Graphics g) {
		if (g == null)
			return;
		Dimension size = getSize();
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, size.width, size.height);
		g.setColor(Color.BLACK);
		int x = leftMargin + sqSize / 2;
		for (int ix = 1; ix <= cols; ix++) {
			int y = getSize().height - bottomMargin - (sqSize + 1) / 2 - 1;
			for (int iy = 1; iy <= rows; iy++) {
				drawCornerMarker(g, x, y);
				y -= sqSize;
			}
			x += sqSize;
		}
	}

	private void drawWorldFrame(Graphics g) {
		if (g == null)
			return;
		g.setColor(Color.BLACK);
		int x = leftMargin;
		int y = getSize().height - bottomMargin - 1 - height;
		if (sqSize >= DOUBLE_WALL_THRESHOLD) {
			g.drawRect(x, y, width - 1, height - 1);
			g.drawRect(x - 1, y - 1, width + 1, height + 1);
		} else {
			g.drawRect(x - 1, y - 1, width, height);
		}
		if (sqSize > NUMBER_THRESHOLD && numberSquaresFlag) {
			g.setFont(BEEPER_NUMBER_FONT);
			FontMetrics fm = g.getFontMetrics();
			x = leftMargin + sqSize / 2;
			y = getSize().height - bottomMargin + 10;
			for (int ix = 1; ix <= cols; ix++) {
				String label = "" + ix;
				g.drawString(label, x - fm.stringWidth(label) / 2, y);
				x += sqSize;
			}
			x = leftMargin - 3;
			y = getSize().height - bottomMargin - sqSize / 2 + 2;
			for (int iy = 1; iy <= rows; iy++) {
				g.drawString("" + iy, x - fm.stringWidth("" + iy), y);
				y -= sqSize;
			}
		}
	}

	// private void updateCorner(Graphics g, Point pt) {
	// updateContents(g, pt);
	// updateWalls(g, pt);
	// }

	public void updateContents(Graphics g, Point pt) {
		if (g == null)
			return;
		if (outOfBounds(pt))
			return;
		int x = leftMargin + (pt.x - 1) * sqSize;
		int y = getSize().height - bottomMargin - 1 - pt.y * sqSize;
		drawCorner(g, x, y, pt);
	}

	public void drawCorner(Graphics g, int x, int y, Point pt) {
		if (g == null)
			return;
		int sqSize = getSquareSize();
		Color color = getCornerColor(pt);
		g.setColor((color == null) ? Color.WHITE : color);
		g.fillRect(x, y, sqSize, sqSize);
		int cx = x + sqSize / 2;
		int cy = y + sqSize / 2;
		int nBeepers = getBeepersOnCorner(pt);
		if (nBeepers > 0) {
			if (nBeepers == 1 && !displayOneFlag)
				nBeepers = BLANKB;
			drawBeeperForStyle(g, cx, cy, sqSize, nBeepers, 1);
		}
		KarelInterface karel = getKarelOnSquare(pt.x, pt.y);
		if (karel != null) {
			drawKarel(g, cx, cy, karel.getDirection(), sqSize);
		} else {
			if (color == null && nBeepers == 0)
				drawCornerMarker(g, cx, cy);
		}
	}

	public static void drawMarkedCorner(Graphics g, int x, int y, int size) {
		if (g == null)
			return;
		int inset = Math.max(2, size / 5);
		g.setColor(Color.WHITE);
		g.fillRect(x, y, size, size);
		g.setColor(MARKED_COLOR);
		g.fillRect(x + inset, y + inset, size - 2 * inset, size - 2 * inset);
	}

	public void drawKarel(Graphics g, int x, int y, int dir, int size) {
		if (g == null)
			return;
		if (size < MIN_FANCY || getLook() == SIMPLE) {
			drawSimpleKarel(g, x, y, dir, size);
		} else {
			drawFancyKarel(g, x, y, dir, size, msKarel);
		}
	}

	public void drawSimpleKarel(Graphics g, int x, int y, int dir, int size) {
		if (g == null)
			return;
		size = (int) Math.round(size * SIMPLE_FRACTION);
		if (size % 2 == 0)
			size--;
		int half = (size + 1) / 2;
		for (int pass = 1; pass <= 2; pass++) {
			KarelRegion r = new KarelRegion();
			r.setOrigin(x, y, -half, -half, dir);
			r.addVector(half, 0, dir);
			r.addVector(half, half, dir);
			r.addVector(-half, half, dir);
			r.addVector(-half, 0, dir);
			r.addVector(0, -size, dir);
			if (pass == 1) {
				g.setColor(Color.WHITE);
				g.fillPolygon(r.getPolygon());
			} else {
				g.setColor(Color.BLACK);
				g.drawPolygon(r.getPolygon());
			}
		}
	}

	public void drawFancyKarel(Graphics g, int x, int y, int dir, int size) {
		drawFancyKarel(g, x, y, dir, size - KAREL_INSET, Color.WHITE);
	}

	public void drawFancyKarel(Graphics g, int x, int y, int dir, int size, boolean msKarel) {
		drawFancyKarel(g, x, y, dir, size - KAREL_INSET, Color.WHITE, msKarel);
	}

	public static void drawFancyKarel(Graphics g, int x, int y, int dir,
			int size, Color color) {
		drawFancyKarel(g, x, y, dir, size, color, /* msKarel */ false);
	}
	
	public static void drawFancyKarel(Graphics g, int x, int y, int dir,
			int size, Color color, boolean msKarel) {
		if (g == null) {
			return;
		}
		for (int pass = 1; pass <= 2; pass++) {
			KarelRegion r = new KarelRegion();
			r.setOrigin(x, y, BODY_OFFSET_X * size, BODY_OFFSET_Y * size
					+ LOWER_NOTCH * size, dir);
			int sx = r.getCurrentX();
			int sy = r.getCurrentY();
			
			// Karel's body outline
			Color bodyColor = (pass == 1) ? color : Color.BLACK;
			g.setColor(bodyColor);
			r.addVector(0, BODY_HEIGHT * size - LOWER_NOTCH * size, dir);
			r.addVector(BODY_WIDTH * size - UPPER_NOTCH * size, 0, dir);
			r.addVector(UPPER_NOTCH * size, -UPPER_NOTCH * size, dir);
			r.addVector(0, -(BODY_HEIGHT * size - UPPER_NOTCH * size), dir);
			r.addVector(-(BODY_WIDTH * size - LOWER_NOTCH * size), 0, dir);
			r.addVector(-LOWER_NOTCH * size, LOWER_NOTCH * size, dir);
			if (pass == 1) {
				r.getPolygon().addPoint(sx, sy);
				r.addVector((SCREEN_OFFSET_X - BODY_OFFSET_X) * size,
						(SCREEN_OFFSET_Y - BODY_OFFSET_Y - LOWER_NOTCH) * size,
						dir);
			} else {
				g.drawPolygon(r.getPolygon());
				r = new KarelRegion();
				r.setOrigin(sx, sy, (SCREEN_OFFSET_X - BODY_OFFSET_X) * size,
						(SCREEN_OFFSET_Y - BODY_OFFSET_Y - LOWER_NOTCH) * size,
						dir);
			}
			
			// monitor outline
			r.addVector(SCREEN_WIDTH * size, 0, dir);
			r.addVector(0, SCREEN_HEIGHT * size, dir);
			r.addVector(-SCREEN_WIDTH * size, 0, dir);
			r.addVector(0, -SCREEN_HEIGHT * size, dir);
			if (pass == 1) {
				r.getPolygon().addPoint(sx, sy);
				g.fillPolygon(r.getPolygon());
				r = new KarelRegion();
				r.setOrigin(sx, sy, (SCREEN_OFFSET_X - BODY_OFFSET_X) * size
						- 1, (SCREEN_OFFSET_Y - BODY_OFFSET_Y - LOWER_NOTCH)
						* size - 1, dir);
				r.addVector(SCREEN_WIDTH * size + 2, 0, dir);
				r.addVector(0, SCREEN_HEIGHT * size + 2, dir);
				r.addVector(-(SCREEN_WIDTH * size + 2), 0, dir);
				r.addVector(0, -(SCREEN_HEIGHT * size + 2), dir);
				g.drawPolygon(r.getPolygon());
			} else {
				g.drawPolygon(r.getPolygon());
			}
			
			if (msKarel && pass == 2) {
				KarelRegion bow = new KarelRegion(Color.BLACK, Color.RED);
				int bowWidth = (int) (BODY_WIDTH * size / 4);
				int bowHeight = (int) (BODY_WIDTH * size / 4);
				double bowX = SCREEN_OFFSET_X * size + SCREEN_WIDTH/2 * size - bowWidth/2;
				double bowY = (SCREEN_OFFSET_Y - BODY_OFFSET_Y) * size;
				int triangleSize = bowWidth;
				
				bow.setOrigin(x, y, bowX, bowY, dir);
				bow.addVector(bowWidth, 0, dir);
				bow.addVector(0, bowHeight, dir);
				bow.addVector(-bowWidth, 0, dir);
				bow.addVector(0, -bowHeight, dir);
				
				KarelRegion triangle1 = new KarelRegion(Color.BLACK, Color.RED);
				triangle1.setOrigin(x, y, bowX, bowY + bowHeight/2, dir);
				triangle1.addVector(-triangleSize, -triangleSize/2, dir);
				triangle1.addVector(0, triangleSize, dir);
				triangle1.addVector(triangleSize, -triangleSize/2, dir);

				KarelRegion triangle2 = new KarelRegion(Color.BLACK, Color.RED);
				triangle2.setOrigin(x, y, bowX + bowWidth, bowY + bowHeight/2, dir);
				triangle2.addVector(triangleSize, triangleSize/2, dir);
				triangle2.addVector(0, -triangleSize, dir);
				triangle2.addVector(-triangleSize, triangleSize/2, dir);

				triangle1.draw(g);
				triangle2.draw(g);
				bow.draw(g);
			}
		}
		
		// little line slot for disk
		double SLOT_REAL_WIDTH = SLOT_WIDTH;
		if (msKarel) {
			// "lipstick"
			double LIP_HEIGHT = 0.07;
			SLOT_REAL_WIDTH *= 1.8;       // widen lips
			
			KarelRegion lips = new KarelRegion(Color.BLACK, Color.RED);
			lips.setOrigin(x, y, SCREEN_OFFSET_X * size + SCREEN_WIDTH * size,
					(SCREEN_OFFSET_Y * size + BODY_OFFSET_Y * size) / 2, dir);
			lips.addVector(-SLOT_REAL_WIDTH * size/2, -LIP_HEIGHT * size, dir);
			lips.addVector(-SLOT_REAL_WIDTH * size/2, LIP_HEIGHT * size, dir);
			lips.addVector(SLOT_REAL_WIDTH * size/2, LIP_HEIGHT * size, dir);
			lips.addVector(SLOT_REAL_WIDTH * size/2, -LIP_HEIGHT * size, dir);
			lips.draw(g);
		}
		
		KarelRegion r = new KarelRegion();
		r.setOrigin(x, y, SCREEN_OFFSET_X * size + SCREEN_WIDTH * size,
				(SCREEN_OFFSET_Y * size + BODY_OFFSET_Y * size) / 2, dir);
		r.addVector(-SLOT_REAL_WIDTH * size, 0, dir);
		g.setColor(Color.BLACK);
		g.drawPolygon(r.getPolygon());
		
		r = new KarelRegion();
		r.setOrigin(x, y, BODY_OFFSET_X * size, SCREEN_OFFSET_Y * size, dir);
		
		// Karel's feet
		r.addVector(-(UPPER_ANKLE * size + FOOT_WIDTH * size), 0, dir);
		r.addVector(0, -FOOT_LENGTH * size, dir);
		r.addVector(FOOT_WIDTH * size, 0, dir);
		r.addVector(0, FOOT_LENGTH * size - FOOT_WIDTH * size, dir);
		r.addVector(UPPER_ANKLE * size, 0, dir);
		r.addVector(0, FOOT_WIDTH * size, dir);
		g.fillPolygon(r.getPolygon());
		g.drawPolygon(r.getPolygon());
		r = new KarelRegion();
		r.setOrigin(x, y, SCREEN_OFFSET_X * size + SCREEN_WIDTH * size
				- SLOT_WIDTH * size, BODY_OFFSET_Y * size, dir);
		r.addVector(0, -(LOWER_ANKLE * size + FOOT_WIDTH * size), dir);
		r.addVector(FOOT_LENGTH * size, 0, dir);
		r.addVector(0, FOOT_WIDTH * size, dir);
		r.addVector(-(FOOT_LENGTH * size - FOOT_WIDTH * size), 0, dir);
		r.addVector(0, LOWER_ANKLE * size, dir);
		r.addVector(-FOOT_WIDTH * size, 0, dir);
		g.fillPolygon(r.getPolygon());
		g.drawPolygon(r.getPolygon());
	}

	public void drawBeeperForStyle(Graphics g, int x, int y, int size, int n,
			int border) {
		String label = "";
		if (size > MIN_LABEL && (displayOneFlag || n != 1)) {
			label = beeperLabel(n);
		}
		drawBeeper(g, x, y, size, label, border, this);
	}

	public static void drawBeeper(Graphics g, int x, int y, int size, int n,
			int border, Component comp) {
		if (g == null)
			return;
		int beeperSize = (int) Math.round(size * BEEPER_FRACTION);
		if (beeperSize % 2 == 0)
			beeperSize--;
		int half = (beeperSize + 1) / 2;
		KarelRegion r = new KarelRegion();
		r.setOrigin(x, y, 0, -half, EAST);
		r.addVector(half, half, EAST);
		r.addVector(-half, half, EAST);
		r.addVector(-half, -half, EAST);
		r.addVector(half, -half, EAST);
		g.setColor(BEEPER_COLOR);
		g.fillPolygon(r.getPolygon());
		g.drawPolygon(r.getPolygon());
		g.setColor(Color.BLACK);
		for (int i = 0; i < border; i++) {
			int delta = half + i;
			g.drawLine(x - delta, y, x, y + delta);
			g.drawLine(x, y + delta, x + delta, y);
			g.drawLine(x + delta, y, x, y - delta);
			g.drawLine(x, y - delta, x - delta, y);
		}
		if (size > MIN_LABEL && n != 1) {
			labelBeeper(g, x, y, size, beeperLabel(n), comp);
		}
	}

	public static void drawBeeper(Graphics g, int x, int y, int size,
			String label, int border, Component comp) {
		if (g == null)
			return;
		int beeperSize = (int) Math.round(size * BEEPER_FRACTION);
		if (beeperSize % 2 == 0)
			beeperSize--;
		int half = (beeperSize + 1) / 2;
		KarelRegion r = new KarelRegion();
		r.setOrigin(x, y, 0, -half, EAST);
		r.addVector(half, half, EAST);
		r.addVector(-half, half, EAST);
		r.addVector(-half, -half, EAST);
		r.addVector(half, -half, EAST);
		g.setColor(BEEPER_COLOR);
		g.fillPolygon(r.getPolygon());
		g.drawPolygon(r.getPolygon());
		g.setColor(Color.BLACK);
		for (int i = 0; i < border; i++) {
			int delta = half + i;
			g.drawLine(x - delta, y, x, y + delta);
			g.drawLine(x, y + delta, x + delta, y);
			g.drawLine(x + delta, y, x, y - delta);
			g.drawLine(x, y - delta, x - delta, y);
		}
		labelBeeper(g, x, y, size, label, comp);
	}

	public static void labelBeeper(Graphics g, int x, int y, int size,
			String label, Component comp) {
		if (label.equals("�")) {
			if (infinityImage == null) {
				infinityImage = MediaTools.createImage(INFINITY);
			}
			g.drawImage(infinityImage, x - 4, y - 2, comp);
		} else {
			int psz = 7;
			switch (label.length()) {
			case 1:
			case 2:
				psz = BEEPER_NUMBER_FONT.getSize();
				break;
			default:
				psz = 8;
				break;
			}
			Font font = new Font(BEEPER_FONT_FAMILY, Font.PLAIN, psz);
			g.setFont(font);
			FontMetrics fm = g.getFontMetrics();
			g.drawString(label, x - fm.stringWidth(label) / 2,
					y + fm.getAscent() / 2);
		}
	}

	public static String beeperLabel(int n) {
		switch (n) {
		case INFINITE:
			return "�";
		case PLUS1:
			return "+1";
		case MINUS1:
			return "-1";
		case BLANKB:
			return "";
		default:
			return "" + n;
		}
	}

	public void updateWalls(Graphics g, Point pt) {
		if (g == null)
			return;
		if (outOfBounds(pt))
			return;
		int x = leftMargin + (pt.x - 1) * sqSize;
		int y = getSize().height - bottomMargin - 1 - pt.y * sqSize;
		g.setColor(Color.BLACK);
		for (int dir = NORTH; dir <= WEST; dir++) {
			if (checkWall(pt, dir))
				drawWall(g, x, y, dir);
			if (sqSize < DOUBLE_WALL_THRESHOLD)
				fixCornerPoint(g, pt, dir);
		}
	}

	private void drawWall(Graphics g, int x, int y, int dir) {
		if (g == null)
			return;
		int x0, y0, x1, y1;
		switch (dir) {
		case NORTH:
			x0 = x;
			y0 = y;
			x1 = x0 + sqSize;
			y1 = y0;
			break;
		case EAST:
			x0 = x + sqSize;
			y0 = y;
			x1 = x0;
			y1 = y0 + sqSize;
			break;
		case SOUTH:
			x0 = x;
			y0 = y + sqSize;
			x1 = x0 + sqSize;
			y1 = y0;
			break;
		case WEST:
			x0 = x;
			y0 = y;
			x1 = x0;
			y1 = y0 + sqSize;
			break;
		default:
			x0 = y0 = x1 = y1 = 0;
			break;
		}
		if (sqSize < DOUBLE_WALL_THRESHOLD) {
			g.drawLine(x0 - 1, y0 - 1, x1 - 1, y1 - 1);
		} else {
			if (x0 == x1) {
				g.drawLine(x0 - 1, y0 - 1, x1 - 1, y1);
				g.drawLine(x0, y0 - 1, x1, y1);
			} else {
				g.drawLine(x0 - 1, y0 - 1, x1, y1 - 1);
				g.drawLine(x0 - 1, y0, x1, y1);
			}
		}
	}

	public void drawCornerMarker(Graphics g, int x, int y) {
		if (g == null)
			return;
		g.setColor(Color.BLACK);
		if (sqSize < CROSS_THRESHOLD) {
			g.drawLine(x, y, x, y);
		} else {
			g.drawLine(x - 1, y, x + 1, y);
			g.drawLine(x, y - 1, x, y + 1);
		}
	}

	private void fixCornerPoint(Graphics g, Point pt, int dir) {
		int left = leftFrom(dir);
		Point pUp = adjacentPoint(pt, dir);
		Point pLeft = adjacentPoint(pt, left);
		if (!outOfBounds(pUp) && checkWall(pUp, left)) {
			int x = leftMargin + (pUp.x - 1) * sqSize;
			int y = getSize().height - bottomMargin - 1 - pUp.y * sqSize;
			drawWall(g, x, y, left);
		} else if (!outOfBounds(pLeft) && checkWall(pLeft, dir)) {
			int x = leftMargin + (pLeft.x - 1) * sqSize;
			int y = getSize().height - bottomMargin - 1 - pLeft.y * sqSize;
			drawWall(g, x, y, dir);
		}
	}

	private Rectangle getCornerRect(Point pt) {
		int x = leftMargin + (pt.x - 1) * sqSize;
		int y = getSize().height - bottomMargin - 1 - pt.y * sqSize;
		return new Rectangle(x - 1, y - 1, sqSize + 2, sqSize + 2);
	}

	/* File saving */

	public void save() {
		if (pathname == null)
			return;
		Point pt = new Point(0, 0);
		try {
			PrintWriter wr = new PrintWriter(new FileWriter(pathname));
			wr.println("Dimension: (" + cols + ", " + rows + ")");
			for (pt.x = 1; pt.x <= cols; pt.x++) {
				for (pt.y = 1; pt.y <= rows; pt.y++) {
					if (pt.x > 1 && checkWall(pt, WEST)) {
						wr.println("Wall: (" + pt.x + ", " + pt.y + ") west");
					}
					if (pt.y > 1 && checkWall(pt, SOUTH)) {
						wr.println("Wall: (" + pt.x + ", " + pt.y + ") south");
					}
				}
			}
			for (pt.x = 1; pt.x <= cols; pt.x++) {
				for (pt.y = 1; pt.y <= rows; pt.y++) {
					Color color = getCornerColor(pt);
					if (color != null) {
						wr.println("Color: (" + pt.x + ", " + pt.y + ") "
								+ encodeColor(color));
					}
					int nBeepers = getBeepersOnCorner(pt);
					if (nBeepers != 0) {
						String str = (nBeepers == INFINITE) ? "INFINITE" : ""
								+ nBeepers;
						wr.println("Beeper: (" + pt.x + ", " + pt.y + ") "
								+ str);
					}
				}
			}
			Iterator<KarelInterface> iterator = karels.iterator();
			while (iterator.hasNext()) {
				KarelInterface karel = iterator.next();
				String dirName = "Error";
				switch (karel.getDirection()) {
				case NORTH:
					dirName = "north";
					break;
				case EAST:
					dirName = "east";
					break;
				case SOUTH:
					dirName = "south";
					break;
				case WEST:
					dirName = "west";
					break;
				}
				Point loc = karel.getLocation();
				wr.println("Karel: (" + loc.x + ", " + loc.y + ") " + dirName);
				int nBeepers = karel.getBeepersInBag();
				String str = (nBeepers == INFINITE) ? "INFINITE" : ""
						+ nBeepers;
				if (getKarelCount() == 1) {
					wr.println();
					wr.println("BeeperBag: " + str);
				} else {
					wr.println(" " + str);
				}
			}
			if (monitor != null) {
				wr.println("Speed: " + speedFormat.format(monitor.getSpeed()));
			}
			wr.close();
		} catch (IOException ex) {
			throw new ErrorException("" + ex);
		}
		Platform.setFileTypeAndCreator(pathname, "TEXT", "CWIE");
	}

	/* File loading */

	public void load(String lines[]) {
		String program = "";
		for (int i = 0; i < lines.length; i++) {
			program += lines[i] + '\n';
		}
		load(new StringReader(program));
	}

	public void load(File file) {
		try {
			pathname = file.getPath();
			Reader rd = new FileReader(file);
			load(rd);
			rd.close();
		} catch (IOException ex) {
			throw new ErrorException("I/O error reading map file");
		}
	}

	public void load(String pathname) {
		try {
			this.pathname = pathname;
			Reader rd = new FileReader(pathname);
			load(rd);
			rd.close();
		} catch (IOException ex) {
			throw new ErrorException("I/O error reading map file");
		}
	}

	public void load(Reader rd) {
		try {
			setRepaintFlag(false);
			lastBeeperCount = INFINITE;
			tokenizer = new StreamTokenizer(rd);
			tokenizer.eolIsSignificant(true);
			tokenizer.lowerCaseMode(true);
			tokenizer.resetSyntax();
			tokenizer.wordChars('A', 'Z');
			tokenizer.wordChars('a', 'z');
			tokenizer.wordChars('0', '9');
			tokenizer.wordChars('.', '.');
			tokenizer.wordChars('_', '_');
			tokenizer.whitespaceChars(' ', ' ');
			tokenizer.whitespaceChars('\t', '\t');
			tokenizer.whitespaceChars('\r', '\r');
			while (readMapLine()) {
				/* Empty */
			}
			rd.close();
			setRepaintFlag(true);
			repaint();
		} catch (IOException ex) {
			setRepaintFlag(true);
			throw new ErrorException("I/O error reading map file");
		}
	}
	
	public void setMsKarel(boolean value) {
		this.msKarel = value;
		repaint();
	}

	public void setInteractiveMode(boolean value) {
		this.interactiveMode = value;
		if (value) {
			String message = "";
			message += "Karel interactive mode keys:\n";
			message += "----------------------------\n";
			message += "Up Arrow = move\n";
			message += "Left Arrow = turnLeft\n";
			message += "Right Arrow = turnRight (SuperKarel only)\n";
			message += "Down Arrow = turnAround (SuperKarel only)\n";
			message += "PgUp = pickBeeper\n";
			message += "PgDown = putBeeper\n";
			JOptionPane.showMessageDialog(this, message);
		}
	}

	private boolean readMapLine() {
		int token = nextToken();
		switch (token) {
		case StreamTokenizer.TT_EOF:
			return false;
		case StreamTokenizer.TT_EOL:
			return true;
		case StreamTokenizer.TT_WORD:
			String cmd = tokenizer.sval;
			if (nextToken() != ':') {
				throw new ErrorException("Missing colon after " + cmd);
			}
			if (cmd.equals("dimension")) {
				dimensionCommand();
			} else if (cmd.equals("karel") || cmd.equals("turtle")) {
				karelCommand();
			} else if (cmd.equals("wall")) {
				wallCommand();
			} else if (cmd.equals("mark") || cmd.equals("color")) {
				setColorCommand();
			} else if (cmd.equals("speed")) {
				speedCommand();
			} else if (cmd.equals("beeper")) {
				beeperCommand();
			} else if (cmd.equals("beeperbag")) {
				beeperBagCommand();
			} else {
				throw new ErrorException("Illegal command: " + cmd);
			}
			break;
		default:
			throw new ErrorException("Illegal character '" + (char) token + "'");
		}
		return (true);
	}

	private void dimensionCommand() {
		verifyToken('(');
		int cols = scanInt();
		verifyToken(',');
		int rows = scanInt();
		verifyToken(')');
		verifyToken(StreamTokenizer.TT_EOL);
		init(cols, rows);
	}

	private void karelCommand() {
		Point pt = new Point(0, 0);
		int dir = EAST;
		int nBeepers = lastBeeperCount;
		verifyToken('(');
		pt.x = scanInt();
		verifyToken(',');
		pt.y = scanInt();
		verifyToken(')');
		if (nextToken() != StreamTokenizer.TT_WORD) {
			throw new ErrorException("Illegal direction");
		}
		if ("north".startsWith(tokenizer.sval)) {
			dir = NORTH;
		} else if ("east".startsWith(tokenizer.sval)) {
			dir = EAST;
		} else if ("south".startsWith(tokenizer.sval)) {
			dir = SOUTH;
		} else if ("west".startsWith(tokenizer.sval)) {
			dir = WEST;
		} else {
			throw new ErrorException("Illegal direction " + tokenizer.sval);
		}
		int token = nextToken();
		if (token == StreamTokenizer.TT_WORD) {
			if ("infinite".startsWith(tokenizer.sval)
					|| "infinity".startsWith(tokenizer.sval)) {
				nBeepers = INFINITE;
			} else {
				try {
					nBeepers = Integer.parseInt(tokenizer.sval);
				} catch (NumberFormatException ex) {
					throw new ErrorException("Illegal beeper bag value");
				}
			}
			token = nextToken();
		}
		if (token != StreamTokenizer.TT_EOL) {
			throw new ErrorException("Unexpected tokens at end of line");
		}
		lastKarel = getKarel();
		if (lastKarel != null) {
			lastKarel.setLocation(pt.x, pt.y);
			lastKarel.setDirection(dir);
			lastKarel.setBeepersInBag(nBeepers);
		}
	}

	private void wallCommand() {
		Point pt = new Point(0, 0);
		int dir = EAST;
		verifyToken('(');
		pt.x = scanInt();
		verifyToken(',');
		pt.y = scanInt();
		verifyToken(')');
		if (nextToken() != StreamTokenizer.TT_WORD) {
			throw new ErrorException("Illegal direction");
		}
		if ("north".startsWith(tokenizer.sval)) {
			dir = NORTH;
		} else if ("east".startsWith(tokenizer.sval)) {
			dir = EAST;
		} else if ("south".startsWith(tokenizer.sval)) {
			dir = SOUTH;
		} else if ("west".startsWith(tokenizer.sval)) {
			dir = WEST;
		} else {
			throw new ErrorException("Illegal direction " + tokenizer.sval);
		}
		verifyToken(StreamTokenizer.TT_EOL);
		setWall(pt, dir);
	}

	private void setColorCommand() {
		Point pt = new Point(0, 0);
		String colorName = null;
		verifyToken('(');
		pt.x = scanInt();
		verifyToken(',');
		pt.y = scanInt();
		verifyToken(')');
		int tt = nextToken();
		if (tt != StreamTokenizer.TT_EOL) {
			if (tt != StreamTokenizer.TT_WORD) {
				throw new ErrorException("Missing color name");
			}
			colorName = tokenizer.sval.toLowerCase();
			verifyToken(StreamTokenizer.TT_EOL);
		}
		setCornerColor(pt, decodeColor(colorName));
	}

	private void speedCommand() {
		if (nextToken() != StreamTokenizer.TT_WORD) {
			throw new ErrorException("I expected a number");
		}
		double speed = Double.valueOf(tokenizer.sval).doubleValue();
		verifyToken(StreamTokenizer.TT_EOL);
		if (monitor != null)
			monitor.setSpeed(speed);
	}

	protected void beeperBagCommand() {
		if (nextToken() != StreamTokenizer.TT_WORD) {
			throw new ErrorException("Illegal beeper count");
		}
		int nBeepers = 0;
		if ("infinite".startsWith(tokenizer.sval)
				|| "infinity".startsWith(tokenizer.sval)) {
			nBeepers = INFINITE;
		} else {
			tokenizer.pushBack();
			nBeepers = scanInt();
		}
		verifyToken(StreamTokenizer.TT_EOL);
		if (lastKarel == null) {
			lastBeeperCount = nBeepers;
		} else {
			lastKarel.setBeepersInBag(nBeepers);
		}
	}

	protected void beeperCommand() {
		Point pt = new Point(0, 0);
		// int dir = EAST;
		verifyToken('(');
		pt.x = scanInt();
		verifyToken(',');
		pt.y = scanInt();
		verifyToken(')');
		int nBeepers = 1;
		int token = nextToken();
		if (token != StreamTokenizer.TT_EOL) {
			if (token != StreamTokenizer.TT_WORD) {
				throw new ErrorException("Illegal beeper count");
			}
			if ("infinite".startsWith(tokenizer.sval)
					|| "infinity".startsWith(tokenizer.sval)) {
				nBeepers = INFINITE;
			} else {
				tokenizer.pushBack();
				nBeepers = scanInt();
			}
			verifyToken(StreamTokenizer.TT_EOL);
		}
		setBeepersOnCorner(pt, nBeepers);
	}

	protected void ignoreCommand() {
		while (nextToken() != StreamTokenizer.TT_EOL) {
			/* Empty */
		}
	}

	private void verifyToken(int token) {
		if (nextToken() != token) {
			if (token == StreamTokenizer.TT_EOL) {
				throw new ErrorException("Unexpected tokens at end of line");
			}
			throw new ErrorException("I expected a '" + (char) token + "'");
		}
	}

	private int scanInt() {
		if (nextToken() != StreamTokenizer.TT_WORD) {
			throw new ErrorException("I expected an integer");
		}
		try {
			return Integer.parseInt(tokenizer.sval);
		} catch (NumberFormatException ex) {
			throw new ErrorException("Illegal integer");
		}
	}

	private int nextToken() {
		int token = 0;
		try {
			token = tokenizer.nextToken();
		} catch (IOException ex) {
			throw new ErrorException("Exception: " + ex);
		}
		if (TOKEN_TRACE) {
			switch (token) {
			case StreamTokenizer.TT_EOF:
				System.out.println("<EOF>");
				break;
			case StreamTokenizer.TT_EOL:
				System.out.println();
				break;
			case StreamTokenizer.TT_WORD:
				System.out.print(tokenizer.sval + " ");
				break;
			default:
				System.out.print((char) token + " ");
				break;
			}
		}
		return ((token == '\n') ? StreamTokenizer.TT_EOL : token);
	}

	private static class Corner {
		public Color color;
		public boolean wallSouth;
		public boolean wallWest;
		public int nBeepers;
	}

	private static class KarelRegion {
		public static final double EPSILON = 0.00000000001;

		/* Private state */
		private Polygon p;
		private Color outline;
		private Color fill;
		private double x;
		private double y;

		public KarelRegion() {
			p = new Polygon();
			x = y = 0;
		}

		public KarelRegion(Color outline, Color fill) {
			this();
			this.outline = outline;
			this.fill = fill;
		}

		public void draw(Graphics g) {
			Color old = g.getColor();
			if (fill != null) {
				g.setColor(fill);
				g.fillPolygon(p);
			}
			if (outline != null) {
				g.setColor(outline);
			}
			g.drawPolygon(p);
			g.setColor(old);
		}
		
		public Polygon getPolygon() {
			return p;
		}

		public void setOrigin(int x, int y) {
			if (p.npoints != 0) {
				throw new ErrorException("setOrigin called on nonempty region");
			}
			this.x = x;
			this.y = y;
		}

		public void setOrigin(int x, int y, double dx, double dy, int dir) {
			setOrigin(x, y);
			addVector(dx, dy, dir);
		}

		public void addVector(double dx, double dy, int dir) {
			x = getRotatedX(x, y, dx, dy, dir);
			y = getRotatedY(x, y, dx, dy, dir);
			p.addPoint((int) Math.round(x + EPSILON), (int) Math.round(y + EPSILON));
		}
		
		public static double getRotatedX(double originX, double originY, double dx, double dy, int dir) {
			switch (dir) {
			case KarelWorld.EAST:
				return originX + dx;
			case KarelWorld.NORTH:
				return originX - dy;
			case KarelWorld.WEST:
				return originX - dx;
			case KarelWorld.SOUTH:
				return originX + dy;
			default:
				return originX + dx;
			}
		}

		public static double getRotatedY(double originX, double originY, double dx, double dy, int dir) {
			switch (dir) {
			case KarelWorld.EAST:
				return originY - dy;
			case KarelWorld.NORTH:
				return originY - dx;
			case KarelWorld.WEST:
				return originY + dy;
			case KarelWorld.SOUTH:
				return originY + dx;
			default:
				return originY - dy;
			}
		}

//		public static void rotatePoint(Point origin, Point delta, int dir) {
//			int newX = (int) getRotatedX(origin.x, origin.y, delta.x, delta.y, dir);
//			int newY = (int) getRotatedY(origin.x, origin.y, delta.x, delta.y, dir);
//			delta.x = newX;
//			delta.y = newY;
//		}

		public int getCurrentX() {
			return p.xpoints[p.npoints - 1];
		}

		public int getCurrentY() {
			return p.ypoints[p.npoints - 1];
		}
	}

	private class KarelWorldListener implements MouseListener,
			MouseMotionListener, ComponentListener, KeyListener {

		public void mousePressed(MouseEvent e) {
			requestFocus();
			mousePressedHook(e);
		}

		public void mouseClicked(MouseEvent e) {
		}

		public void mouseReleased(MouseEvent e) {
		}

		public void mouseEntered(MouseEvent e) {
			requestFocus();
		}

		public void mouseExited(MouseEvent e) {
		}

		public void mouseDragged(MouseEvent e) {
			requestFocus();
			mouseDraggedHook(e);
		}

		public void mouseMoved(MouseEvent e) {
			requestFocus();
		}

		public void componentResized(ComponentEvent e) {
			componentResizedHook();
		}

		public void componentMoved(ComponentEvent e) {
		}

		public void componentShown(ComponentEvent e) {
		}

		public void componentHidden(ComponentEvent e) {
		}
		
		public void keyPressed(KeyEvent e) {
			// don't handle key presses unless we are in interactive mode
			// and Karel world is not being edited
			KarelInterface karel = activeKarel != null ? activeKarel : lastKarel;
			if (!interactiveMode || editMode || karel == null) {
				return;
			}
			
			try {
				int key = e.getKeyCode();
				if (key == KeyEvent.VK_LEFT) {
					karel.turnLeft();
					System.out.println("turnLeft();");
					repaint();
				} else if (key == KeyEvent.VK_RIGHT && karel instanceof SuperKarel) {
					((SuperKarel) karel).turnRight();
					System.out.println("turnRight();");
					repaint();
				} else if (key == KeyEvent.VK_DOWN && karel instanceof SuperKarel) {
					((SuperKarel) karel).turnAround();
					System.out.println("turnAround();");
					repaint();
				} else if (key == KeyEvent.VK_UP) {
					karel.move();
					System.out.println("move();");
					repaint();
				} else if (key == KeyEvent.VK_PAGE_UP) {
					karel.pickBeeper();
					System.out.println("pickBeeper();");
					repaint();
				} else if (key == KeyEvent.VK_PAGE_DOWN) {
					karel.putBeeper();
					System.out.println("putBeeper();");
					repaint();
				}
			} catch (ErrorException ex) {
				KarelErrorDialog errorDialog = new KarelErrorDialog(KarelWorld.this);
				errorDialog.error(ex.getMessage());
			}
		}
		
		public void keyReleased(KeyEvent e) {
			
		}
		
		public void keyTyped(KeyEvent e) {
			
		}
	}
}

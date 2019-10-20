/*
 * This file contains classes that implement the world editor.
 * - 2015/03/31: Changed to use Swing graphical components.
 */

package stanford.karel;

import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

class KarelWorldEditor extends JComponent implements MouseListener {
	/* Private constants */
	private static String[] BEEPER_BAG = {
			"47494638396123002F00D52000CBFFCB66330066660099999999993397976498",
			"3200986500CC9898323200CCCC98FFCC99CB9833CC9966333333643131643164",
			"CC660098CC98993333996633976464CCCCCCFFCBCBCB6533CC6666CCCC664444",
			"44646431DDDDDDCBFFFFFFFFCBEEEEEE777777888888AAAAAABBBBBB55555566",
			"6666440000320000910000910000910000910000910000910000910000910000",
			"9100009100009100009100009100009100009100009100009100009100009100",
			"0091000091000091000091000021F9040100001E002C0000000023002F004506",
			"FF408F70482C1A8FC8A4E7A3785428D007E7615228AF9F69E5C0E5620E182834",
			"C40929411C28664D6178BF847537CCA08429A147D1F1B4830F19737E5D6F6C62",
			"13254668057D62057F5E908415010A1F4A1F210E0E0F0E1B09280F2328091B0F",
			"A79B24574A080F1413141C014E96AB450A288C0C61111107BE847762091C4705",
			"097D727273070D6F0775116A150F0D441F01504F18056276616B5FE26C770561",
			"B61622260E010207D40F090D15B6469A9B2256460A229B0E66F5865CE0402AC0",
			"8904A71E0400C5E142402421BC75A300E221920F0A097561648082AA87253854",
			"A8D04B1AB077E00C256095A68FC93F6E0865F8468100850A280014F9D0C0891A",
			"FF68C19E11EA45E1801D07FA9630F2168E99A43FE40854C0A0F0929058DFA42E",
			"53F32D523843154C402862210D856E66ED74FBA2319C9D2805AE2028F5202897",
			"9169E23520D1C1A2DF800A4A7C4C42A244525B1D4A6C4A90C001070B23868070",
			"C0785389BE8039209C95D0714262871F7E3041CC5582130B1398B0FA57C88527",
			"136F3EE8E0B035916C8DF0D8BE75EA40802EB23820C0EC57C180BA810241ADF9",
			"00C500D14DB670E915CC8E9BB08393609432154A0492D5C1BDD3A3448314EF50",
			"BA482F0AC69BD107082A1A21512183390AD435561713A68283EC427400853985",
			"1C500001EFF8C15618ED3DF14076596C33C824FA8DB3C613D4D023C40710A401",
			"7F4E4D6C01156224604803450652B0868209DEE9E7A2206FD9F1C4064364E1CA",
			"3723EE32E15D776C034E08AC51900D1436C978873031A9275101E4090182905C",
			"39C2143336D55107060D04C0DA864E60F08459D22913941C0E2E90041E6671E0",
			"CE8B771D80480900D93240028F04C0C12415A481406DB675308004769A105010",
			"0021FF0B4D414347436F6E2004031039000000015772697474656E2062792047",
			"4946436F6E76657274657220322E342E33206F66204D6F6E6461792C204D6179",
			"2032352C2031393938003B" };
	private static final int NORTH = KarelWorld.NORTH;
	private static final int EAST = KarelWorld.EAST;
	private static final int SOUTH = KarelWorld.SOUTH;
	private static final int WEST = KarelWorld.WEST;
	private static final int INFINITE = KarelWorld.INFINITE;
	private static final int PLUS1 = KarelWorld.PLUS1;
	private static final int MINUS1 = KarelWorld.MINUS1;
	private static final int WALL_TOOL = 1;
	private static final int COLOR_TOOL = 2;
	private static final int ROBOT_TOOL = 3;
	private static final int BEEPER_TOOL = 4;
	private static final int BEEPER_BAG_TOOL = 5;
	private static final int BIG_TOOL_SIZE = 20;
	private static final int COLOR_TOOL_SIZE = 12;
	private static final int KAREL_TOOL_SIZE = 28;
	private static final int BEEPER_TOOL_SIZE = 28;
	private static final int TOOL_SEP = 6;
	private static final int TOOL_Y_DELTA = 8;
	// private static final int TOOL_MARGIN = 20;
	private static final int TOOL_X = 8;
	private static final int TOOL_Y = 3;
	// private static final int LABEL_SEP = 5;
	// private static final int ROBOT_DELTA = 300;
	// private static final int ROBOT_SIZE = 22;
	// private static final int ROBOT_SEP = 15;
	private static final int SELECTED_PIXELS = 3;
	private static final int WALL_LENGTH = 12;
	private static final int BEEPER_BAG_WIDTH = 35;
	private static final int BEEPER_BAG_HEIGHT = 47;
	private static final int BAG_LABEL_DELTA_Y = 28;
	private static final int WIDTH = 7 * BIG_TOOL_SIZE + 10 * TOOL_SEP;
	private static final int HEIGHT = 3
			* (TOOL_Y + BIG_TOOL_SIZE + TOOL_Y_DELTA) + 3 * COLOR_TOOL_SIZE
			+ TOOL_Y_DELTA;
	private static final Color COLORS[] = { null, Color.BLACK, Color.DARK_GRAY,
			Color.GRAY, Color.LIGHT_GRAY, Color.WHITE, Color.RED, Color.PINK,
			Color.ORANGE, Color.YELLOW, Color.GREEN, Color.CYAN, Color.BLUE,
			Color.MAGENTA };
	private static final int NCOLORS = COLORS.length;

	/* Private state */
	private ArrayList<MapTool> tools;
	private KarelWorld world;
	private MapTool selectedTool;
	private MapTool oldTool;
	private MapTool beeperBagTool;
	private Image beeperBagImage;

	public KarelWorldEditor(KarelWorld world) {
		this.world = world;
		initEditorCanvas();
		addMouseListener(this);
	}

	public void initEditorCanvas() {
		tools = new ArrayList<MapTool>();
		int x = TOOL_X;
		int y = TOOL_Y;
		createWallTool(x, y, "Draw Wall");
		createWallTool(x += BIG_TOOL_SIZE + TOOL_SEP, y, "Erase Wall");
		createBeeperTool(x += BIG_TOOL_SIZE + 2 * TOOL_SEP, y, "Single Beeper",
				1);
		createBeeperTool(x += BIG_TOOL_SIZE + TOOL_SEP, y, "Add Beeper", PLUS1);
		createBeeperTool(x += BIG_TOOL_SIZE + TOOL_SEP, y, "Subtract Beeper",
				MINUS1);
		createBeeperTool(x += BIG_TOOL_SIZE + TOOL_SEP, y, "Clear Beepers", 0);
		createBeeperTool(x += BIG_TOOL_SIZE + TOOL_SEP, y, "Infinite Beepers",
				INFINITE);
		if (world.getKarelCount() == 1) {
			Karel karel = world.getKarel();
			x = TOOL_X;
			y += BIG_TOOL_SIZE + TOOL_Y_DELTA;
			createBeeperBagTool(x + 2 * KAREL_TOOL_SIZE + TOOL_SEP + 2
					* BIG_TOOL_SIZE, y);
			createKarelTool(x, y, "East", EAST);
			createKarelTool(x += KAREL_TOOL_SIZE, y, "North", NORTH);
			x = TOOL_X;
			createKarelTool(x, y + KAREL_TOOL_SIZE, "West", WEST);
			createKarelTool(x += KAREL_TOOL_SIZE, y + KAREL_TOOL_SIZE, "South",
					SOUTH);
			if (karel instanceof SuperKarel) {
				int x0 = TOOL_X + 2 * BIG_TOOL_SIZE + 2 * TOOL_SEP
						- COLOR_TOOL_SIZE;
				x = x0;
				y += Math.max(2 * KAREL_TOOL_SIZE, BEEPER_BAG_HEIGHT)
						+ TOOL_Y_DELTA;
				for (int i = 0; i < NCOLORS; i++) {
					createColorTool(x += COLOR_TOOL_SIZE + TOOL_SEP, y,
							COLOR_TOOL_SIZE, COLORS[i]);
					if (COLORS[i] == Color.RED) {
						x = x0;
						y += BIG_TOOL_SIZE;
					}
				}
			}
		}
		selectedTool = tools.get(0);
	}

	public Dimension getPreferredSize() {
		return new Dimension(WIDTH, HEIGHT);
	}

	public MapTool getSelectedTool() {
		return selectedTool;
	}

	public KarelWorld getWorld() {
		return world;
	}

	public void drawTools(Graphics g) {
		Iterator<MapTool> iterator = tools.iterator();
		while (iterator.hasNext()) {
			drawTool(g, (MapTool) iterator.next());
		}
		if (beeperBagTool != null)
			drawBeeperBag(g);
	}

	public void drawKarelTool(Graphics g, MapTool tool) {
		world.drawFancyKarel(g, tool.x + tool.size / 2, tool.y + tool.size / 2,
				tool.dir, tool.size);
	}

	public void drawBeeperTool(Graphics g, MapTool tool) {
		int border = (tool == getSelectedTool()) ? SELECTED_PIXELS : 1;
		KarelWorld.drawBeeper(g, tool.x + tool.size / 2,
				tool.y + tool.size / 2, BEEPER_TOOL_SIZE, tool.beeperDelta,
				border, this);
	}

	public void drawBeeperBag(Graphics g) {
		int x = beeperBagTool.x;
		int y = beeperBagTool.y;
		if (beeperBagImage == null) {
			beeperBagImage = MediaTools.createImage(BEEPER_BAG);
		}
		g.drawImage(beeperBagImage, beeperBagTool.x, beeperBagTool.y, this);
		x += BEEPER_BAG_WIDTH / 2;
		y += BAG_LABEL_DELTA_Y;
		Karel karel = world.getKarel();
		int nBeepers = (karel == null) ? 0 : karel.getBeepersInBag();
		KarelWorld.drawBeeper(g, x, y, BEEPER_TOOL_SIZE, nBeepers, 1, this);
	}

	public void defineTool(MapTool tool) {
		tools.add(tool);
	}

	public MapTool createWallTool(int x, int y, String label) {
		MapTool tool = new MapTool(WALL_TOOL, x, y, BIG_TOOL_SIZE);
		tool.label = label;
		defineTool(tool);
		return tool;
	}

	public MapTool createColorTool(int x, int y, int size, Color color) {
		MapTool tool = new MapTool(COLOR_TOOL, x, y, size);
		tool.color = color;
		defineTool(tool);
		return tool;
	}

	public MapTool createKarelTool(int x, int y, String label, int dir) {
		MapTool tool = new MapTool(ROBOT_TOOL, x, y, KAREL_TOOL_SIZE);
		tool.label = label;
		tool.dir = dir;
		defineTool(tool);
		return tool;
	}

	public MapTool createBeeperTool(int x, int y, String label, int beeperDelta) {
		MapTool tool = new MapTool(BEEPER_TOOL, x, y, BIG_TOOL_SIZE);
		tool.label = label;
		tool.beeperDelta = beeperDelta;
		defineTool(tool);
		return tool;
	}

	public MapTool createBeeperBagTool(int x, int y) {
		beeperBagTool = new MapTool(BEEPER_BAG_TOOL, x, y, 0);
		return beeperBagTool;
	}

	public void drawTool(Graphics g, MapTool tool) {
		g.setColor(getBackground());
		int span = tool.size + 2 * (SELECTED_PIXELS - 1) + 1;
		g.fillRect(tool.x - (SELECTED_PIXELS - 1), tool.y
				- (SELECTED_PIXELS - 1), span, span);
		g.setColor(Color.BLACK);
		switch (tool.toolClass) {
		case WALL_TOOL:
			drawWallTool(g, tool);
			break;
		case COLOR_TOOL:
			drawColorTool(g, tool);
			break;
		case ROBOT_TOOL:
			drawKarelTool(g, tool);
			break;
		case BEEPER_TOOL:
			drawBeeperTool(g, tool);
			break;
		}
	}

	public void drawWallTool(Graphics g, MapTool tool) {
		int border = (tool == selectedTool) ? SELECTED_PIXELS : 1;
		drawSquare(g, tool.x, tool.y, tool.size, border, null);
		int x = tool.x + (tool.size - WALL_LENGTH + 1) / 2;
		int y = tool.y + (tool.size + 1) / 2;
		if (tool.label.equals("Erase Wall")) {
			g.setColor(Color.GRAY);
			g.drawRect(x, y - 1, WALL_LENGTH, 2);
			g.setColor(Color.BLACK);
		} else {
			g.fillRect(x, y - 1, WALL_LENGTH, 2);
		}
	}

	public void drawColorTool(Graphics g, MapTool tool) {
		int border = (tool == selectedTool) ? SELECTED_PIXELS : 1;
		Color color = null;
		if (tool.color == null) {
			color = null;
			int x = tool.x + tool.size / 2;
			int y = tool.y + tool.size / 2;
			g.setColor(Color.WHITE);
			g.fillRect(tool.x, tool.y, tool.size, tool.size);
			g.setColor(Color.BLACK);
			g.drawLine(x - 1, y, x + 1, y);
			g.drawLine(x, y - 1, x, y + 1);
		} else {
			color = tool.color;
		}
		drawSquare(g, tool.x, tool.y, tool.size, border, color);
	}

	public boolean inBeeperBag(Point pt) {
		if (beeperBagTool == null)
			return false;
		int x = beeperBagTool.x;
		int y = beeperBagTool.y;
		return (pt.x > x && pt.x < x + BEEPER_BAG_WIDTH && pt.y > y && pt.y < y
				+ BEEPER_BAG_HEIGHT);
	}

	/* Action callbacks */

	public void wallAction(Point pt, int dir) {
		MapTool tool = getSelectedTool();
		if (tool.toolClass != WALL_TOOL)
			return;
		if (tool.label.equals("Draw Wall")) {
			world.setWall(pt, dir);
			world.repaint();
		} else if (tool.label.equals("Erase Wall")) {
			world.clearWall(pt, dir);
			world.repaint();
		}
	}

	public void cornerAction(Point pt) {
		MapTool tool = getSelectedTool();
		if (tool.toolClass == COLOR_TOOL) {
			world.setCornerColor(pt, tool.color);
			world.repaint();
		} else if (tool.toolClass == BEEPER_TOOL) {
			int nBeepers = world.getBeepersOnCorner(pt);
			nBeepers = KarelWorld.setBeepers(nBeepers, tool.beeperDelta);
			world.setBeepersOnCorner(pt, nBeepers);
			world.repaint();
		}
	}

	public void toolAction(Point pt) {
		if (inBeeperBag(pt)) {
			MapTool tool = getSelectedTool();
			if (tool == null)
				return;
			if (tool.toolClass == BEEPER_TOOL) {
				Karel karel = world.getKarel();
				if (karel != null) {
					int nBeepers = karel.getBeepersInBag();
					nBeepers = KarelWorld
							.setBeepers(nBeepers, tool.beeperDelta);
					karel.setBeepersInBag(nBeepers);
					drawBeeperBag(getGraphics());
					repaint();
				}
			}
		} else {
			MapTool tool = findTool(pt);
			if (tool == null)
				return;
			if (tool.toolClass == ROBOT_TOOL) {
				Karel karel = world.getKarel();
				if (karel != null) {
					karel.setDirection(tool.dir);
				}
				world.repaint();
			} else {
				oldTool = selectedTool;
				selectedTool = tool;
				drawTool(getGraphics(), tool);
				drawTool(getGraphics(), oldTool);
				repaint();
			}
		}
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		if (g instanceof Graphics2D) {
			Graphics2D g2 = (Graphics2D) g;
			g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		}
		drawTools(g);
	}

	/* MouseListener interface */

	public void mousePressed(MouseEvent e) {
		toolAction(e.getPoint());
	}

	public void mouseClicked(MouseEvent e) {
	}

	public void mouseReleased(MouseEvent e) {
	}

	public void mouseEntered(MouseEvent e) {
	}

	public void mouseExited(MouseEvent e) {
	}

	/* Private methods */

	private void drawSquare(Graphics g, int x, int y, int size, int border,
			Color color) {
		if (color != null) {
			g.setColor(color);
			g.fillRect(x, y, size, size);
		}
		g.setColor(Color.BLACK);
		for (int i = 0; i < border; i++) {
			g.drawRect(x - i, y - i, size + 2 * i, size + 2 * i);
		}
	}

	private MapTool findTool(Point pt) {
		Iterator<MapTool> iterator = tools.iterator();
		while (iterator.hasNext()) {
			MapTool tool = iterator.next();
			if (tool.contains(pt))
				return tool;
		}
		return null;
	}

	private static class MapTool {
		public MapTool(int toolClass, int x, int y, int size) {
			this.toolClass = toolClass;
			this.x = x;
			this.y = y;
			this.size = size;
		}

		public boolean contains(Point pt) {
			return (pt.x >= x && pt.x < x + size && pt.y >= y && pt.y < y + size);
		}

		public int toolClass;
		public int x, y, dir, size, beeperDelta;
		public String label;
		public Color color;
	}
}

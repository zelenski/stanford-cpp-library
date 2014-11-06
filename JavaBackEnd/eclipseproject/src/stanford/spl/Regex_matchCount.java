package stanford.spl;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import acm.util.TokenScanner;

class Regex_matchCount extends JBECommand {
	// "Regex.matchCount(\"hello abcd hi abcccd how abccd are abd you? abccccccd ^_^\", \"ab[c]+d\")")"
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String s = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String regexp = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		Pattern pattern = Pattern.compile(regexp);
		Matcher matcher = pattern.matcher(s);
		int count = 0;
		while (matcher.find()) {
			count++;
		}

		SplPipeDecoder.writeResult(count);
	}

	
	// for testing only
	public static void main(String[] args) {
		String testCode = "/*\n * CS 106B, Homework 1, Game of Life\n * INSTRUCTOR'S SOLUTION, DO NOT DISTRIBUTE!\n */\n\n#include <fstream>     // for ifstream\n#include <iostream>    // for cout, cin, etc.\n#include <string>\n#include \"console.h\"   // required of all files that contain the main function\n#include \"filelib.h\"   // for openFile, fileExists, etc.\n#include \"grid.h\"      // for Grid class\n#include \"gwindow.h\"   // for pause\n#include \"simpio.h\"    // for getLine, getInteger, etc.\n#include \"lifegui.h\"   // for graphical display (optional)\n\nusing namespace std;\n\n// bad style, global vars, LOLOLOL\nint globalx;\nint globaly = 3;\nGrid<bool> badGrid;\n\n// bad style, function that takes a grid not-by-reference\nvoid foo(Grid<double> g, int x, int y) {\n    if (true == true) {         // bad boolean zen\n        int a = 3;\n        if (a < 2 != false) {   // bad boolean zen\n            // woooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooow line 28 sure is really long!  kekeke ^_^\n        }\n    }\n    // woooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooow line 31 sure is really long!  kekekekekekekeke ^_^\n}\n\n// to avoid conflicting with provided life.cpp,\n// comment out the define right below this comment to disable this file\n\n// function prototype declarations\nint countNeighbors(Grid<bool>& grid, int row, int col);\nvoid display(Grid<bool>& grid);\nvoid intro();\nvoid readFile(Grid<bool>& grid);\nvoid tick(Grid<bool>& grid);\nvoid updateGui(Grid<bool>& grid, LifeGUI& gui);\n\n\nint main() {\n    setConsoleSize(750, 500);\n    setConsoleFont(\"Monospaced-Bold-14\");\n    setConsoleEcho(true);\n\n    intro();\n    Grid<bool> grid;\n    readFile(grid);\n    display(grid);\n\n    LifeGUI gui;\n    gui.resize(grid.numRows(), grid.numCols());\n    updateGui(grid, gui);\n\n    // main menu loop\n    while (true) {\n        string input = getLine(\"a)nimate, t)ick, q)uit? \");\n        if (input == \"a\") {\n            int reps = getInteger(\"How many frames? \");\n            for (int i = 0; i < reps; i++) {\n                tick(grid);\n                clearConsole();\n                display(grid);\n                updateGui(grid, gui);\n                pause(50);\n            }\n        } else if (input == \"q\") {\n            break;\n        } else if (input == \"t\") {\n            tick(grid);\n            display(grid);\n            updateGui(grid, gui);\n        } else {\n            cout << \"Invalid choice; please try again.\" << endl;\n        }\n    }\n\n    cout << \"Have a nice Life!\" << endl;\n    // exitGraphics();\n    return 0;\n}\n\n/*\n * Returns the number of live neighbors of the given cell.\n */\nint countNeighbors(Grid<bool>& grid, int row, int col) {\n    int neighbors = 0;\n    for (int r = row - 1; r <= row + 1; r++) {\n        for (int c = col - 1; c <= col + 1; c++) {\n            // check each neighbor's state, if neighbor is on the board\n            if (grid.inBounds(r, c) && !(r == row && c == col)) {\n                if (grid[r][c]) {\n                    neighbors++;\n                }\n            }\n        }\n    }\n    return neighbors;\n}\n\n/*\n * Draws a text display of the given grid of cells on the console.\n */\nvoid display(Grid<bool>& grid) {\n    for (int row = 0; row < grid.numRows(); row++) {\n        for (int col = 0; col < grid.numCols(); col++) {\n            if (grid[row][col]) {\n                cout << 'X';\n            } else {\n                cout << '-';\n            }\n        }\n        cout << endl;\n    }\n}\n\n/*\n * Prints a short introductory message explaining the program.\n */\nvoid intro() {\n    cout << \"Welcome to the CS 106B Game of Life,\" << endl;\n    cout << \"a simulation of the lifecycle of a bacteria colony.\" << endl;\n    cout << \"Cells (X) live and die by the following rules:\" << endl;\n    cout << \"- A cell with 1 or fewer neighbors dies.\" << endl;\n    cout << \"- Locations with 2 neighbors remain stable.\" << endl;\n    cout << \"- Locations with 3 neighbors will create life.\" << endl;\n    cout << \"- A cell with 4 or more neighbors dies.\" << endl << endl;\n}\n\n/*\n * Reads input text data from the given file into the given\n * grid of rows and columns, where true represents a cell and false is empty.\n * Assumes the file exists and its contents are valid.  (not, for ex.cr?)\n */\nvoid readFile(Grid<bool>& grid) {\n    ifstream infile;\n//    string filename;\n//    while (true) {\n//        filename = getLine(\"Grid input file name? \");\n//        if (fileExists(filename)) {\n//            break;\n//        } else {\n//            cout << \"File not found; please try again.\" << endl;\n//        }\n//    }\n//    infile.open(filename);\n    promptUserForFile(infile, \"Grid input file name? \");\n\n    // first two lines of file are # of rows and columns\n    string rowColLine;\n    getline(infile, rowColLine);\n    int rows = stringToInteger(rowColLine);\n    getline(infile, rowColLine);\n    int cols = stringToInteger(rowColLine);\n\n    // remaining 'rows' number of lines are the grid square data\n    grid.resize(rows, cols);\n    for (int row = 0; row < rows; row++) {\n        string line;\n        getline(infile, line);\n        for (int col = 0; col < cols; col++) {\n            grid[row][col] = (line[col] == 'X');\n        }\n    }\n    infile.close();\n}\n\n/*\n * Advances the board state by one cycle ('tick') in the animation.\n */\nvoid tick(Grid<bool>& grid) {\n    // fill a temporary copy for new generation\n    Grid<bool> grid2(grid.numRows(), grid.numCols());\n    for (int row = 0; row < grid.numRows(); row++) {\n        for (int col = 0; col < grid.numCols(); col++) {\n            int neighbors = countNeighbors(grid, row, col);\n            if (neighbors <= 1 || neighbors >= 4) {\n                grid2[row][col] = false;\n            } else if (neighbors == 2) {\n                grid2[row][col] = grid[row][col];\n            } else {  // neighbors == 3\n                grid2[row][col] = true;\n            }\n        }\n    }\n    grid = grid2;   // copy grid2 back into grid\n}\n\n/*\n * Updates the state of the GUI to be in sync with the grid data.\n */\nvoid updateGui(Grid<bool>& grid, LifeGUI& gui) {\n    for (int r = 0; r < grid.numRows(); r++) {\n        for (int c = 0; c < grid.numCols(); c++) {\n            gui.drawCell(r, c, grid[r][c]);\n        }\n    }\n}\n";
		testCode = testCode.replace("\\n", "\n");
		testCode = testCode.replace("\\\"", "\"");
		System.out.println("testCode before = \n" + testCode + "\n===================================");
		testCode = SplPipeDecoder.encode(testCode);
		System.out.println("testCode after  = \n" + testCode + "\n===================================");
		
		Regex_matchCount command = new Regex_matchCount();
		String regexp = ".*\n";
		TokenScanner scanner = new TokenScanner("(\"" + testCode + "\",\"" + regexp + "\")");
		command.execute(scanner, null);
	}
}

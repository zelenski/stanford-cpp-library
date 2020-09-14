/**
 * File: mazegraphics.h
 * ---------------------
 * @author Julie Zelenski, Spring 2020
 * Defines a class to display a maze in a graphics window.
 */

#pragma once
#include <string>
#include "grid.h"
#include "stack.h"

// A namespace is just a grouping that separates these functions
// into their own category
namespace MazeGraphics {


    /**
     * Draws a visual rendition of the Grid to a graphics windows.
     * Cell with value "true" is an open corridor, drawn in white.
     * Cell with value "false" is a wall, drawn in dark gray.
     */
    void drawGrid(const Grid<bool>& g);


    /**
     * Draws a filled dot in center of each location in path in
     * the specified color.
     * Color string is either a named color such as "red", "blue"
     * "green" or a hex color "#33ff33"
     */
    void highlightPath(Stack<GridLocation> p, std::string color);


   /**
    * Prints current grid and path to console. Used for debugging and
    * text-only version of the program. In the printed form of a maze,
    * the character '@' is used for wall, space for corridor, and '+'
    * marks highlighted locations on the path.
    *
    * Example output:
    *                     +  @  @     @  @
    *                     +  +  +  +  +  @
    *                     @  @  @  @  +  @
    *                                 +  +
    */
     void printMaze();
 }





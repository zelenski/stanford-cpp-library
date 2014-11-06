/*
 * File: gfilechooser.cpp
 * ----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/07/09
 */

#include "gfilechooser.h"
#include "platform.h"

static Platform* pp = getPlatform();

std::string GFileChooser::showOpenDialog(std::string currentDir) {
    return pp->gfilechooser_showOpenDialog(currentDir);
}

std::string GFileChooser::showSaveDialog(std::string currentDir) {
    return pp->gfilechooser_showSaveDialog(currentDir);
}

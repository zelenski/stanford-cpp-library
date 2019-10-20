/*
 * File: platform.h
 * ----------------
 * This interface defines the platform-specific parts of the
 * <code>cslib</code> library.  This file is logically part of the
 * implementation and should not be interesting to clients.
 */

#ifndef _platform_h
#define _platform_h

#include "cslib.h"
#include "gevents.h"
#include "gobjects.h"
#include "ginteractors.h"
#include "gtypes.h"
#include "gwindow.h"
#include "sound.h"

void initPlatform(void);
string getId(void *ptr);

// Implementing the console requires another process and a select call
//
// void clearConsoleOp(void);
// void setConsoleFontOp(string font);
// void setConsoleSizeOp(double width, double height);

// string openFileDialogOp(string title, string mode, string path);

void createGWindowOp(GWindow gw, double width, double height, GObject top);
void deleteGWindowOp(GWindow gw);
void closeGWindowOp(GWindow gw);
void requestFocusOp(GWindow gw);
void clearOp(GWindow gw);
void repaintOp(GWindow gw);
void setVisibleGWindowOp(GWindow gw, bool flag);
void setResizableOp(GWindow gw, bool flag);
void setWindowTitleOp(GWindow gw, string title);
void setRegionAlignmentOp(GWindow gw, string region, string align);
void addToRegionOp(GWindow gw, GObject gobj, string region);
double getScreenWidthOp(void);
double getScreenHeightOp(void);
void exitGraphicsOp(void);
GEvent getNextEventOp(int mask);
GEvent waitForEventOp(int mask);
void createTimerOp(GTimer timer, double delay);
void deleteTimerOp(GTimer timer);
void startTimerOp(GTimer timer);
void stopTimerOp(GTimer timer);
void pauseOp(double milliseconds);
void createSoundOp(Sound sound, string filename);
void deleteSoundOp(Sound sound);
void playSoundOp(Sound sound);

void deleteGObjectOp(GObject gobj);

void createGRectOp(GRect rect, double width, double height);

void createGRoundRectOp(GRoundRect rect, double width, double height,
                                         double corner);

void createG3DRectOp(G3DRect rect, double width, double height, bool raised);
void setRaisedOp(G3DRect rect, bool raised);

void createGOvalOp(GOval oval, double width, double height);

void createGArcOp(GArc arc, double width, double height,
                            double start, double sweep);
void setStartAngleOp(GArc arc, double angle);
void setSweepAngleOp(GArc arc, double angle);
void setFrameRectangleOp(GArc arc, double x, double y,
                                   double width, double height);

void createGLineOp(GLine line, double x1, double y1, double x2, double y2);
void setStartPointOp(GObject gobj, double x, double y);
void setEndPointOp(GObject gobj, double x, double y);

GDimension createGImageOp(GObject gobj, string filename);

void createGPolygonOp(GPolygon poly);
void addVertexOp(GPolygon poly, double x, double y);

void setActionCommandOp(GInteractor interactor, string cmd);
GDimension getSizeOp(GInteractor interactor);
void createGButtonOp(GButton button, string label);
void createGCheckBoxOp(GCheckBox chkbox, string label);
bool isSelectedOp(GCheckBox chkbox);
void setSelectedOp(GCheckBox chkbox, bool state);
void createGSliderOp(GSlider slider, int min, int max, int value);
int getValueOp(GSlider slider);
void setValueOp(GSlider slider, int value);
void createGTextFieldOp(GTextField field, int nChars);
string getTextOp(GTextField field);
void setTextOp(GTextField field, string str);
void createGChooserOp(GChooser chooser);
void addItemOp(GChooser chooser, string item);
string getSelectedItemOp(GChooser chooser);
void setSelectedItemOp(GChooser chooser, string item);

void createGCompoundOp(GObject gobj);
void addOp(GObject compound, GObject gobj);
void removeOp(GObject gobj);
void sendForwardOp(GObject gobj);
void sendToFrontOp(GObject gobj);
void sendBackwardOp(GObject gobj);
void sendToBackOp(GObject gobj);

void setVisibleGObjectOp(GObject gobj, bool flag);
void setColorOp(GObject gobj, string color);
void setLocationOp(GObject gobj, double x, double y);
void setSizeOp(GObject gobj, double width, double height);

void drawOp(GWindow gw, GObject gobj);
void setFilledOp(GObject gobj, bool flag);
void setFillColorOp(GObject gobj, string color);

void createGLabelOp(GObject gobj, string label);
void setFontOp(GObject gobj, string font);
void setLabelOp(GObject gobj, string str);
double getFontAscentOp(GObject gobj);
double getFontDescentOp(GObject gobj);
GDimension getGLabelSizeOp(GObject gobj);

#endif

/*
 * File: platform.h
 * ----------------
 * This file defines the <code>Platform</code> class, which encapsulates
 * the platform-specific parts of the StanfordCPPLib package.  This file is
 * logically part of the implementation and is not interesting to clients.
 */

#ifndef _platform_h
#define _platform_h

#include <string>
#include <vector>
#include "gevents.h"
#include "gwindow.h"
#include "sound.h"

class Platform {
private:
   Platform();
   friend Platform *getPlatform();

public:
   virtual ~Platform();
   void clearConsole();
   void setConsoleFont(const std::string & font);
   void setConsoleSize(double width, double height);
   bool fileExists(std::string filename);
   bool isFile(std::string filename);
   bool isSymbolicLink(std::string filename);
   bool isDirectory(std::string filename);
   void setCurrentDirectory(std::string path);
   std::string getCurrentDirectory();
   void createDirectory(std::string path);
   std::string getDirectoryPathSeparator();
   std::string getSearchPathSeparator();
   std::string expandPathname(std::string filename);
   void listDirectory(std::string path, vector<std::string> & list);
   std::string openFileDialog(std::string title, std::string mode,
                              std::string path);
   void createGWindow(const GWindow & gw, double width, double height,
                      GObject *topCompound);
   void deleteGWindow(const GWindow & gw);
   void close(const GWindow & gw);
   void requestFocus(const GWindow & gw);
   void clear(const GWindow & gw);
   void repaint(const GWindow & gw);
   void setVisible(const GWindow & gw, bool flag);
   void setResizable(const GWindow & gw, bool flag);
   void setWindowTitle(const GWindow & gw, std::string title);
   void setRegionAlignment(const GWindow & gw, std::string region,
                                               std::string align);
   void addToRegion(const GWindow & gw, GObject *gobj, std::string region);
   void removeFromRegion(const GWindow & gw, GObject *gobj,
                                             std::string region);
   void pause(double milliseconds);
   double getScreenWidth();
   double getScreenHeight();
   GEvent waitForEvent(int mask);
   GEvent getNextEvent(int mask);
   void exitGraphics();
   void createTimer(const GTimer & timer, double delay);
   void deleteTimer(const GTimer & timer);
   void startTimer(const GTimer & timer);
   void stopTimer(const GTimer & timer);
   void createSound(Sound *sound, std::string filename);
   void deleteSound(Sound *sound);
   void playSound(Sound *sound);
   void createGRect(GObject *gobj, double width, double height);
   void createGRoundRect(GObject *gobj, double width, double height,
                                        double corner);
   void createG3DRect(GObject *gobj, double width, double height, bool raised);
   void setRaised(GObject *gobj, bool raised);
   void createGOval(GObject *gobj, double width, double height);
   void createGArc(GObject *gobj, double width, double height,
                                  double start, double sweep);
   void setStartAngle(GObject *gobj, double angle);
   void setSweepAngle(GObject *gobj, double angle);
   void createGLine(GObject *gobj, double x1, double y1, double x2, double y2);
   void setStartPoint(GObject *gobj, double x, double y);
   void setEndPoint(GObject *gobj, double x, double y);
   void createGLabel(GObject *gobj, std::string label);
   GDimension createGImage(GObject *gobj, std::string filename);
   void createGPolygon(GObject *gobj);
   void addVertex(GObject *gobj, double x, double y);
   void setActionCommand(GObject *gobj, std::string cmd);
   GDimension getSize(GObject *gobj);
   void createGButton(GObject *gobj, std::string label);
   void createGCheckBox(GObject *gobj, std::string label);
   bool isSelected(GObject *gobj);
   void setSelected(GObject *gobj, bool state);
   void createGSlider(GObject *gobj, int min, int max, int value);
   int getValue(GObject *gobj);
   void setValue(GObject *gobj, int value);
   void createGTextField(GObject *gobj, int nChars);
   std::string getText(GObject *gobj);
   void setText(GObject *gobj, std::string str);
   void createGChooser(GObject *gobj);
   void addItem(GObject *gobj, std::string item);
   std::string getSelectedItem(GObject *gobj);
   void setSelectedItem(GObject *gobj, std::string item);
   void createGCompound(GObject *gobj);
   void deleteGObject(GObject *gobj);
   void add(GObject *compound, GObject *gobj);
   void remove(GObject *gobj);
   void sendForward(GObject *gobj);
   void sendToFront(GObject *gobj);
   void sendBackward(GObject *gobj);
   void sendToBack(GObject *gobj);
   void setVisible(GObject *gobj, bool flag);
   void setColor(GObject *gobj, std::string color);
   void scale(GObject *gobj, double sx, double sy);
   void rotate(GObject *gobj, double theta);
   GRectangle getBounds(const GObject *gobj);
   bool contains(const GObject *gobj, double x, double y);
   void setLineWidth(GObject *gobj, double lineWidth);
   void setLocation(GObject *gobj, double x, double y);
   void setSize(GObject *gobj, double width, double height);
   void setFrameRectangle(GObject *gobj, double x, double y,
                                         double width, double height);
   void draw(const GWindow & gw, const GObject *gobj);
   void setFilled(GObject *gobj, bool flag);
   void setFillColor(GObject *gobj, std::string color);
   void setFont(GObject *gobj, std::string font);
   void setLabel(GObject *gobj, std::string str);
   double getFontAscent(const GObject *gobj);
   double getFontDescent(const GObject *gobj);
   GDimension getGLabelSize(const GObject *gobj);

};

Platform *getPlatform();

#endif

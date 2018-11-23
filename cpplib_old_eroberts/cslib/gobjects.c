/*
 * File: gobjects.c
 * ----------------
 * This file implements both the gobjects.h and ginteractors.h interfaces.
 */

#include <stdio.h>
#include <math.h>
#include "gmath.h"
#include "gobjects.h"
#include "platform.h"
#include "vector.h"

/* Constants */

#define LINE_TOLERANCE 1.5
#define ARC_TOLERANCE 2.5
#define DEFAULT_CORNER 10
#define DEFAULT_GLABEL_FONT "Dialog-13"

/*
 * Type: GArcCDT
 * -------------
 * This type holds the extra information needed for a GArc.
 */

struct GArcCDT {
   double start;
   double sweep;
};

/*
 * Type: GLabelCDT
 * ---------------
 * This type holds the extra information needed for a GLabel.
 */

struct GLabelCDT {
   string font;
   string str;
   double ascent;
   double descent;
};

/*
 * Type: GPolygonCDT
 * -----------------
 * This type holds the extra information needed for a GPolygon.
 */

struct GPolygonCDT {
   Vector vertices;
   double cx;
   double cy;
};

/*
 * Type: GCompoundCDT
 * ------------------
 * This type holds the extra information needed for a GCompound.
 */

struct GCompoundCDT {
   Vector contents;
};

/*
 * Type: G3DRectCDT
 * ----------------
 * This type holds the extra information needed for a G3DRect.
 */

struct G3DRectCDT {
   bool raised;
};

/*
 * Type: GInteractorCDT
 * --------------------
 * This type includes all the extra information required for any of
 * the interactor subtypes.
 */

struct GInteractorCDT {
   string actionCommand;
   string label;
};

/*
 * Type: GObjectCDT
 * ----------------
 * This structure is the common concrete type for all GObjects.
 */

struct GObjectCDT {
   int type;
   double x;
   double y;
   double width;
   double height;
   string color;
   string fillColor;
   bool filled;
   bool visible;
   GObject parent;
   union {
      struct GArcCDT arcRep;
      struct GLabelCDT labelRep;
      struct G3DRectCDT g3dRectRep;
      struct GPolygonCDT polygonRep;
      struct GCompoundCDT compoundRep;
      struct GInteractorCDT interactorRep;
   } u;
};

/* Private function prototypes */

static GDimension getSizeGInteractor(GInteractor interactor);

static GRectangle getBoundsGLine(GLine line);
static GRectangle getBoundsGLabel(GLabel label);
static GRectangle getBoundsGArc(GArc arc);
static GRectangle getBoundsGPolygon(GPolygon poly);

static bool containsGOval(GOval oval, double x, double y);
static bool containsGLine(GLine line, double x, double y);
static bool containsGArc(GArc arc, double x, double y);
static bool containsGPolygon(GArc arc, double x, double y);

static bool containsAngle(GArc arc, double theta);
static double dsq(double x0, double y0, double x1, double y1);

/* GObject operations */

static GObject newGObject(int type) {
   GObject gobj;

   gobj = newBlock(GObject);
   gobj->type = type;
   gobj->x = 0;
   gobj->y = 0;
   gobj->width = 0;
   gobj->height = 0;
   gobj->color = "BLACK";
   gobj->fillColor = NULL;
   gobj->filled = false;
   gobj->visible = true;
   gobj->parent = NULL;
   return gobj;
}

void freeGObject(GObject gobj) {
   Vector contents;
   int i, n;

   if (gobj->type == GPOLYGON) {
      contents = gobj->u.polygonRep.vertices;
      n = sizeVector(contents);
      for (i = 0; i < n; i++) {
         freeBlock(getVector(contents, i));
      }
      freeBlock(contents);
   } else if (gobj->type == GCOMPOUND) {
      freeBlock(gobj->u.compoundRep.contents);
   }
   freeBlock(gobj);
}

double getXGObject(GObject gobj) {
   return gobj->x;
}

double getYGObject(GObject gobj) {
   return gobj->y;
}

GPoint getLocation(GObject gobj) {
   return createGPoint(gobj->x, gobj->y);
}

void setLocation(GObject gobj, double x, double y) {
   gobj->x = x;
   gobj->y = y;
   setLocationOp(gobj, x, y);
}

void move(GObject gobj, double dx, double dy) {
   gobj->x += dx;
   gobj->y += dy;
   setLocation(gobj, gobj->x, gobj->y);
}

double getWidthGObject(GObject gobj) {
   return getBounds(gobj).width;
}

double getHeightGObject(GObject gobj) {
   return getBounds(gobj).height;
}

GDimension getSize(GObject gobj) {
   GRectangle bounds;

   if ((gobj->type & GINTERACTOR) != 0) return getSizeGInteractor(gobj);
   bounds = getBounds(gobj);
   return createGDimension(bounds.width, bounds.height);
}

GRectangle getBounds(GObject gobj) {
   if (gobj->type == GARC) return getBoundsGArc(gobj);
   if (gobj->type == GLINE) return getBoundsGLine(gobj);
   if (gobj->type == GLABEL) return getBoundsGLabel(gobj);
   if (gobj->type == GPOLYGON) return getBoundsGPolygon(gobj);
   return createGRectangle(gobj->x, gobj->y, gobj->width, gobj->height);
}

void setColorGObject(GObject gobj, string color) {
   gobj->color = color;
   setColorOp(gobj, color);
}

string getColorGObject(GObject gobj) {
   return gobj->color;
}

void setVisibleGObject(GObject gobj, bool flag) {
   gobj->visible = flag;
   setVisibleGObjectOp(gobj, flag);
}

bool isVisibleGObject(GObject gobj) {
   return gobj->visible;
}

void sendForward(GObject gobj) {
   sendForwardOp(gobj);
}

void sendToFront(GObject gobj) {
   sendToFrontOp(gobj);
}

void sendBackward(GObject gobj) {
   sendBackwardOp(gobj);
}

void sendToBack(GObject gobj) {
   sendToBackOp(gobj);
}

bool containsGObject(GObject gobj, double x, double y) {
   if (gobj->type == GOVAL) return containsGOval(gobj, x, y);
   if (gobj->type == GARC) return containsGArc(gobj, x, y);
   if (gobj->type == GPOLYGON) return containsGPolygon(gobj, x, y);
   return x >= gobj->x && y >= gobj->y && x <= gobj->x + gobj->width
                                       && y <= gobj->y + gobj->height;
}

string getType(GObject gobj) {
   switch (gobj->type) {
    case GARC: return "GArc";
    case GCOMPOUND: return "GCompound";
    case GIMAGE: return "GImage";
    case GLABEL: return "GLabel";
    case GLINE: return "GLine";
    case GOVAL: return "GOval";
    case GPOLYGON: return "GPolygon";
    case GRECT: return "GRect";
    case G3DRECT: return "G3DRect";
    case GROUNDRECT: return "GRoundRect";
    case GBUTTON: return "GButton";
    case GCHECKBOX: return "GCheckBox";
    case GCHOOSER: return "GChooser";
    case GSLIDER: return "GSlider";
    case GTEXTFIELD: return "GTextField";
    default: return NULL;
   }
}

GObject getParent(GObject gobj) {
   return gobj->parent;
}

void setSize(GObject gobj, double width, double height) {
   if ((gobj->type & (GRECT | GOVAL | GIMAGE | GINTERACTOR)) == 0) {
      error("setSize: Illegal GObject type");
   }
   gobj->width = width;
   gobj->height = height;
   setSizeOp(gobj, width, height);
}

void setBounds(GObject gobj, double x, double y, double width, double height) {
   setSize(gobj, width, height);
   setLocation(gobj, x, y);
}

void setFilled(GObject gobj, bool flag) {
   gobj->filled = flag;
   setFilledOp(gobj, flag);
}

bool isFilled(GObject gobj) {
   return gobj->filled;
}

void setFillColor(GObject gobj, string color) {
   gobj->fillColor = color;
   setFillColorOp(gobj, color);
}

string getFillColor(GObject gobj) {
   return gobj->fillColor;
}

/*
 * Implementation notes: GRect
 * ---------------------------
 * This section of the implementation includes the subclasses.
 */

GRect newGRect(double x, double y, double width, double height) {
   GObject rect;

   rect = newGObject(GRECT);
   rect->x = x;
   rect->y = y;
   rect->width = width;
   rect->height = height;
   createGRectOp(rect, width, height);
   setLocationOp(rect, x, y);
   return rect;
}

GRoundRect newGRoundRect(double x, double y, double width, double height,
                                             double corner) {
   GObject rect;

   rect = newGObject(GROUNDRECT);
   rect->x = x;
   rect->y = y;
   rect->width = width;
   rect->height = height;
   createGRoundRectOp(rect, width, height, corner);
   setLocationOp(rect, x, y);
   return rect;
}

G3DRect newG3DRect(double x, double y, double width, double height,
                                       bool raised) {
   GObject rect;

   rect = newGObject(G3DRECT);
   rect->x = x;
   rect->y = y;
   rect->width = width;
   rect->height = height;
   rect->u.g3dRectRep.raised = raised;
   createG3DRectOp(rect, width, height, raised);
   setLocationOp(rect, x, y);
   return rect;
}

void setRaised(G3DRect rect, bool raised) {
   rect->u.g3dRectRep.raised = raised;
   setRaisedOp(rect, raised);
}

bool isRaised(G3DRect rect) {
   return rect->u.g3dRectRep.raised;
}

/*
 * Implementation notes: GOval
 * ---------------------------
 * The GOval class requires only the constructor.
 */

GOval newGOval(double x, double y, double width, double height) {
   GOval oval;

   oval = newGObject(GOVAL);
   oval->x = x;
   oval->y = y;
   oval->width = width;
   oval->height = height;
   createGOvalOp(oval, width, height);
   setLocationOp(oval, x, y);
   return oval;
}

static bool containsGOval(GOval oval, double x, double y) {
   double rx, ry, dx, dy;

   rx = oval->width / 2;
   ry = oval->height / 2;
   if (rx == 0 || ry == 0) return false;
   dx = x - (oval->x + rx);
   dy = y - (oval->y + ry);
   return (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry) <= 1.0;
}

/*
 * Implementation notes: GLine
 * ---------------------------
 * The GLine type uses the width and height fields to specify the
 * displacement from the start point to the end point.  These values
 * may be negative.  This type also includes functions to set the
 * start and end points.
 */

GLine newGLine(double x0, double y0, double x1, double y1) {
   GLine line;

   line = newGObject(GLINE);
   line->x = x0;
   line->y = y0;
   line->width = x1 - x0;
   line->height = y1 - y0;
   createGLineOp(line, x0, y0, x1, y1);
   return line;
}

void setStartPoint(GLine line, double x, double y) {
   if (line->type != GLINE) error("setStartPoint: Illegal argument type");
   line->width += line->x - x;
   line->height += line->y - y;
   line->x = x;
   line->y = y;
   setStartPointOp(line, x, y);
}

void setEndPoint(GObject line, double x, double y) {
   if (line->type != GLINE) error("setStartPoint: Illegal argument type");
   line->width = x - line->x;
   line->height = y - line->y;
   setEndPointOp(line, x, y);
}

GPoint getStartPoint(GObject gobj) {
   if (gobj->type != GLINE) {
      return createGPoint(gobj->x, gobj->y);
   }
   error("getStartPoint: Illegal argument type");
   return createGPoint(0, 0);
}

GPoint getEndPoint(GObject gobj) {
   if (gobj->type != GLINE) {
      return createGPoint(gobj->x + gobj->width, gobj->y + gobj->height);
   }
   error("getEndPoint: Illegal argument type");
   return createGPoint(0, 0);
}

static GRectangle getBoundsGLine(GLine line) {
   return createGRectangle(fmin(line->x, line->x + line->width),
                           fmin(line->x, line->x + line->width),
                           fabs(line->width), fabs(line->height));
}

static bool containsGLine(GLine line, double x, double y) {
   double x0, y0, x1, y1, tsq, u;
   x0 = line->x;
   y0 = line->y;
   x1 = x0 + line->width;
   y1 = y0 + line->height;
   tsq = LINE_TOLERANCE * LINE_TOLERANCE;
   if (dsq(x, y, x0, y0) < tsq) return true;
   if (dsq(x, y, x1, y1) < tsq) return true;
   if (x < fmin(x0, x1) - LINE_TOLERANCE) return false;
   if (x > fmax(x0, x1) + LINE_TOLERANCE) return false;
   if (y < fmin(y0, y1) - LINE_TOLERANCE) return false;
   if (y > fmax(y0, y1) + LINE_TOLERANCE) return false;
   if ((float) (x0 - x1) == 0 && (float) (y0 - y1) == 0) return false;
   u = ((x - x0) * (x1 - x0) + (y - y0) * (y1 - y0)) / dsq(x0, y0, x1, y1);
   return dsq(x, y, x0 + u * (x1 - x0), y0 + u * (y1 - y0)) < tsq;
}

/*
 * Implementation notes: GArc
 * --------------------------
 * Parts of the GArc implementation appear in the general GObject code
 * because the functions are often overloaded.
 */

GArc newGArc(double x, double y, double width, double height,
                                 double start, double sweep) {
   GObject arc;

   arc = newGObject(GARC);
   arc->x = x;
   arc->y = y;
   arc->width = width;
   arc->height = height;
   arc->u.arcRep.start = start;
   arc->u.arcRep.sweep = sweep;
   createGArcOp(arc, width, height, start, sweep);
   setLocationOp(arc, x, y);
   return arc;
}

void setStartAngle(GArc arc, double start) {
   arc->u.arcRep.start = start;
   setStartAngleOp(arc, start);
}

double getStartAngle(GArc arc) {
   return arc->u.arcRep.start;
}

void setSweepAngle(GArc arc, double sweep) {
   arc->u.arcRep.sweep = sweep;
   setSweepAngleOp(arc, sweep);
}

double getSweepAngle(GArc arc) {
   return arc->u.arcRep.sweep;
}

void setFrameRectangle(GArc arc, double x, double y,
                                 double width, double height) {
   arc->x = x;
   arc->y = y;
   arc->width = width;
   arc->height = height;
   setFrameRectangleOp(arc, x, y, width, height);
}

GRectangle getFrameRectangle(GArc arc) {
   return createGRectangle(arc->x, arc->y, arc->width, arc->height);
}

static GRectangle getBoundsGArc(GArc arc) {
   double rx, ry, cx, cy, p1x, p1y, p2x, p2y, xMin, xMax, yMin, yMax;

   rx = arc->width / 2;
   ry = arc->height / 2;
   cx = arc->x + rx;
   cy = arc->y + ry;
   p1x = cx + cosDegrees(arc->u.arcRep.start) * rx;
   p1y = cy - sinDegrees(arc->u.arcRep.start) * ry;
   p2x = cx + cosDegrees(arc->u.arcRep.start + arc->u.arcRep.sweep) * rx;
   p2y = cy - sinDegrees(arc->u.arcRep.start + arc->u.arcRep.sweep) * ry;
   xMin = fmin(p1x, p2x);
   xMax = fmax(p1x, p2x);
   yMin = fmin(p1y, p2y);
   yMax = fmax(p1y, p2y);
   if (containsAngle(arc, 0)) xMax = cx + rx;
   if (containsAngle(arc, 90)) yMin = cy - ry;
   if (containsAngle(arc, 180)) xMin = cx - rx;
   if (containsAngle(arc, 270)) yMax = cy + ry;
   if (arc->filled) {
      xMin = fmin(xMin, cx);
      yMin = fmin(yMin, cy);
      xMax = fmax(xMax, cx);
      yMax = fmax(yMax, cy);
   }
   return createGRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
}

static bool containsGArc(GArc arc, double x, double y) {
   double rx, ry, dx, dy, r, t;

   rx = arc->width / 2;
   ry = arc->height / 2;
   if (rx == 0 || ry == 0) return false;
   dx = x - (arc->x + rx);
   dy = y - (arc->y + ry);
   r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
   if (arc->filled) {
      if (r > 1.0) return false;
   } else {
      t = ARC_TOLERANCE / ((rx + ry) / 2);
      if (abs(1.0 - r) > t) return false;
   }
   return containsAngle(arc, atan2(-dy, dx) * 180 / PI);
}

static bool containsAngle(GArc arc, double theta) {
   double start, sweep;

   start = fmin(arc->u.arcRep.start,
                arc->u.arcRep.start + arc->u.arcRep.sweep);
   sweep = fabs(arc->u.arcRep.sweep);
   if (sweep >= 360) return true;
   theta = (theta < 0) ? 360 - fmod(-theta, 360) : fmod(theta, 360);
   start = (start < 0) ? 360 - fmod(-start, 360) : fmod(start, 360);
   if (start + sweep > 360) {
      return theta >= start || theta <= start + sweep - 360;
   } else {
      return theta >= start && theta <= start + sweep;
   }
}

/*
 * Implementation notes: GLabel
 * ----------------------------
 */

GLabel newGLabel(string str) {
   GObject label;
   GDimension size;

   label = newGObject(GLABEL);
   label->u.labelRep.str = str;
   label->u.labelRep.font = DEFAULT_GLABEL_FONT;
   createGLabelOp(label, str);
   setFont(label, DEFAULT_GLABEL_FONT);
   setLabel(label, str);
   return label;
}

void setFont(GLabel label, string font) {
   GDimension size;

   label->u.labelRep.font = font;
   setFontOp(label, font);
   label->u.labelRep.ascent = getFontAscentOp(label);
   label->u.labelRep.descent = getFontDescentOp(label);
   size = getGLabelSizeOp(label);
   label->width = size.width;
   label->height = size.height;
}

string getFont(GLabel label) {
   return label->u.labelRep.font;
}

void setLabel(GLabel label, string str) {
   GDimension size;

   label->u.labelRep.str = str;
   setLabelOp(label, str);
   size = getGLabelSizeOp(label);
   label->width = size.width;
   label->height = size.height;
}

string getLabel(GLabel label) {
   return label->u.labelRep.str;
}

double getFontAscent(GLabel label) {
   return label->u.labelRep.ascent;
}

double getFontDescent(GLabel label) {
   return label->u.labelRep.descent;
}

static GRectangle getBoundsGLabel(GLabel label) {
   return createGRectangle(label->x, label->y - label->u.labelRep.ascent,
                           label->width, label->height);
}

GImage newGImage(string filename) {
   GPolygon image;
   GDimension size;

   image = newGObject(GIMAGE);
   size = createGImageOp(image, filename);
   image->width = size.width;
   image->height = size.height;
   return image;
}

GPolygon newGPolygon(void) {
   GPolygon poly = newGObject(GPOLYGON);
   poly->u.polygonRep.vertices = newVector();
   poly->u.polygonRep.cx = 0;
   poly->u.polygonRep.cy = 0;
   createGPolygonOp(poly);
   return poly;
}

void addVertex(GPolygon poly, double x, double y) {
   GPoint *ppt;

   ppt = newBlock(GPoint *);
   poly->u.polygonRep.cx = ppt->x = x;
   poly->u.polygonRep.cy = ppt->y = y;
   addVector(poly->u.polygonRep.vertices, ppt);
   addVertexOp(poly, x, y);
}

void addEdge(GPolygon poly, double dx, double dy) {
   addVertex(poly, poly->u.polygonRep.cx + dx, poly->u.polygonRep.cy + dy);
}

void addPolarEdge(GPolygon poly, double r, double theta) {
   addEdge(poly, r * cosDegrees(theta), -r * sinDegrees(theta));
}

Vector getVertices(GPolygon poly) {
   return poly->u.polygonRep.vertices;
}

static GRectangle getBoundsGPolygon(GPolygon poly) {
   Vector vertices;
   double xMin, yMin, xMax, yMax, x, y;
   int i, n;

   xMin = 0;
   yMin = 0;
   xMax = 0;
   yMax = 0;
   vertices = poly->u.polygonRep.vertices;
   n = sizeVector(vertices);
   for (i = 0; i < n; i++) {
      x = ((GPoint *) getVector(vertices, i))->x;
      y = ((GPoint *) getVector(vertices, i))->y;
      if (i == 0 || x < xMin) xMin = x;
      if (i == 0 || y < yMin) yMin = y;
      if (i == 0 || x > xMax) xMax = x;
      if (i == 0 || y > yMax) yMax = y;
   }
   return createGRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
}

static bool containsGPolygon(GArc arc, double x, double y) {
   double x0, y0, x1, y1;
   int crossings, i, n;
   GPoint *p0, *p1;
   Vector vertices;

   vertices = arc->u.polygonRep.vertices;
   crossings = 0;
   n = sizeVector(vertices);
   if (n < 2) return false;
   p0 = (GPoint *) getVector(vertices, 0);
   x0 = p0->x;
   y0 = p0->y;
   p1 = (GPoint *) getVector(vertices, n - 1);
   if (p0->x == p1->x && p0->y == p1->y) n--;
   for (i = 1; i <= n; i++) {
      p1 = (GPoint *) getVector(vertices, i % n);
      x1 = p1->x;
      y1 = p1->y;
      if ((y0 > y) != (y1 > y) && x - x0 < (x1 - x0) * (y - y0) / (y1 - y0)) {
         crossings++;
      }
      x0 = x1;
      y0 = y1;
   }
   return (crossings % 2 == 1);
}

/*
 * Implementation notes: GCompound
 * -------------------------------
 * The GCompound type stores the individual components in a vector
 * that runs from back to front in the z direction.
 */

GCompound newGCompound(void) {
   GCompound gcompound = newGObject(GCOMPOUND);
   gcompound->u.compoundRep.contents = newVector();
   createGCompoundOp(gcompound);
   return gcompound;
}

void addGCompound(GObject compound, GObject gobj) {
   if (compound->type != GCOMPOUND) {
      error("add: First argument is not a GCompound");
   }
   addVector(compound->u.compoundRep.contents, gobj);
   addOp(compound, gobj);
}

void removeGCompound(GCompound compound, GObject gobj) {
   Vector contents;
   int i, n;

   if (compound->type != GCOMPOUND) {
      error("remove: First argument is not a GCompound");
   }
   contents = compound->u.compoundRep.contents;
   n = sizeVector(contents);
   for (i = 0; i < n; i++) {
      if (gobj == getVector(contents, i)) {
         removeVector(contents, i);
         break;
      }
   }
   removeOp(gobj);
}

GObject getGObjectCompound(GCompound compound, double x, double y) {
   Vector contents;
   GObject gobj;
   int i, n;

   contents = compound->u.compoundRep.contents;
   n = sizeVector(contents);
   for (i = n - 1; i >= 0; i--) {
      gobj = getVector(contents, i);
      if (containsGObject(gobj, x, y)) return gobj;
   }
   return NULL;
}

static double dsq(double x0, double y0, double x1, double y1) {
   return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

/* GInteractor operations */

void setActionCommand(GInteractor interactor, string cmd) {
   interactor->u.interactorRep.actionCommand = cmd;
   setActionCommandOp(interactor, cmd);
}

string getActionCommandGInteractor(GInteractor interactor) {
   return interactor->u.interactorRep.actionCommand;
}

static GDimension getSizeGInteractor(GInteractor interactor) {
   return getSizeOp(interactor);
}

/* GButton operations */

GButton newGButton(string label) {
   GButton button = newGObject(GBUTTON);
   button->u.interactorRep.label = label;
   createGButtonOp(button, label);
   setActionCommand(button, label);
   return button;
}

/* GCheckBox operations */

GCheckBox newGCheckBox(string label)  {
   GCheckBox chkbox = newGObject(GCHECKBOX);
   chkbox->u.interactorRep.label = label;
   createGCheckBoxOp(chkbox, label);
   return chkbox;
}

/*
 * Function: setSelected
 * Usage: setSelected(chkbox, state);
 * ----------------------------------
 * Sets the state of the check box.
 */

void setSelected(GCheckBox chkbox, bool state) {
   setSelectedOp(chkbox, state);
}

/*
 * Function: isSelected
 * Usage: if (isSelected(chkbox)) ...
 * ----------------------------------
 * Returns <code>true</code> if the check box is selected.
 */

bool isSelected(GCheckBox chkbox) {
   return isSelectedOp(chkbox);
}

/* GSlider operations */

GSlider newGSlider(int min, int max, int value) {
   GSlider slider = newGObject(GSLIDER);
   createGSliderOp(slider, min, max, value);
   return slider;
}

/*
 * Function: setValue
 * Usage: setValue(slider, value);
 * -------------------------------
 * Sets the current value of the slider.
 */

void setValue(GSlider slider, int value) {
   setValueOp(slider, value);
}

/*
 * Function: getValue
 * Usage: value = getValue(slider);
 * --------------------------------
 * Returns the current value of the slider.
 */

int getValue(GSlider slider) {
   return getValueOp(slider);
}

/* GTextField operations */

GTextField newGTextField(int nChars) {
   GTextField field = newGObject(GTEXTFIELD);
   createGTextFieldOp(field, nChars);
   return field;
}

void setText(GTextField field, string str) {
   setTextOp(field, str);
}

string getText(GTextField field) {
   return getTextOp(field);
}

/* GChooser operations */

GChooser newGChooser(void) {
   GChooser chooser = newGObject(GCHOOSER);
   createGChooserOp(chooser);
   return chooser;
}

void addItem(GChooser chooser, string item) {
   addItemOp(chooser, item);
}

void setSelectedItem(GChooser chooser, string item) {
   setSelectedItemOp(chooser, item);
}

// Think about memory management here

string getSelectedItem(GChooser chooser) {
   return getSelectedItemOp(chooser);
}

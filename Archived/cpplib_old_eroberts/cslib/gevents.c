/*
 * File: gevents.c
 * ---------------
 * This file implements the machine-independent functions for the event
 * types defined in the gevents.h interface.  The actual functions for
 * receiving events from the environment are implemented in the platform
 * package.
 */

#include "cslib.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gtimer.h"
#include "gtypes.h"
#include "hashmap.h"
#include "platform.h"
#include "strlib.h"

/* Constants */

#define EVENT_SUBTYPE_MASK 0xF

/* Global variables */

static HashMap timerTable;

/* Implementation of GEvent class */

struct GEventCDT {
   int eventType;
   int modifiers;
   double eventTime;
   GWindow gw;
   GObject source;
   string actionCommand;
   double x;
   double y;
   int keyChar;
   int keyCode;
   GTimer timer;
};

EventClassType getEventClass(GEvent e) {
   return e->eventType & ~EVENT_SUBTYPE_MASK;
}

EventType getEventType(GEvent e) {
   return e->eventType;
}

double getEventTime(GEvent e) {
   return e->eventTime;
}

void setEventTime(GEvent e, double time) {
   e->eventTime = time;
}

int getModifiers(GEvent e) {
   return e->modifiers;
}

void setModifiers(GEvent e, int modifiers) {
   e->modifiers = modifiers;
}

void waitForClick() {
   freeEvent(waitForEvent(CLICK_EVENT));
}

GEvent waitForEvent(int mask) {
   return waitForEventOp(mask);
}

GEvent getNextEvent(int mask) {
   return getNextEventOp(mask);
}

void freeEvent(GEvent e) {
   if (e->eventType == ACTION_EVENT && e->actionCommand != NULL) {
      freeBlock(e->actionCommand);
   }
   freeBlock(e);
}

GEvent newGWindowEvent(EventType type, GWindow gw) {
   GEvent e;

   e = newBlock(GEvent);
   e->eventType = type;
   e->gw = gw;
   return e;
}

GEvent newGActionEvent(EventType type, GObject source, string actionCommand) {
   GEvent e;

   e = newBlock(GEvent);
   e->eventType = type;
   e->source = source;
   e->actionCommand = copyString(actionCommand);
   return e;
}

/*
 * Function: newGMouseEvent
 * Usage: e = newGMouseEvent(type, gw, x, y);
 * ------------------------------------------
 * Creates a <code>GEvent</code> of the <code>MOUSE_EVENT</code> class.
 * The parameters are the specific event type, the <code>GWindow</code>
 * in which the event occurred, and the coordinates of the mouse.
 */

GEvent newGMouseEvent(EventType type, GWindow gw, double x, double y) {
   GEvent e;

   e = newBlock(GEvent);
   e->eventType = type;
   e->gw = gw;
   e->x = x;
   e->y = y;
   return e;
}

GEvent newGKeyEvent(EventType type, GWindow gw, int keyChar, int keyCode) {
   GEvent e;

   e = newBlock(GEvent);
   e->eventType = type;
   e->gw = gw;
   e->keyChar = keyChar;
   e->keyCode = keyCode;
   return e;
}

GEvent newGTimerEvent(EventType type, GTimer timer) {
   GEvent e;

   e = newBlock(GEvent);
   e->eventType = type;
   e->timer = timer;
   return e;
}

GWindow getGWindow(GEvent e) {
   if (e->eventType & (WINDOW_EVENT | MOUSE_EVENT | KEY_EVENT)) {
      return e->gw;
   }
   error("getGWindow: Illegal event type");
   return NULL;
}

GObject getSource(GEvent e) {
   if (e->eventType & ACTION_EVENT) {
      return e->source;
   }
   error("getSource: Illegal event type");
   return NULL;
}

string getActionCommand(void *arg) {
   GEvent e;
   string type;

   type = getBlockType(arg);
   if (endsWith(type, "GObject")) {
      return getActionCommandGInteractor(arg);
   } else if (endsWith(type, "GEvent")) {
      e = (GEvent) arg;
      if (e->eventType & ACTION_EVENT) {
         return e->actionCommand;
      }
   }
   error("getActionCommand: Illegal argument type");
   return NULL;
}

double getXGEvent(GEvent e) {
   if (e->eventType & MOUSE_EVENT) {
      return e->x;
   }
   error("getX: Illegal event type");
   return 0;
}

double getYGEvent(GEvent e) {
   if (e->eventType & MOUSE_EVENT) {
      return e->y;
   }
   error("getY: Illegal event type");
   return 0;
}

char getKeyChar(GEvent e) {
   if (e->eventType & KEY_EVENT) {
      return e->keyChar;
   }
   error("getKeyChar: Illegal event type");
   return '\0';
}

int getKeyCode(GEvent e) {
   if (e->eventType & KEY_EVENT) {
      return e->keyCode;
   }
   error("getKeyCode: Illegal event type");
   return 0;
}

GTimer getGTimer(GEvent e) {
   if (e->eventType & TIMER_EVENT) {
      return e->timer;
   }
   error("getGTimer: Illegal event type");
   return NULL;
}

/**********************************************************************/
/* Unit test for the gevents module                                   */
/**********************************************************************/

#ifndef _NOTEST_

#include "unittest.h"

/* Unit test */

void testGEventsModule(void) {
   GEvent e;

   trace(e = newGMouseEvent(MOUSE_CLICKED, NULL, 2.3, 4.5));
   test(getEventClass(e), (int) MOUSE_EVENT);
   test(getEventType(e), (int) MOUSE_CLICKED);
   test(getGWindow(e), NULL);
   test(getX(e), 2.3);
   test(getY(e), 4.5);
}

#endif

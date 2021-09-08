/*
 * File: ginteractors.h
 * ---------------------
 *
 * This file serves as a convenient way to include all Qt-based graphical
 * interactors in your project with a single include statement.
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to ginteractors.h to replace Java version
 * @version 2018/07/04
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _ginteractors_h
#define _ginteractors_h

// not wrapped with INTERNAL_INCLUDE since it would lose signal of student
// including this file in their code; no internal lib files include this header
#include "gbrowserpane.h"
#include "gbufferedimage.h"
#include "gbutton.h"
#include "gcanvas.h"
#include "gcheckbox.h"
#include "gchooser.h"
#include "gcolor.h"
#include "gcontainer.h"
#include "gfilechooser.h"
#include "gfont.h"
#include "ginteractor.h"
#include "glabel.h"
#include "goptionpane.h"
#include "gradiobutton.h"
#include "gscrollbar.h"
#include "gscrollpane.h"
#include "gslider.h"
#include "gspacer.h"
#include "gtable.h"
#include "gtextarea.h"
#include "gtextfield.h"
#include "gwindow.h"

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _ginteractors_h

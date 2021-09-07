/*
 * File: gformattedpane.h
 * ----------------------
 *
 * This file is simply an alias for gbrowserpane.h, provided for consistency
 * because the old Java-based graphics library called its file "gformattedpane.h".
 * Future clients are advised to use the new name, GBrowserPane,
 * and include gbrowserpane.h rather than this file.
 *
 * @author Marty Stepp
 * @version 2018/08/27
 * - initial version
 */

#ifndef _gformattedpane_h
#define _gformattedpane_h

#include "gbrowserpane.h"

// alias as GFormattedPane for backward compatibility
typedef GBrowserPane GFormattedPane;

#endif // _gformattedpane_h

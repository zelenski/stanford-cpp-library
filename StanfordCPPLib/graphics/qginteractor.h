/*
 * File: qginteractor.h
 * --------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qginteractor_h
#define _qginteractor_h

#include <string>
#include <QObject>
#include <QWidget>

/*
 * ...
 */
class QGInteractor : public QObject {
    Q_OBJECT

public:
    virtual QWidget* getWidget() const = 0;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qginteractor_h

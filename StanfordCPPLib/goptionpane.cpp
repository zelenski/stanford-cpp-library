#include "goptionpane.h"
#include "platform.h"
using namespace std;

static Platform* pp = getPlatform();

void GOptionPane::showMessageDialog(string message, int /*type*/) {
    pp->goptionpane_showMessageDialog(message);
}

GOptionPaneResult GOptionPane::showConfirmDialog(string message) {
    int result = pp->goptionpane_showConfirmDialog(message);
    if (result == OK_OPTION) {
        return GOPTIONPANE_OK;
    } else if (result == CANCEL_OPTION) {
        return GOPTIONPANE_CANCEL;
    } else {
        return GOPTIONPANE_ERROR;
    }
}

string GOptionPane::showInputDialog(string message) {
    return pp->goptionpane_showInputDialog(message);
}

string GOptionPane::showOptionDialog(const Vector<string>& options, string initiallySelected) {
    return pp->goptionpane_showOptionDialog(options.toStlVector(), initiallySelected);
}

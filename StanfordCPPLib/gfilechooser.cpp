#include "gfilechooser.h"
#include "platform.h"
using namespace std;

static Platform* pp = getPlatform();

string GFileChooser::showOpenDialog(string currentDir) {
    return pp->gfilechooser_showOpenDialog(currentDir);
}

string GFileChooser::showSaveDialog(string currentDir) {
    return pp->gfilechooser_showSaveDialog(currentDir);
}

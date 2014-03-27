package stanford.spl;

import acm.graphics.GArc;
import acm.graphics.GObject;
import acm.util.TokenScanner;

class GArc_setFrameRectangle
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d1 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d2 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d3 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d4 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    if (localGObject != null) {
      ((GArc)localGObject).setFrameRectangle(d1, d2, d3, d4);
    }
  }
}

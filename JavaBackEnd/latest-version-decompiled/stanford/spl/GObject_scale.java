package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

class GObject_scale
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    paramTokenScanner.verifyToken(",");
    double d1 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d2 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    if (localGObject != null) {
      localGObject.scale(d1, d2);
    }
  }
}

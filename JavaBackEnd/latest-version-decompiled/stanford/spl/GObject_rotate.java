package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

class GObject_rotate
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    paramTokenScanner.verifyToken(",");
    double d = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    if (localGObject != null) {
      localGObject.rotate(d);
    }
  }
}

package stanford.spl;

import acm.graphics.GRect;
import acm.util.TokenScanner;

class GRect_create
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
    paramTokenScanner.verifyToken(")");
    GRect localGRect = new GRect(d1, d2);
    paramJavaBackEnd.defineGObject(str, localGRect);
  }
}

package stanford.spl;

import acm.graphics.GOval;
import acm.util.TokenScanner;

class GOval_create
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
    GOval localGOval = new GOval(d1, d2);
    paramJavaBackEnd.defineGObject(str, localGOval);
  }
}

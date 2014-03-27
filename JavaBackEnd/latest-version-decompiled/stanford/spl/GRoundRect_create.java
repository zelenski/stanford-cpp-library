package stanford.spl;

import acm.graphics.GRoundRect;
import acm.util.TokenScanner;

class GRoundRect_create
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
    paramTokenScanner.verifyToken(")");
    GRoundRect localGRoundRect = new GRoundRect(0.0D, 0.0D, d1, d2, d3);
    paramJavaBackEnd.defineGObject(str, localGRoundRect);
  }
}

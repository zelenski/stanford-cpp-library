package stanford.spl;

import acm.graphics.GLine;
import acm.util.TokenScanner;

class GLine_create
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
    GLine localGLine = new GLine(d1, d2, d3, d4);
    paramJavaBackEnd.defineGObject(str, localGLine);
  }
}

package stanford.spl;

import acm.graphics.GArc;
import acm.util.TokenScanner;

class GArc_create
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
    GArc localGArc = new GArc(d1, d2, d3, d4);
    paramJavaBackEnd.defineGObject(str, localGArc);
  }
}

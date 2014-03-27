package stanford.spl;

import acm.util.TokenScanner;

class GTimer_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.createTimer(str, d);
  }
}

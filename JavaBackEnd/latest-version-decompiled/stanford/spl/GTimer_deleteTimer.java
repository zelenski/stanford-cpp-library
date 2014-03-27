package stanford.spl;

import acm.util.TokenScanner;

class GTimer_deleteTimer
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.deleteTimer(str);
  }
}

package stanford.spl;

import acm.util.TokenScanner;

class GEvent_waitForEvent
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    int i = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.waitForEvent(i);
  }
}

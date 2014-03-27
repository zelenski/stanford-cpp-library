package stanford.spl;

import acm.util.TokenScanner;

class GTimer_pause
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    double d = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    try
    {
      Thread.sleep((int)Math.round(d));
    }
    catch (Exception localException) {}
    paramJavaBackEnd.println("result:ok");
  }
}

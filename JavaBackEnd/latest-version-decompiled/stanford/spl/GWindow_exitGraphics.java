package stanford.spl;

import acm.util.TokenScanner;

class GWindow_exitGraphics
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    paramTokenScanner.verifyToken(")");
    System.exit(0);
  }
}

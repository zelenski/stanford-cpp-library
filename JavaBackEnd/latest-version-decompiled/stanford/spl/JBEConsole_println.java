package stanford.spl;

import acm.util.TokenScanner;

class JBEConsole_println
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.endLineConsole();
  }
}

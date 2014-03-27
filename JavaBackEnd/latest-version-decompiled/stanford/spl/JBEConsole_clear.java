package stanford.spl;

import acm.util.TokenScanner;

class JBEConsole_clear
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.clearConsole();
  }
}

package stanford.spl;

import acm.util.TokenScanner;

class JBEConsole_setSize
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    int i = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    int j = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.setConsoleSize(i, j);
  }
}

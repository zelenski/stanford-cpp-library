package stanford.spl;

import acm.util.TokenScanner;

class JBEConsole_print
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.putConsole(str);
  }
}

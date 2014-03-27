package stanford.spl;

import acm.util.TokenScanner;

class Sound_delete
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.deleteSound(str);
  }
}

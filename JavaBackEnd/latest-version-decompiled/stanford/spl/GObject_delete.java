package stanford.spl;

import acm.util.TokenScanner;

class GObject_delete
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.deleteGObject(str);
  }
}

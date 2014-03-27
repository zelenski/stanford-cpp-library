package stanford.spl;

import acm.util.TokenScanner;

class GLabel_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    JBELabel localJBELabel = new JBELabel(str2);
    paramJavaBackEnd.defineGObject(str1, localJBELabel);
  }
}

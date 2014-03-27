package stanford.spl;

import acm.util.TokenScanner;

class GWindow_setTitle
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    if (localJBEWindow != null) {
      localJBEWindow.setTitle(str2);
    }
  }
}

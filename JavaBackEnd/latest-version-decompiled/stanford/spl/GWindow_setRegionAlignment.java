package stanford.spl;

import acm.util.TokenScanner;

class GWindow_setRegionAlignment
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str3 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
    if (localJBEWindow != null) {
      localJBEWindow.setRegionAlignment(str2, str3);
    }
  }
}

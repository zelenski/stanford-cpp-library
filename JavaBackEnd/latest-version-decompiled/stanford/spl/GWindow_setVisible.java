package stanford.spl;

import acm.util.TokenScanner;

class GWindow_setVisible
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str);
    paramTokenScanner.verifyToken(",");
    boolean bool = paramTokenScanner.nextToken().equals("true");
    paramTokenScanner.verifyToken(")");
    if (localJBEWindow != null) {
      localJBEWindow.setVisible(bool);
    }
  }
}

package stanford.spl;

import acm.util.TokenScanner;

class GWindow_requestFocus
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str);
    paramTokenScanner.verifyToken(")");
    if (localJBEWindow != null)
    {
      localJBEWindow.toFront();
      localJBEWindow.getCanvas().requestFocus();
    }
  }
}

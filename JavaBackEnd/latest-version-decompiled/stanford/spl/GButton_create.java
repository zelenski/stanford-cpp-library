package stanford.spl;

import acm.util.TokenScanner;

class GButton_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GButton localGButton = new GButton(str2, paramJavaBackEnd);
    localGButton.setActionCommand(str2);
    paramJavaBackEnd.defineGObject(str1, localGButton);
    paramJavaBackEnd.defineSource(localGButton.getInteractor(), str1);
  }
}

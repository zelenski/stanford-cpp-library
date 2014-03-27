package stanford.spl;

import acm.util.TokenScanner;

class GTextField_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    int i = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GTextField localGTextField = new GTextField(i, paramJavaBackEnd);
    paramJavaBackEnd.defineGObject(str, localGTextField);
    paramJavaBackEnd.defineSource(localGTextField.getInteractor(), str);
  }
}

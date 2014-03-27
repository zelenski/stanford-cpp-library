package stanford.spl;

import acm.util.TokenScanner;

class GCheckBox_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GCheckBox localGCheckBox = new GCheckBox(str2, paramJavaBackEnd);
    paramJavaBackEnd.defineGObject(str1, localGCheckBox);
    paramJavaBackEnd.defineSource(localGCheckBox.getInteractor(), str1);
  }
}

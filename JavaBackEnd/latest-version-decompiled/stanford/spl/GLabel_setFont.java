package stanford.spl;

import acm.graphics.GLabel;
import acm.util.TokenScanner;

class GLabel_setFont
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GLabel localGLabel = (GLabel)paramJavaBackEnd.getGObject(str1);
    if (localGLabel != null) {
      localGLabel.setFont(str2);
    }
  }
}

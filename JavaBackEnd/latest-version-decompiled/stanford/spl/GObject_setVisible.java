package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

class GObject_setVisible
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    paramTokenScanner.verifyToken(",");
    boolean bool = nextBoolean(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    if (localGObject != null) {
      localGObject.setVisible(bool);
    }
  }
}

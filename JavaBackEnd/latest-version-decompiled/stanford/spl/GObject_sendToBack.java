package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

class GObject_sendToBack
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    paramTokenScanner.verifyToken(")");
    if (localGObject != null) {
      localGObject.sendToBack();
    }
  }
}

package stanford.spl;

import acm.graphics.GFillable;
import acm.graphics.GObject;
import acm.util.TokenScanner;

class GObject_setFilled
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    boolean bool = paramTokenScanner.nextToken().equals("true");
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    if (localGObject != null) {
      ((GFillable)localGObject).setFilled(bool);
    }
  }
}

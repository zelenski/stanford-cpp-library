package stanford.spl;

import acm.graphics.G3DRect;
import acm.graphics.GObject;
import acm.util.TokenScanner;

class G3DRect_setRaised
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    boolean bool = nextBoolean(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    if (localGObject != null) {
      ((G3DRect)localGObject).setRaised(bool);
    }
  }
}

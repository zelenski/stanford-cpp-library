package stanford.spl;

import acm.graphics.G3DRect;
import acm.util.TokenScanner;

class G3DRect_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d1 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d2 = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    boolean bool = paramTokenScanner.nextToken().equals("true");
    paramTokenScanner.verifyToken(")");
    G3DRect localG3DRect = new G3DRect(0.0D, 0.0D, d1, d2, bool);
    paramJavaBackEnd.defineGObject(str, localG3DRect);
  }
}

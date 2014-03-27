package stanford.spl;

import acm.graphics.GPolygon;
import acm.util.TokenScanner;

class GPolygon_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GPolygon localGPolygon = new GPolygon();
    paramJavaBackEnd.defineGObject(str, localGPolygon);
  }
}

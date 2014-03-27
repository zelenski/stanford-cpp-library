package stanford.spl;

import acm.graphics.GArc;
import acm.graphics.GObject;
import acm.util.TokenScanner;

class GArc_setSweepAngle
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    double d = nextDouble(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    if (localGObject != null) {
      ((GArc)localGObject).setSweepAngle(d);
    }
  }
}

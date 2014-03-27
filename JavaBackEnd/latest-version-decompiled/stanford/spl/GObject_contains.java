package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.io.PrintStream;

class GObject_contains
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
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    System.out.println("result:" + ((localGObject != null) && (localGObject.contains(d1, d2))));
    System.out.flush();
  }
}

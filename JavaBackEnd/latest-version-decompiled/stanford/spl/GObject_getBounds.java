package stanford.spl;

import acm.graphics.GObject;
import acm.graphics.GRectangle;
import acm.util.TokenScanner;
import java.io.PrintStream;

class GObject_getBounds
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    if (localGObject == null)
    {
      System.out.println("error: NULL object");
    }
    else
    {
      GRectangle localGRectangle = localGObject.getBounds();
      System.out.println("result:GRectangle(" + localGRectangle.getX() + ", " + localGRectangle.getY() + ", " + localGRectangle.getWidth() + ", " + localGRectangle.getHeight() + ")");
    }
    System.out.flush();
  }
}

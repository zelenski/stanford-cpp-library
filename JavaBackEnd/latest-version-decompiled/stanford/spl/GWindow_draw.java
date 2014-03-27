package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.awt.Graphics2D;

class GWindow_draw
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str);
    paramTokenScanner.verifyToken(",");
    str = nextString(paramTokenScanner);
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    paramTokenScanner.verifyToken(")");
    if ((localJBEWindow != null) && (localGObject != null) && (localGObject.isVisible()))
    {
      JBECanvas localJBECanvas = localJBEWindow.getCanvas();
      Graphics2D localGraphics2D = localJBECanvas.getOSG();
      localGraphics2D.setColor(localGObject.getColor());
      localGObject.paint(localGraphics2D);
      localJBECanvas.repaint();
    }
  }
}

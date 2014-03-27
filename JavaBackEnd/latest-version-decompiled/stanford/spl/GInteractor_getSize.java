package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.awt.Dimension;
import java.io.PrintStream;
import javax.swing.JComponent;

class GInteractor_getSize
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    if (localGObject != null)
    {
      JComponent localJComponent = ((GInteractor)localGObject).getInteractor();
      Dimension localDimension = localJComponent.isShowing() ? localJComponent.getSize() : localJComponent.getPreferredSize();
      
      System.out.println("result:GDimension(" + localDimension.width + ", " + localDimension.height + ")");
      
      System.out.flush();
    }
    else
    {
      System.out.println("result:GDimension(0, 0)");
      System.out.flush();
    }
  }
}

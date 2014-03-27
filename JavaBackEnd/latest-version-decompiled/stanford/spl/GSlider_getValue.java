package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.io.PrintStream;
import javax.swing.JSlider;

class GSlider_getValue
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
      JSlider localJSlider = (JSlider)((GSlider)localGObject).getInteractor();
      System.out.println("result:" + localJSlider.getValue());
      System.out.flush();
    }
    else
    {
      System.out.println("result:0");
      System.out.flush();
    }
  }
}

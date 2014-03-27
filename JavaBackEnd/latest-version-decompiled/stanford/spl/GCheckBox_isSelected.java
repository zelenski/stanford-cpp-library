package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.io.PrintStream;
import javax.swing.JCheckBox;

class GCheckBox_isSelected
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
      JCheckBox localJCheckBox = (JCheckBox)((GCheckBox)localGObject).getInteractor();
      System.out.println("result:" + localJCheckBox.isSelected());
      System.out.flush();
    }
    else
    {
      System.out.println("result:false");
      System.out.flush();
    }
  }
}

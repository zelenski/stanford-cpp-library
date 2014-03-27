package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.io.PrintStream;
import javax.swing.JComboBox;

class GChooser_getSelectedItem
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
      JComboBox localJComboBox = (JComboBox)((GChooser)localGObject).getInteractor();
      System.out.println("result:" + localJComboBox.getSelectedItem());
      System.out.flush();
    }
    else
    {
      System.out.println("result:");
      System.out.flush();
    }
  }
}

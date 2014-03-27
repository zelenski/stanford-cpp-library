package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.io.PrintStream;
import javax.swing.JTextField;

class GTextField_getText
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
      JTextField localJTextField = (JTextField)((GTextField)localGObject).getInteractor();
      System.out.println("result:" + localJTextField.getText());
      System.out.flush();
    }
    else
    {
      System.out.println("result:");
      System.out.flush();
    }
  }
}

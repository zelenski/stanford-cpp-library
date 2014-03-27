package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import javax.swing.JTextField;

class GTextField_setText
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str1);
    if (localGObject != null)
    {
      JTextField localJTextField = (JTextField)((GTextField)localGObject).getInteractor();
      localJTextField.setText(str2);
    }
  }
}

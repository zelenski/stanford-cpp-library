package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import javax.swing.JCheckBox;

class GCheckBox_setSelected
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    boolean bool = nextBoolean(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    if (localGObject != null)
    {
      JCheckBox localJCheckBox = (JCheckBox)((GCheckBox)localGObject).getInteractor();
      localJCheckBox.setSelected(bool);
    }
  }
}

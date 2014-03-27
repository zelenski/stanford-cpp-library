package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import javax.swing.JComboBox;

class GChooser_setSelectedItem
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
      JComboBox localJComboBox = (JComboBox)((GChooser)localGObject).getInteractor();
      localJComboBox.setSelectedItem(str2);
    }
  }
}

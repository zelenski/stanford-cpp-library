package stanford.spl;

import acm.graphics.GFillable;
import acm.graphics.GObject;
import acm.util.JTFTools;
import acm.util.TokenScanner;
import java.awt.Color;

class GObject_setFillColor
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
      Color localColor = str2.equals("") ? null : JTFTools.decodeColor(str2);
      ((GFillable)localGObject).setFillColor(localColor);
    }
  }
}

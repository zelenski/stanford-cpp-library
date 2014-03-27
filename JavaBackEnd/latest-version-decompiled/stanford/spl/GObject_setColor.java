package stanford.spl;

import acm.graphics.GObject;
import acm.util.JTFTools;
import acm.util.TokenScanner;

class GObject_setColor
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    GObject localGObject = paramJavaBackEnd.getGObject(str1);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    if (localGObject != null) {
      localGObject.setColor(str2.equals("") ? null : JTFTools.decodeColor(str2));
    }
  }
}

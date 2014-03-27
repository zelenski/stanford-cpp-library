package stanford.spl;

import acm.graphics.GCompound;
import acm.graphics.GObject;
import acm.util.TokenScanner;

class GObject_remove
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    GObject localGObject = paramJavaBackEnd.getGObject(str);
    paramTokenScanner.verifyToken(")");
    if (localGObject != null)
    {
      GCompound localGCompound = (GCompound)localGObject.getParent();
      if (localGCompound != null) {
        localGCompound.remove(localGObject);
      }
    }
  }
}

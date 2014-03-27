package stanford.spl;

import acm.graphics.GDimension;
import acm.graphics.GLabel;
import acm.util.TokenScanner;
import java.io.PrintStream;

class GLabel_getGLabelSize
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GLabel localGLabel = (GLabel)paramJavaBackEnd.getGObject(str);
    GDimension localGDimension = localGLabel.getSize();
    System.out.println("result:GDimension(" + localGDimension.getWidth() + ", " + localGDimension.getHeight() + ")");
    
    System.out.flush();
  }
}

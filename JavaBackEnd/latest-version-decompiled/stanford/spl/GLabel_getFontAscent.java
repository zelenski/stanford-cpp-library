package stanford.spl;

import acm.graphics.GLabel;
import acm.util.TokenScanner;
import java.io.PrintStream;

class GLabel_getFontAscent
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GLabel localGLabel = (GLabel)paramJavaBackEnd.getGObject(str);
    System.out.println("result:" + localGLabel.getAscent());
    System.out.flush();
  }
}

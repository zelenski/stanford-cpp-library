package stanford.spl;

import acm.util.TokenScanner;
import java.awt.Dimension;
import java.awt.Toolkit;

class GWindow_getScreenWidth
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    paramTokenScanner.verifyToken(")");
    Dimension localDimension = Toolkit.getDefaultToolkit().getScreenSize();
    paramJavaBackEnd.println("result:" + localDimension.width);
  }
}

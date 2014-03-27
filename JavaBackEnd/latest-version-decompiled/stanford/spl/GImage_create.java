package stanford.spl;

import acm.graphics.GImage;
import acm.util.TokenScanner;
import java.io.PrintStream;

class GImage_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    try
    {
      GImage localGImage = new GImage(str2);
      paramJavaBackEnd.defineGObject(str1, localGImage);
      System.out.println("result:GDimension(" + localGImage.getWidth() + ", " + localGImage.getHeight() + ")");
      
      System.out.flush();
    }
    catch (Exception localException)
    {
      System.out.println("result:" + localException.getMessage());
      System.out.flush();
    }
  }
}

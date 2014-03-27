package stanford.spl;

import acm.util.TokenScanner;

class GSlider_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    int i = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    int j = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    int k = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GSlider localGSlider = new GSlider(i, j, k, paramJavaBackEnd);
    paramJavaBackEnd.defineGObject(str, localGSlider);
    paramJavaBackEnd.defineSource(localGSlider.getInteractor(), str);
  }
}

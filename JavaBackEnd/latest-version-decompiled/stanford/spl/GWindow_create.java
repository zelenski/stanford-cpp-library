package stanford.spl;

import acm.util.TokenScanner;

class GWindow_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    int i = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    int j = nextInt(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.createWindow(str1, i, j, (TopCompound)paramJavaBackEnd.getGObject(str2));
    paramJavaBackEnd.println("result:ok");
  }
}

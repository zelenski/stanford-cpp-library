package stanford.spl;

import acm.util.TokenScanner;

class Sound_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str1 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(",");
    String str2 = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    String str3 = "ok";
    try
    {
      paramJavaBackEnd.createSound(str1, str2);
    }
    catch (Exception localException)
    {
      str3 = localException.getMessage();
    }
    paramJavaBackEnd.println("result:" + str3);
  }
}

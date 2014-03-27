package stanford.spl;

import acm.util.TokenScanner;

class TopCompound_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    TopCompound localTopCompound = new TopCompound();
    paramJavaBackEnd.defineGObject(str, localTopCompound);
  }
}

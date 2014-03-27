package stanford.spl;

import acm.util.TokenScanner;

class Sound_play
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    paramJavaBackEnd.playSound(str);
  }
}

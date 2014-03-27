package stanford.spl;

import acm.util.TokenScanner;

class GChooser_create
  extends JBECommand
{
  public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd)
  {
    paramTokenScanner.verifyToken("(");
    String str = nextString(paramTokenScanner);
    paramTokenScanner.verifyToken(")");
    GChooser localGChooser = new GChooser(paramJavaBackEnd);
    paramJavaBackEnd.defineGObject(str, localGChooser);
    paramJavaBackEnd.defineSource(localGChooser.getInteractor(), str);
  }
}

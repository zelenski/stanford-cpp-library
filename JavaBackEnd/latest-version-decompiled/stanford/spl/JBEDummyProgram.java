package stanford.spl;

import acm.program.Program;

class JBEDummyProgram
  extends Program
{
  private JavaBackEnd jbe;
  
  public JBEDummyProgram(JavaBackEnd paramJavaBackEnd)
  {
    this.jbe = paramJavaBackEnd;
  }
}

package stanford.spl;

import java.awt.event.ActionEvent;

import acm.program.Program;

class JBEDummyProgram extends Program {
	private static final long serialVersionUID = 1L;

	private JavaBackEnd jbe;

	public JBEDummyProgram(JavaBackEnd paramJavaBackEnd) {
		this.jbe = paramJavaBackEnd;
	}

	@Override
	public boolean menuAction(ActionEvent event) {
		String s = event.getActionCommand();
		if (s.equals("Quit") || s.equals("Exit")) {
			// notify the C++ library that the JBE was shut down
			System.out.print("event:consoleWindowClosed()");
			System.out.flush();
			jbe.acknowledgeEvent();
			// give it a little time to make sure the message goes through
			try {
				Thread.sleep(100);
			} catch (InterruptedException ie) {}
			System.exit(0);
		}
		return super.menuAction(event);
	}
}

/*
 * @version 2016/10/12
 * - initial version
 */

package stanford.spl;

/**
 * A JBECommand that should be run on the Swing event thread.
 */
public abstract class JBESwingCommand extends JBECommand {
	@Override
	public boolean shouldRunOnSwingEventThread() {
		return true;
	}
}

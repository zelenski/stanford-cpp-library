/*
 * @(#)Animator.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

// REVISION HISTORY
//
// -- V2.0 --
// Bug fix 12-Sep-07 (ESR, JTFBug 2007-013)
//   1. Fixed bug in handling of ChangeListener compatibility.

package acm.util;

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.lang.reflect.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

/* Class: Animator */
/**
 * This class extends <code>Thread</code> to provide several features that make it
 * easier to build animations.  These features include a <code>pause</code>
 * method that does not raise an exception and a <code>requestTermination</code>
 * method that signals that the execution of this thread should stop at its
 * next opportunity.  It also includes hooks to support a start/stop/single-step
 * model for algorithm animation.
 */
public class Animator extends Thread {

/* Constant: INITIAL */
/** Constant indicating that the animator has not yet started. */
	public final static int INITIAL = 0;

/* Constant: RUNNING */
/** Constant indicating that the animator is running. */
	public final static int RUNNING = 1;

/* Constant: STEPPING */
/** Constant indicating that the animator is running in single-step mode. */
	public final static int STEPPING = 2;

/* Constant: CALLING */
/** Constant indicating that the animator is running until the end of the current call. */
	public final static int CALLING = 3;

/* Constant: STOPPING */
/** Constant indicating that the animator should stop at the next trace point. */
	public final static int STOPPING = 4;

/* Constant: STOPPED */
/** Constant indicating that the animator is suspended waiting for restart. */
	public final static int STOPPED = 5;

/* Constant: FINISHED */
/** Constant indicating that the animator has finished its <code>run</code> method. */
	public final static int FINISHED = 6;

/* Constant: TERMINATING */
/** Constant indicating that the animator has been asked to terminate. */
	public final static int TERMINATING = 7;

/* Constructor: Animator() */
/**
 * Creates a new <code>Animator</code> object.
 *
 * @usage Animator animator = new Animator();
 */
	public Animator() {
		initAnimator();
	}

/* Constructor: Animator(group) */
/**
 * Creates a new <code>Animator</code> object and assigns it to the
 * specified thread group.
 *
 * @usage Animator animator = new Animator(group);
 * @param group The <code>ThreadGroup</code> to which the new thread is assigned
 * @noshow
 */
	public Animator(ThreadGroup group) {
		super(group, (Runnable) null);
		initAnimator();
	}

/* Constructor: Animator(runnable) */
/**
 * Creates a new <code>Animator</code> object with the specified runnable object.
 *
 * @usage Animator animator = new Animator(runnable);
 * @param runnable Any object that implements the <code>Runnable</code> interface
 * @noshow
 */
	public Animator(Runnable runnable) {
		super(runnable);
		initAnimator();
	}

/* Constructor: Animator(group, runnable) */
/**
 * Creates a new <code>Animator</code> object with the specified runnable object
 * and assigns it to the specified thread group.
 *
 * @usage Animator animator = new Animator(group, runnable);
 * @param group The <code>ThreadGroup</code> to which the new thread is assigned
 * @param runnable Any object that implements the <code>Runnable</code> interface
 * @noshow
 */
	public Animator(ThreadGroup group, Runnable runnable) {
		super(group, runnable);
		initAnimator();
	}

/* Method: run() */
/**
 * Specifies the code for the animator.  Subclasses should override this
 * method with the code they need to execute to implement the animator's
 * function.
 */
	public void run() {
		/* Empty */
	}

/* Method: getAnimatorState() */
/**
 * Returns the state of the animator. This value will be one of the constants
 * <a href="#INITIAL"><code>INITIAL</code></a>,
 * <a href="#RUNNING"><code>RUNNING</code></a>,
 * <a href="#STEPPING"><code>STEPPING</code></a>,
 * <a href="#CALLING"><code>CALLING</code></a>,
 * <a href="#STOPPING"><code>STOPPING</code></a>,
 * <a href="#STOPPED"><code>STOPPED</code></a>,
 * <a href="#FINISHED"><code>FINISHED</code></a>, or
 * <a href="#TERMINATING"><code>TERMINATING</code></a>,
 * as defined in this class.
 *
 * @usage int state = animator.getAnimatorState();
 * @return The current state of the animator
 */
	public int getAnimatorState() {
		return animatorState;
	}

/* Method: pause(milliseconds) */
/**
 * Delays this thread for the specified time, which is expressed in
 * milliseconds.  Unlike <code>Thread.sleep</code>, this method never
 * throws an exception.
 *
 * @usage animator.pause(milliseconds);
 * @param milliseconds The sleep time in milliseconds
 */
	public void pause(double milliseconds) {
		if (animatorState == TERMINATING) terminate();
		JTFTools.pause(milliseconds);
	}

/* Method: startAction() */
/**
 * Triggers a <code>"start"</code> action, as if the <code>Start</code> button
 * is pushed.
 *
 * @usage animator.startAction();
 */
	public void startAction() {
		start(RUNNING);
	}

/* Method: stopAction() */
/**
 * Triggers a <code>"stop"</code> action, as if the <code>Stop</code> button
 * is pushed.
 *
 * @usage animator.stopAction();
 */
	public void stopAction() {
		switch (animatorState) {
		  case RUNNING: case STEPPING: case CALLING:
			animatorState = STOPPING;
			break;
		  default:
			break;
		}
	}

/* Method: stepAction() */
/**
 * Triggers a <code>"step"</code> action, as if the <code>Step</code> button
 * is pushed.
 *
 * @usage animator.stepAction();
 */
	public void stepAction() {
		start(STEPPING);
	}

/* Method: callAction() */
/**
 * Triggers a <code>"call"</code> action, as if the <code>Call</code> button
 * is pushed.
 *
 * @usage animator.callAction();
 */
	public void callAction() {
		callDepth = currentDepth;
		start(CALLING);
	}

/* Method: buttonAction(actionCommand) */
/**
 * Triggers an action for the action specified by the
 * action command.
 *
 * @usage boolean ok = animator.buttonAction(actionCommand);
 * @param actionCommand The action command from the button
 * @return <code>true</code> if the action command is recognized
 */
	public boolean buttonAction(String actionCommand) {
		if (actionCommand.equals("Start")) {
			startAction();
		} else if (actionCommand.equals("Stop")) {
			stopAction();
		} else if (actionCommand.equals("Step")) {
			stepAction();
		} else if (actionCommand.equals("Call")) {
			callAction();
		} else {
			return false;
		}
		return true;
	}

/* Method: setSpeed(speed) */
/**
 * Sets the speed parameter for the animator.  The speed is a <code>double</code>
 * between 0.0 and 1.0, for which 0.0 is very slow and 1.0 is as fast as the
 * system can manage.
 *
 * @usage animator.setSpeed(speed);
 * @param speed A double between 0.0 (slow) and 1.0 (fast)
 */
	public void setSpeed(double speed) {
		animatorSpeed = speed;
		if (speedBar instanceof JSlider) {
			JSlider slider = (JSlider) speedBar;
			int min = slider.getMinimum();
			int max = slider.getMaximum();
			slider.setValue((int) Math.round(min + speed * (max - min)));
		} else if (speedBar instanceof JScrollBar) {
			JScrollBar scrollBar = (JScrollBar) speedBar;
			int min = scrollBar.getMinimum();
			int max = scrollBar.getMaximum();
			scrollBar.setValue((int) Math.round(min + speed * (max - min)));
		}
	}

/* Method: getSpeed() */
/**
 * Returns the speed parameter for the animator.  The speed is a <code>double</code>
 * between 0.0 and 1.0, for which 0.0 is very slow and 1.0 is as fast as the
 * system can manage.
 *
 * @usage double speed = animator.getSpeed();
 * @return A double between 0.0 (slow) and 1.0 (fast)
 */
	public double getSpeed() {
		return animatorSpeed;
	}

/* Method: trace() */
/**
 * Checks the state of the animator and executes any actions have been requested.
 *
 * @usage animator.trace();
 */
	public void trace() {
		trace(0);
	}

/* Method: trace(depth) */
/**
 * Checks the state of the animator and executes any actions have been requested
 * to occur at the specified call stack depth.  This method is useful only to
 * clients who are making use of the <code>Call</code> button functionality.
 *
 * @usage animator.trace(depth);
 * @param depth The current call stack depth.
 */
	public void trace(int depth) {
		if (Thread.currentThread() != this) {
			throw new ErrorException("trace() can be called only by the animator thread itself");
		}
		currentDepth = depth;
		switch (animatorState) {
		  case RUNNING:
			delay();
			break;
		  case STOPPING: case STEPPING:
			breakpoint();
			break;
		  case CALLING:
			if (callDepth < currentDepth) {
				delay();
			} else {
				breakpoint();
			}
			break;
		  case TERMINATING:
			terminate();
			break;
		}
	}

/* Method: breakpoint() */
/**
 * Suspends the animator until one of the restart actions is triggered.
 *
 * @usage animator.breakpoint();
 */
	public void breakpoint() {
		if (Thread.currentThread() != this) {
			throw new ErrorException("breakpoint() can be called only by the animator thread itself");
		}
		animatorState = STOPPED;
		breakHook();
		suspendAnimator();
	}

/******************************************************************************/
/* Implementation notes: delay()                                              */
/* -----------------------------                                              */
/* The delay method turns out to be tricky to code.  The intent is that the   */
/* speed value should suggest a smoothly varying speed.   Unfortunately,      */
/* implementing delay so that it "feels right" requires a nonlinear approach. */
/* This implementation varies the speed steeply from 0.0 to 0.25 and then     */
/* quadratically from 0.25 to 0.8.  From there it interposes delays in only   */
/* a fraction of the calls to delay.                                          */
/******************************************************************************/

/* Method: delay() */
/**
 * Delays the calling thread according to the speed.
 *
 * @usage animator.delay();
 */
	public void delay() {
		boolean yield = true;
		double delay = 0;
		if (animatorSpeed < 0.25) {
			delay = SLOW_DELAY + animatorSpeed / 0.25 * (CLIP_DELAY - SLOW_DELAY);
		} else if (animatorSpeed < 0.9) {
			delay = CLIP_DELAY + Math.sqrt((animatorSpeed - 0.25) / 0.65) * (FAST_DELAY - CLIP_DELAY);
		} else {
			switch ((int) (animatorSpeed * 99.99 - 90)) {
			  case 0: yield = true; break;
			  case 1: yield = delayCount % 10 != 0; break;
			  case 2: yield = delayCount % 7 != 0; break;
			  case 3: yield = delayCount % 5 != 0; break;
			  case 4: yield = delayCount % 3 != 0; break;
			  case 5: yield = delayCount % 2 == 0; break;
			  case 6: yield = delayCount % 3 == 0; break;
			  case 7: yield = delayCount % 4 == 0; break;
			  case 8: yield = delayCount % 6 == 0; break;
			  case 9: yield = false; break;
			}
			delayCount = (delayCount + 1) % (2 * 2 * 3 * 5 * 7);
		}
		if (yield) {
			delayHook();
			JTFTools.pause(delay);
		}
	}

/* Public method: registerSpeedBar(slider) */
/**
 * Registers the specified slider as the delay controller for the animator.
 *
 * @usage registerSpeedBar(slider);
 * @param slider The slider which will serve as the speed bar for this animator
 */
	public void registerSpeedBar(JSlider slider) {
		SpeedBarListener.register(this, slider);
		speedBar = slider;
	}

/* Public method: registerSpeedBar(scrollBar) */
/**
 * Registers the specified scroll bar as the delay controller for the animator.
 *
 * @usage registerSpeedBar(scrollBar);
 * @param scrollBar The scroll bar which will serve as the speed bar for this animator
 */
	public void registerSpeedBar(JScrollBar scrollBar) {
		SpeedBarListener.register(this, scrollBar);
		speedBar = scrollBar;
	}

/* Public method: getSpeedBar() */
/**
 * Returns the speed bar for the animator, if any.  The speed bar can be a
 * <code>JSlider</code> or a <code>JScrollBar</code>; it is the caller's
 * responsibility to cast the result to the appropriate class.
 *
 * @return The speed bar, or <code>null</code> if none exists
 */
	public Component getSpeedBar() {
		return speedBar;
	}

/* Method: requestTermination() */
/**
 * Signals the <code>Animator</code> that it should stop running at the
 * next available opportunity, which is when the client next calls
 * <code>pause</code> or <code>checkForTermination</code>.  Making this
 * check at well-managed times makes it possible for the client to
 * ensure that objects are left in a consistent state and thereby
 * avoids the problems that led Sun to deprecate <code>Thread.stop</code>.
 *
 * @usage requestTermination();
 */
	public void requestTermination() {
		animatorState = TERMINATING;
	}

/* Method: checkForTermination() */
/**
 * Checks to see whether this <code>Animator</code> has been asked
 * to terminate.  If so, the <code>Animator</code> stops running,
 * and the call never returns.  If not, other threads are given a
 * chance to run, after which this <code>Animator</code> will return
 * to the caller.
 *
 * @usage checkForTermination();
 */
	public void checkForTermination() {
		if (animatorState == TERMINATING) {
			terminate();
		} else {
			yield();
		}
	}

/* Static method: shutdown(applet) */
/**
 * Destroys all animator threads started by the specified applet.  This method
 * ensures that applet cleanup occurs in browsers that refuse to terminate an
 * applet that still has threads running.
 *
 * @usage Animator.shutdown(applet);
 * @param applet The applet that is ending its execution
 * @noshow
 */
	public static void shutdown(Applet applet) {
		try {
			Class<?> threadClass = Class.forName("java.lang.Thread");
			Method stop = threadClass.getMethod("stop", new Class[0]);
			Object[] args = new Object[0];
			ArrayList<Thread> list = animatorTable.get(applet);
			if (list != null) {
				animatorTable.remove(applet);
				int nThreads = list.size();
				for (int i = 0; i < nThreads; i++) {
					Thread t = list.get(i);
					stop.invoke(t, args);
				}
			}
		} catch (Exception ex) {
			/* Empty */
		}
	}

/* Hook method: delayHook() */
/**
 * This method is called before the animator enters a timing delay.
 * Subclasses should override this method if they need to perform some
 * kind of action at this point, such as updating the display.
 */
	protected void delayHook() {
		/* Empty */
	}

/* Hook method: breakHook() */
/**
 * This method is called before the animator executes a breakpoint.
 * Subclasses should override this method if they need to perform
 * some kind of action at this point, such as updating the display.
 */
	protected void breakHook() {
		/* Empty */
	}

/* Hook method: resumeHook() */
/**
 * This method is called before the animator starts or restarts after
 * a breakpoint.  Subclasses should override this method if they need
 * to perform some kind of action at this point.
 */
	protected void resumeHook() {
		/* Empty */
	}

/* Hook method: controllerHook() */
/**
 * This method is called before the animator changes state in a way
 * that might interest a controller.  Subclasses will typically override
 * this hook to update the enabled status of buttons and menu items.
 */
	protected void controllerHook() {
		/* Empty */
	}

/* Overridden method: start() */
/**
 * Starts the thread.  The only difference in this method is that it
 * also sets the state.
 *
 * @usage animator.start();
 */
	public void start() {
		start(RUNNING);
	}

/* Private method: initAnimator() */
/**
 * Makes sure that this thread inherits the applet identity of its creator, if any.
 */
	private void initAnimator() {
		Applet applet = JTFTools.getApplet();
		if (applet != null) {
			JTFTools.registerApplet(applet, this);
			ArrayList<Thread> list = animatorTable.get(applet);
			if (list == null) {
				list = new ArrayList<Thread>();
				animatorTable.put(applet, list);
			}
			list.add(this);
		}
	}

/* Private method: start(state) */
/**
 * Restarts the animator in the specified state.
 */
	private void start(int state) {
		switch (animatorState) {
		  case INITIAL: case FINISHED:
			animatorState = state;
			resumeHook();
			controllerHook();
			super.start();
			break;
		  case STOPPED:
			animatorState = state;
			resumeHook();
			controllerHook();
			resumeAnimator();
			break;
		  default:
			break;
		}
	}

/* Private method: suspendAnimator() */
/**
 * Suspends the animator thread pending a later restart.
 */
	private synchronized void suspendAnimator() {
		resumed = false;
		while (!resumed) {
			try {
				wait();
			} catch (InterruptedException e) {
				/* Empty */
			}
		}
	}

/* Private method: resumeAnimator() */
/**
 * Resumes the animator where it left off.
 */
	private synchronized void resumeAnimator() {
		resumed = true;
		notifyAll();
	}

/* Private method: terminate() */
/**
 * Terminates the animator by throwing a <code>ThreadDeath</code> exception.
 */
	private void terminate() {
		if (Thread.currentThread() == this) {
			throw new ThreadDeath();
		} else {
			throw new ErrorException("Illegal call to terminate");
		}
	}

/* Private constants */
	private static final double SLOW_DELAY = 1000.0;
	private static final double CLIP_DELAY = 200.0;
	private static final double FAST_DELAY = 0.0;

/* Static table of animator threads stored by applet */
	private static HashMap<Applet,ArrayList<Thread>> animatorTable = new HashMap<Applet,ArrayList<Thread>>();

/* Private instance variables */
	private int animatorState = INITIAL;
	private int currentDepth = 0;
	private int callDepth = 0;
	private int delayCount = 0;
	private double animatorSpeed = 0.5;
	private Component speedBar;
	private boolean resumed;

}

/* Private class: SpeedBarListener */
/**
 * This class acts as a generic listener to any kind of speed bar.  The
 * two principal models are <code>JSlider</code> or a <code>JScrollBar</code>,
 * but the listener can be anything that implements the methods <code>getValue</code>,
 * <code>getMinimum</code>, and <code>getMaximum</code> along with either
 * <code>addChangeListener</code> or <code>addAdjustmentListener</code>.
 * The extra generality is not really important here.  The key is to allow
 * both scrollbars and sliders.
 */
class SpeedBarListener implements AdjustmentListener, ChangeListener {

/* Static method: register(animator, speedBar) */
/**
 * Registers the speed bar as the delay controller for the animator.
 */
	public static void register(Animator animator, Object speedBar) {
		SpeedBarListener listener = new SpeedBarListener();
		listener.animator = animator;
		listener.speedBar = speedBar;
		Class<?> speedBarClass = speedBar.getClass();
		Method addListener = lookForMethod(speedBarClass, "addAdjustmentListener");
		if (addListener == null) {
			addListener = lookForMethod(speedBarClass, "addChangeListener");
		}
		if (addListener == null) {
			addListener = lookForMethod(speedBarClass, "addChangeListener");
		}
		try {
			listener.getValue = speedBarClass.getMethod("getValue", new Class[0]);
			listener.getMinimum = speedBarClass.getMethod("getMinimum", new Class[0]);
			listener.getMaximum = speedBarClass.getMethod("getMaximum", new Class[0]);
			Object[] args = { listener };
			addListener.invoke(speedBar, args);
		} catch (Exception ex) {
			throw new ErrorException("Illegal speed bar object");
		}
		listener.setSpeed();
	}

/* Method: adjustmentValueChanged(e) */
/**
 * Indicates that an adjustment value has changed.  Implements AdjustmentListener.
 */
	public void adjustmentValueChanged(AdjustmentEvent e) {
		if (e != e) /* Avoid unused parameter warning */;
		setSpeed();
	}

/* Method: stateChanged(e) */
/**
 * Indicates that a change has occurred in a slider.  Implements ChangeListener.
 */
	public void stateChanged(ChangeEvent e) {
		if (e != e) /* Avoid unused parameter warning */;
		setSpeed();
	}

/* Method: setSpeed() */
/**
 * Sets the speed of the animator according to the speed bar.
 */
	public void setSpeed() {
		try {
			int min = ((Integer) getMinimum.invoke(speedBar, new Object[0])).intValue();
			int max = ((Integer) getMaximum.invoke(speedBar, new Object[0])).intValue();
			int value = ((Integer) getValue.invoke(speedBar, new Object[0])).intValue();
			double fraction = (double) (value - min) / (max - min);
			animator.setSpeed(fraction);
		} catch (Exception ex) {
			throw new ErrorException(ex);
		}
	}

/* Private static method: lookForMethod(speedBarClass, methodName) */
/**
 * Looks for a method in the class with the appropriate name.
 */
	private static Method lookForMethod(Class<?> speedBarClass, String methodName) {
		Method[] methods = speedBarClass.getMethods();
		for (int i = 0; i < methods.length; i++) {
			if (methodName.equals(methods[i].getName())) return methods[i];
		}
		return null;
	}

/* Private instance variables */
	private Animator animator;
	private Object speedBar;
	private Method getValue;
	private Method getMinimum;
	private Method getMaximum;

}

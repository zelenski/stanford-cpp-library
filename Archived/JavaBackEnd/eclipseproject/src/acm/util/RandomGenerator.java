/*
 * @version 2017/04/14
 * - bug fix for nextInt(int, int) with fixed rigged integers
 * @version 2016/05/19
 * - added choice(T...) method
 * @version 2016/04/18
 * - added code to "rig" random generator by feeding it a set of
 *   numbers to always return
 * - added setSharedSeed
 */

/*
 * @(#)RandomGenerator.java   1.99.1 08/12/08
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
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.util;

import java.awt.*;
import java.util.*;
import java.util.List;

/**
 * This class implements a simple random number generator that allows clients to
 * generate pseudorandom integers, doubles, booleans, and colors. To use it, the
 * first step is to declare an instance variable to hold the random generator as
 * follows:
 *
 * <p>
 * 
 * <pre>
 * <code>
 * &nbsp;    private RandomGenerator rgen = RandomGenerator.getInstance();
 * </code>
 * </pre>
 *
 * <p>
 * By default, the <code>RandomGenerator</code> object is initialized to begin
 * at an unpredictable point in a pseudorandom sequence. During debugging, it is
 * often useful to set the internal seed for the random generator explicitly so
 * that it always returns the same sequence. To do so, you need to invoke the
 * <a href="http://com/j2se/1.4.2/docs/api/java/util/Random.html#setSeed(int)">
 * <code>setSeed</code></a> method.
 *
 * <p>
 * The <code>RandomGenerator</code> object returned by <code>getInstance</code>
 * is shared across all classes in an application. Using this shared instance of
 * the generator is preferable to allocating new instances of
 * <code>RandomGenerator</code>. If you create several random generators in
 * succession, they will typically generate the same sequence of values.
 */
public class RandomGenerator extends Random {
	/**
	 * The serialization code for this class. This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */
	static final long serialVersionUID = 1L;

	private static RandomGenerator standardInstance;

	private static final List<Integer> fixedIntegers = new LinkedList<Integer>();

	// shared seed for all RandomGenerators to use (default none)
	private static Long ourSeed = null;

	/**
	 * Creates a new random generator. Most clients will not use the constructor
	 * directly but will instead call <a href="#getInstance()">
	 * <code>getInstance</code></a> to obtain a <code>RandomGenerator</code>
	 * object that is shared by all classes in the application.
	 *
	 * @usage RandomGenerator rgen = new RandomGenerator();
	 */
	public RandomGenerator() {
		if (ourSeed != null) {
			setSeed(ourSeed);
		}
	}

	/**
	 * Creates a new random generator, using the given seed value to influence
	 * its random generation of values.
	 */
	public RandomGenerator(long seed) {
		super(ourSeed != null ? ourSeed : seed);
	}
	
	/**
	 * Randomly chooses one of the values passed and returns it.
	 * @param choices a set of choices
	 * @return a randomly chosen choice
	 */
	public <T> T nextChoice(T... choices) {
		if (choices.length == 0) {
			throw new IllegalArgumentException("must pass at least one choice");
		} else {
			int index = nextInt(choices.length);
			return choices[index];
		}
	}
	
	/**
	 * Randomly chooses one of the values passed and returns it.
	 * @param choices a set of choices
	 * @return a randomly chosen choice
	 */
	public <T> T nextChoice(List<T> choices) {
		if (choices.isEmpty()) {
			throw new IllegalArgumentException("must pass at least one choice");
		} else {
			int index = nextInt(choices.size());
			return choices.get(index);
		}
	}
	
	/**
	 * Randomly chooses one of the values passed and returns it.
	 * @param choices a set of choices
	 * @return a randomly chosen choice
	 */
	public <T> T nextChoice(Iterable<T> choices) {
		List<T> list = new ArrayList<T>();
		for (T choice : choices) {
			list.add(choice);
		}
		return nextChoice(list);
	}

	/**
	 * Returns the next random integer, which can be any legal integer value.
	 */
	public int nextInt() {
		// return fixed number if one is present
		if (!fixedIntegers.isEmpty()) {
			synchronized (fixedIntegers) {
				if (!fixedIntegers.isEmpty()) {
					int fixed = fixedIntegers.remove(0);
					return fixed;
				}
			}
		}

		return super.nextInt();
	}

	/**
	 * Returns the next random integer between 0 and <code>n</code>-1,
	 * inclusive. This method is in modern implementations of the
	 * <code>Random</code> class, but is missing from JDK 1.1.
	 * 
	 * @noshow
	 */
	public int nextInt(int n) {
		return nextInt(0, n - 1);
	}

	/**
	 * Returns a random <code>boolean</code> value that is <code>true</code> or
	 * <code>false</code> with equal probability. This method is in modern
	 * implementations of the <code>Random</code> class, but is missing from JDK
	 * 1.1.
	 * 
	 * @noshow
	 */
	public boolean nextBoolean() {
		return nextBoolean(0.5);
	}

	/**
	 * Returns the next random integer in the specified range. For example, you
	 * can generate the roll of a six-sided die by calling
	 *
	 * <p>
	 * 
	 * <pre>
	 * <code>
	 * &nbsp;    rgen.nextInt(1, 6);
	 * </code>
	 * </pre>
	 *
	 * <p>
	 * or a random decimal digit by calling
	 *
	 * <p>
	 * 
	 * <pre>
	 * <code>
	 * &nbsp;    rgen.nextInt(0, 9);
	 * </code>
	 * </pre>
	 *
	 * @usage int k = rgen.nextInt(low, high)
	 * @param low
	 *            The low end of the range
	 * @param high
	 *            The high end of the range
	 * @return The next random <code>int</code> between <code>low</code> and
	 *         <code>high</code>, inclusive
	 */
	public int nextInt(int low, int high) {
		if (high < low) {
			// swap
			int temp = low;
			low = high;
			high = temp;
		}
		
		// return fixed number if one is present
		if (!fixedIntegers.isEmpty()) {
			synchronized (fixedIntegers) {
				if (!fixedIntegers.isEmpty()) {
					int fixed = fixedIntegers.remove(0);
					
					// shift the fixed number to be inside the range [low .. high]
					if (fixed < low || fixed > high) {
						int range = high - low + 1;
						while (fixed < low) {
							fixed += range;
						}
						while (fixed > high) {
							fixed -= range;
						}
					}
					
					return fixed;
				}
			}
		}

		// fall back to normal random behavior
		return low + (int) ((high - low + 1) * nextDouble());
	}

	/**
	 * Returns the next random real number in the specified range. The resulting
	 * value is always at least <code>low</code> but always strictly less than
	 * <code>high</code>. You can use this method to generate continuous random
	 * values. For example, you can set the variables <code>x</code> and
	 * <code>y</code> to specify a random point inside the unit square as
	 * follows:
	 *
	 * <p>
	 * 
	 * <pre>
	 * <code>
	 * &nbsp;    double x = rgen.nextDouble(0.0, 1.0);
	 * &nbsp;    double y = rgen.nextDouble(0.0, 1.0);
	 * </code>
	 * </pre>
	 *
	 * @usage double d = rgen.nextDouble(low, high)
	 * @param low
	 *            The low end of the range
	 * @param high
	 *            The high end of the range
	 * @return A random <code>double</code> value <i>d</i> in the range
	 *         <code>low</code> &le; <i>d</i> &lt; <code>high</code>
	 */
	public double nextDouble(double low, double high) {
		return low + (high - low) * nextDouble();
	}

	/**
	 * Returns a random <code>boolean</code> value with the specified
	 * probability. You can use this method to simulate an event that occurs
	 * with a particular probability. For example, you could simulate the result
	 * of tossing a coin like this:
	 *
	 * <p>
	 * 
	 * <pre>
	 * <code>
	 * &nbsp;    String coinFlip = rgen.nextBoolean(0.5) ? "HEADS" : "TAILS";
	 * </code>
	 * </pre>
	 *
	 * @usage if (rgen.nextBoolean(p)) . . .
	 * @param p
	 *            A value between 0 (impossible) and 1 (certain) indicating the
	 *            probability
	 * @return The value <code>true</code> with probability <code>p</code>
	 */
	public boolean nextBoolean(double p) {
		return nextDouble() < p;
	}

	/**
	 * Returns a random opaque color whose components are chosen uniformly in
	 * the 0-255 range.
	 *
	 * @usage Color color = rgen.newColor()
	 * @return A random opaque
	 *         <a href="http://com/j2se/1.4.2/docs/api/java/awt/Color.html">
	 *         <code>Color</code></a>
	 */
	public Color nextColor() {
		return new Color(nextInt(256), nextInt(256), nextInt(256));
	}

	/**
	 * Returns a <code>RandomGenerator</code> instance that can be shared among
	 * several classes.
	 *
	 * @usage RandomGenerator rgen = RandomGenerator.getInstance();
	 * @return A shared <code>RandomGenerator</code> object
	 */
	public static synchronized RandomGenerator getInstance() {
		if (standardInstance == null) {
			standardInstance = new RandomGenerator();
		}
		return standardInstance;
	}

	/**
	 * Removes any previously added fixed numbers that were passed to
	 * feedFixedIntegers.
	 */
	public static synchronized void clearFixedIntegers() {
		fixedIntegers.clear();
	}

	/**
	 * Adds a set of fixed numbers that will be returned by future calls to
	 * nextInt. This is done to "rig" the random generator so it will always
	 * return the same sequence of integers, to aid in testing of programs.
	 * Another way to achieve a similar effect is to use the setSeed method.
	 * 
	 * @param numbers
	 */
	public static synchronized void feedFixedIntegers(int... numbers) {
		for (int n : numbers) {
			fixedIntegers.add(n);
		}
	}

	/**
	 * Adds a set of fixed numbers that will be returned by future calls to
	 * nextInt. This is done to "rig" the random generator so it will always
	 * return the same sequence of integers, to aid in testing of programs.
	 * Another way to achieve a similar effect is to use the setSeed method.
	 * 
	 * @param numbers
	 */
	public static synchronized void feedFixedIntegers(Iterable<Integer> numbers) {
		for (int n : numbers) {
			fixedIntegers.add(n);
		}
	}

	/**
	 * Sets a seed value that will be used by all RandomGenerators when they are
	 * constructed. Note: This will not change the seed of any previously
	 * constructed RandomGenerator object. To do that, call
	 * {@code rg.setSeed(seed);} on that object.
	 * 
	 * @param seed
	 */
	public static void setSharedSeed(long seed) {
		ourSeed = seed;
	}
}

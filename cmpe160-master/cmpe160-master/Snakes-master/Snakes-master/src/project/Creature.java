package project;

import game.Drawable;

/**
 * <p>The {@code Creature} class is the super class of {@link Node} and {@link Food}. 
 * {@code Creature} is abstract because a creature must be neither a Food or Node 
 * not just a creature. Since information of a creature has to be drawn
 * at the window, it implements {@link game.Drawable}.</p>
 * 
 * @version		31 Mar 2018
 * @author 		Emre Girgin
 */

public abstract class Creature implements Drawable{
	
	/**
	 * X-coordinate.	The horizontal component of the coordinates of the creature
	 * with respect to left-top. 
	 */
	private int x;
	
	/**
	 * Y-coordinate.	The vertical component of the coordinates of the creature
	 * with respect to left-top. 
	 */
	private int y;
	/**
	 * Determines whether this creature is head of any snake.
	 */
	private boolean isHead = false;
		
	/**
	 * @return the isHead
	 */
	public boolean isHead() {
		return isHead;
	}

	/**
	 * @param isHead the isHead to set
	 */
	public void setHead(boolean isHead) {
		this.isHead = isHead;
	}

	/**
	 * The method that is used to get X.
	 * 
	 * @return the x
	 */
	public int getX() {
		return x;
	}

	/**
	 * The method that is used to set X.
	 * @param x the x to set
	 */
	public void setX(int x) {
		this.x = x;
	}

	/**
	 * The method that is used to get Y.
	 * @return the y
	 */
	public int getY() {
		return y;
	}

	/**
	 * The method that is used to set Y.
	 * @param y the y to set
	 */
	public void setY(int y) {
		this.y = y;
	}
}

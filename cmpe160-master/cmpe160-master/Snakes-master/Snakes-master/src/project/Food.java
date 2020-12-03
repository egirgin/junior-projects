package project;

import java.awt.Color;

import game.Drawable;
import ui.GridPanel;
/**
 * {@code Food} class represents the food of the turn.
 * 
 * @author Emre Girgin
 *
 */
public class Food extends Creature implements Drawable{
	/**
	 * Sets food to given coordinates
	 * @param x	X-coordinate
	 * @param y Y-coordinate
	 */
	public Food(int x, int y) {
		this.setX(x);
		this.setY(y);
	}
	/**
	 * {@inheritDoc}
	 */
	@Override
	public void draw(GridPanel panel) {
		panel.drawSmallSquare(this.getX(), this.getY(), Color.ORANGE);
	}
	
}

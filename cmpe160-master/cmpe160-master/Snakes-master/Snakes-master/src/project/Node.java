package project;

import java.awt.Color;
import java.util.ArrayList;
import java.util.List;
import game.Direction;
import naturesimulator.LocalInformation;
import ui.GridPanel;
/**
 * Represents the segments of {@link Snake}
 * @author Emre Girgin
 *
 */
public class Node extends Creature{
	
	/**
	 * Sets a node to given coordinates
	 * @param x X-coordinate
	 * @param y	Y-coordinate
	 */
	public Node(int x, int y) {
		this.setX(x);
		this.setY(y);
	}

	/**
	 * Returns a list of the direction of neighbor Food
	 * if there is any. 
	 * 
	 * @param localInformation	The local information of {@code this} Head of snake.
	 * @return	a list of attackable directions.
	 */
	List<Direction> getAttackableDirections(LocalInformation localInformation){
		ArrayList<Direction> toAttack = new ArrayList<>();
		if(localInformation.getCreatureUp() instanceof Food)
			toAttack.add(Direction.UP);
		if(localInformation.getCreatureDown() instanceof Food)
			toAttack.add(Direction.DOWN);
		if(localInformation.getCreatureRight() instanceof Food)
			toAttack.add(Direction.RIGHT);
		if(localInformation.getCreatureLeft() instanceof Food)
			toAttack.add(Direction.LEFT);
		return toAttack;
	}
	/**
	 * {@inheritDoc}
	 */
	@Override
	public void draw(GridPanel panel) {
		if(this.isHead()==true)
			panel.drawSquare(getX(), getY(), Color.RED);
		else
			panel.drawSquare(getX(), getY(), Color.BLUE);
	}

}

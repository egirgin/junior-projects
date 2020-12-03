package project;

import java.util.LinkedList;
import java.util.Random;
import game.Direction;
import naturesimulator.Action;
import naturesimulator.LocalInformation;
/**
 * Represents the snake which consist of {@link Node}s.
 * @author Emre Girgin
 *
 */
public class Snake {
	/**
	 * Stores the nodes of snake.
	 */
	public LinkedList<Node> snake = new LinkedList<>();
	/**
	 * Creates the initial snake.
	 */
	public Snake() {
		this.snake.add(new Node(4,1));
		this.snake.add(new Node(3,1));
		this.snake.add(new Node(2,1));
		this.snake.add(new Node(1,1));
		snake.get(0).setHead(true);
	}
	/**
	 * Creates a new snake from parents last four nodes.
	 * @param parent	The snake whose second half is going to return to a new snake
	 */
	public Snake(LinkedList<Node> parent) {
		for(int i=0;i<4;i++)
		this.snake.add(parent.removeLast());
		this.snake.get(0).setHead(true);
	}
	/**
	 * Method that returns the appropriate action. 
	 * 
	 * @see		naturesimulator.Action
	 * @param 	localInformation	Local information of given creature.
	 * @param	food Current food of game
	 * @return	Returns chosen Action.
	 */
	public Action chooseAction(LocalInformation localInformation,Food food) {
		if(this.snake.size()==8) {
			return new Action(Action.Type.REPRODUCE);
		}
		else if(this.snake.getFirst().getAttackableDirections(localInformation).size()>0)
			return new Action(Action.Type.ATTACK,LocalInformation.getRandomDirection(this.snake.getFirst().getAttackableDirections(localInformation)));
		else {
			if(localInformation.getFreeDirections().size()==0)
				return null;
			Direction direction = this.findDirection(food);
			while(localInformation.getFreeDirections().contains(direction)==false) {
				direction = LocalInformation.getRandomDirection(localInformation.getFreeDirections());
			}
			return new Action(Action.Type.MOVE,direction);
		}
	}

	/**
	 * Snake moves towards to given direction.
	 * @param direction	Direction that is wanted to go to.
	 */
	public void move(Direction direction) {
		int firstX=snake.getFirst().getX();
		int firstY=snake.getFirst().getY();
		Node step = new Node(firstX,firstY);
		if(direction==Direction.UP)//top
			step.setY(firstY-1);
		else if(direction==Direction.DOWN)//bottom
			step.setY(firstY+1);
		else if(direction==Direction.RIGHT)//right
			step.setX(firstX+1);
		else if(direction==Direction.LEFT)//left
			step.setX(firstX-1);

		snake.addFirst(step);
		snake.get(1).setHead(false);
		snake.getFirst().setHead(true);
	}
	/**
	 * Determines the direction which points towards from head of snake to food. 
	 * @param food	Current food of game
	 * @return	Direction from head of snake to food
	 */
	private Direction findDirection(Food food) {
		int firstX=snake.getFirst().getX();
		int firstY=snake.getFirst().getY();
		
		Random rand = new Random();
		int randomInt = rand.nextInt(2);
		
		if(firstY>food.getY() && firstX<food.getX()) {//right top
			if(randomInt==0) return Direction.UP;
			else return Direction.RIGHT;
		}
		else if(firstY>food.getY() && firstX>food.getX()) {//left top
			if(randomInt==0) return Direction.UP;
			else return Direction.LEFT;
		}
		else if(firstY<food.getY() && firstX<food.getX()) {//right bottom
			if(randomInt==0) return Direction.DOWN;
			else return Direction.RIGHT;
		}
		else if(firstY<food.getY() && firstX>food.getX()) {//left bottom
			if(randomInt==0) return Direction.DOWN;
			else return Direction.LEFT;		
		}
		else if(firstY>food.getY() && firstX==food.getX())//top
			return Direction.UP;
		else if(firstY<food.getY() && firstX==food.getX())//bottom
			return Direction.DOWN;
		else if(firstX<food.getX() && firstY==food.getY())//right
			return Direction.RIGHT;
		else if(firstX>food.getX() && firstY==food.getY())//left
			return Direction.LEFT;
		else {
			if(randomInt==0) return Direction.LEFT;
			else return Direction.RIGHT;
		}
	}
	/**
	 * Creature attacks to given creature
	 * if {@code attackedCreature} is Food.
	 * Also, extends towards the position of {@code attackedCreature}.
	 * @param attackedCreature Targeted creature.
	 */
	public void attack(Creature attackedCreature) {
		int targetX = attackedCreature.getX();
		int targetY = attackedCreature.getY();
		this.snake.getFirst().setHead(false);
		this.snake.addFirst(new Node(targetX,targetY));
		this.snake.getFirst().setHead(true);
	}
	/**
	 * A Snake produces a new instance of snake 
	 * which is consisted of the second half of snake.
	 * 
	 * @return	Returns a new Snake 
	 */
	public Snake reproduce() {
		return new Snake(this.snake);	
	}

}

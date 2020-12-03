		package naturesimulator;

import game.Direction;
import game.GridGame;
import project.Creature;
import project.Food;
import project.Node;
import project.Snake;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Class that implements the game logic for Nature Simulator.
 *
 */
public class NatureSimulator extends GridGame {
	/**
	 * List that stores {@code Node}s and {@code Food}
	 */
	private List<Creature> creatures;
	/**
	 * Represents the map
	 */
	private Creature[][] creaturesMap;
	/**
	 * List that stores {@code Snake}s
	 */
	private List<Snake> snakes;
	/**
	 * Food of turn
	 */
	private Food food;

	/**
	 * Creates a new Nature Simulator game instance
	 * 
	 * @param gridWidth
	 *            number of grid squares along the width
	 * @param gridHeight
	 *            number of grid squares along the height
	 * @param gridSquareSize
	 *            size of a grid square in pixels
	 * @param frameRate
	 *            frame rate (number of timer ticks per second)
	 */
	public NatureSimulator(int gridWidth, int gridHeight, int gridSquareSize, int frameRate) {
		super(gridWidth, gridHeight, gridSquareSize, frameRate);

		creatures = new ArrayList<>();
		creaturesMap = new Creature[gridWidth][gridHeight];
		snakes = new ArrayList<>();
	}

	/**
	 * Method that represents a turn of game.
	 * {@inheritDoc}
	 */
	@Override
	protected void timerTick() {
		// Determine and execute actions for all creatures
		ArrayList<Creature> creaturesCopy = new ArrayList<>(creatures);

		ArrayList<Snake> snakesCopy = new ArrayList<>(snakes);

		if (this.isFoodExist(creaturesCopy) == false) {
			int x = (int) (Math.random() * this.getGridWidth());
			int y = (int) (Math.random() * this.getGridHeight());
			while (creaturesMap[x][y] != null) {
				x = (int) (Math.random() * this.getGridWidth());
				y = (int) (Math.random() * this.getGridHeight());
			}
			food = new Food(x, y);
			this.addCreature(food);
		}

		for (Snake snake : snakesCopy) {

			Creature creature = snake.snake.getFirst();

			if (creature.isHead()==false) {
				// Creature is not head of any snake, hence continue with the next creature
				continue;
			}

			// Choose action
			Action action = snake.chooseAction(createLocalInformationForCreature(creature), food);

			// Reset current creature's map position (its position will be marked again)
			for (Node node : snake.snake) {
				updateCreaturesMap(node.getX(), node.getY(), null);
			}

			// Execute action
			if (action != null) {
				if (action.getType() == Action.Type.REPRODUCE) {
					// REPRODUCE
					Snake newSnake = snake.reproduce();
					snakes.add(newSnake);

				} else if (action.getType() == Action.Type.MOVE) {
					// MOVE
					if (isDirectionFree(creature.getX(), creature.getY(), action.getDirection())) {
						snake.move(action.getDirection());
						addCreature(snake.snake.getFirst());
						removeCreature(snake.snake.getLast());
						snake.snake.removeLast();
					}

				} else if (action.getType() == Action.Type.ATTACK) {
					// ATTACK
					Creature attackedCreature = getCreatureAtDirection(creature.getX(), creature.getY(),
							action.getDirection());
					if (attackedCreature != null) {
						snake.attack(attackedCreature);
						if (attackedCreature instanceof Food) {
							// Remove attacked creature since it was eaten.
							removeCreature(attackedCreature);
						}
						addCreature(snake.snake.getFirst());
					}
				}
			}

			// Mark current creature's map position
			for (Node node : snake.snake) {
				updateCreaturesMap(node.getX(), node.getY(), node);
			}
		}
	}

	/**
	 * Determines if is there any food in map.
	 * 
	 * @param creatures
	 *            List that contains all the {@link Node} and {@link Food}
	 * @return True if map contains a food.
	 */
	public boolean isFoodExist(List<Creature> creatures) {
		for (Creature creature : creatures) {
			if (creature instanceof Food)
				return true;
		}
		return false;
	}
	
	public void addSnake(Snake snake) {
		snakes.add(snake);
	}

	/**
	 * Adds a new creature to the game
	 * 
	 * @param creature
	 *            creature to be added
	 * @return boolean indicating the success of addition
	 */
	public boolean addCreature(Creature creature) {
		if (creature != null) {
			if (isPositionInsideGrid(creature.getX(), creature.getY())) {
				if (creaturesMap[creature.getX()][creature.getY()] == null) {
					creatures.add(creature);
					addDrawable(creature);
					creaturesMap[creature.getX()][creature.getY()] = creature;
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
		return false;
	}

	/**
	 * Removes given creature from game
	 * 
	 * @param creature	creature to be removed
	 */
	private void removeCreature(Creature creature) {
		if (creature != null) {
			creatures.remove(creature);
			removeDrawable(creature);
			if (isPositionInsideGrid(creature.getX(), creature.getY())) {
				creaturesMap[creature.getX()][creature.getY()] = null;
			}
		}
	}
	/**
	 * Creates a localinformation for given creature
	 * @param creature	Creature whose localinformation is needed.
	 * @return	localinformation of given creature
	 */
	private LocalInformation createLocalInformationForCreature(Creature creature) {
		int x = creature.getX();
		int y = creature.getY();

		HashMap<Direction, Creature> creatures = new HashMap<>();
		creatures.put(Direction.UP, getCreatureAtPosition(x, y - 1));
		creatures.put(Direction.DOWN, getCreatureAtPosition(x, y + 1));
		creatures.put(Direction.LEFT, getCreatureAtPosition(x - 1, y));
		creatures.put(Direction.RIGHT, getCreatureAtPosition(x + 1, y));

		ArrayList<Direction> freeDirections = new ArrayList<>();
		if (creatures.get(Direction.UP) == null && isPositionInsideGrid(x, y - 1)) {
			freeDirections.add(Direction.UP);
		}
		if (creatures.get(Direction.DOWN) == null && isPositionInsideGrid(x, y + 1)) {
			freeDirections.add(Direction.DOWN);
		}
		if (creatures.get(Direction.LEFT) == null && isPositionInsideGrid(x - 1, y)) {
			freeDirections.add(Direction.LEFT);
		}
		if (creatures.get(Direction.RIGHT) == null && isPositionInsideGrid(x + 1, y)) {
			freeDirections.add(Direction.RIGHT);
		}

		return new LocalInformation(getGridWidth(), getGridHeight(), creatures, freeDirections);
	}
	/**
	 * Checks whether given coordinates in the grid
	 * @param x X-coordinate
	 * @param y	Y-coordinate
	 * @return	True if the position is inside the grid
	 */
	private boolean isPositionInsideGrid(int x, int y) {
		return (x >= 0 && x < getGridWidth()) && (y >= 0 && y < getGridHeight());
	}
	/**
	 * Assigns given creature to given coordinates
	 * @param x	X-coordinate
	 * @param y	Y-coordinate
	 * @param creature	creature to be assigned
	 */
	private void updateCreaturesMap(int x, int y, Creature creature) {
		if (isPositionInsideGrid(x, y)) {
			creaturesMap[x][y] = creature;
		}
	}
	/**
	 * Returns the creature at given position
	 * @param x	X-coordinate
	 * @param y	Y-coordinate
	 * @return	null if it is empty
	 */
	private Creature getCreatureAtPosition(int x, int y) {
		if (!isPositionInsideGrid(x, y)) {
			return null;
		}
		return creaturesMap[x][y];
	}
	/**
	 * Returns the neighbor creature which is at given direction 
	 * @param x	X-coordinate
	 * @param y	Y-coordinate
	 * @param direction	direction to be controlled
	 * @return	null if it is empty
	 */
	private Creature getCreatureAtDirection(int x, int y, Direction direction) {
		if (direction == null) {
			return null;
		}
		int xTarget = x;
		int yTarget = y;
		if (direction == Direction.UP) {
			yTarget--;
		} else if (direction == Direction.DOWN) {
			yTarget++;
		} else if (direction == Direction.LEFT) {
			xTarget--;
		} else if (direction == Direction.RIGHT) {
			xTarget++;
		}
		return getCreatureAtPosition(xTarget, yTarget);
	}
	/**
	 * true if given position is free
	 * @param x	X-coordinate
	 * @param y	Y-coordinate
	 * @return	false if given position is not null
	 */
	private boolean isPositionFree(int x, int y) {
		return isPositionInsideGrid(x, y) && getCreatureAtPosition(x, y) == null;
	}
	/**
	 * true if given direction of given coordinate is null
	 * @param x	X-coordinate
	 * @param y	Y-coordinate
	 * @param direction	direction that is looked for
	 * @return	false if given direction is not null
	 */
	private boolean isDirectionFree(int x, int y, Direction direction) {
		if (direction == null) {
			return false;
		}
		int xTarget = x;
		int yTarget = y;
		if (direction == Direction.UP) {
			yTarget--;
		} else if (direction == Direction.DOWN) {
			yTarget++;
		} else if (direction == Direction.LEFT) {
			xTarget--;
		} else if (direction == Direction.RIGHT) {
			xTarget++;
		}
		return isPositionFree(xTarget, yTarget);
	}

}

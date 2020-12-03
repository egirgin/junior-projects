	package main;

import naturesimulator.NatureSimulator;
import project.Node;
import project.Snake;
import ui.ApplicationWindow;

import java.awt.*;


/**
 * The main class that can be used as a playground to test your project.
 * This class will be discarded and replaced with our own grading class.
 */
public class Main {

    /**
     * Main entry point for the application.
     *
     * @param args application arguments
     */
    public static void main(String[] args) {
        EventQueue.invokeLater(() -> {
            try {
                // Create game
                // You can change the world width and height, size of each grid square in pixels or the game speed
                NatureSimulator game = new NatureSimulator(25, 25, 25, 10);
                    
                    Snake firstSnake = new Snake();
                    //System.out.println(firstSnake);
                    for(Node node: firstSnake.snake) {
                    	game.addCreature(node);
                    }
                    game.addSnake(firstSnake);

                // Create application window that contains the game panel
                ApplicationWindow window = new ApplicationWindow(game.getGamePanel());
                window.getFrame().setVisible(true);

                // Start game
                game.start();

            } catch (Exception e) {
                e.printStackTrace();
            }
        });
    }

}

# CS 352 Project | Football Penalty Shootout Game
As the name suggests, Football Penalty Shooter is a game similar to football penalties in real life. It can be a 2-player game or an interactive game with the computer.

The Functionalities are:-
1. Set the level of game to be played(2 player, easy , medium, hard)
2. Move around the camera with the mouse/touchpad to get a better field of view.
3. Zoom in/out using the keyboard, again, to get a better field of view.
4. Set the angle of the shot and thus point it in the desired direction.
5. Set the ball's spin, the angle by which you want the ball to turn and its direction.
6. Set the power of the shot, that is, the speed of the ball.
7. View the goal statistics and switch from one Category to the other using the keyboard.
8. The goal is defended by a moving and jumping Goalkeeper, who moves in the direction of the shot(in the case of an interactive game).
9. In the 2-player Category, One player can be the attacker and kick the ball to score a goal. The attacker has five chances, after which the game ends and the winner is declared.

<h2>Project Installation Guide</h2>

The main library used in our project is Open Graphics Library, an API for rendering 2D and 3D Vector Graphics(use of polygons to represent images).

 - Clone the repo
 - Install the Opengl library
 - Move to the required folder
 - Run using a C++ compiler with the help of the command 

```
g++ main.cpp functionalities.cpp shapes.cpp constants.h -lGL -lGLU -lglut -lm -o run && ./run
```

 - The project will start working now

<h2>Important files and folder structure:-</h2>

 - constants.h - values of different constants are stored in it, like dimensions
 - functionalities.h - declarations of different functions used
 - functionalities.cpp - definitions of different functions used like collision between objects
 - shapes.h - declarations of different objects used
 - shapes.cpp - definitions of rendering of different objects
 - main.cpp - it calls the sequential execution of different components used in the project
 - textureloader.py - to convert image into RGB format to render into the project
 - fontloader.py - to convert image into RGB format to render into the project
 - Resources - contains different images, text and sound used

<h2>Contributers:-</h2>

 - Krish Agrawal
 - Pilla Venkata Sekhar
 - Jha Rohan
 
This project is part of the course CS352 Computer Graphics and Visualization Lab under the guidance of Dr Somnath Dey.<br>
Reference: [GitHub Repository](https://github.com/amitmakkad/football_penalty_kick_opengl_cpp)

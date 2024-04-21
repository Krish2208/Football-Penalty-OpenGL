# CS 352 Project | Football Penalty Shootout Game
Welcome to Football Penalty Shooter, a thrilling OpenGL C++ game that brings the excitement of football penalty shootouts to your computer screen. Whether you're a solo player looking for a challenge or seeking to compete head-to-head against a friend, Football Penalty Shooter offers an immersive gaming experience that captures the essence of the sport.

## Features

- **Customizable Difficulty**: Choose from easy, medium, and hard difficulty levels to suit your skill level and preference.
- **Intuitive Controls**: Maneuver the camera, adjust shot angles and power, and apply spin to the ball with responsive and user-friendly controls.
- **Dynamic Gameplay**: Engage in penalty shootouts against a computer-controlled goalkeeper or challenge a friend in two-player mode.
- **Immersive Environment**: Enjoy stunning visuals and immersive sound effects that enhance the overall gaming experience.
- **Goal Statistics**: Track your goal-scoring prowess and switch between different statistical categories using intuitive keyboard commands.

## Controls

- **Camera Movement**: Use the mouse or touchpad to move the camera for a better field of view.
- **Zoom**: Adjust the zoom level using keyboard commands.
- **Shot Angle**: Set the angle of the shot to point it in the desired direction.
- **Shot Power**: Adjust the power of the shot to control the speed of the ball.
- **Spin**: Apply spin to the ball by setting the angle and direction.
- **Goalkeeper Control**: In two-player mode, one player serves as the attacker, while the other controls the goalkeeper. The attacker has five chances to score goals before the game ends.

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

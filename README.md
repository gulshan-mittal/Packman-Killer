# Graphics

### **Pacman Killer game in OpenGL 3**. 

 

### Libraries Needed

- libao-dev
- libmpg123-dev
- freeglut3-dev
- All other glut libraries 
  - For ubuntu
    - `sudo apt-get install libao-dev`
    - `sudo apt-get install libmpg123-dev`
    - `sudo apt-get install freeglut3-dev`
    - `sudo apt-get install freeglut3`
    - `sudo apt-get install freeglut3-dbg`



### Compile & Run

- cmake CMakeLists.txt
  - *if any library is not installed installed it*.
- make
- ./pacman-killer



## Controls

- A  - Move Player left

- D  - Move Player right

- SPACE - Jump in Air by player

- Arrow keys / Mouse drag - Pan (when zoomed)

  - LEFT ARROW KEY - Move Screen left (Panning)
  - RIGHT ARROW KEY - Move Screen right (Panning)
  - UP ARROW KEY - Move Screen up (Panning)
  - DOWN ARROW KEY - Move Screen down (Panning)

- Mousewheel - Zoom

- Mouse click and move up/down/left/right - To move screen

  ​

## Features

- Nice cool background and color scheme
- Fully Randomise Position of Pacman, Speed, Porcupines position, Speed, Magnets 
- Background music
- Moving Pacmans (Enemies)
- 3 Lifes
- Pond 
- Trampoline 
- Trees
- Levels and Time counter
- Reflection through plank/log
- Magnet Appearing on both sides randomly 
- Score and Life counter
- Zooming/panning using both mouse and keyboard
- Modular codebase
- Jump by player, move left , right



## Scoring & Lives

- Jump on Enemy (Pacman) without plank = +25
- Jump on Enemy (Pacman) with plank = +40
- Jump on Porcupines = - 1 life
- Time Counter = 60 (level 1)  
  - `if Time counter == 0` , Game finish




##### *Copyright © 2018  Gulshan Kumar*



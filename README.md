EELU Pac-Man 🎮

Welcome to EELU Pac-Man, a modern take on the classic arcade game developed for the Computer Graphics course at the Egyptian E-Learning University (EELU) - Faculty of IT. Built with C++ and OpenGL (GLUT), this project features a 37x37 maze, intelligent ghost AI, and smooth animations. Navigate Pac-Man, eat dots, and outsmart four ghosts in this nostalgic yet technically impressive game!

  



🚀 Features

Classic Gameplay: Guide Pac-Man to collect dots and power pellets while avoiding ghosts.
Intelligent Ghost AI:
🟥 Blinky: Relentless chaser.
🟪 Pinky: Predicts and ambushes Pac-Man.
🟦 Inky: Uses Blinky’s position for strategic pursuit.
🟧 Clyde: Switches between chasing and scattering.


Dynamic Modes: Ghosts cycle through Chase, Scatter, Frightened, and Eaten states.
Smooth Animations: Pac-Man’s mouth animates with a sine wave; ghosts have expressive eyes.
Optimized Rendering: Leverages OpenGL display lists for efficient maze drawing.
Tunnels: Teleport across the maze for strategic escapes.
Scoring & Lives: Earn points, track lives, and pause/resume with P.
Responsive Controls: Use WASD or arrow keys for seamless navigation.


🛠️ Installation
Prerequisites

C++ Compiler: GCC, Clang, or MSVC (C++17).
OpenGL: Ensure your system supports OpenGL.
GLUT: FreeGLUT or OpenGL Utility Toolkit.

Steps

Clone the Repository:git clone https://github.com/Xeahmed/eelu-pacman.git
cd eelu-pacman


Install Dependencies:
Ubuntu/Debian:sudo apt-get install freeglut3-dev


macOS:brew install freeglut


Windows: Install FreeGLUT and configure with your IDE (e.g., Visual Studio).


Compile the Code:g++ main.cpp -o pacman -lGL -lGLU -lglut


Run the Game:./pacman




🎲 Usage

Start the Game: Press Enter to begin.
Controls:
WASD or Arrow Keys: Move Pac-Man (W/Up: North, S/Down: South, A/Left: West, D/Right: East).
P: Pause/Resume.
Enter: Start or restart after game over.
Esc: Exit.


Objective:
Eat dots (2) for 1 point each.
Grab power pellets (3) for 10 points and to make ghosts vulnerable.
Avoid ghosts in Chase/Scatter modes to preserve lives.



Example
./pacman
# Press Enter, use WASD to navigate, P to pause


📸 Screenshots

  
  



🎥 Demo Video

  
    
  



🐛 Known Issues

Collision Edge Cases: Rare issues at tunnel edges.
Performance: May lag on low-end systems due to OpenGL rendering.
Windows Setup: GLUT configuration may vary across IDEs.


🚧 Roadmap

Add sound effects for dots and power pellets.
Implement level progression with increasing difficulty.
Support custom maze layouts.
Add a high-score leaderboard.


🤝 Contributing
Contributions are welcome to enhance EELU Pac-Man! To contribute:

Fork the repository.
Create a feature branch (git checkout -b feature/your-feature).
Commit changes (git commit -m "Add your feature").
Push to the branch (git push origin feature/your-feature).
Open a Pull Request.

See CONTRIBUTING.md for details.

📜 License
This project is licensed under the MIT License. See LICENSE for details.

📬 Contact
Developed by Eng. Ahmed Refat Mohamed for the Computer Graphics course at EELU - Faculty of IT.  

📧 Email: a.xp2121@gmail.com  
🐦 X: @xeahmed2  
🌐 LinkedIn: Ahmed Refat Mohamed  
🌐 GitHub: Xeahmed


🎉 Acknowledgments

Inspired by the classic Pac-Man arcade game.
Developed as part of the Computer Graphics course at EELU - Faculty of IT, Sohag.
Thanks to the OpenGL and GLUT communities for their tools.
Gratitude to EELU for fostering innovative student projects.


  



@keyframes fadeIn {
  0% { opacity: 0; }
  100% { opacity: 1; }
}
@keyframes bounce {
  0%, 100% { transform: translateY(0); }
  50% { transform: translateY(-10px); }
}

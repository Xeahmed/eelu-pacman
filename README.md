# EELU Pac-Man 🎮✨

![Pac-Man](https://img.shields.io/badge/Pac-Man-Classic%20Game-yellow?style=for-the-badge&logo=gamepad&labelColor=orange)  
[![GitHub](https://img.shields.io/badge/GitHub-Xeahmed-181717?style=for-the-badge&logo=github)](https://github.com/Xeahmed/eelu-pacman)  
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Ahmed%20Refat%20Mohamed-0A66C2?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/ahmed-refat-mohamed/)  
[![X](https://img.shields.io/badge/X-@xeahmed2-000000?style=for-the-badge&logo=x)](https://x.com/xeahmed2)  
[![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)](LICENSE)  
![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus)  
![OpenGL](https://img.shields.io/badge/OpenGL-GLUT-5586A4?style=for-the-badge&logo=opengl)  
[![EELU](https://img.shields.io/badge/EELU-Computer%20Graphics-green?style=for-the-badge)](https://eelu.edu.eg)

Welcome to **EELU Pac-Man**, a vibrant reimagination of the classic arcade game crafted for the **Computer Graphics** course at the **Egyptian E-Learning University (EELU) - Faculty of IT**. Built with **C++** and **OpenGL (GLUT)**, this project boasts a 37x37 maze, smart ghost AI, and fluid animations. Guide Pac-Man, gobble dots, and outwit four ghosts in this nostalgic yet cutting-edge experience! 🎉

<p align="center">
  <img src="screenshots/StartScreen.png" alt="Start Screen" width="600" style="border-radius: 15px; box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3); animation: fadeIn 2s;">
</p>

---

## 🚀 Features

- **Classic Gameplay**: Navigate Pac-Man to eat dots and power pellets while dodging ghosts.  
- **Intelligent Ghost AI**:  
  - 🟥 **Blinky**: Aggressive chaser.  
  - 🟪 **Pinky**: Ambushes with prediction.  
  - 🟦 **Inky**: Strategic pursuit using Blinky's position.  
  - 🟧 **Clyde**: Toggles between chase and scatter.  
- **Dynamic Modes**: Ghosts switch between Chase, Scatter, Frightened, and Eaten.  
- **Smooth Animations**: Pac-Man’s mouth moves with a sine wave; ghosts have expressive eyes.  
- **Optimized Rendering**: Utilizes OpenGL display lists for efficient maze drawing.  
- **Tunnels**: Teleport across the maze for tactical moves.  
- **Scoring & Lives**: Track points and lives, with pause (`P`) and restart (`Enter`).  
- **Responsive Controls**: Use WASD or arrow keys for intuitive movement.

---

## 🛠️ Installation

### Prerequisites
- **C++ Compiler**: GCC, Clang, or MSVC (C++17).  
- **OpenGL**: Ensure OpenGL support on your system.  
- **GLUT**: Install FreeGLUT or OpenGL Utility Toolkit.

### Steps
1. **Clone the Repository**:  
   ```bash
   git clone https://github.com/Xeahmed/eelu-pacman.git
   cd eelu-pacman
   ```
2. **Install Dependencies**:  
   - **Ubuntu/Debian**:  
     ```bash
     sudo apt-get install freeglut3-dev
     ```
   - **macOS**:  
     ```bash
     brew install freeglut
     ```
   - **Windows**: Install FreeGLUT and configure with your IDE (e.g., Visual Studio).  
3. **Compile the Code**:  
   ```bash
   g++ main.cpp -o pacman -lGL -lGLU -lglut
   ```
4. **Run the Game**:  
   ```bash
   ./pacman
   ```

---

## 🎮 Usage

1. **Start the Game**: Press `Enter` to begin.  
2. **Controls**:  
   - **WASD** or **Arrow Keys**: Move Pac-Man (W/Up: North, S/Down: South, A/Left: West, D/Right: East).  
   - **P**: Pause/Resume.  
   - **Enter**: Restart after game over.  
   - **Esc**: Exit.  
3. **Objective**:  
   - Eat dots (`2`) for 1 point each.  
   - Grab power pellets (`3`) for 10 points to make ghosts vulnerable.  
   - Avoid ghosts in Chase/Scatter modes to save lives.

### Example
```bash
./pacman
# Press Enter, use WASD to navigate, P to pause
```

---

## 📸 Screenshots

<p align="center">
  <img src="screenshots/StartScreen.png" alt="Start Screen" width="300" style="border-radius: 10px; margin: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);">
  <img src="screenshots/gameScreen.png" alt="Game Screen" width="300" style="border-radius: 10px; margin: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);">
  <img src="screenshots/gameoverScreen.png" alt="Game Over Screen" width="300" style="border-radius: 10px; margin: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);">
</p>

---

## 🎥 Demo Video

<p align="center">
  <video controls width="600" style="border-radius: 10px; box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3);">
    <source src="screenshots/demo.mp4" type="video/x-matroska">
    Your browser does not support the video tag.
  </video>
</p>

---

## 🐛 Known Issues
- **Collision Edge Cases**: Rare glitches at tunnel edges.  
- **Performance**: Potential lag on low-end systems due to OpenGL.  
- **Windows Setup**: GLUT configuration may differ across IDEs.

---

## 🚧 Roadmap
- Add sound effects for eating dots and pellets.  
- Implement multi-level progression.  
- Enable custom maze designs.  
- Include a high-score leaderboard.

---

## 🤝 Contributing

Contributions are welcome! To contribute:  
1. Fork the repository.  
2. Create a branch (`git checkout -b feature/your-feature`).  
3. Commit changes (`git commit -m "Add your feature"`).  
4. Push to your branch (`git push origin feature/your-feature`).  
5. Open a Pull Request.  

See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

---

## 📜 License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

## 📬 Contact

Developed by **Eng. Ahmed Refat Mohamed** for the Computer Graphics course at EELU - Faculty of IT, Sohag.  
- 📧 Email: [a.xp2121@gmail.com](mailto:a.xp2121@gmail.com)  
- 🐦 X: [@xeahmed2](https://x.com/xeahmed2)  
- 🌐 LinkedIn: [Ahmed Refat Mohamed](https://www.linkedin.com/in/ahmed-refat-mohamed/)  
- 🌐 GitHub: [Xeahmed](https://github.com/Xeahmed)

---

## 🎉 Acknowledgments

- Inspired by the iconic Pac-Man arcade game.  
- Created for the Computer Graphics course at EELU - Faculty of IT, Sohag.  
- Gratitude to the OpenGL and GLUT communities.  
- Thanks to EELU for supporting student innovation.

<p align="center">
  <img src="https://media.giphy.com/media/3o6Zt8dX5nTLVGRl6w/giphy.gif" alt="Pac-Man Animation" width="250" style="animation: bounce 2s infinite;">
</p>

<style>
@keyframes fadeIn {
  0% { opacity: 0; }
  100% { opacity: 1; }
}
@keyframes bounce {
  0%, 100% { transform: translateY(0); }
  50% { transform: translateY(-15px); }
}
</style>

# Contributing to EELU Pac-Man

Thank you for your interest in contributing to **EELU Pac-Man**! This project is a collaborative effort, and we welcome contributions from the community to enhance its features, fix bugs, and improve documentation. Please follow these guidelines to ensure a smooth and effective contribution process.

## Code of Conduct
- Be respectful and inclusive to all contributors.
- Use constructive feedback and avoid personal attacks.
- Adhere to the project's open-source spirit and MIT License.

## How to Contribute

### 1. Fork the Repository
- Click the "Fork" button on the top right of the [GitHub repository](https://github.com/Xeahmed/eelu-pacman).
- Clone your forked repository to your local machine:
  ```bash
  git clone https://github.com/your-username/eelu-pacman.git
  cd eelu-pacman
  ```

### 2. Set Up Your Environment
- Ensure you have a C++ compiler (GCC, Clang, or MSVC) with C++17 support.
- Install OpenGL and GLUT (e.g., `sudo apt-get install freeglut3-dev` on Ubuntu).
- Verify your setup by compiling the project:
  ```bash
  g++ source.cpp -o pacman -lGL -lGLU -lglut
  ```

### 3. Create a Branch
- Create a new branch for your feature or bug fix:
  ```bash
  git checkout -b feature/your-feature-name
  ```
- Use descriptive names (e.g., `feature/add-sound-effects` or `bugfix/fix-collision`).

### 4. Make Changes
- Edit the code in `source.cpp` or other files as needed.
- Follow the existing coding style (e.g., consistent indentation, comments in English).
- Test your changes locally:
  ```bash
  ./pacman
  ```

### 5. Commit Your Changes
- Stage your changes:
  ```bash
  git add .
  ```
- Commit with a clear message:
  ```bash
  git commit -m "Add [feature/bugfix]: Brief description of change"
  ```

### 6. Push to Your Branch
- Push your branch to your forked repository:
  ```bash
  git push origin feature/your-feature-name
  ```

### 7. Open a Pull Request (PR)
- Go to the original repository and click "New Pull Request".
- Select your branch and provide a detailed description of your changes.
- Wait for review and feedback from maintainers.

## Guidelines
- **Bug Reports**: Open an issue with steps to reproduce and expected vs. actual behavior.
- **Feature Requests**: Suggest new features via issues with a clear use case.
- **Code Quality**: Ensure no new warnings or errors; add tests if applicable.
- **Documentation**: Update `README.md` or this file if your changes affect usage.

## Review Process
- Pull requests will be reviewed by maintainers.
- Expect feedback or requests for changes before merging.
- Once approved, your contribution will be merged into the `main` branch.

## Contact
For questions or assistance, reach out to **Eng. Ahmed Refat Mohamed**:
- Email: [a.xp2121@gmail.com](mailto:a.xp2121@gmail.com)
- GitHub: [Xeahmed](https://github.com/Xeahmed)

We appreciate your efforts to make **EELU Pac-Man** better! 🚀

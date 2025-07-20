#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <vector>

#define M_PI 3.14159265358979323846
#define MAZE_WIDTH 37
#define MAZE_HEIGHT 37
#define GRID_SIZE 0.054f
#define WALL_THICKNESS 0.005f
#define PACMAN_RADIUS 0.03f
#define GHOST_RADIUS 0.03f
#define PACMAN_SPEED 0.2f
#define GHOST_SPEED 0.15f
#define COLLISION_DISTANCE (PACMAN_RADIUS + GHOST_RADIUS)

// Maze layout (37x32)
const char InitialMaze[MAZE_HEIGHT][MAZE_WIDTH + 1] = {
    "##############PP################",
    "#......#.P.P##PP##PPP.#........#",
    "#..F...#.P.P##PP##PPP.#...###..#",
    "#.......PP.P##PP##PPP.....###..#",
    "#..#....PP.PPPPPPPPPPPP........#",
    "#..#....P..PPPPPPPPPPPP.#..##..#",
    "#..###..P..PPPPPPPPPPPP.#..##..#",
    "#..#....P#.PPPPPPPPPPPPP#......#",
    "#..#....P#.PPPPPPPPPPPP.....F..#",
    "#........#.PP#######PPP..#######",
    "#..F.....#.PP#PP#PP#PPP........#",
    "#....#...#.PP#PP#PP#PPP....##..#",
    "#....#...#.PPPPPPPPPPP..#..##F.#",
    "#..###.....PPPPPPPPPPP..#..##..#",
    "#....#.....PP#######PP..#..##..#",
    "#....#...#.PP#PP#PP#PPP........#",
    "#........#.PP#PP#PP#PPP....#####",
    "#..#.....#.PPPPPPPPPPPP........#",
    "#..#.......PPPPPPPPPPPP........#",
    "#..###...F.PP#######PPP.#....#.#",
    "#..#.......PPPPPPPP#PPP.#....#.#",
    "#..#.....#.PPPPPPPP#PPP.#......#",
    "#.F......#.PPPPPPPP#PPP.##..####",
    "#....#...#.PPPPPPPPPPPP........#",
    "#....#....PPP#######PPP........#",
    "#..###.....PPPPPPPP#PPP..#######",
    "#....#...#.PPPPPPPP#PPP..#.....#",
    "#....#...#.PP#######PPP.....F..#",
    "#........#.PPPPPPPPPPPP........#",
    "#..#.....#.PPPPPPPPPP.....######",
    "#..#.....P.PPPPPPPPPP..........#",
    "#..#.....P.PPPPPPPPPP..........#",
    "#.###....P.PPPPPPPPPP......##..#",
    "#...#....P.P##PP##PPP.....##...#",
    "#..F..#..PPP##PP##PPP..#....F..#",
    "#.....#..PPP##PP##PPP..#.......#",
    "##############PP################",
};

// Game state
struct PacMan {
    float x, y;
    char direction;
    char nextDirection;
    float mouthOpenPercentage;
};

struct Ghost {
    float x, y;
    float r, g, b;
    char direction;
    enum Mode { CHASE, SCATTER, FRIGHTENED, EATEN } mode;
    float scatterX, scatterY;
};

PacMan pacman = { 1 * GRID_SIZE, 1 * GRID_SIZE, 'E', 'E', 0.0f };
char maze[MAZE_HEIGHT][MAZE_WIDTH];
Ghost ghosts[4] = {
    {10 * GRID_SIZE, 15 * GRID_SIZE, 1.0f, 0.0f, 0.0f, 'N', Ghost::CHASE, 34 * GRID_SIZE, 0 * GRID_SIZE},
    {11 * GRID_SIZE, 15 * GRID_SIZE, 1.0f, 0.5f, 0.5f, 'S', Ghost::CHASE, 34 * GRID_SIZE, 29 * GRID_SIZE},
    {12 * GRID_SIZE, 15 * GRID_SIZE, 0.0f, 1.0f, 1.0f, 'N', Ghost::CHASE, 0 * GRID_SIZE, 0 * GRID_SIZE},
    {13 * GRID_SIZE, 15 * GRID_SIZE, 1.0f, 0.5f, 0.0f, 'S', Ghost::CHASE, 0 * GRID_SIZE, 29 * GRID_SIZE}
};
int score = 0;
int lives = 3;
bool gameStarted = false;
bool gameOver = false;
bool paused = false;
float frightenedTimer = 0.0f;
float lastTime = 0.0f;
GLuint mazeDisplayList;

// Convert pixel to grid coordinates
int pixelToGrid(float pos) {
    return static_cast<int>(pos / GRID_SIZE);
}

// Check if position is a wall
bool isWall(int gridX, int gridY) {
    if (gridX < 0 || gridX >= MAZE_WIDTH || gridY < 0 || gridY >= MAZE_HEIGHT) return true;
    return maze[gridY][gridX] == 1;
}

// Get available directions at a grid position
std::vector<char> getAvailableDirections(int gridX, int gridY, char currentDir) {
    std::vector<char> directions;
    if (gridY > 0 && maze[gridY - 1][gridX] != 1 && currentDir != 'S') directions.push_back('N');
    if (gridY < MAZE_HEIGHT - 1 && maze[gridY + 1][gridX] != 1 && currentDir != 'N') directions.push_back('S');
    if (gridX > 0 && maze[gridY][gridX - 1] != 1 && currentDir != 'E') directions.push_back('W');
    if (gridX < MAZE_WIDTH - 1 && maze[gridY][gridX + 1] != 1 && currentDir != 'W') directions.push_back('E');
    return directions;
}

// Draw rounded corner for outer walls
void drawRoundedCorner(float x, float y, float radius, int startAngle, int endAngle) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = startAngle; i <= endAngle; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(x + radius * std::cos(rad), y + radius * std::sin(rad));
    }
    glEnd();
}

// Draw maze with Display List
void compileMaze() {
    mazeDisplayList = glGenLists(1);
    glNewList(mazeDisplayList, GL_COMPILE);
    glPushMatrix();
    for (int row = 0; row < MAZE_HEIGHT; row++) {
        for (int col = 0; col < MAZE_WIDTH; col++) {
            float x = col * GRID_SIZE;
            float y = (MAZE_HEIGHT - 1 - row) * GRID_SIZE; // Flip y-axis to rotate maze
            if (maze[row][col] == 1) {
                glColor3f(0.0f, 0.0f, 1.0f); // Blue walls
                if (row == 0 && col == 0) {
                    drawRoundedCorner(x + WALL_THICKNESS, y + WALL_THICKNESS, WALL_THICKNESS, 90, 180);
                    glBegin(GL_QUADS);
                    glVertex2f(x + WALL_THICKNESS, y);
                    glVertex2f(x + GRID_SIZE, y);
                    glVertex2f(x + GRID_SIZE, y + GRID_SIZE);
                    glVertex2f(x + WALL_THICKNESS, y + GRID_SIZE);
                    glVertex2f(x, y + WALL_THICKNESS);
                    glVertex2f(x + GRID_SIZE, y + WALL_THICKNESS);
                    glVertex2f(x + GRID_SIZE, y + GRID_SIZE);
                    glVertex2f(x, y + GRID_SIZE);
                    glEnd();
                }
                else if (row == 0 && col == MAZE_WIDTH - 1) {
                    drawRoundedCorner(x + GRID_SIZE - WALL_THICKNESS, y + WALL_THICKNESS, WALL_THICKNESS, 0, 90);
                    glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + GRID_SIZE - WALL_THICKNESS, y);
                    glVertex2f(x + GRID_SIZE - WALL_THICKNESS, y + GRID_SIZE);
                    glVertex2f(x, y + GRID_SIZE);
                    glVertex2f(x, y + WALL_THICKNESS);
                    glVertex2f(x + GRID_SIZE, y + WALL_THICKNESS);
                    glVertex2f(x + GRID_SIZE, y + GRID_SIZE);
                    glVertex2f(x, y + GRID_SIZE);
                    glEnd();
                }
                else if (row == MAZE_HEIGHT - 1 && col == 0) {
                    drawRoundedCorner(x + WALL_THICKNESS, y + GRID_SIZE - WALL_THICKNESS, WALL_THICKNESS, 180, 270);
                    glBegin(GL_QUADS);
                    glVertex2f(x + WALL_THICKNESS, y);
                    glVertex2f(x + GRID_SIZE, y);
                    glVertex2f(x + GRID_SIZE, y + GRID_SIZE);
                    glVertex2f(x + WALL_THICKNESS, y + GRID_SIZE);
                    glVertex2f(x, y);
                    glVertex2f(x + GRID_SIZE, y);
                    glVertex2f(x + GRID_SIZE, y + GRID_SIZE - WALL_THICKNESS);
                    glVertex2f(x, y + GRID_SIZE - WALL_THICKNESS);
                    glEnd();
                }
                else if (row == MAZE_HEIGHT - 1 && col == MAZE_WIDTH - 1) {
                    drawRoundedCorner(x + GRID_SIZE - WALL_THICKNESS, y + GRID_SIZE - WALL_THICKNESS, WALL_THICKNESS, 270, 360);
                    glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + GRID_SIZE - WALL_THICKNESS, y);
                    glVertex2f(x + GRID_SIZE - WALL_THICKNESS, y + GRID_SIZE);
                    glVertex2f(x, y + GRID_SIZE);
                    glVertex2f(x, y);
                    glVertex2f(x + GRID_SIZE, y);
                    glVertex2f(x + GRID_SIZE, y + GRID_SIZE - WALL_THICKNESS);
                    glVertex2f(x, y + GRID_SIZE - WALL_THICKNESS);
                    glEnd();
                }
                else {
                    glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + GRID_SIZE, y);
                    glVertex2f(x + GRID_SIZE, y + GRID_SIZE);
                    glVertex2f(x, y + GRID_SIZE);
                    glEnd();
                }
            }
            else if (maze[row][col] == 2) { // Dot
                glColor3f(1.0f, 1.0f, 1.0f);
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(x + GRID_SIZE / 2, y + GRID_SIZE / 2);
                for (int i = 0; i <= 360; i += 5) {
                    float rad = i * M_PI / 180.0f;
                    glVertex2f(x + GRID_SIZE / 2 + 0.005f * std::cos(rad), y + GRID_SIZE / 2 + 0.005f * std::sin(rad));
                }
                glEnd();
            }
            else if (maze[row][col] == 3) { // Power pellet
                glColor3f(1.0f, 1.0f, 0.0f);
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(x + GRID_SIZE / 2, y + GRID_SIZE / 2);
                for (int i = 0; i <= 360; i += 5) {
                    float rad = i * M_PI / 180.0f;
                    glVertex2f(x + GRID_SIZE / 2 + 0.01f * std::cos(rad), y + GRID_SIZE / 2 + 0.01f * std::sin(rad));
                }
                glEnd();
            }
        }
    }
    glPopMatrix();
    glEndList();
}

// Render Pac-Man
void renderPacMan() {
    glPushMatrix();
    glTranslatef(pacman.x, (MAZE_HEIGHT - 1 - pixelToGrid(pacman.y)) * GRID_SIZE, 0.0f); // Adjust for maze rotation
    if (pacman.direction == 'E') glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    else if (pacman.direction == 'W') glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    else if (pacman.direction == 'N') glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    else if (pacman.direction == 'S') glRotatef(270.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    float mouthOpen = pacman.mouthOpenPercentage * 0.3f * 360 / 2;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = static_cast<int>(mouthOpen); i <= 360 - static_cast<int>(mouthOpen); i += 2) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(PACMAN_RADIUS * std::cos(rad), PACMAN_RADIUS * std::sin(rad));
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f); // Black eye
    float eyeX = PACMAN_RADIUS * 0.4f;
    float eyeY = PACMAN_RADIUS * 0.4f;
    if (pacman.direction == 'W') eyeX = -eyeX;
    glVertex2f(eyeX, eyeY);
    for (int i = 0; i <= 360; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(eyeX + PACMAN_RADIUS * 0.2f * std::cos(rad), eyeY + PACMAN_RADIUS * 0.2f * std::sin(rad));
    }
    glEnd();

    glPopMatrix();
}

// Render ghost
void renderGhost(const Ghost& ghost) {
    glPushMatrix();
    glTranslatef(ghost.x, (MAZE_HEIGHT - 1 - pixelToGrid(ghost.y)) * GRID_SIZE, 0.0f); // Adjust for maze rotation

    // Body color
    if (ghost.mode == Ghost::FRIGHTENED) {
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
    }
    else {
        glColor3f(ghost.r, ghost.g, ghost.b);
    }

    // Body
    glBegin(GL_QUADS);
    glVertex2f(-GHOST_RADIUS, -GHOST_RADIUS);
    glVertex2f(GHOST_RADIUS, -GHOST_RADIUS);
    glVertex2f(GHOST_RADIUS, GHOST_RADIUS * 0.5f);
    glVertex2f(-GHOST_RADIUS, GHOST_RADIUS * 0.5f);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, GHOST_RADIUS * 0.5f);
    for (int i = 0; i <= 180; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(GHOST_RADIUS * std::cos(rad), GHOST_RADIUS * 0.5f + GHOST_RADIUS * std::sin(rad));
    }
    glEnd();

    // Eyes
    if (ghost.mode != Ghost::FRIGHTENED) {
        glColor3f(1.0f, 1.0f, 1.0f); // White
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-GHOST_RADIUS * 0.4f, GHOST_RADIUS * 0.7f);
        for (int i = 0; i <= 360; i += 5) {
            float rad = i * M_PI / 180.0f;
            glVertex2f(-GHOST_RADIUS * 0.4f + GHOST_RADIUS * 0.2f * std::cos(rad), GHOST_RADIUS * 0.7f + GHOST_RADIUS * 0.2f * std::sin(rad));
        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(GHOST_RADIUS * 0.4f, GHOST_RADIUS * 0.7f);
        for (int i = 0; i <= 360; i += 5) {
            float rad = i * M_PI / 180.0f;
            glVertex2f(GHOST_RADIUS * 0.4f + GHOST_RADIUS * 0.2f * std::cos(rad), GHOST_RADIUS * 0.7f + GHOST_RADIUS * 0.2f * std::sin(rad));
        }
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f); // Black pupils
        float pupilX = (ghost.direction == 'E') ? GHOST_RADIUS * 0.5f : (ghost.direction == 'W') ? GHOST_RADIUS * 0.3f : GHOST_RADIUS * 0.4f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-pupilX, GHOST_RADIUS * 0.7f);
        for (int i = 0; i <= 360; i += 5) {
            float rad = i * M_PI / 180.0f;
            glVertex2f(-pupilX + GHOST_RADIUS * 0.1f * std::cos(rad), GHOST_RADIUS * 0.7f + GHOST_RADIUS * 0.1f * std::sin(rad));
        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(pupilX, GHOST_RADIUS * 0.7f);
        for (int i = 0; i <= 360; i += 5) {
            float rad = i * M_PI / 180.0f;
            glVertex2f(pupilX + GHOST_RADIUS * 0.1f * std::cos(rad), GHOST_RADIUS * 0.7f + GHOST_RADIUS * 0.1f * std::sin(rad));
        }
        glEnd();
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f); // Frightened expression
        glBegin(GL_LINES);
        glVertex2f(-GHOST_RADIUS * 0.5f, GHOST_RADIUS * 0.7f);
        glVertex2f(-GHOST_RADIUS * 0.3f, GHOST_RADIUS * 0.5f);
        glVertex2f(-GHOST_RADIUS * 0.3f, GHOST_RADIUS * 0.5f);
        glVertex2f(-GHOST_RADIUS * 0.1f, GHOST_RADIUS * 0.7f);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(GHOST_RADIUS * 0.1f, GHOST_RADIUS * 0.7f);
        glVertex2f(GHOST_RADIUS * 0.3f, GHOST_RADIUS * 0.5f);
        glVertex2f(GHOST_RADIUS * 0.3f, GHOST_RADIUS * 0.5f);
        glVertex2f(GHOST_RADIUS * 0.5f, GHOST_RADIUS * 0.7f);
        glEnd();
    }

    glPopMatrix();
}

// Update ghost movement
void updateGhost(Ghost& ghost, int index, float deltaTime) {
    int gridX = pixelToGrid(ghost.x);
    int gridY = pixelToGrid(ghost.y);

    // Handle tunnels
    if (maze[gridY][gridX] == 4 && ghost.x <= 0) {
        ghost.x = (MAZE_WIDTH - 1) * GRID_SIZE;
    }
    else if (maze[gridY][gridX] == 4 && ghost.x >= MAZE_WIDTH * GRID_SIZE) {
        ghost.x = GRID_SIZE;
    }

    // Decision at intersections
    if (std::fmod(ghost.x, GRID_SIZE) < GHOST_SPEED * deltaTime && std::fmod(ghost.y, GRID_SIZE) < GHOST_SPEED * deltaTime) {
        std::vector<char> directions = getAvailableDirections(gridX, gridY, ghost.direction);
        if (!directions.empty()) {
            float targetX = pacman.x, targetY = pacman.y;
            if (ghost.mode == Ghost::CHASE) {
                if (index == 1) { // Pinky
                    if (pacman.direction == 'N') targetY += 4 * GRID_SIZE;
                    else if (pacman.direction == 'S') targetY -= 4 * GRID_SIZE;
                    else if (pacman.direction == 'E') targetX += 4 * GRID_SIZE;
                    else if (pacman.direction == 'W') targetX -= 4 * GRID_SIZE;
                }
                else if (index == 2) { // Inky
                    float pivotX = pacman.x, pivotY = pacman.y;
                    if (pacman.direction == 'N') pivotY += 2 * GRID_SIZE;
                    else if (pacman.direction == 'S') pivotY -= 2 * GRID_SIZE;
                    else if (pacman.direction == 'E') pivotX += 2 * GRID_SIZE;
                    else if (pacman.direction == 'W') pivotX -= 2 * GRID_SIZE;
                    targetX = 2 * pivotX - ghosts[0].x;
                    targetY = 2 * pivotY - ghosts[0].y;
                }
                else if (index == 3) { // Clyde
                    float dist = std::sqrt(std::pow(ghost.x - pacman.x, 2) + std::pow(ghost.y - pacman.y, 2));
                    if (dist < 8 * GRID_SIZE) {
                        targetX = ghost.scatterX;
                        targetY = ghost.scatterY;
                    }
                }
            }
            else if (ghost.mode == Ghost::SCATTER) {
                targetX = ghost.scatterX;
                targetY = ghost.scatterY;
            }
            else if (ghost.mode == Ghost::FRIGHTENED) {
                int randIndex = std::rand() % directions.size();
                ghost.direction = directions[randIndex];
            }

            if (ghost.mode != Ghost::FRIGHTENED) {
                float minDist = std::numeric_limits<float>::max();
                char bestDir = ghost.direction;
                for (char dir : directions) {
                    float nextX = gridX * GRID_SIZE, nextY = gridY * GRID_SIZE;
                    if (dir == 'N') nextY -= GRID_SIZE;
                    else if (dir == 'S') nextY += GRID_SIZE;
                    else if (dir == 'E') nextX += GRID_SIZE;
                    else if (dir == 'W') nextX -= GRID_SIZE;
                    float dist = std::sqrt(std::pow(nextX - targetX, 2) + std::pow(nextY - targetY, 2));
                    if (dist < minDist) {
                        minDist = dist;
                        bestDir = dir;
                    }
                }
                ghost.direction = bestDir;
            }
        }
    }

    // Move ghost
    float speed = (ghost.mode == Ghost::FRIGHTENED) ? GHOST_SPEED * 0.5f : GHOST_SPEED;
    float newX = ghost.x, newY = ghost.y;
    if (ghost.direction == 'N') newY -= speed * deltaTime;
    else if (ghost.direction == 'S') newY += speed * deltaTime;
    else if (ghost.direction == 'E') newX += speed * deltaTime;
    else if (ghost.direction == 'W') newX -= speed * deltaTime;
    int nextGridX = pixelToGrid(newX);
    int nextGridY = pixelToGrid(newY);
    if (!isWall(nextGridX, nextGridY)) {
        ghost.x = newX;
        ghost.y = newY;
    }
}

// Render UI
void renderUI() {
    char text[128];
    glColor3f(1.0f, 1.0f, 1.0f);
#ifdef _MSC_VER
    _snprintf_s(text, sizeof(text), _TRUNCATE, "Score: %d", score);
    glRasterPos2f(0.1f, MAZE_HEIGHT * GRID_SIZE + 0.1f);
    for (char* p = text; *p; p++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    _snprintf_s(text, sizeof(text), _TRUNCATE, "Lives: %d", lives);
    glRasterPos2f(MAZE_WIDTH * GRID_SIZE - 0.3f, MAZE_HEIGHT * GRID_SIZE + 0.1f);
    for (char* p = text; *p; p++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
#else
    std::snprintf(text, sizeof(text), "Score: %d", score);
    glRasterPos2f(0.1f, MAZE_HEIGHT * GRID_SIZE + 0.1f);
    for (char* p = text; *p; p++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    std::snprintf(text, sizeof(text), "Lives: %d", lives);
    glRasterPos2f(MAZE_WIDTH * GRID_SIZE - 0.3f, MAZE_HEIGHT * GRID_SIZE + 0.1f);
    for (char* p = text; *p; p++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
#endif
    if (!gameStarted) {
        glColor3f(1.0f, 1.0f, 0.0f);
        glRasterPos2f(MAZE_WIDTH * GRID_SIZE / 2 - 0.3f, MAZE_HEIGHT * GRID_SIZE / 2);
        const char* msg = "Press ENTER to Start";
        for (const char* p = msg; *p; p++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
    }
    else if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(MAZE_WIDTH * GRID_SIZE / 2 - 0.4f, MAZE_HEIGHT * GRID_SIZE / 2);
        const char* msg = "GAME OVER! Press ENTER to Restart";
        for (const char* p = msg; *p; p++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
    }
    else if (paused) {
        glColor3f(1.0f, 1.0f, 0.0f);
        glRasterPos2f(MAZE_WIDTH * GRID_SIZE / 2 - 0.3f, MAZE_HEIGHT * GRID_SIZE / 2);
        const char* msg = "PAUSED";
        for (const char* p = msg; *p; p++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glCallList(mazeDisplayList);
    renderPacMan();
    for (int i = 0; i < 4; i++) renderGhost(ghosts[i]);
    renderUI();
    glutSwapBuffers();
}

// Update game state
void updateGame(int value) {
    if (!gameStarted || gameOver || paused) {
        glutPostRedisplay();
        glutTimerFunc(16, updateGame, 0);
        return;
    }

    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Update Pac-Man
    pacman.mouthOpenPercentage = 0.5f + 0.5f * std::sin(currentTime * 5.0f);

    int gridX = pixelToGrid(pacman.x);
    int gridY = pixelToGrid(pacman.y);
    float nextX = pacman.x, nextY = pacman.y;
    if (pacman.nextDirection == 'N' && !isWall(gridX, gridY - 1)) {
        nextY -= PACMAN_SPEED * deltaTime;
        pacman.direction = 'N';
    }
    else if (pacman.nextDirection == 'S' && !isWall(gridX, gridY + 1)) {
        nextY += PACMAN_SPEED * deltaTime;
        pacman.direction = 'S';
    }
    else if (pacman.nextDirection == 'E' && !isWall(gridX + 1, gridY)) {
        nextX += PACMAN_SPEED * deltaTime;
        pacman.direction = 'E';
    }
    else if (pacman.nextDirection == 'W' && !isWall(gridX - 1, gridY)) {
        nextX -= PACMAN_SPEED * deltaTime;
        pacman.direction = 'W';
    }

    if (!isWall(pixelToGrid(nextX), pixelToGrid(nextY))) {
        pacman.x = nextX;
        pacman.y = nextY;
    }

    // Handle tunnels
    if (pacman.x <= 0 && maze[gridY][gridX] == 4) pacman.x = (MAZE_WIDTH - 1) * GRID_SIZE;
    else if (pacman.x >= MAZE_WIDTH * GRID_SIZE && maze[gridY][gridX] == 4) pacman.x = GRID_SIZE;

    // Collect dots and power pellets
    if (maze[gridY][gridX] == 2) {
        maze[gridY][gridX] = 0;
        score += 1;
        glDeleteLists(mazeDisplayList, 1); // Delete old Display List
        compileMaze(); // Recompile Display List to update maze
    }
    else if (maze[gridY][gridX] == 3) {
        maze[gridY][gridX] = 0;
        score += 10;
        frightenedTimer = 7.0f;
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].mode != Ghost::EATEN) ghosts[i].mode = Ghost::FRIGHTENED;
        }
        glDeleteLists(mazeDisplayList, 1); // Delete old Display List
        compileMaze(); // Recompile Display List to update maze
    }

    // Move ghosts
    for (int i = 0; i < 4; i++) {
        updateGhost(ghosts[i], i, deltaTime);
    }

    // Check collisions
    int ghostEatenScore = 20;
    for (int i = 0; i < 4; i++) {
        Ghost& ghost = ghosts[i];
        float dist = std::sqrt(std::pow(pacman.x - ghost.x, 2) + std::pow(pacman.y - ghost.y, 2));
        if (dist < COLLISION_DISTANCE) {
            if (ghost.mode == Ghost::FRIGHTENED) {
                ghost.x = 10 * GRID_SIZE;
                ghost.y = 15 * GRID_SIZE;
                ghost.mode = Ghost::EATEN;
                score += ghostEatenScore;
                ghostEatenScore *= 2;
            }
            else if (ghost.mode != Ghost::EATEN) {
                lives--;
                if (lives <= 0) gameOver = true;
                pacman.x = 1 * GRID_SIZE;
                pacman.y = 1 * GRID_SIZE;
                pacman.direction = 'E';
                pacman.nextDirection = 'E';
                for (int j = 0; j < 4; j++) {
                    ghosts[j].x = (10 + j) * GRID_SIZE;
                    ghosts[j].y = 15 * GRID_SIZE;
                    ghosts[j].mode = Ghost::CHASE;
                    ghosts[j].direction = (j % 2 == 0) ? 'N' : 'S';
                }
                break;
            }
        }
    }

    // Update Frightened mode timer
    if (frightenedTimer > 0) {
        frightenedTimer -= deltaTime;
        if (frightenedTimer <= 0) {
            for (int i = 0; i < 4; i++) {
                if (ghosts[i].mode == Ghost::FRIGHTENED) ghosts[i].mode = Ghost::CHASE;
            }
        }
    }

    // Update eaten ghosts
    for (int i = 0; i < 4; i++) {
        if (ghosts[i].mode == Ghost::EATEN) {
            float targetX = 10 * GRID_SIZE, targetY = 15 * GRID_SIZE;
            float dist = std::sqrt(std::pow(ghosts[i].x - targetX, 2) + std::pow(ghosts[i].y - targetY, 2));
            if (dist < GHOST_SPEED * deltaTime) {
                ghosts[i].mode = Ghost::CHASE;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateGame, 0);
}

// Keyboard input
void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 27) std::exit(0);
    if (key == 13) {
        if (!gameStarted || gameOver) {
            for (int i = 0; i < MAZE_HEIGHT; i++) {
                for (int j = 0; j < MAZE_WIDTH; j++) {
                    char c = InitialMaze[j][i];
                    maze[i][j] = (c == '#') ? 1 : (c == '.') ? 2 : (c == 'F') ? 3 : (c == 'P') ? 4 : 0;
                }
            }
            score = 0;
            lives = 3;
            pacman.x = 1 * GRID_SIZE;
            pacman.y = 1 * GRID_SIZE;
            pacman.direction = 'E';
            pacman.nextDirection = 'E';
            for (int i = 0; i < 4; i++) {
                ghosts[i].x = (10 + i) * GRID_SIZE;
                ghosts[i].y = 15 * GRID_SIZE;
                ghosts[i].direction = (i % 2 == 0) ? 'N' : 'S';
                ghosts[i].mode = Ghost::CHASE;
            }
            gameStarted = true;
            gameOver = false;
            paused = false;
            glDeleteLists(mazeDisplayList, 1);
            compileMaze();
        }
    }
    if (key == 'p' || key == 'P') {
        paused = !paused;
    }
    if (key == 'w' || key == 'W') pacman.nextDirection = 'N';
    if (key == 's' || key == 'S') pacman.nextDirection = 'S';
    if (key == 'a' || key == 'A') pacman.nextDirection = 'W';
    if (key == 'd' || key == 'D') pacman.nextDirection = 'E';
}

// Special keyboard input
void handleSpecialKeyboard(int key, int x, int y) {
    if (!gameStarted || gameOver || paused) return;
    switch (key) {
    case GLUT_KEY_UP: pacman.nextDirection = 'N'; break;
    case GLUT_KEY_DOWN: pacman.nextDirection = 'S'; break;
    case GLUT_KEY_LEFT: pacman.nextDirection = 'W'; break;
    case GLUT_KEY_RIGHT: pacman.nextDirection = 'E'; break;
    }
}

// Initialize OpenGL and maze
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, MAZE_WIDTH * GRID_SIZE, 0.0f, MAZE_HEIGHT * GRID_SIZE + 0.2f);
    glMatrixMode(GL_MODELVIEW);

    // Initialize maze
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            char c = InitialMaze[j][i];
            maze[i][j] = (c == '#') ? 1 : (c == '.') ? 2 : (c == 'F') ? 3 : (c == 'P') ? 4 : 0;
        }
    }
    compileMaze();

    std::srand(std::time(0));
    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("EELU Pac-Man by Eng. Ahmed Refat Mohamed");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(handleSpecialKeyboard);
    glutTimerFunc(0, updateGame, 0);
    glutMainLoop();
    return 0;
}
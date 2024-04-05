#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>

using namespace std;

static int counter = 1; // Counter to control enemy movement speed

// Function to set cursor position on console window
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Class definition for the SpaceShooter game
class SpaceShooter {
private:
    int width, height;
    int x, y;
    int score;
    bool gameover;

    // Structure for coordinates
    struct Coordinate {
        int x, y;
    };

    // Vectors to store bullets, enemies, and stars
    vector<Coordinate> bullets;
    vector<Coordinate> enemies;
    vector<Coordinate> stars;

public:
    // Constructor for SpaceShooter class
    SpaceShooter(int width, int height) : width(width), height(height),
        x(width / 2), y(height - 5), score(0), gameover(false) {}

    // Function to draw player's spaceship
    void drawPlayer() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        gotoxy(x, y);
        cout << "  A" << endl;
        gotoxy(x, y + 1);
        cout << " /|\\" << endl;
        gotoxy(x, y + 2);
        cout << "/_|_\\" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // Reset to default color
    }

    // Function to erase player's spaceship
    void erasePlayer() {
        gotoxy(x, y);
        cout << "   " << endl;
        cout << "     " << endl;
        cout << "      ";
    }

    // Function to move player's spaceship left
    void moveLeft() {
        if (x > 1)
            x -= 3;
    }

    // Function to move player's spaceship right
    void moveRight() {
        if (x < width - 8)
            x += 3;
    }

    // Function to shoot bullets
    void shoot() {
        bullets.push_back({ x + 2, y - 1 });
    }

    // Function to draw bullets
    void drawBullets() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);

        for (int i = 0; i < bullets.size(); i++) {
            gotoxy(bullets[i].x, bullets[i].y);
            cout << "A" << endl;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // Reset to default color
    }

    // Function to move bullets
    void moveBullets() {
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i].y--;

            // Check for collision with enemies
            bool collided = false; // Flag to track if collision occurred
            for (int j = 0; j < enemies.size(); j++) {
                // Define bounding boxes for bullet and enemy
                int bulletLeft = bullets[i].x;
                int bulletRight = bullets[i].x + 1; // Assuming bullets are just single characters
                int bulletTop = bullets[i].y;
                int bulletBottom = bullets[i].y;

                int enemyLeft = enemies[j].x;
                int enemyRight = enemies[j].x + 4; // Assuming enemies are 5 characters wide
                int enemyTop = enemies[j].y;
                int enemyBottom = enemies[j].y + 1; // Assuming enemies are 2 characters tall

                // Check for overlap of bounding boxes
                if (bulletRight >= enemyLeft && bulletLeft <= enemyRight && bulletBottom >= enemyTop && bulletTop <= enemyBottom) {
                    collided = true;
                    enemies.erase(enemies.begin() + j);
                    score += 10; // Increase the score
                    break;
                }
            }

            // If collision occurred, remove the bullet
            if (collided || bullets[i].y <= 1) {
                bullets.erase(bullets.begin() + i);
                i--; // Adjust loop index to prevent skipping bullets
            }
        }
    }

    // Function to draw enemies
    void drawEnemies() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
        for (int i = 0; i < enemies.size(); i++) {
            gotoxy(enemies[i].x, enemies[i].y);
            cout << "|++|";
            gotoxy(enemies[i].x, enemies[i].y + 1);
            cout << "\\||/";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // Reset to default color
    }

    // Function to move enemies
    void moveEnemies() {
        // Move enemies every 15 frames
        if (counter % 15 == 0) {
            for (int i = 0; i < enemies.size(); i++) {
                static vector<int> enemyMoveTimers(enemies.size(), 0);
                enemies[i].y++;

                // Check for collision with player
                if (enemies[i].x >= x - 3 && enemies[i].x <= x + 4 && enemies[i].y >= y && enemies[i].y <= y + 2) {
                    gameover = true;
                    break;
                }

                if (enemies[i].y >= height - 1) {
                    enemies.erase(enemies.begin() + i);
                    i--;
                }
            }
        }
        counter++;
    }

    // Function to generate enemies
    void generateEnemies() {
        if (rand() % 100 < 5) { // 5% chance of generating an enemy in each frame
            int enemyX = rand() % (width - 4) + 2; // Ensure enemies don't spawn too close to the edges
            enemies.push_back({ enemyX, 1 });
        }
        if (rand() % 100 < 2) { // Increase the chance of generating enemies for more frequent spawns
            int enemyX = rand() % (width - 4) + 2;
            enemies.push_back({ enemyX, 1 });
        }
    }

    // Function to draw stars
    void drawStars() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        for (int i = 0; i < stars.size(); i++) {
            gotoxy(stars[i].x, stars[i].y);
            cout << ".";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // Reset to default color
    }

    // Function to move stars
    void moveStars() {
        for (int i = 0; i < stars.size(); i++) {
            stars[i].y++;

            if (stars[i].y >= height - 1) {
                stars[i].x = rand() % width;
                stars[i].y = 1;
            }
        }
    }

    // Function to check if the game is over
    bool isGameOver() const {
        return gameover;
    }

    // Function to get the score
    int getScore() const {
        return score;
    }

    // Function to initialize stars
    void initializeStars(int count) {
        for (int i = 0; i < count; i++) {
            stars.push_back({ rand() % width, rand() % height });
        }
    }
};

// Main function
int main() {
    system("mode con: lines=40 cols=80");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    srand(time(0));
    int width = 80, height = 40;
    SpaceShooter game(width, height);
    game.initializeStars(50); // Initialize stars

    cout << "Instructions" << endl;
    cout << "" << endl;
    cout << "1. Use right and left arrow keys to move" << endl;
    cout << "2. Press space bar to fire" << endl;
    cout << "3. Press Esc to quit the game" << endl << endl;

    system("pause");

    while (!game.isGameOver()) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 75) // Left arrow key
                game.moveLeft();
            else if (ch == 77) // Right arrow key
                game.moveRight();
            else if (ch == ' ') // Space bar
                game.shoot();
            else if (ch == 27) // Escape key
                break;
        }

        game.generateEnemies();
        game.drawPlayer();
        game.drawBullets();
        game.drawEnemies();
        game.drawStars(); // Draw stars after drawing other objects

        game.moveBullets();
        game.moveEnemies();
        game.moveStars(); // Move stars after moving other objects

        Sleep(50); // Sleep for smoother animation

        system("cls"); // Clear the screen
    }

    // Print "Game Over" and the score in large font and colored text
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    gotoxy(width / 2 , height / 2);
    cout << "GAME OVER" << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    gotoxy(width / 2 - 2, height / 2 + 2);
    cout << "YOUR SCORE: " << game.getScore() << endl;

    return 0;
}

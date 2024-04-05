#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>

#define LEFT_ARROW   97
#define RIGHT_ARROW  100
#define SPACE_BAR    32
#define ESC          27
#define MAX_BULLETS  500
#define ENEMY_COUNT  5
#define BASE_ENEMY_SPEED 1 //

//"I defined arrays for enemies and bullets because there are multiple enemies and multiple bullet shots in the game.
int playerX, playerY, enemyX[ENEMY_COUNT], enemyY[ENEMY_COUNT], bulletX[MAX_BULLETS], bulletY[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];
int score = 0;
int lives = 3;

void drawPlayer() {
    setcolor(GREEN); //Green
    setfillstyle(SOLID_FILL, GREEN); // Make the interior of the shape green.
    // The spaceship is being drawn as a triangle.
    //int points[] = { playerX - 20, playerY + 20, playerX + 20, playerY + 20, playerX, playerY - 20, playerX - 20, playerY + 20 };
    int points[] = { playerX - 10, playerY + 20, playerX + 10, playerY + 20, playerX, playerY - 20, playerX - 10, playerY + 20 };
    fillpoly(4, points);
}

void drawBullet(int i) {
    setcolor(RED); //Let the bullets be red in color.
    setfillstyle(SOLID_FILL, RED); // Make the interiors of the bullets red.
    rectangle(bulletX[i] - 4, bulletY[i] - 4, bulletX[i] + 4, bulletY[i] + 4); // Kare þeklinde mermi çiz
    floodfill(bulletX[i], bulletY[i], RED);
}

void drawEnemy(int i) {
    setcolor(YELLOW); // Let the targets be yellow in color.
    setfillstyle(SOLID_FILL, YELLOW); // Make the interiors of the enemies yellow.
    rectangle(enemyX[i] - 10, enemyY[i] - 10, enemyX[i] + 10, enemyY[i] + 10);
    floodfill(enemyX[i], enemyY[i], YELLOW);
}

void drawScore() {
    setcolor(WHITE); // Make the color of the score white.
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); // Decrease the font size.
    outtextxy(10, 10, "Score=  "); //Print the text on the screen and append ' = ' to the end.
    char buffer[5];
    sprintf(buffer, "%d", score);
    outtextxy(120, 10, buffer); // Print the score on the screen (New position established).
}
//The function where the game starts.
void initializeGame() {
    playerX = getmaxx() / 2;
    playerY = getmaxy() - 30;

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletActive[i] = false;
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemyX[i] = rand() % (getmaxx() - 20) + 10;
        enemyY[i] = rand() % 200 + 10;
    }
}
// This is for moving the player
void movePlayer(int direction) {
    if (direction == LEFT_ARROW && playerX > 10) {
        playerX -= 10;
    } else if (direction == RIGHT_ARROW && playerX < getmaxx() - 10) {
        playerX += 10;
    }
}
//This is for creating a new bullet
void fireBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bulletActive[i]) {
            bulletActive[i] = true;
            bulletX[i] = playerX;
            bulletY[i] = playerY - 20;
            return;
        }
    }
}
//This is for moving all the bullets if the bullets are active.
void moveBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) {
            bulletY[i] -= 10;
            if (bulletY[i] <= 0) {
                bulletActive[i] = false;
            }
        }
    }
}
//This is for moving enemies which are seen on the screen
void moveEnemies() {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemyY[i] += BASE_ENEMY_SPEED + (score / 50); // Increase the speed when the score becomes 50 times the points
        if (enemyY[i] >= getmaxy() - 10) {
            enemyX[i] = rand() % (getmaxx() - 20) + 10;
            enemyY[i] = rand() % 200 + 10;
        }
    }
}
// This is for to controlling the enemy and bullet collision
void checkCollision() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) {
            for (int j = 0; j < ENEMY_COUNT; j++) {
                //Check for collision between bullets and enemies.
                if (bulletX[i] >= enemyX[j] - 10 && bulletX[i] <= enemyX[j] + 10 &&
                    bulletY[i] >= enemyY[j] - 10 && bulletY[i] <= enemyY[j] + 10) {
                    bulletActive[i] = false;
                    score += 10;
                    // Reposition the enemy when a collision occurs.
                    enemyX[j] = rand() % (getmaxx() - 20) + 10;
                    enemyY[j] = rand() % 200 + 10;
                }
            }
        }
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {


        // Check for collision between the spaceship and the enemy.
        if (playerX - 10 <= enemyX[i] + 10 && playerX + 10 >= enemyX[i] - 10 &&
            playerY - 10 <= enemyY[i] + 10 && playerY + 10 >= enemyY[i] - 10) {
            lives = 0;
        }
    }
}




void displayGameOver() {
    cleardevice(); // Clean the screen
    setcolor(YELLOW); // Make the 'Game Over' text yellow in color.
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 6); // Decrease the font size
    outtextxy(getmaxx() / 2 - 150, getmaxy() / 2 - 50, "GAME OVER!");

    setcolor(BLUE); // Make the 'Score' text blue in color
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4); // Decrease the font size
    outtextxy(getmaxx() / 2 - 100, getmaxy() / 2 + 50, "Score: ");
    char buffer[5];
    sprintf(buffer, "%d", score);
    outtextxy(getmaxx() / 2 + 100, getmaxy() / 2 + 50, buffer);
}

int main() {
    int gd = DETECT, gm, ch;
    initgraph(&gd, &gm, "");

    initializeGame();

    while (1) {
        cleardevice();

        drawPlayer();
        drawScore();

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                drawBullet(i);
            }
        }

        for (int i = 0; i < ENEMY_COUNT; i++) {
            drawEnemy(i);
        }

        moveBullets();
        moveEnemies();
        checkCollision();

        if (lives <= 0) {
            displayGameOver();
            break;
        }

        if (kbhit()) {
            ch = getch();
            switch (ch) {
                case LEFT_ARROW:
                case RIGHT_ARROW:
                    movePlayer(ch);
                    break;
                case SPACE_BAR:
                    fireBullet();
                    break;
                case ESC:
                    exit(0);
                default:
                    break;
            }
        }

        delay(15);
    }

    getch();
    closegraph();
    return 0;
}

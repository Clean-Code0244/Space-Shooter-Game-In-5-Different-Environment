#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BULLETS  500
#define ENEMY_COUNT  5
#define BASE_ENEMY_SPEED 1 // Initial speed

int playerX, playerY, enemyX[ENEMY_COUNT], enemyY[ENEMY_COUNT], bulletX[MAX_BULLETS], bulletY[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];
int score = 0;
int lives = 3;
bool gameover = false; // A flag indicating whether the game has ended or not.

void drawPlayer() {
    // The spaceship is drawn as a triangle.
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0); // Green Color
    glVertex2f(playerX - 10, playerY - 20); // The spaceship is shifted downwards.
    glVertex2f(playerX + 10, playerY - 20);
    glVertex2f(playerX, playerY + 20);
    glEnd();
}

void drawBullet(int i) {
    glColor3f(1.0, 0.0, 0.0); //Let the bullets be red in color.
    glBegin(GL_QUADS);
    glVertex2f(bulletX[i] - 4, bulletY[i] - 4);
    glVertex2f(bulletX[i] + 4, bulletY[i] - 4);
    glVertex2f(bulletX[i] + 4, bulletY[i] + 4);
    glVertex2f(bulletX[i] - 4, bulletY[i] + 4);
    glEnd();
}

void drawEnemy(int i) {
    glColor3f(1.0, 1.0, 0.0); //Let the enemies be yellow in color.
    glBegin(GL_QUADS);
    glVertex2f(enemyX[i] - 10, enemyY[i] - 10);
    glVertex2f(enemyX[i] + 10, enemyY[i] - 10);
    glVertex2f(enemyX[i] + 10, enemyY[i] + 10);
    glVertex2f(enemyX[i] - 10, enemyY[i] + 10);
    glEnd();
}

void initializeGame() {
    srand(time(NULL));
    playerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    playerY = 30; // The height value has been changed.

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletActive[i] = false;
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        // Only targets coming from the top of the screen.
        enemyX[i] = rand() % (glutGet(GLUT_WINDOW_WIDTH) - 20) + 10; //Position is randomly generated.
        enemyY[i] = glutGet(GLUT_WINDOW_HEIGHT) - 10;
    }
}

void movePlayer(int direction) {
    if (direction == GLUT_KEY_LEFT && playerX > 10) {
        playerX -= 10;
    }
    else if (direction == GLUT_KEY_RIGHT && playerX < glutGet(GLUT_WINDOW_WIDTH) - 10) {
        playerX += 10;
    }
}

void fireBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bulletActive[i]) {
            bulletActive[i] = true;
            bulletX[i] = playerX;
            bulletY[i] = playerY + 20; // The height value is changed.
            return;
        }
    }
}

// For moving the bullets which are on the screen.
void moveBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) {
            bulletY[i] += 10; // The increment was applied since the bullets move upwards. (Actually different from other environments)
            if (bulletY[i] >= glutGet(GLUT_WINDOW_HEIGHT)) { // Bullets will become inactive when they go out of the screen.
                bulletActive[i] = false;
            }
        }
    }
}

// For moving enemies.
void moveEnemies() {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        // Enemies only move along the vertical axis.
        enemyY[i] -= BASE_ENEMY_SPEED + (score / 50);
        if (enemyY[i] <= -10) { // If the enemy goes off the screen, reposition it.
            enemyX[i] = rand() % (glutGet(GLUT_WINDOW_WIDTH) - 20) + 10;
            enemyY[i] = glutGet(GLUT_WINDOW_HEIGHT); // Set the enemy's height value to the top of the screen.
        }
    }
}

bool checkCollision(int object1X, int object1Y, int object1Width, int object1Height,
    int object2X, int object2Y, int object2Width, int object2Height) {
    return (object1X < object2X + object2Width &&
        object1X + object1Width > object2X &&
        object1Y < object2Y + object2Height &&
        object1Y + object1Height > object2Y);
}

void checkCollisions() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) {
            for (int j = 0; j < ENEMY_COUNT; j++) {
                if (checkCollision(bulletX[i] - 4, bulletY[i] - 4, 8, 8, enemyX[j] - 10, enemyY[j] - 10, 20, 20)) {
                    bulletActive[i] = false;
                    score += 10;
                    enemyX[j] = rand() % (glutGet(GLUT_WINDOW_WIDTH) - 20) + 10;
                    enemyY[j] = glutGet(GLUT_WINDOW_HEIGHT); // Set the enemy's height to the top of the screen.
                }
            }
        }
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (checkCollision(playerX - 10, playerY - 20, 20, 40, enemyX[i] - 10, enemyY[i] - 10, 20, 20)) {
            lives = 0;
        }
    }

    if (lives <= 0) {
        gameover = true;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!gameover) {
        drawPlayer();

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                drawBullet(i);
            }
        }

        for (int i = 0; i < ENEMY_COUNT; i++) {
            drawEnemy(i);
        }

        // Draw the score text on the screen
        glColor3f(1.0, 1.0, 1.0); // White color

        glRasterPos2f(10, glutGet(GLUT_WINDOW_HEIGHT) - 30); // Set the position of the text
        char scoreText[50];
        sprintf_s(scoreText, "Score: %d", score);
        int len = strlen(scoreText);
        for (int i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreText[i]); // Change font and size
        }
    }
    else {
        // Draw "GAME OVER!" text on the screen when the game is over
        glColor3f(1.0, 1.0, 0.0); // Yellow color
        glRasterPos2f(glutGet(GLUT_WINDOW_WIDTH) / 2 - 60, glutGet(GLUT_WINDOW_HEIGHT) / 2); // Set the position of the text
        char gameOverText[] = "GAME OVER!";
        int len = strlen(gameOverText);
        for (int i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverText[i]); // Change font and size
        }

        // Draw the score text on the screen
        glColor3f(0.0, 0.0, 1.0); // Blue color
        glRasterPos2f(glutGet(GLUT_WINDOW_WIDTH) / 2 - 50, glutGet(GLUT_WINDOW_HEIGHT) / 2 - 30); // Set the position of the text
        char scoreText[50];
        sprintf_s(scoreText, "SCORE: %d", score);
        len = strlen(scoreText);
        for (int i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreText[i]); // Change font and size
        }
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(int value) {
    moveBullets();
    moveEnemies();
    checkCollisions();

    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case ' ':
        fireBullet();
        break;
    }
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
        movePlayer(key);
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Space Shooter");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard); // Use special keyboard function
    glutKeyboardFunc(keyboard); // Use keyboard function
    glutTimerFunc(0, update, 0);

    initializeGame();

    glutMainLoop();
    return 0;
}

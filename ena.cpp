<<<<<<< HEAD
// ============================================================
// MACROS - These tell the compiler to use static linking
// ============================================================
#define GLEW_STATIC    // Use static version of GLEW (no DLL)
#define GLFW_STATIC    // Use static version of GLFW (no DLL)

// ============================================================
// HEADER FILES
// ============================================================
#include <GL/glew.h>    // GLEW: Manages OpenGL extensions
#include <GLFW/glfw3.h> // GLFW: Creates windows and handles input
#include <iostream>     // For printing error messages
#include <cmath>        // For sin(), cos() math functions
#include <string>       // For std::string text handling

// ============================================================
// WINDOW DIMENSIONS (initial size)
// ============================================================
int WINDOW_WIDTH = 800;   // Initial window width in pixels
int WINDOW_HEIGHT = 600;  // Initial window height in pixels

// ============================================================
// PADDLE DIMENSIONS
// ============================================================
const float PADDLE_WIDTH = 10.0f;   // Paddle thickness (pixels)
const float PADDLE_HEIGHT = 80.0f;  // Paddle height (pixels)
const float PADDLE_SPEED = 300.0f;  // How fast paddle moves (pixels/second)

// ============================================================
// BALL DIMENSIONS
// ============================================================
const float BALL_SIZE = 10.0f;      // Ball radius (pixels)
const float BALL_SPEED = 250.0f;    // How fast ball moves (pixels/second)

// ============================================================
// GAME STATE STRUCTS
// ============================================================
struct Paddle {
    float x, y;       // Position (top-left corner)
    float width, height; // Size
};

struct Ball {
    float x, y;       // Center position
    float size;       // Radius
    float vx, vy;     // Velocity (speed in x and y directions)
};

// ============================================================
// GLOBAL VARIABLES
// ============================================================
Paddle leftPaddle, rightPaddle;  // The two paddles
Ball ball;                       // The ball
int leftScore = 0, rightScore = 0; // Score tracking
float lastFrameTime = 0.0f;      // For delta time calculation
bool gameOver = false;           // Is the game finished?
int winner = 0;                  // 1 = left player (green), 2 = right player (red)

// ============================================================
// BUTTON DIMENSIONS (will be recalculated on resize)
// ============================================================
float BUTTON_WIDTH = 200.0f;     // Width of "TRY AGAIN" button
float BUTTON_HEIGHT = 60.0f;     // Height of button
float BUTTON_X = 0.0f;           // X position (calculated later)
float BUTTON_Y = 0.0f;           // Y position (calculated later)

// ============================================================
// 8x8 BITMAP FONT DATA
// Each character is an 8x8 pixel grid
// 1 = pixel on, 0 = pixel off
// ============================================================

// Digits 0-9
const int font_0[8] = { 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b01100110, 0b00111100 };
const int font_1[8] = { 0b00011000, 0b00111000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00111110 };
const int font_2[8] = { 0b00111100, 0b01100110, 0b00000110, 0b00001100, 0b00110000, 0b01100000, 0b11111110, 0b00000000 };
const int font_3[8] = { 0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_4[8] = { 0b00001100, 0b00011100, 0b00110110, 0b01100110, 0b11111110, 0b00000110, 0b00000110, 0b00000000 };
const int font_5[8] = { 0b11111110, 0b11000000, 0b11111100, 0b00000110, 0b00000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_6[8] = { 0b00011100, 0b00110000, 0b01100000, 0b11111100, 0b11000110, 0b11000110, 0b00111100, 0b00000000 };
const int font_7[8] = { 0b11111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b00110000, 0b00110000, 0b00000000 };
const int font_8[8] = { 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b00000000 };
const int font_9[8] = { 0b00111100, 0b01100110, 0b01100110, 0b00111110, 0b00000110, 0b00001100, 0b00111000, 0b00000000 };

// Letters A-Z
const int font_A[8] = { 0b00111100, 0b01100110, 0b01100110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000 };
const int font_B[8] = { 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b00000000 };
const int font_C[8] = { 0b00111100, 0b01100110, 0b11000000, 0b11000000, 0b11000000, 0b01100110, 0b00111100, 0b00000000 };
const int font_D[8] = { 0b11111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b11111100, 0b00000000 };
const int font_E[8] = { 0b11111110, 0b01100000, 0b01100000, 0b11111100, 0b01100000, 0b01100000, 0b11111110, 0b00000000 };
const int font_F[8] = { 0b11111110, 0b01100000, 0b01100000, 0b11111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000 };
const int font_G[8] = { 0b00111100, 0b01100110, 0b11000000, 0b11001110, 0b11000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_H[8] = { 0b01100110, 0b01100110, 0b01100110, 0b11111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000 };
const int font_I[8] = { 0b01111110, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b01111110, 0b00000000 };
const int font_J[8] = { 0b00000110, 0b00000110, 0b00000110, 0b00000110, 0b11000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_K[8] = { 0b01100110, 0b01100110, 0b01101100, 0b01111000, 0b01111000, 0b01101100, 0b01100110, 0b00000000 };
const int font_L[8] = { 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b11111110, 0b00000000 };
const int font_M[8] = { 0b01100110, 0b11011011, 0b11011011, 0b11001110, 0b11000110, 0b11000110, 0b01100110, 0b00000000 };
const int font_N[8] = { 0b01100110, 0b11000110, 0b11000110, 0b11001011, 0b11011011, 0b11101011, 0b01100110, 0b00000000 };
const int font_O[8] = { 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b11000011, 0b01100110, 0b00111100, 0b00000000 };
const int font_P[8] = { 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000 };
const int font_Q[8] = { 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b11010011, 0b01101110, 0b00111101, 0b00000000 };
const int font_R[8] = { 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b01101100, 0b01100110, 0b01100110, 0b00000000 };
const int font_S[8] = { 0b00111100, 0b01100110, 0b11000000, 0b00111100, 0b00000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_T[8] = { 0b11111110, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000 };
const int font_U[8] = { 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00000000 };
const int font_V[8] = { 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00000000 };
const int font_W[8] = { 0b01100110, 0b01100110, 0b01100110, 0b01101110, 0b01111011, 0b01111011, 0b00110011, 0b00000000 };
const int font_X[8] = { 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00111100, 0b01100110, 0b01100110, 0b00000000 };
const int font_Y[8] = { 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000 };
const int font_Z[8] = { 0b11111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b11111110, 0b00000000 };

// Special characters
const int font_space[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
const int font_excl[8] = { 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000, 0b00011000, 0b00000000 };

// ============================================================
// FONT LOOKUP TABLES
// ============================================================
const int* digitFonts[10] = {
    font_0, font_1, font_2, font_3, font_4, font_5, font_6, font_7, font_8, font_9
};

const int* letterFonts[26] = {
    font_A, font_B, font_C, font_D, font_E, font_F, font_G, font_H,
    font_I, font_J, font_K, font_L, font_M, font_N, font_O, font_P,
    font_Q, font_R, font_S, font_T, font_U, font_V, font_W, font_X,
    font_Y, font_Z
};

// ============================================================
// FUNCTION: getCharFont
// Returns the 8x8 bitmap data for a given character
// ============================================================
const int* getCharFont(char c) {
    if (c >= '0' && c <= '9') {
        return digitFonts[c - '0'];  // Return digit font
    }
    c = toupper(c);  // Convert to uppercase
    if (c >= 'A' && c <= 'Z') {
        return letterFonts[c - 'A'];  // Return letter font
    }
    if (c == ' ') return font_space;  // Space
    if (c == '!') return font_excl;   // Exclamation
    return font_space;  // Default to space
}

// ============================================================
// FUNCTION: drawRect
// Draws a filled rectangle at (x,y) with given width/height
// ============================================================
void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);  // Set color (RGB values 0.0-1.0)
    glBegin(GL_QUADS);   // Start drawing a quad (4 vertices)
    glVertex2f(x, y);                    // Top-left
    glVertex2f(x + width, y);            // Top-right
    glVertex2f(x + width, y + height);   // Bottom-right
    glVertex2f(x, y + height);           // Bottom-left
    glEnd();  // End drawing
}

// ============================================================
// FUNCTION: drawCircle
// Draws a filled circle at (cx,cy) with given radius
// Uses triangle fan - 36 triangles = smooth circle
// ============================================================
void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);  // Set color
    glBegin(GL_TRIANGLE_FAN);  // Start triangle fan
    glVertex2f(cx, cy);  // Center point
    for (int i = 0; i <= 36; i++) {
        // Calculate angle for each vertex (0 to 360 degrees)
        float angle = i * 2.0f * 3.14159f / 36.0f;
        // Calculate vertex position using trigonometry
        glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
    }
    glEnd();  // End drawing
}

// ============================================================
// FUNCTION: drawChar
// Draws a single character using 8x8 bitmap font
// Each pixel is drawn as a small rectangle
// ============================================================
void drawChar(char c, float x, float y, float size, float r, float g, float b) {
    const int* fontData = getCharFont(c);  // Get bitmap data
    float pixelSize = size / 8.0f;         // Size of each pixel

    glColor3f(r, g, b);  // Set color
    for (int row = 0; row < 8; row++) {    // For each row
        for (int col = 0; col < 8; col++) { // For each column
            // Check if this pixel should be drawn
            if (fontData[row] & (1 << (7 - col))) {
                // Draw the pixel as a small rectangle
                drawRect(x + col * pixelSize, y + row * pixelSize,
                    pixelSize, pixelSize, r, g, b);
            }
        }
    }
}

// ============================================================
// FUNCTION: drawString
// Draws a full string by drawing each character
// ============================================================
void drawString(const std::string& text, float x, float y, float size, float r, float g, float b) {
    float currentX = x;  // Current X position
    float charWidth = size;  // Width of each character
    for (char c : text) {  // Loop through each character
        drawChar(c, currentX, y, size, r, g, b);  // Draw the character
        currentX += charWidth + size * 0.15f;  // Move to next position (with spacing)
    }
}

// ============================================================
// FUNCTION: getStringWidth
// Calculates how wide a string will be when drawn
// ============================================================
float getStringWidth(const std::string& text, float size) {
    return text.length() * (size + size * 0.15f);  // Length * (char size + spacing)
}

// ============================================================
// FUNCTION: isMouseOverButton
// Checks if mouse position is inside the button
// ============================================================
bool isMouseOverButton(double mouseX, double mouseY) {
    return (mouseX >= BUTTON_X && mouseX <= BUTTON_X + BUTTON_WIDTH &&
        mouseY >= BUTTON_Y && mouseY <= BUTTON_Y + BUTTON_HEIGHT);
}

// ============================================================
// FUNCTION: windowSizeCallback
// Called automatically when window is resized
// ============================================================
void windowSizeCallback(GLFWwindow* window, int width, int height) {
    WINDOW_WIDTH = width;   // Update global width
    WINDOW_HEIGHT = height; // Update global height

    // Recalculate button position
    BUTTON_WIDTH = 200.0f;
    BUTTON_HEIGHT = 60.0f;
    BUTTON_X = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
    BUTTON_Y = WINDOW_HEIGHT / 2.0f + 50.0f;

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // Update viewport
    glMatrixMode(GL_PROJECTION);  // Switch to projection matrix
    glLoadIdentity();  // Reset matrix
    glOrtho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);  // Set 2D projection
    glMatrixMode(GL_MODELVIEW);  // Switch back to modelview
    glLoadIdentity();  // Reset
}

// ============================================================
// FUNCTION: resetBall
// Resets ball to center after a score
// ============================================================
void resetBall(bool leftScored) {
    ball.x = WINDOW_WIDTH / 2.0f;       // Center X
    ball.y = WINDOW_HEIGHT / 2.0f;      // Center Y
    // Send ball toward the player who scored
    ball.vx = leftScored ? BALL_SPEED : -BALL_SPEED;
    // Random vertical direction
    ball.vy = (rand() % 100 - 50) / 50.0f * BALL_SPEED * 0.5f;
}

// ============================================================
// FUNCTION: initGame
// Sets up initial game state
// ============================================================
void initGame() {
    // Left paddle position (30 pixels from left edge)
    leftPaddle.x = 30.0f;
    leftPaddle.y = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0f;
    leftPaddle.width = PADDLE_WIDTH;
    leftPaddle.height = PADDLE_HEIGHT;

    // Right paddle position (30 pixels from right edge)
    rightPaddle.x = WINDOW_WIDTH - 30.0f - PADDLE_WIDTH;
    rightPaddle.y = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0f;
    rightPaddle.width = PADDLE_WIDTH;
    rightPaddle.height = PADDLE_HEIGHT;

    // Ball at center
    ball.x = WINDOW_WIDTH / 2.0f;
    ball.y = WINDOW_HEIGHT / 2.0f;
    ball.size = BALL_SIZE;
    ball.vx = BALL_SPEED;  // Move right initially
    ball.vy = 0.0f;        // No vertical movement

    // Reset scores
    leftScore = 0;
    rightScore = 0;
    gameOver = false;
    winner = 0;

    // Recalculate button position
    BUTTON_WIDTH = 200.0f;
    BUTTON_HEIGHT = 60.0f;
    BUTTON_X = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
    BUTTON_Y = WINDOW_HEIGHT / 2.0f + 50.0f;
}

// ============================================================
// FUNCTION: updateGame
// Updates ball position and handles collisions
// ============================================================
void updateGame(float deltaTime) {
    if (gameOver) return;  // Don't update if game is over

    // Move ball based on velocity and time
    ball.x += ball.vx * deltaTime;
    ball.y += ball.vy * deltaTime;

    // Ball collision with top and bottom walls
    if (ball.y - ball.size <= 0.0f || ball.y + ball.size >= WINDOW_HEIGHT) {
        ball.vy = -ball.vy;  // Reverse vertical direction
        // Keep ball inside screen
        if (ball.y - ball.size < 0.0f) ball.y = ball.size;
        if (ball.y + ball.size > WINDOW_HEIGHT) ball.y = WINDOW_HEIGHT - ball.size;
    }

    // Ball collision with left paddle
    if (ball.x - ball.size <= leftPaddle.x + leftPaddle.width &&  // Ball touches paddle right side
        ball.x + ball.size >= leftPaddle.x &&                     // Ball not past paddle left
        ball.y >= leftPaddle.y && ball.y <= leftPaddle.y + leftPaddle.height) { // Y overlap
        ball.vx = -ball.vx;  // Reverse horizontal direction
        // Calculate where on paddle it hit (for angle)
        float hitPos = (ball.y - leftPaddle.y) / leftPaddle.height - 0.5f;
        ball.vy = hitPos * BALL_SPEED * 0.8f;  // Add angle based on hit position
        ball.x = leftPaddle.x + leftPaddle.width + ball.size;  // Push ball out of paddle
    }

    // Ball collision with right paddle (same logic as left)
    if (ball.x + ball.size >= rightPaddle.x &&
        ball.x - ball.size <= rightPaddle.x + rightPaddle.width &&
        ball.y >= rightPaddle.y && ball.y <= rightPaddle.y + rightPaddle.height) {
        ball.vx = -ball.vx;
        float hitPos = (ball.y - rightPaddle.y) / rightPaddle.height - 0.5f;
        ball.vy = hitPos * BALL_SPEED * 0.8f;
        ball.x = rightPaddle.x - ball.size;
    }

    // Ball goes past left edge - right player scores
    if (ball.x - ball.size < 0.0f) {
        rightScore++;
        if (rightScore >= 3) {  // Win condition
            gameOver = true;
            winner = 2;  // Right player (red) wins
        }
        else {
            resetBall(true);  // Reset ball, send toward left
        }
    }

    // Ball goes past right edge - left player scores
    if (ball.x + ball.size > WINDOW_WIDTH) {
        leftScore++;
        if (leftScore >= 3) {  // Win condition
            gameOver = true;
            winner = 1;  // Left player (green) wins
        }
        else {
            resetBall(false);  // Reset ball, send toward right
        }
    }
}

// ============================================================
// FUNCTION: handleInput
// Processes keyboard input
// ============================================================
void handleInput(GLFWwindow* window, float deltaTime) {
    if (!gameOver) {
        // Left paddle controls (W = up, S = down)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            leftPaddle.y -= PADDLE_SPEED * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            leftPaddle.y += PADDLE_SPEED * deltaTime;
        }

        // Right paddle controls (UP = up, DOWN = down)
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            rightPaddle.y -= PADDLE_SPEED * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            rightPaddle.y += PADDLE_SPEED * deltaTime;
        }

        // Keep paddles inside screen
        if (leftPaddle.y < 0.0f) leftPaddle.y = 0.0f;
        if (leftPaddle.y + leftPaddle.height > WINDOW_HEIGHT)
            leftPaddle.y = WINDOW_HEIGHT - leftPaddle.height;

        if (rightPaddle.y < 0.0f) rightPaddle.y = 0.0f;
        if (rightPaddle.y + rightPaddle.height > WINDOW_HEIGHT)
            rightPaddle.y = WINDOW_HEIGHT - rightPaddle.height;
    }

    // Restart game with R key (works anytime)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        initGame();
    }
}

// ============================================================
// FUNCTION: mouseButtonCallback
// Handles mouse click events
// ============================================================
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    // Only respond to left mouse button press when game is over
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && gameOver) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);  // Get mouse position

        // FIXED: GLFW gives Y from top, OpenGL draws from bottom
        // So we need to flip Y: (WINDOW_HEIGHT - mouseY)
        mouseY = WINDOW_HEIGHT - mouseY;

        // Check if click is on the button
        if (isMouseOverButton(mouseX, mouseY)) {
            initGame();  // Restart game
        }
    }
}

// ============================================================
// FUNCTION: drawGame
// Draws everything on screen
// ============================================================
void drawGame() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear screen (black)

    if (!gameOver) {
        // --- DRAW CENTER LINE ---
        glColor3f(0.5f, 0.5f, 0.5f);  // Gray color
        glBegin(GL_LINES);  // Draw lines
        for (int i = 0; i < WINDOW_HEIGHT; i += 40) {  // Dashed line
            glVertex2f(WINDOW_WIDTH / 2.0f, i);        // Start of dash
            glVertex2f(WINDOW_WIDTH / 2.0f, i + 20);   // End of dash
        }
        glEnd();

        // --- DRAW PADDLES ---
        // Left paddle: green
        drawRect(leftPaddle.x, leftPaddle.y, leftPaddle.width, leftPaddle.height, 0.0f, 1.0f, 0.0f);
        // Right paddle: red
        drawRect(rightPaddle.x, rightPaddle.y, rightPaddle.width, rightPaddle.height, 1.0f, 0.0f, 0.0f);

        // --- DRAW BALL ---
        drawCircle(ball.x, ball.y, ball.size, 1.0f, 1.0f, 1.0f);  // White ball

        // --- DRAW SCORES ---
        // Left score (green)
        drawChar('0' + leftScore, WINDOW_WIDTH / 2.0f - 80, 50, 60.0f, 0.0f, 1.0f, 0.0f);
        // Right score (red)
        drawChar('0' + rightScore, WINDOW_WIDTH / 2.0f + 40, 50, 60.0f, 1.0f, 0.0f, 0.0f);
    }
    else {
        // --- GAME OVER SCREEN ---

        // Semi-transparent black overlay
        glColor4f(0.0f, 0.0f, 0.0f, 0.85f);  // Black with 85% opacity
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(WINDOW_WIDTH, 0);
        glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glVertex2f(0, WINDOW_HEIGHT);
        glEnd();

        // --- WINNER MESSAGE ---
        std::string winMessage;
        float winColorR, winColorG, winColorB;

        if (winner == 1) {
            winMessage = "GREEN WIN!";
            winColorR = 0.0f; winColorG = 1.0f; winColorB = 0.0f;  // Green
        }
        else {
            winMessage = "RED WIN!";
            winColorR = 1.0f; winColorG = 0.0f; winColorB = 0.0f;  // Red
        }

        // Center the text
        float textWidth = getStringWidth(winMessage, 60.0f);
        float textX = (WINDOW_WIDTH - textWidth) / 2.0f;
        float textY = WINDOW_HEIGHT / 2.0f - 80;

        drawString(winMessage, textX, textY, 60.0f, winColorR, winColorG, winColorB);

        // --- TRY AGAIN BUTTON ---
        // Update button position (in case window was resized)
        BUTTON_WIDTH = 300.0f;
        BUTTON_HEIGHT = 60.0f;
        BUTTON_X = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
        BUTTON_Y = WINDOW_HEIGHT / 2.0f + 50.0f;

        // Draw button background (matching winner color)
        if (winner == 1) {
            drawRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 0.0f, 0.7f, 0.0f);  // Green
        }
        else {
            drawRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 0.7f, 0.0f, 0.0f);  // Red
        }

        // Button border (white)
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(BUTTON_X, BUTTON_Y);
        glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y);
        glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
        glVertex2f(BUTTON_X, BUTTON_Y + BUTTON_HEIGHT);
        glEnd();

        // Button text "TRY AGAIN" (white)
        std::string buttonText = "PRESS R";
        float btnTextWidth = getStringWidth(buttonText, 35.0f);
        float btnTextX = BUTTON_X + (BUTTON_WIDTH - btnTextWidth) / 2.0f;
        float btnTextY = BUTTON_Y + (BUTTON_HEIGHT - 35.0f) / 2.0f;

        drawString(buttonText, btnTextX, btnTextY, 35.0f, 1.0f, 1.0f, 1.0f);  // White text
    }
}

// ============================================================
// FUNCTION: main
// Entry point - sets up window and runs game loop
// ============================================================
int main() {
    // --- INITIALIZE GLFW ---
    if (!glfwInit()) {  // Start GLFW
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set window hint: window should be resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // --- CREATE WINDOW ---
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong", NULL, NULL);
    if (!window) {  // Check if window creation failed
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();  // Clean up GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);  // Make this window the current OpenGL context

    // --- INITIALIZE GLEW ---
    if (glewInit() != GLEW_OK) {  // Load OpenGL extensions
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // --- SET WINDOW RESIZE CALLBACK ---
    glfwSetWindowSizeCallback(window, windowSizeCallback);  // Called when window resizes

    // --- SET UP 2D PROJECTION ---
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // Set viewport
    glMatrixMode(GL_PROJECTION);  // Switch to projection matrix
    glLoadIdentity();  // Reset
    glOrtho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);  // 2D orthographic projection
    glMatrixMode(GL_MODELVIEW);  // Switch to modelview
    glLoadIdentity();  // Reset

    // --- ENABLE BLENDING (for transparency) ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // --- SET MOUSE CALLBACK ---
    glfwSetMouseButtonCallback(window, mouseButtonCallback);  // Handle mouse clicks

    // --- START THE GAME ---
    initGame();  // Set initial game state

    // --- GAME LOOP ---
    while (!glfwWindowShouldClose(window)) {  // Loop until window is closed

        // Calculate delta time (time between frames)
        float currentTime = glfwGetTime();  // Current time in seconds
        float deltaTime = currentTime - lastFrameTime;  // Time since last frame
        lastFrameTime = currentTime;  // Update for next frame

        // Clamp delta time to prevent jumps (if window was minimized)
        if (deltaTime > 0.02f) deltaTime = 0.02f;

        // --- UPDATE ---
        handleInput(window, deltaTime);  // Process keyboard input
        updateGame(deltaTime);           // Update game logic

        // --- DRAW ---
        drawGame();  // Render everything

        // --- SWAP BUFFERS ---
        glfwSwapBuffers(window);  // Display the rendered frame
        glfwPollEvents();         // Process events (keyboard, mouse, etc.)

        // --- EXIT ON ESC KEY ---
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);  // Signal to close window
        }
    }

    // --- CLEAN UP ---
    glfwTerminate();  // Shut down GLFW
    return 0;  // Exit program
=======
// ============================================================
// MACROS - These tell the compiler to use static linking
// ============================================================
#define GLEW_STATIC    // Use static version of GLEW (no DLL)
#define GLFW_STATIC    // Use static version of GLFW (no DLL)

// ============================================================
// HEADER FILES
// ============================================================
#include <GL/glew.h>    // GLEW: Manages OpenGL extensions
#include <GLFW/glfw3.h> // GLFW: Creates windows and handles input
#include <iostream>     // For printing error messages
#include <cmath>        // For sin(), cos() math functions
#include <string>       // For std::string text handling

// ============================================================
// WINDOW DIMENSIONS (initial size)
// ============================================================
int WINDOW_WIDTH = 800;   // Initial window width in pixels
int WINDOW_HEIGHT = 600;  // Initial window height in pixels

// ============================================================
// PADDLE DIMENSIONS
// ============================================================
const float PADDLE_WIDTH = 10.0f;   // Paddle thickness (pixels)
const float PADDLE_HEIGHT = 80.0f;  // Paddle height (pixels)
const float PADDLE_SPEED = 300.0f;  // How fast paddle moves (pixels/second)

// ============================================================
// BALL DIMENSIONS
// ============================================================
const float BALL_SIZE = 10.0f;      // Ball radius (pixels)
const float BALL_SPEED = 250.0f;    // How fast ball moves (pixels/second)

// ============================================================
// GAME STATE STRUCTS
// ============================================================
struct Paddle {
    float x, y;       // Position (top-left corner)
    float width, height; // Size
};

struct Ball {
    float x, y;       // Center position
    float size;       // Radius
    float vx, vy;     // Velocity (speed in x and y directions)
};

// ============================================================
// GLOBAL VARIABLES
// ============================================================
Paddle leftPaddle, rightPaddle;  // The two paddles
Ball ball;                       // The ball
int leftScore = 0, rightScore = 0; // Score tracking
float lastFrameTime = 0.0f;      // For delta time calculation
bool gameOver = false;           // Is the game finished?
int winner = 0;                  // 1 = left player (green), 2 = right player (red)

// ============================================================
// BUTTON DIMENSIONS (will be recalculated on resize)
// ============================================================
float BUTTON_WIDTH = 200.0f;     // Width of "TRY AGAIN" button
float BUTTON_HEIGHT = 60.0f;     // Height of button
float BUTTON_X = 0.0f;           // X position (calculated later)
float BUTTON_Y = 0.0f;           // Y position (calculated later)

// ============================================================
// 8x8 BITMAP FONT DATA
// Each character is an 8x8 pixel grid
// 1 = pixel on, 0 = pixel off
// ============================================================

// Digits 0-9
const int font_0[8] = { 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b11000011, 0b11000011, 0b01100110, 0b00111100 };
const int font_1[8] = { 0b00011000, 0b00111000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00111110 };
const int font_2[8] = { 0b00111100, 0b01100110, 0b00000110, 0b00001100, 0b00110000, 0b01100000, 0b11111110, 0b00000000 };
const int font_3[8] = { 0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_4[8] = { 0b00001100, 0b00011100, 0b00110110, 0b01100110, 0b11111110, 0b00000110, 0b00000110, 0b00000000 };
const int font_5[8] = { 0b11111110, 0b11000000, 0b11111100, 0b00000110, 0b00000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_6[8] = { 0b00011100, 0b00110000, 0b01100000, 0b11111100, 0b11000110, 0b11000110, 0b00111100, 0b00000000 };
const int font_7[8] = { 0b11111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b00110000, 0b00110000, 0b00000000 };
const int font_8[8] = { 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b00000000 };
const int font_9[8] = { 0b00111100, 0b01100110, 0b01100110, 0b00111110, 0b00000110, 0b00001100, 0b00111000, 0b00000000 };

// Letters A-Z
const int font_A[8] = { 0b00111100, 0b01100110, 0b01100110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000 };
const int font_B[8] = { 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b00000000 };
const int font_C[8] = { 0b00111100, 0b01100110, 0b11000000, 0b11000000, 0b11000000, 0b01100110, 0b00111100, 0b00000000 };
const int font_D[8] = { 0b11111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b11111100, 0b00000000 };
const int font_E[8] = { 0b11111110, 0b01100000, 0b01100000, 0b11111100, 0b01100000, 0b01100000, 0b11111110, 0b00000000 };
const int font_F[8] = { 0b11111110, 0b01100000, 0b01100000, 0b11111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000 };
const int font_G[8] = { 0b00111100, 0b01100110, 0b11000000, 0b11001110, 0b11000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_H[8] = { 0b01100110, 0b01100110, 0b01100110, 0b11111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000 };
const int font_I[8] = { 0b01111110, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b01111110, 0b00000000 };
const int font_J[8] = { 0b00000110, 0b00000110, 0b00000110, 0b00000110, 0b11000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_K[8] = { 0b01100110, 0b01100110, 0b01101100, 0b01111000, 0b01111000, 0b01101100, 0b01100110, 0b00000000 };
const int font_L[8] = { 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b11111110, 0b00000000 };
const int font_M[8] = { 0b01100110, 0b11011011, 0b11011011, 0b11001110, 0b11000110, 0b11000110, 0b01100110, 0b00000000 };
const int font_N[8] = { 0b01100110, 0b11000110, 0b11000110, 0b11001011, 0b11011011, 0b11101011, 0b01100110, 0b00000000 };
const int font_O[8] = { 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b11000011, 0b01100110, 0b00111100, 0b00000000 };
const int font_P[8] = { 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000 };
const int font_Q[8] = { 0b00111100, 0b01100110, 0b11000011, 0b11000011, 0b11010011, 0b01101110, 0b00111101, 0b00000000 };
const int font_R[8] = { 0b11111100, 0b01100110, 0b01100110, 0b11111100, 0b01101100, 0b01100110, 0b01100110, 0b00000000 };
const int font_S[8] = { 0b00111100, 0b01100110, 0b11000000, 0b00111100, 0b00000110, 0b01100110, 0b00111100, 0b00000000 };
const int font_T[8] = { 0b11111110, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000 };
const int font_U[8] = { 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00000000 };
const int font_V[8] = { 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00000000 };
const int font_W[8] = { 0b01100110, 0b01100110, 0b01100110, 0b01101110, 0b01111011, 0b01111011, 0b00110011, 0b00000000 };
const int font_X[8] = { 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00111100, 0b01100110, 0b01100110, 0b00000000 };
const int font_Y[8] = { 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000 };
const int font_Z[8] = { 0b11111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b11111110, 0b00000000 };

// Special characters
const int font_space[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
const int font_excl[8] = { 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000, 0b00011000, 0b00000000 };

// ============================================================
// FONT LOOKUP TABLES
// ============================================================
const int* digitFonts[10] = {
    font_0, font_1, font_2, font_3, font_4, font_5, font_6, font_7, font_8, font_9
};

const int* letterFonts[26] = {
    font_A, font_B, font_C, font_D, font_E, font_F, font_G, font_H,
    font_I, font_J, font_K, font_L, font_M, font_N, font_O, font_P,
    font_Q, font_R, font_S, font_T, font_U, font_V, font_W, font_X,
    font_Y, font_Z
};

// ============================================================
// FUNCTION: getCharFont
// Returns the 8x8 bitmap data for a given character
// ============================================================
const int* getCharFont(char c) {
    if (c >= '0' && c <= '9') {
        return digitFonts[c - '0'];  // Return digit font
    }
    c = toupper(c);  // Convert to uppercase
    if (c >= 'A' && c <= 'Z') {
        return letterFonts[c - 'A'];  // Return letter font
    }
    if (c == ' ') return font_space;  // Space
    if (c == '!') return font_excl;   // Exclamation
    return font_space;  // Default to space
}

// ============================================================
// FUNCTION: drawRect
// Draws a filled rectangle at (x,y) with given width/height
// ============================================================
void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);  // Set color (RGB values 0.0-1.0)
    glBegin(GL_QUADS);   // Start drawing a quad (4 vertices)
    glVertex2f(x, y);                    // Top-left
    glVertex2f(x + width, y);            // Top-right
    glVertex2f(x + width, y + height);   // Bottom-right
    glVertex2f(x, y + height);           // Bottom-left
    glEnd();  // End drawing
}

// ============================================================
// FUNCTION: drawCircle
// Draws a filled circle at (cx,cy) with given radius
// Uses triangle fan - 36 triangles = smooth circle
// ============================================================
void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);  // Set color
    glBegin(GL_TRIANGLE_FAN);  // Start triangle fan
    glVertex2f(cx, cy);  // Center point
    for (int i = 0; i <= 36; i++) {
        // Calculate angle for each vertex (0 to 360 degrees)
        float angle = i * 2.0f * 3.14159f / 36.0f;
        // Calculate vertex position using trigonometry
        glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
    }
    glEnd();  // End drawing
}

// ============================================================
// FUNCTION: drawChar
// Draws a single character using 8x8 bitmap font
// Each pixel is drawn as a small rectangle
// ============================================================
void drawChar(char c, float x, float y, float size, float r, float g, float b) {
    const int* fontData = getCharFont(c);  // Get bitmap data
    float pixelSize = size / 8.0f;         // Size of each pixel

    glColor3f(r, g, b);  // Set color
    for (int row = 0; row < 8; row++) {    // For each row
        for (int col = 0; col < 8; col++) { // For each column
            // Check if this pixel should be drawn
            if (fontData[row] & (1 << (7 - col))) {
                // Draw the pixel as a small rectangle
                drawRect(x + col * pixelSize, y + row * pixelSize,
                    pixelSize, pixelSize, r, g, b);
            }
        }
    }
}

// ============================================================
// FUNCTION: drawString
// Draws a full string by drawing each character
// ============================================================
void drawString(const std::string& text, float x, float y, float size, float r, float g, float b) {
    float currentX = x;  // Current X position
    float charWidth = size;  // Width of each character
    for (char c : text) {  // Loop through each character
        drawChar(c, currentX, y, size, r, g, b);  // Draw the character
        currentX += charWidth + size * 0.15f;  // Move to next position (with spacing)
    }
}

// ============================================================
// FUNCTION: getStringWidth
// Calculates how wide a string will be when drawn
// ============================================================
float getStringWidth(const std::string& text, float size) {
    return text.length() * (size + size * 0.15f);  // Length * (char size + spacing)
}

// ============================================================
// FUNCTION: isMouseOverButton
// Checks if mouse position is inside the button
// ============================================================
bool isMouseOverButton(double mouseX, double mouseY) {
    return (mouseX >= BUTTON_X && mouseX <= BUTTON_X + BUTTON_WIDTH &&
        mouseY >= BUTTON_Y && mouseY <= BUTTON_Y + BUTTON_HEIGHT);
}

// ============================================================
// FUNCTION: windowSizeCallback
// Called automatically when window is resized
// ============================================================
void windowSizeCallback(GLFWwindow* window, int width, int height) {
    WINDOW_WIDTH = width;   // Update global width
    WINDOW_HEIGHT = height; // Update global height

    // Recalculate button position
    BUTTON_WIDTH = 200.0f;
    BUTTON_HEIGHT = 60.0f;
    BUTTON_X = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
    BUTTON_Y = WINDOW_HEIGHT / 2.0f + 50.0f;

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // Update viewport
    glMatrixMode(GL_PROJECTION);  // Switch to projection matrix
    glLoadIdentity();  // Reset matrix
    glOrtho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);  // Set 2D projection
    glMatrixMode(GL_MODELVIEW);  // Switch back to modelview
    glLoadIdentity();  // Reset
}

// ============================================================
// FUNCTION: resetBall
// Resets ball to center after a score
// ============================================================
void resetBall(bool leftScored) {
    ball.x = WINDOW_WIDTH / 2.0f;       // Center X
    ball.y = WINDOW_HEIGHT / 2.0f;      // Center Y
    // Send ball toward the player who scored
    ball.vx = leftScored ? BALL_SPEED : -BALL_SPEED;
    // Random vertical direction
    ball.vy = (rand() % 100 - 50) / 50.0f * BALL_SPEED * 0.5f;
}

// ============================================================
// FUNCTION: initGame
// Sets up initial game state
// ============================================================
void initGame() {
    // Left paddle position (30 pixels from left edge)
    leftPaddle.x = 30.0f;
    leftPaddle.y = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0f;
    leftPaddle.width = PADDLE_WIDTH;
    leftPaddle.height = PADDLE_HEIGHT;

    // Right paddle position (30 pixels from right edge)
    rightPaddle.x = WINDOW_WIDTH - 30.0f - PADDLE_WIDTH;
    rightPaddle.y = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0f;
    rightPaddle.width = PADDLE_WIDTH;
    rightPaddle.height = PADDLE_HEIGHT;

    // Ball at center
    ball.x = WINDOW_WIDTH / 2.0f;
    ball.y = WINDOW_HEIGHT / 2.0f;
    ball.size = BALL_SIZE;
    ball.vx = BALL_SPEED;  // Move right initially
    ball.vy = 0.0f;        // No vertical movement

    // Reset scores
    leftScore = 0;
    rightScore = 0;
    gameOver = false;
    winner = 0;

    // Recalculate button position
    BUTTON_WIDTH = 200.0f;
    BUTTON_HEIGHT = 60.0f;
    BUTTON_X = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
    BUTTON_Y = WINDOW_HEIGHT / 2.0f + 50.0f;
}

// ============================================================
// FUNCTION: updateGame
// Updates ball position and handles collisions
// ============================================================
void updateGame(float deltaTime) {
    if (gameOver) return;  // Don't update if game is over

    // Move ball based on velocity and time
    ball.x += ball.vx * deltaTime;
    ball.y += ball.vy * deltaTime;

    // Ball collision with top and bottom walls
    if (ball.y - ball.size <= 0.0f || ball.y + ball.size >= WINDOW_HEIGHT) {
        ball.vy = -ball.vy;  // Reverse vertical direction
        // Keep ball inside screen
        if (ball.y - ball.size < 0.0f) ball.y = ball.size;
        if (ball.y + ball.size > WINDOW_HEIGHT) ball.y = WINDOW_HEIGHT - ball.size;
    }

    // Ball collision with left paddle
    if (ball.x - ball.size <= leftPaddle.x + leftPaddle.width &&  // Ball touches paddle right side
        ball.x + ball.size >= leftPaddle.x &&                     // Ball not past paddle left
        ball.y >= leftPaddle.y && ball.y <= leftPaddle.y + leftPaddle.height) { // Y overlap
        ball.vx = -ball.vx;  // Reverse horizontal direction
        // Calculate where on paddle it hit (for angle)
        float hitPos = (ball.y - leftPaddle.y) / leftPaddle.height - 0.5f;
        ball.vy = hitPos * BALL_SPEED * 0.8f;  // Add angle based on hit position
        ball.x = leftPaddle.x + leftPaddle.width + ball.size;  // Push ball out of paddle
    }

    // Ball collision with right paddle (same logic as left)
    if (ball.x + ball.size >= rightPaddle.x &&
        ball.x - ball.size <= rightPaddle.x + rightPaddle.width &&
        ball.y >= rightPaddle.y && ball.y <= rightPaddle.y + rightPaddle.height) {
        ball.vx = -ball.vx;
        float hitPos = (ball.y - rightPaddle.y) / rightPaddle.height - 0.5f;
        ball.vy = hitPos * BALL_SPEED * 0.8f;
        ball.x = rightPaddle.x - ball.size;
    }

    // Ball goes past left edge - right player scores
    if (ball.x - ball.size < 0.0f) {
        rightScore++;
        if (rightScore >= 3) {  // Win condition
            gameOver = true;
            winner = 2;  // Right player (red) wins
        }
        else {
            resetBall(true);  // Reset ball, send toward left
        }
    }

    // Ball goes past right edge - left player scores
    if (ball.x + ball.size > WINDOW_WIDTH) {
        leftScore++;
        if (leftScore >= 3) {  // Win condition
            gameOver = true;
            winner = 1;  // Left player (green) wins
        }
        else {
            resetBall(false);  // Reset ball, send toward right
        }
    }
}

// ============================================================
// FUNCTION: handleInput
// Processes keyboard input
// ============================================================
void handleInput(GLFWwindow* window, float deltaTime) {
    if (!gameOver) {
        // Left paddle controls (W = up, S = down)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            leftPaddle.y -= PADDLE_SPEED * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            leftPaddle.y += PADDLE_SPEED * deltaTime;
        }

        // Right paddle controls (UP = up, DOWN = down)
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            rightPaddle.y -= PADDLE_SPEED * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            rightPaddle.y += PADDLE_SPEED * deltaTime;
        }

        // Keep paddles inside screen
        if (leftPaddle.y < 0.0f) leftPaddle.y = 0.0f;
        if (leftPaddle.y + leftPaddle.height > WINDOW_HEIGHT)
            leftPaddle.y = WINDOW_HEIGHT - leftPaddle.height;

        if (rightPaddle.y < 0.0f) rightPaddle.y = 0.0f;
        if (rightPaddle.y + rightPaddle.height > WINDOW_HEIGHT)
            rightPaddle.y = WINDOW_HEIGHT - rightPaddle.height;
    }

    // Restart game with R key (works anytime)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        initGame();
    }
}

// ============================================================
// FUNCTION: mouseButtonCallback
// Handles mouse click events
// ============================================================
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    // Only respond to left mouse button press when game is over
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && gameOver) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);  // Get mouse position

        // FIXED: GLFW gives Y from top, OpenGL draws from bottom
        // So we need to flip Y: (WINDOW_HEIGHT - mouseY)
        mouseY = WINDOW_HEIGHT - mouseY;

        // Check if click is on the button
        if (isMouseOverButton(mouseX, mouseY)) {
            initGame();  // Restart game
        }
    }
}

// ============================================================
// FUNCTION: drawGame
// Draws everything on screen
// ============================================================
void drawGame() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear screen (black)

    if (!gameOver) {
        // --- DRAW CENTER LINE ---
        glColor3f(0.5f, 0.5f, 0.5f);  // Gray color
        glBegin(GL_LINES);  // Draw lines
        for (int i = 0; i < WINDOW_HEIGHT; i += 40) {  // Dashed line
            glVertex2f(WINDOW_WIDTH / 2.0f, i);        // Start of dash
            glVertex2f(WINDOW_WIDTH / 2.0f, i + 20);   // End of dash
        }
        glEnd();

        // --- DRAW PADDLES ---
        // Left paddle: green
        drawRect(leftPaddle.x, leftPaddle.y, leftPaddle.width, leftPaddle.height, 0.0f, 1.0f, 0.0f);
        // Right paddle: red
        drawRect(rightPaddle.x, rightPaddle.y, rightPaddle.width, rightPaddle.height, 1.0f, 0.0f, 0.0f);

        // --- DRAW BALL ---
        drawCircle(ball.x, ball.y, ball.size, 1.0f, 1.0f, 1.0f);  // White ball

        // --- DRAW SCORES ---
        // Left score (green)
        drawChar('0' + leftScore, WINDOW_WIDTH / 2.0f - 80, 50, 60.0f, 0.0f, 1.0f, 0.0f);
        // Right score (red)
        drawChar('0' + rightScore, WINDOW_WIDTH / 2.0f + 40, 50, 60.0f, 1.0f, 0.0f, 0.0f);
    }
    else {
        // --- GAME OVER SCREEN ---

        // Semi-transparent black overlay
        glColor4f(0.0f, 0.0f, 0.0f, 0.85f);  // Black with 85% opacity
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(WINDOW_WIDTH, 0);
        glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glVertex2f(0, WINDOW_HEIGHT);
        glEnd();

        // --- WINNER MESSAGE ---
        std::string winMessage;
        float winColorR, winColorG, winColorB;

        if (winner == 1) {
            winMessage = "GREEN WIN!";
            winColorR = 0.0f; winColorG = 1.0f; winColorB = 0.0f;  // Green
        }
        else {
            winMessage = "RED WIN!";
            winColorR = 1.0f; winColorG = 0.0f; winColorB = 0.0f;  // Red
        }

        // Center the text
        float textWidth = getStringWidth(winMessage, 60.0f);
        float textX = (WINDOW_WIDTH - textWidth) / 2.0f;
        float textY = WINDOW_HEIGHT / 2.0f - 80;

        drawString(winMessage, textX, textY, 60.0f, winColorR, winColorG, winColorB);

        // --- TRY AGAIN BUTTON ---
        // Update button position (in case window was resized)
        BUTTON_WIDTH = 300.0f;
        BUTTON_HEIGHT = 60.0f;
        BUTTON_X = (WINDOW_WIDTH - BUTTON_WIDTH) / 2.0f;
        BUTTON_Y = WINDOW_HEIGHT / 2.0f + 50.0f;

        // Draw button background (matching winner color)
        if (winner == 1) {
            drawRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 0.0f, 0.7f, 0.0f);  // Green
        }
        else {
            drawRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 0.7f, 0.0f, 0.0f);  // Red
        }

        // Button border (white)
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(BUTTON_X, BUTTON_Y);
        glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y);
        glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
        glVertex2f(BUTTON_X, BUTTON_Y + BUTTON_HEIGHT);
        glEnd();

        // Button text "TRY AGAIN" (white)
        std::string buttonText = "PRESS R";
        float btnTextWidth = getStringWidth(buttonText, 35.0f);
        float btnTextX = BUTTON_X + (BUTTON_WIDTH - btnTextWidth) / 2.0f;
        float btnTextY = BUTTON_Y + (BUTTON_HEIGHT - 35.0f) / 2.0f;

        drawString(buttonText, btnTextX, btnTextY, 35.0f, 1.0f, 1.0f, 1.0f);  // White text
    }
}

// ============================================================
// FUNCTION: main
// Entry point - sets up window and runs game loop
// ============================================================
int main() {
    // --- INITIALIZE GLFW ---
    if (!glfwInit()) {  // Start GLFW
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set window hint: window should be resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // --- CREATE WINDOW ---
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong", NULL, NULL);
    if (!window) {  // Check if window creation failed
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();  // Clean up GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);  // Make this window the current OpenGL context

    // --- INITIALIZE GLEW ---
    if (glewInit() != GLEW_OK) {  // Load OpenGL extensions
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // --- SET WINDOW RESIZE CALLBACK ---
    glfwSetWindowSizeCallback(window, windowSizeCallback);  // Called when window resizes

    // --- SET UP 2D PROJECTION ---
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // Set viewport
    glMatrixMode(GL_PROJECTION);  // Switch to projection matrix
    glLoadIdentity();  // Reset
    glOrtho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);  // 2D orthographic projection
    glMatrixMode(GL_MODELVIEW);  // Switch to modelview
    glLoadIdentity();  // Reset

    // --- ENABLE BLENDING (for transparency) ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // --- SET MOUSE CALLBACK ---
    glfwSetMouseButtonCallback(window, mouseButtonCallback);  // Handle mouse clicks

    // --- START THE GAME ---
    initGame();  // Set initial game state

    // --- GAME LOOP ---
    while (!glfwWindowShouldClose(window)) {  // Loop until window is closed

        // Calculate delta time (time between frames)
        float currentTime = glfwGetTime();  // Current time in seconds
        float deltaTime = currentTime - lastFrameTime;  // Time since last frame
        lastFrameTime = currentTime;  // Update for next frame

        // Clamp delta time to prevent jumps (if window was minimized)
        if (deltaTime > 0.02f) deltaTime = 0.02f;

        // --- UPDATE ---
        handleInput(window, deltaTime);  // Process keyboard input
        updateGame(deltaTime);           // Update game logic

        // --- DRAW ---
        drawGame();  // Render everything

        // --- SWAP BUFFERS ---
        glfwSwapBuffers(window);  // Display the rendered frame
        glfwPollEvents();         // Process events (keyboard, mouse, etc.)

        // --- EXIT ON ESC KEY ---
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);  // Signal to close window
        }
    }

    // --- CLEAN UP ---
    glfwTerminate();  // Shut down GLFW
    return 0;  // Exit program
>>>>>>> ad4935b779044d0f5bcc9e2ec5db539e02ae9c62
}
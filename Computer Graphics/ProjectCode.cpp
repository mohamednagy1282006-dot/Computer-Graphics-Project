#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

// Global transformation variables
float tx = 200.0f;  // Translation X
float ty = -20.0f;  // Translation Y
float angle = 0.0f;  // Rotation
float sx = 1.0f;  // Scale X
float sy = 1.0f;  // Scale Y
float shearX = 0.0f;
float shearY = 0.0f;
int   flipped = 0;
int   showHelp = 1;

#define PI 3.14159265f

// Utility functions
void drawText(float x, float y, string str) {
    glRasterPos2f(x, y);
    for (char ch : str) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ch);
    }
}

// Drawing basic shapes
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * PI * i / segments;
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

// Main ball object
void drawColorfulBall(float cx, float cy, float r) {
    float colors[6][3] = {
        {1.0f, 0.20f, 0.20f},
        {1.0f, 0.65f, 0.00f},
        {0.20f, 0.80f, 0.20f},
        {0.10f, 0.55f, 1.00f},
        {0.90f, 0.20f, 0.90f},
        {1.0f, 0.95f, 0.10f},
    };

    float segAngle = 2.0f * PI / 6;

    for (int s = 0; s < 6; s++) {
        glColor3fv(colors[s]);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        float aStart = s * segAngle - PI / 2.0f;
        int steps = 30;
        for (int i = 0; i <= steps; i++) {
            float a = aStart + segAngle * i / steps;
            glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
        }
        glEnd();
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 60; i++) {
        float a = 2.0f * PI * i / 60;
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();

    glLineWidth(2.0f);
    for (int s = 0; s < 6; s++) {
        float a = s * segAngle - PI / 2.0f;
        glBegin(GL_LINES);
        glVertex2f(cx, cy);
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
        glEnd();
    }
}

// Child figure
void drawChild(float ox, float oy, int mirrorX) {
    glPushMatrix();
    glTranslatef(ox, oy, 0);
    glScalef((float)mirrorX, 1.0f, 1.0f);

    // Body and Neck
    glColor3f(0.30f, 0.55f, 0.95f);
    drawRect(-18, -20, 36, 55);
    glColor3f(1.0f, 0.85f, 0.70f);
    drawRect(-8, 30, 16, 12);

    // Head
    drawCircle(0, 65, 22, 40);

    // Eyes
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(-8, 70, 5, 20);
    drawCircle(8, 70, 5, 20);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(-6, 70, 2.5, 20);
    drawCircle(10, 70, 2.5, 20);

    // Eyebrows
    glColor3f(0.25f, 0.15f, 0.10f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i += 10) {
        float a = i * PI / 180.0f;
        glVertex2f(-8 + 6 * cosf(a), 74 + 3 * sinf(a));
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i += 10) {
        float a = i * PI / 180.0f;
        glVertex2f(8 + 6 * cosf(a), 74 + 3 * sinf(a));
    }
    glEnd();

    // Smile
    glColor3f(0.50f, 0.20f, 0.10f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = -40; i <= 40; i += 3) {
        float a = i * PI / 180.0f;
        glVertex2f(12.0f * sinf(a), 60.0f - 8.0f * cosf(a));
    }
    glEnd();

    // Cheeks & Nose
    glColor4f(1.0f, 0.50f, 0.50f, 0.6f);
    drawCircle(-14, 63, 5, 20);
    drawCircle(14, 63, 5, 20);
    glColor3f(0.80f, 0.60f, 0.50f);
    drawCircle(0, 65, 2, 10);

    // Arms
    glColor3f(1.0f, 0.80f, 0.65f);
    drawRect(18, 15, 30, 10);
    drawRect(-30, 15, 15, 10);

    // Legs and Shoes
    glColor3f(0.20f, 0.40f, 0.70f);
    drawRect(-15, -70, 12, 50);
    drawRect(3, -70, 12, 50);
    glColor3f(0.15f, 0.15f, 0.15f);
    drawCircle(-9, -75, 8, 20);
    drawCircle(9, -75, 8, 20);

    glPopMatrix();
}

// Scene environment
void drawBackground() {
    // Sky
    glBegin(GL_QUADS);
    glColor3f(0.53f, 0.81f, 0.98f);
    glVertex2f(-400, 400);
    glVertex2f(400, 400);
    glColor3f(0.87f, 0.95f, 1.00f);
    glVertex2f(400, 0);
    glVertex2f(-400, 0);
    glEnd();

    // Ground
    glColor3f(0.45f, 0.70f, 0.35f);
    drawRect(-400, -300, 800, 300);

    // Clouds
    glColor4f(1.0f, 1.0f, 1.0f, 0.85f);
    drawCircle(-250, 300, 30, 30);
    drawCircle(-220, 310, 35, 30);
    drawCircle(-190, 300, 28, 30);
    drawCircle(150, 280, 25, 30);
    drawCircle(180, 290, 30, 30);
    drawCircle(210, 280, 25, 30);
}

// Apply transformations and draw the ball
void drawTransformableBall() {
    glPushMatrix();

    glTranslatef(tx, ty + 30.0f, 0.0f);
    glRotatef(angle, 0, 0, 1);
    glScalef(sx, sy, 1.0f);

    if (flipped) glScalef(-1.0f, 1.0f, 1.0f);

    float shearMatrix[16] = {
        1.0f,   shearY, 0.0f, 0.0f,
        shearX, 1.0f,   0.0f, 0.0f,
        0.0f,   0.0f,   1.0f, 0.0f,
        0.0f,   0.0f,   0.0f, 1.0f
    };
    glMultMatrixf(shearMatrix);

    drawColorfulBall(0, 0, 40.0f);

    glPopMatrix();
}

// UI Panels
void drawInfo() {
    char buf[128];

    glColor4f(0.0f, 0.0f, 0.0f, 0.55f);
    drawRect(-395, -240, 210, 130);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-390, -120, "=== Transform State ===");
    snprintf(buf, sizeof(buf), "Translate : (%.0f, %.0f)", tx, ty);
    drawText(-390, -137, buf);
    snprintf(buf, sizeof(buf), "Angle     : %.0f deg", angle);
    drawText(-390, -154, buf);
    snprintf(buf, sizeof(buf), "Scale     : %.2f x %.2f", sx, sy);
    drawText(-390, -171, buf);
    snprintf(buf, sizeof(buf), "Shear X,Y : %.2f, %.2f", shearX, shearY);
    drawText(-390, -188, buf);
    snprintf(buf, sizeof(buf), "Flipped   : %s", flipped ? "Yes" : "No");
    drawText(-390, -205, buf);
}

void drawHelp() {
    if (!showHelp) return;

    glColor4f(0.0f, 0.0f, 0.0f, 0.55f);
    drawRect(130, -254, 265, 144);

    glColor3f(0.90f, 0.90f, 1.00f);
    drawText(140, -120, "=========  HELP  =========");
    drawText(140, -134, "Arrow Keys  : Translate");
    drawText(140, -148, "r / e       : Rotate");
    drawText(140, -162, "+ / -       : Scale up / down");
    drawText(140, -176, "f           : Flip (Reflection)");
    drawText(140, -190, "h / j       : Shear X (+/-)");
    drawText(140, -204, "u / i       : Shear Y (+/-)");
    drawText(140, -218, "0           : RESET all");
    drawText(140, -232, "?           : Hide/Show Help");
    drawText(140, -246, "ESC         : Exit");
}

// OpenGL Callbacks
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawBackground();
    drawChild(-260, -10, 1);
    drawChild(260, -10, -1);
    drawTransformableBall();
    drawInfo();
    drawHelp();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r': angle -= 10.0f; if (angle <= -360.0f) angle += 360.0f; break;
    case 'e': angle += 10.0f; if (angle >= 360.0f) angle -= 360.0f; break;
    case '+': if ((sx + 0.1f) * 40.0f < 242.0f - fabsf(tx)) { sx += 0.1f; sy += 0.1f; } break;
    case '-': if (sx > 0.15f) { sx -= 0.1f; sy -= 0.1f; } break;
    case 'f': flipped = !flipped;  break;
    case 'h': shearX += 0.1f; if (shearX > 6.0f) shearX = 6.0f; break;
    case 'j': shearX -= 0.1f; if (shearX < -6.0f) shearX = -6.0f; break;
    case 'u': shearY += 0.1f; if (shearY > 6.5f) shearY = 6.5f; break;
    case 'i': shearY -= 0.1f; if (shearY < -6.5f) shearY = -6.5f; break;
    case '0':
        tx = 200.0f; ty = -20.0f; angle = 0; sx = 1; sy = 1;
        shearX = 0; shearY = 0; flipped = 0;
        break;
    case '?': showHelp = !showHelp; break;
    case 27:  exit(0); break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    float ballRadius = 40.0f * sx;
    float rightLimit = 242.0f - ballRadius;
    float leftLimit = -242.0f + ballRadius;

    float topLimit = 270.0f - ballRadius;
    float bottomLimit = -330.0f + ballRadius;

    switch (key) {
    case GLUT_KEY_LEFT:
        if (tx - 10.0f > leftLimit) tx -= 10.0f;
        break;
    case GLUT_KEY_RIGHT:
        if (tx + 10.0f < rightLimit) tx += 10.0f;
        break;
    case GLUT_KEY_UP:
        if (ty + 10.0f < topLimit) ty += 10.0f;
        break;
    case GLUT_KEY_DOWN:
        if (ty - 10.0f > bottomLimit) ty -= 10.0f;
        break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / h;
    if (w >= h)
        glOrtho(-400 * aspect, 400 * aspect, -300, 300, -1, 1);
    else
        glOrtho(-400, 400, -300 / aspect, 300 / aspect, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 80);
    glutCreateWindow("Sharing is Caring");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.67f, 0.85f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}

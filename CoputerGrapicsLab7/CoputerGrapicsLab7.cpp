#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

struct Point3D {
    float x, y, z;
    Point3D(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Particle {
public:
    float x, y, z;
    float velocityY;
    float life;

    Particle(float startX, float startY, float startZ) {
        x = startX;
        y = startY;
        z = startZ;
        velocityY = ((rand() % 50) / 100.0f) + 0.02f;
        life = 1.0f;
    }

    void update() {
        y += velocityY;
        life -= 0.02f;
        if (life <= 0) {
            reset();
        }
    }

    void reset() {
        y = 0.6f;
        x = (rand() % 10 - 5) / 50.0f;
        z = (rand() % 10 - 5) / 50.0f;
        velocityY = ((rand() % 50) / 100.0f) + 0.02f;
        life = 1.0f;
    }
};

vector<Particle> particles;

void initParticles(int numParticles) {
    for (int i = 0; i < numParticles; i++) {
        particles.push_back(Particle(0.0f, 0.6f, 0.0f));
    }
}

void drawParticles() {
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (auto& p : particles) {
        glColor4f(0.6f, 0.8f, 1.0f, p.life);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

void drawWhale() {
    glColor3f(0.0f, 0.0f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, -0.3f, 0.0f);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2.0f);

    drawWhale();
    drawParticles();

    for (auto& p : particles) {
        p.update();
    }

    glutSwapBuffers();
}

void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Whale with Fountain");

    init();
    initParticles(100);

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}

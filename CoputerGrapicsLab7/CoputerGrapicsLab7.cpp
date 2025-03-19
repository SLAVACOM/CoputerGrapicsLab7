#include <GL/glut.h>
#include <vector>
#include <cmath>

using namespace std;

struct Point3D {
    float x, y, z;
    Point3D(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Parallelepiped {
public:
    vector<Point3D> vertices;
    Parallelepiped(float width, float height, float depth) {
        float w = width / 2, h = height / 2, d = depth / 2;
        vertices = {
            { -w, -h, -d }, { w, -h, -d }, { w, h, -d }, { -w, h, -d },
            { -w, -h, d },  { w, -h, d },  { w, h, d },  { -w, h, d }
        };
    }
};

class Oval {
public:
    vector<Point3D> vertices;
    Oval(float a, float b, int segments) {
        for (int i = 0; i < segments; i++) {
            float theta = 2.0f * M_PI * float(i) / float(segments);
            float x = a * cos(theta);
            float y = b * sin(theta);
            vertices.push_back(Point3D(x, y, 0));
        }
    }
};

float tweenFactor = 0.0f;
Parallelepiped startBox(1, 1, 1);
Parallelepiped endBox(2, 2, 2);
Oval startOval(1, 0.5, 50);
Oval endOval(2, 1, 50);

void tween(Parallelepiped& p1, Parallelepiped& p2, float t, Parallelepiped& result) {
    for (size_t i = 0; i < p1.vertices.size(); i++) {
        result.vertices[i].x = p1.vertices[i].x + (p2.vertices[i].x - p1.vertices[i].x) * t;
        result.vertices[i].y = p1.vertices[i].y + (p2.vertices[i].y - p1.vertices[i].y) * t;
        result.vertices[i].z = p1.vertices[i].z + (p2.vertices[i].z - p1.vertices[i].z) * t;
    }
}

void tween(Oval& o1, Oval& o2, float t, Oval& result) {
    for (size_t i = 0; i < o1.vertices.size(); i++) {
        result.vertices[i].x = o1.vertices[i].x + (o2.vertices[i].x - o1.vertices[i].x) * t;
        result.vertices[i].y = o1.vertices[i].y + (o2.vertices[i].y - o1.vertices[i].y) * t;
        result.vertices[i].z = o1.vertices[i].z + (o2.vertices[i].z - o1.vertices[i].z) * t;
    }
}

void drawParallelepiped(Parallelepiped& p) {
    int faces[6][4] = {
        { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 0, 1, 5, 4 },
        { 2, 3, 7, 6 }, { 0, 3, 7, 4 }, { 1, 2, 6, 5 }
    };
    glBegin(GL_QUADS);
    for (auto& face : faces) {
        for (int i = 0; i < 4; i++) {
            glVertex3f(p.vertices[face[i]].x, p.vertices[face[i]].y, p.vertices[face[i]].z);
        }
    }
    glEnd();
}

void drawOval(Oval& o) {
    glBegin(GL_POLYGON);
    for (auto& v : o.vertices) {
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);

    Parallelepiped currentBox = startBox;
    tween(startBox, endBox, tweenFactor, currentBox);
    drawParallelepiped(currentBox);

    Oval currentOval = startOval;
    tween(startOval, endOval, tweenFactor, currentOval);
    drawOval(currentOval);

    tweenFactor += 0.01f;
    if (tweenFactor > 1.0f) tweenFactor = 0.0f;

    glutSwapBuffers();
}

void init() {
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tweening Shapes");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

struct Point {
    float x, y;
};

void drawPoint(Point p, float r, float g, float b) {
    glColor3f(r, g, b);
    glPointSize(8);
    glBegin(GL_POINTS);
        glVertex2f(p.x, p.y);
    glEnd();
}

void drawLine(Point a, Point b, float r, float g, float b_) {
    glColor3f(r, g, b_);
    glBegin(GL_LINES);
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
    glEnd();
}

void drawControlLines(const std::vector<Point>& points) {
    for (size_t i = 0; i < points.size() - 1; i++) {
        drawLine(points[i], points[i+1], 0.5f, 0.5f, 0.5f);
    }
}

Point calculateMidpoint(const std::vector<Point>& points) {
    if (points.size() == 1) return points[0];

    std::vector<Point> newPoints;
    for (size_t i = 0; i < points.size() - 1; i++) {
        Point mid;
        mid.x = (points[i].x + points[i + 1].x) / 2;
        mid.y = (points[i].y + points[i + 1].y) / 2;
        newPoints.push_back(mid);
    }
    return calculateMidpoint(newPoints);
}

void subdivideBezier(const std::vector<Point>& controlPoints, std::vector<Point>& left, std::vector<Point>& right) {
    if (controlPoints.size() == 1) {
        left.push_back(controlPoints[0]);
        right.push_back(controlPoints[0]);
        return;
    }

    left.push_back(controlPoints.front());
    right.push_back(controlPoints.back());

    std::vector<Point> nextLevel;
    for (size_t i = 0; i < controlPoints.size() - 1; i++) {
        Point mid;
        mid.x = (controlPoints[i].x + controlPoints[i + 1].x) / 2;
        mid.y = (controlPoints[i].y + controlPoints[i + 1].y) / 2;
        nextLevel.push_back(mid);
    }

    subdivideBezier(nextLevel, left, right);
}

void drawBezierRecursive(const std::vector<Point>& points, float threshold) {
    static const float colors[6][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 0},
        {1, 0, 1},
        {0, 1, 1} 
    };

    for (size_t i = 0; i < points.size(); i++) {
        drawPoint(points[i], colors[i][0], colors[i][1], colors[i][2]);
    }

    drawControlLines(points);

    glFlush();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    float dx = points.back().x - points.front().x;
    float dy = points.back().y - points.front().y;

    float maxDist = 0;
    for (size_t i = 1; i < points.size() - 1; i++) {
        float d = std::abs((points[i].x - points.front().x) * dy - (points[i].y - points.front().y) * dx);
        if (d > maxDist) maxDist = d;
    }

    float dist = maxDist / std::sqrt(dx * dx + dy * dy);

    if (dist < threshold) {
        drawLine(points.front(), points.back(), 1, 0, 1);
        glFlush();
        return;
    }

    std::vector<Point> left, right;
    subdivideBezier(points, left, right);

    drawBezierRecursive(left, threshold);
    drawBezierRecursive(right, threshold);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<Point> controlPoints = {
        {-0.9f, -0.9f},
        {0.0f,  0.0f},
        {0.9f,  0.9f},
        {0.0f,  0.9f},
        {-0.9f,  0.9f},
        {0.9f,  -0.9f},
        
    };

    drawBezierRecursive(controlPoints, 0.001f);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Curva Bézier - Recursão com Subdivisão De Casteljau");
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

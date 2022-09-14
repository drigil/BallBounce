#include <iostream>

#include <vector>

#include "simulationVariables.h"
#include "Ball.h"
#include "BoundingBox.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <chrono>
#include <thread>

BoundingBox boundingBox; //Bounding Box in the scene

// Time related variables
float totalTime = 0.0f; // Simulation elapsed time
std::chrono::steady_clock::time_point startTime, endTime;
float elapsedTime; // Display elapsed time

// Ball related variables
int numBalls = 1;
std::vector<Ball> ballArray; //Vector holding all the current balls in the scene

void constructBoundingBox() {

    // Construct a bounding box
    std::vector<Face> faces;

    // Face 1 - Bottom
    Vertex v0(-10, -10, -10);
    Vertex v1(10, -10, -10);
    Vertex v2(10, -10, 10);
    Vertex v3(-10, -10, 10);
    std::vector<Vertex> vertices_f0{ v0, v1, v2, v3 };

    Face f0(vertices_f0);

    faces.push_back(f0);

    // Face 2 - left
    Vertex v4(-10, -10, -10);
    Vertex v5(-10, 10, -10);
    Vertex v6(-10, 10, 10);
    Vertex v7(-10, -10, 10);
    std::vector<Vertex> vertices_f1{ v4, v5, v6, v7 };

    Face f1(vertices_f1);

    faces.push_back(f1);

    // Face 3 - right
    Vertex v8(10, 10, -10);
    Vertex v9(10, -10, -10);
    Vertex v10(10, -10, 10);
    Vertex v11(10, 10, 10);
    std::vector<Vertex> vertices_f2{ v8, v9, v10, v11 };

    Face f2(vertices_f2);

    faces.push_back(f2);

    // Face 4 - top
    Vertex v12(-10, 10, -10);
    Vertex v13(10, 10, -10);
    Vertex v14(10, 10, 10);
    Vertex v15(-10, 10, 10);
    std::vector<Vertex> vertices_f3{ v12, v13, v14, v15 };

    Face f3(vertices_f3);

    faces.push_back(f3);

    // Face 5 - front
    Vertex v16(-10, 10, 10);
    Vertex v17(10, 10, 10);
    Vertex v18(10, -10, 10);
    Vertex v19(-10, -10, 10);
    std::vector<Vertex> vertices_f4{ v16, v17, v18, v19 };

    Face f4(vertices_f4);

    faces.push_back(f4);

    // Face 6 - back
    Vertex v20(-10, 10, -10);
    Vertex v21(-10, -10, -10);
    Vertex v22(10, -10, -10);
    Vertex v23(10, 10, -10);
    std::vector<Vertex> vertices_f5{ v20, v21, v22, v23 };

    Face f5(vertices_f5);

    faces.push_back(f5);

    boundingBox.setFaces(faces);
}

float getNoise() {
    float noise = rand() % 200 + 1;
    noise = noise - 100;
    noise = noise / 10.0f;

    return noise;
}

void constructBall() {

    // Spawn a ball
    // Set its position, velocity, radius and mass
    float posX = 0;
    float posY = 0;
    float posZ = 0;

    float velX = 6;
    float velY = 12;
    float velZ = 5;
        
    float radius = 1.0f;
    float mass = 20;

    if (numBalls == 1) {
        Ball ball;
        ball.setParams(0, posX, posY, posZ, velX, velY, velZ, radius, mass, boundingBox, &ballArray);
        ballArray.push_back(ball);
    }

    else if (numBalls == 2) {
        Ball ball;
        velX = 5;
        velY = 6;
        velZ = 0;

        ball.setParams(0, posX, posY, posZ, velX, velY, velZ, radius, mass, boundingBox, &ballArray);
        ballArray.push_back(ball);

        Ball ball2;
        ball2.setParams(1, posX + 5, posY, posZ, -velX, velY, velZ, radius, mass, boundingBox, &ballArray);
        ballArray.push_back(ball2);
    }

    else {
        Ball ball;
        ball.setParams(0, posX, posY, posZ, velX, velY, velZ, radius, mass, boundingBox, &ballArray);
        ballArray.push_back(ball);

        for (int i = 1; i < numBalls; i++) {
            Ball ball_i;
            ball_i.setParams(i, posX + getNoise(), posY + getNoise(), posZ + getNoise(), velX + getNoise(), velY + getNoise(), velZ + getNoise(), radius, mass, boundingBox, &ballArray);
            ballArray.push_back(ball_i);
        }
    }

}

void display()
{   

    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the bounding box faces
    glPushMatrix();

    //GLfloat colorWalls[] = { 0.72f, .7f, .7f, 0.6f };
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorWalls);

    //for (int faceIndex = 0; faceIndex < boundingBox.faces.size(); faceIndex++) {

    //    glBegin(GL_POLYGON);

    //    for (int edgeIndex = 0; edgeIndex < boundingBox.faces[0].faceVertices.size(); edgeIndex++) {
    //        Vertex vertex = boundingBox.faces[faceIndex].faceVertices[edgeIndex];
    //        Vertex normal = boundingBox.faces[faceIndex].getNormal();
    //        Vertex normalPoint = boundingBox.faces[faceIndex].faceVertices[0];
    //        Vertex origin(0, 0, 0);
    //        Vertex originVector = origin - normalPoint;

    //        if (normal.dot(originVector) > 0) {
    //            normal = normal * (-1);
    //        }
    //        glNormal3f(normal.x, normal.y, normal.z);
    //        glVertex3f(vertex.x, vertex.y, vertex.z);
    //    
    //    }

    //    glEnd();
    //    
    //}
    //glPopMatrix();
    //
    // Draw box borders
    GLfloat colorWallsBorder[] = { 0.f, 0.0f, 0.0f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorWallsBorder);

    glPushMatrix();
    for (int faceIndex = 0; faceIndex < boundingBox.faces.size(); faceIndex++) {

        glBegin(GL_LINE_LOOP);
        for (int edgeIndex = 0; edgeIndex < boundingBox.faces[0].faceVertices.size(); edgeIndex++) {
            Vertex vertex = boundingBox.faces[faceIndex].faceVertices[edgeIndex];
            glVertex3d(vertex.x, vertex.y, vertex.z);
        }
        glEnd();

    }
    glPopMatrix();

    glPushMatrix();
    GLfloat colorCube[] = { 0.72f, .7f, .7f, 0.6f };
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, colorCube);
    glutSolidCube(20);
    glPopMatrix();

    glColor3f(1.0, 0.0, 0.0);
    GLfloat colorBall[] = { 0.8f, .0f, .0f, 1.f };
    glMaterialfv(GL_BACK, GL_DIFFUSE, colorBall);
    
    for (int ballID = 0; ballID < numBalls; ballID++) {
        glPushMatrix();
        glTranslatef(ballArray[ballID].posX, ballArray[ballID].posY, ballArray[ballID].posZ);
        glutSolidSphere(1.0, 20.0, 20.0);
        glPopMatrix();
    }
    

    glutSwapBuffers();
}

void idle() {

    // Running the simulation

    if (totalTime >= (1.0f / simulationFPS)) { // Checks simulation time

        endTime = std::chrono::steady_clock::now();
        elapsedTime = (std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime)).count() / pow(10, 6);

        if (elapsedTime >= (1.0f / displayFPS)) { // Checks display time
            startTime = std::chrono::steady_clock::now();
            totalTime = 0;
            glutPostRedisplay();
        }

        else {
            return;
        }
    }

    for (int ballID = 0; ballID < numBalls; ballID++) {
        // Vector to keep track of all the forces on an object
        std::vector<std::vector<float>> forceVector;

        // Apply gravity
        std::vector<float> gravityForce = { 0, -gravity * ballArray[ballID].mass, 0 };
        forceVector.push_back(gravityForce);
        
        // Ball-Ball forces
        if (interballForces) {
            for (int otherBallID = 0; otherBallID < numBalls; otherBallID++) {
                if (otherBallID == ballID) {
                    continue;
                }
                Vertex interBallVector(ballArray[ballID].posX - ballArray[otherBallID].posX, ballArray[ballID].posY - ballArray[otherBallID].posY, ballArray[ballID].posZ - ballArray[otherBallID].posZ);
                float interBallForceMagnitude = interBallVector.magnitude();
                interBallVector = interBallVector / interBallForceMagnitude;
                float forceCoeff = 10000.0f;
                interBallVector = (interBallVector / (pow(interBallForceMagnitude, 3))) * forceCoeff;

                std::vector<float> interBallForce = { interBallVector.x, interBallVector.y, interBallVector.z };
                forceVector.push_back(interBallForce);
            }
        }

        // Air resistance
        if (isAirResistance) {
            float airResistanceCoeff = 2.0f;
            std::vector<float> airResistance = { -ballArray[ballID].velX * airResistanceCoeff, -ballArray[ballID].velY * airResistanceCoeff, -ballArray[ballID].velZ * airResistanceCoeff };
            forceVector.push_back(airResistance);
        }

        ballArray[ballID].moveBall(timestep, forceVector);
    }
       
    totalTime = totalTime + timestep;

}

void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, 1.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0, 0, 25, 0, 0, 0, 0, 1, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);


    GLfloat lightpos[] = { 0, -10, 5, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    GLfloat lightColor[] = { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightColor);

    constructBoundingBox();
    constructBall();

    startTime = std::chrono::steady_clock::now();

}


int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Ball Bounce");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    myInit();
    glutMainLoop();

}

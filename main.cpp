#include "main.h"

#include <iostream>
#include <fstream> 
#include <algorithm>
#include "controller.h"

using namespace std;
using namespace glm;

Solid* Cuboid();
Solid* Cuboid_with_hole();
Solid* Cuboid_with_2_holes();


int main()
{
    // Init GLFW
    InitGLFW();
    InitController();
    // Enable settings
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1.0);

    cout << "Initialized" << endl;

    vector<Solid*> solidList;
    cout << "choose your shape:" << endl << "1.Cuboid" << endl << "2.Cuboid with hole" << endl << "3.Cuboid with 2 holes" << endl;
    int choice;
    cin >> choice;
    if (choice > 3 || choice < 1) {
        cout << "invalid input" << endl;
        return -1;
    }
    if (choice == 1) {
        solidList.push_back(Cuboid());
    }
    else if (choice == 2) {
        solidList.push_back(Cuboid_with_hole());
    }
    else if (choice == 3) {
        solidList.push_back(Cuboid_with_2_holes());
    }

    GLfloat deltaTime = 0, currentTime = 0;
    GLfloat startTime = glfwGetTime();
    GLfloat lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        Solid *drawSolid = solidList[Controller::count % solidList.size()];
        glfwPollEvents();
        GLfloat newTime = glfwGetTime();
        Controller::Movement(newTime - lastTime);
        lastTime = newTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        glViewport(0, 0, SCREEN_SIZE.x / 2, SCREEN_SIZE.y);
        
        // Perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(45.0f, SCREEN_SIZE.x / 2.0 / SCREEN_SIZE.y, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        glTranslatef(0.0f, 0.0f, -10.0f);

        glRotatef(Controller::zAngle, 0.0f, 1.0f, 0.0f);
        glRotatef(Controller::yAngle, 1.0f, 0.0f, 1.0f);

        glRotatef(180.0, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);

        DrawSolid(drawSolid);

        glViewport(SCREEN_SIZE.x / 2, 0, SCREEN_SIZE.x / 2, SCREEN_SIZE.y);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(45.0f, SCREEN_SIZE.x / 2.0 / SCREEN_SIZE.y, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        glTranslatef(0.0f, 0.0f, -10.0f);
        

        glRotatef(Controller::zAngle, 0.0f, 1.0f, 0.0f);
        glRotatef(Controller::yAngle, 1.0f, 0.0f, 1.0f);

        glRotatef(180.0, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);
        DrawSolid(drawSolid, true);

        glfwSwapBuffers(window);
    }

    ReleaseController();

    glfwTerminate();
    return 0;
}

int InitGLFW()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(SCREEN_SIZE.x/2, SCREEN_SIZE.y, "Euler Operation", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // Windowed
    glfwMakeContextCurrent(window);
    // Options
    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();
    // Define the viewport dimensions
    // glEnable(GL_MULTISAMPLE);
    // glfwSwapInterval(1);

    // Print out some info about the graphics drivers
    cout << "GLFW Initialized" << endl;
    cout << "--------------------------------------" << endl;

    return 0;
}

void InitController()
{
    if (!Controller::Initialize())
    {
        std::cout << "Unable to initialize camera!" << endl;
    }

    glfwSetKeyCallback(window, Controller::KeyBoard);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ReleaseController()
{
    Controller::Release();
}

Solid* Cuboid_with_hole()
{
    Solid* newSolid;

    Vertex* topUpperLeft;
    newSolid = mvfs(Point(-1.0, 0.5, 2.0), topUpperLeft);
    Loop* topLoop = newSolid->Faces->FLoop;//0
    HalfEdge* topUpperHalfEdge = mev(topUpperLeft, Point(1.0, 0.5, 2.0), topLoop);
    Vertex* topUpperRight = topUpperHalfEdge->endV;
    HalfEdge* topLeftHalfEdge = mev(topUpperLeft, Point(-1.0, 0.5, -2.0), topLoop);
    Vertex* topBottomLeft = topLeftHalfEdge->endV;
    HalfEdge* topRightHalfEdge = mev(topUpperRight, Point(1.0, 0.5, -2.0), topLoop);
    Vertex* topBottomRight = topRightHalfEdge->endV;
    // Make top ring
    Loop* downLoop = mef(topBottomLeft, topBottomRight, topLoop);//1

    HalfEdge* upperLeftHalfEdge = mev(topUpperLeft, Point(-1.0, -0.5, 2.0), downLoop);
    Vertex* downUpperLeft = upperLeftHalfEdge->endV;
    HalfEdge* upperRightHalfEdge = mev(topUpperRight, Point(1.0, -0.5, 2.0), downLoop);
    Vertex* downUpperRight = upperRightHalfEdge->endV;
    HalfEdge* bottomLeftHalfEdge = mev(topBottomLeft, Point(-1.0, -0.5, -2.0), downLoop);
    Vertex* downBottomLeft = bottomLeftHalfEdge->endV;
    HalfEdge* bottomRightHalfEdge = mev(topBottomRight, Point(1.0, -0.5, -2.0), downLoop);
    Vertex* downBottomRight = bottomRightHalfEdge->endV; //7

    Loop* upperLoop = mef(downUpperLeft, downUpperRight, downLoop);//2
    Loop* rightLoop = mef(downUpperRight, downBottomRight, downLoop);//3
    Loop* bottomLoop = mef(downBottomRight, downBottomLeft, downLoop);//4
    Loop* leftLoop = mef(downBottomLeft, downUpperLeft, downLoop);//5

    // Top inner ring

    HalfEdge* topBridge = mev(topUpperLeft, Point(-0.5, 0.5, 1.5), topLoop);
    Vertex* topInnerUpperLeft = topBridge->endV;//8

    HalfEdge* topInnerUpperHalfEdge = mev(topInnerUpperLeft, Point(0.5, 0.5, 1.5), topLoop);
    Vertex* topInnerUpperRight = topInnerUpperHalfEdge->endV;//9
    HalfEdge* topInnerRightHalfEdge = mev(topInnerUpperRight, Point(0.5, 0.5, -1.5), topLoop);
    Vertex* topInnerBottomRight = topInnerRightHalfEdge->endV;//10
    HalfEdge* topInnerLeftHalfEdge = mev(topInnerUpperLeft, Point(-0.5, 0.5, -1.5), topLoop);
    Vertex* topInnerBottomLeft = topInnerLeftHalfEdge->endV;//11
    Loop* downInnerLoop = mef(topInnerBottomLeft, topInnerBottomRight, topLoop);//6

    Loop* topInnerLoop = kemr(topUpperLeft, topInnerUpperLeft, topLoop);//7

    HalfEdge* upperLeftInnerHalfEdge = mev(topInnerUpperLeft, Point(-0.5, -0.5, 1.5), downInnerLoop);
    Vertex* downInnerUpperLeft = upperLeftInnerHalfEdge->endV;//12
    HalfEdge* upperRightInnerHalfEdge = mev(topInnerUpperRight, Point(0.5, -0.5, 1.5), downInnerLoop);
    Vertex* downInnerUpperRight = upperRightInnerHalfEdge->endV;//13
    HalfEdge* bottomLeftInnerHalfEdge = mev(topInnerBottomLeft, Point(-0.5, -0.5, -1.5), downInnerLoop);
    Vertex* downInnerBottomLeft = bottomLeftInnerHalfEdge->endV;//14
    HalfEdge* bottomRightInnerHalfEdge = mev(topInnerBottomRight, Point(0.5, -0.5, -1.5), downInnerLoop);
    Vertex* downInnerBottomRight = bottomRightInnerHalfEdge->endV;//15

    Loop* upperInnerLoop = mef(downInnerUpperLeft, downInnerUpperRight, downInnerLoop);//8
    Loop* rightInnerLoop = mef(downInnerUpperRight, downInnerBottomRight, downInnerLoop);//9
    Loop* bottomInnerLoop = mef(downInnerBottomRight, downInnerBottomLeft, downInnerLoop);//10
    Loop* leftInnerLoop = mef(downInnerBottomLeft, downInnerUpperLeft, downInnerLoop);//11

    kfmrh(downLoop, downInnerLoop);


    topInnerLoop->SetInnerLoop(true);
    downInnerLoop->SetInnerLoop(true);

    return newSolid;
}

Solid* Cuboid()
{
    Solid* newSolid;
    Vertex* topUpperLeft;
    newSolid = mvfs(Point(-1.0, 0.5, 2.0), topUpperLeft);
    Loop* topLoop = newSolid->Faces->FLoop;

    HalfEdge* topUpperHalfEdge = mev(topUpperLeft, Point(1.0, 0.5, 2.0), topLoop);
    Vertex* topUpperRight = topUpperHalfEdge->endV;

    HalfEdge* topLeftHalfEdge = mev(topUpperLeft, Point(-1.0, 0.5, -2.0), topLoop);
    Vertex* topBottomLeft = topLeftHalfEdge->endV;

    HalfEdge* topRightHalfEdge = mev(topUpperRight, Point(1.0, 0.5, -2.0), topLoop);
    Vertex* topBottomRight = topRightHalfEdge->endV;
    // Make top ring
    Loop* downLoop = mef(topBottomLeft, topBottomRight, topLoop);

    sweep(topLoop->LFace, glm::vec3(0, -1, 0), 1);

    // traverseSolid(newSolid, "Solid One");
    return newSolid;
}

Solid* Cuboid_with_2_holes()
{
    Solid* newSolid;

    Vertex* topUpperLeft;
    newSolid = mvfs(Point(-1.0, 0.5, 2.0), topUpperLeft);
    Loop* topLoop = newSolid->Faces->FLoop;
    HalfEdge* topUpperHalfEdge = mev(topUpperLeft, Point(1.0, 0.5, 2.0), topLoop);
    Vertex* topUpperRight = topUpperHalfEdge->endV;
    HalfEdge* topLeftHalfEdge = mev(topUpperLeft, Point(-1.0, 0.5, -2.0), topLoop);
    Vertex* topBottomLeft = topLeftHalfEdge->endV;
    HalfEdge* topRightHalfEdge = mev(topUpperRight, Point(1.0, 0.5, -2.0), topLoop);
    Vertex* topBottomRight = topRightHalfEdge->endV;
    // Make top ring
    Loop* downLoop = mef(topBottomLeft, topBottomRight, topLoop);

    HalfEdge* upperLeftHalfEdge = mev(topUpperLeft, Point(-1.0, -0.5, 2.0), downLoop);
    Vertex* downUpperLeft = upperLeftHalfEdge->endV;
    HalfEdge* upperRightHalfEdge = mev(topUpperRight, Point(1.0, -0.5, 2.0), downLoop);
    Vertex* downUpperRight = upperRightHalfEdge->endV;
    HalfEdge* bottomLeftHalfEdge = mev(topBottomLeft, Point(-1.0, -0.5, -2.0), downLoop);
    Vertex* downBottomLeft = bottomLeftHalfEdge->endV;
    HalfEdge* bottomRightHalfEdge = mev(topBottomRight, Point(1.0, -0.5, -2.0), downLoop);
    Vertex* downBottomRight = bottomRightHalfEdge->endV;

    Loop* upperLoop = mef(downUpperLeft, downUpperRight, downLoop);
    Loop* rightLoop = mef(downUpperRight, downBottomRight, downLoop);
    Loop* bottomLoop = mef(downBottomRight, downBottomLeft, downLoop);
    Loop* leftLoop = mef(downBottomLeft, downUpperLeft, downLoop);

    // Debug
    // traverseLoop(topLoop, "Top Loop");
    // traverseLoop(downLoop, "Down Loop");
    // traverseLoop(upperLoop, "Upper Loop");
    // traverseLoop(bottomLoop, "Bottom Loop");
    // traverseLoop(rightLoop, "Right Loop");
    // traverseLoop(leftLoop, "Left Loop");

    // Top inner ring

    HalfEdge* topBridge = mev(topUpperLeft, Point(-0.5, 0.5, 1.5), topLoop);
    Vertex* topInnerUpperLeft = topBridge->endV;

    HalfEdge* topInnerUpperHalfEdge = mev(topInnerUpperLeft, Point(0.5, 0.5, 1.5), topLoop);
    Vertex* topInnerUpperRight = topInnerUpperHalfEdge->endV;
    HalfEdge* topInnerRightHalfEdge = mev(topInnerUpperRight, Point(0.5, 0.5, 0.5), topLoop);
    Vertex* topInnerBottomRight = topInnerRightHalfEdge->endV;
    HalfEdge* topInnerLeftHalfEdge = mev(topInnerUpperLeft, Point(-0.5, 0.5, 0.5), topLoop);
    Vertex* topInnerBottomLeft = topInnerLeftHalfEdge->endV;
    Loop* downInnerLoop = mef(topInnerBottomLeft, topInnerBottomRight, topLoop);

    Loop* topInnerLoop = kemr(topUpperLeft, topInnerUpperLeft, topLoop);

    HalfEdge* upperLeftInnerHalfEdge = mev(topInnerUpperLeft, Point(-0.5, -0.5, 1.5), downInnerLoop);
    Vertex* downInnerUpperLeft = upperLeftInnerHalfEdge->endV;
    HalfEdge* upperRightInnerHalfEdge = mev(topInnerUpperRight, Point(0.5, -0.5, 1.5), downInnerLoop);
    Vertex* downInnerUpperRight = upperRightInnerHalfEdge->endV;
    HalfEdge* bottomLeftInnerHalfEdge = mev(topInnerBottomLeft, Point(-0.5, -0.5, 0.5), downInnerLoop);
    Vertex* downInnerBottomLeft = bottomLeftInnerHalfEdge->endV;
    HalfEdge* bottomRightInnerHalfEdge = mev(topInnerBottomRight, Point(0.5, -0.5, 0.5), downInnerLoop);
    Vertex* downInnerBottomRight = bottomRightInnerHalfEdge->endV;

    Loop* upperInnerLoop = mef(downInnerUpperLeft, downInnerUpperRight, downInnerLoop);
    Loop* rightInnerLoop = mef(downInnerUpperRight, downInnerBottomRight, downInnerLoop);
    Loop* bottomInnerLoop = mef(downInnerBottomRight, downInnerBottomLeft, downInnerLoop);
    Loop* leftInnerLoop = mef(downInnerBottomLeft, downInnerUpperLeft, downInnerLoop);

    kfmrh(downLoop, downInnerLoop);

    topInnerLoop->SetInnerLoop(true);
    downInnerLoop->SetInnerLoop(true);

    HalfEdge* topBridge2 = mev(topUpperLeft, Point(-0.5, 0.5, -0.5), topLoop);
    Vertex* topInnerUpperLeft2 = topBridge2->endV;

    HalfEdge* topInnerUpperHalfEdge2 = mev(topInnerUpperLeft2, Point(0.5, 0.5, -0.5), topLoop);
    Vertex* topInnerUpperRight2 = topInnerUpperHalfEdge2->endV;
    HalfEdge* topInnerRightHalfEdge2 = mev(topInnerUpperRight2, Point(0.5, 0.5, -1.5), topLoop);
    Vertex* topInnerBottomRight2 = topInnerRightHalfEdge2->endV;
    HalfEdge* topInnerLeftHalfEdge2 = mev(topInnerUpperLeft2, Point(-0.5, 0.5, -1.5), topLoop);
    Vertex* topInnerBottomLeft2 = topInnerLeftHalfEdge2->endV;
    Loop* downInnerLoop2 = mef(topInnerBottomLeft2, topInnerBottomRight2, topLoop);

    Loop* topInnerLoop2 = kemr(topUpperLeft, topInnerUpperLeft2, topLoop);

    HalfEdge* upperLeftInnerHalfEdge2 = mev(topInnerUpperLeft2, Point(-0.5, -0.5, -0.5), downInnerLoop2);
    Vertex* downInnerUpperLeft2 = upperLeftInnerHalfEdge2->endV;
    HalfEdge* upperRightInnerHalfEdge2 = mev(topInnerUpperRight2, Point(0.5, -0.5, -0.5), downInnerLoop2);
    Vertex* downInnerUpperRight2 = upperRightInnerHalfEdge2->endV;
    HalfEdge* bottomLeftInnerHalfEdge2 = mev(topInnerBottomLeft2, Point(-0.5, -0.5, -1.5), downInnerLoop2);
    Vertex* downInnerBottomLeft2 = bottomLeftInnerHalfEdge2->endV;
    HalfEdge* bottomRightInnerHalfEdge2 = mev(topInnerBottomRight2, Point(0.5, -0.5, -1.5), downInnerLoop2);
    Vertex* downInnerBottomRight2 = bottomRightInnerHalfEdge2->endV;

    Loop* upperInnerLoop2 = mef(downInnerUpperLeft2, downInnerUpperRight2, downInnerLoop2);
    Loop* rightInnerLoop2 = mef(downInnerUpperRight2, downInnerBottomRight2, downInnerLoop2);
    Loop* bottomInnerLoop2 = mef(downInnerBottomRight2, downInnerBottomLeft2, downInnerLoop2);
    Loop* leftInnerLoop2 = mef(downInnerBottomLeft2, downInnerUpperLeft2, downInnerLoop2);

    kfmrh(downLoop, downInnerLoop2);

    topInnerLoop2->SetInnerLoop(true);
    downInnerLoop2->SetInnerLoop(true);

    return newSolid;
}
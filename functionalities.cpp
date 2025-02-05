// This file contains different functionalities used in our project

#include "functionalities.h"
#include "shapes.h"
#include <bits/stdc++.h>
using namespace std;
level currentLevel = NIL;
mode currentMode = ADJUSTING;

bool currentlyWaiting;
bool downKeys[127];
bool scoredGoal;
int mouseX, mouseY;
bool firstTime = true;
PhysicalState sphere, *determineSphere = NULL;

void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION); // Switch to setting the camera perspective

    // Set the camera perspective
    glLoadIdentity(); // Reset the camera
    glFrustum(-1.0 * w / h, 1.0 * w / h, -1, 1, 2, 100);
    glMatrixMode(GL_MODELVIEW);
}

double &axes::operator[](int index) // swtching .x,.y,.z into arrays format
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        cout << "Out of Bound Axis!" << endl;
        exit(1);
    }
}

double distanceBW(axes axes1, axes axes2)
{
    double sum = 0.0;
    double sqr;
    for (int i = 0; i < 3; ++i)
    {
        sqr = axes1[i] - axes2[i];
        sqr *= sqr;
        sum += sqr;
    }
    return sqrt(sum);
}

PhysicalState::PhysicalState()
{
    positionInitial = positionCurrent = velocityInitial = velocityCurrent = accelerationCurrent = {0.0, 0.0, 0.0};
    timePassed = 0;
    elasticity = 1.0;
    friction = 0.0;
}

ostream &operator<<(ostream &out, PhysicalState &p)
{
    out << "Current Position : " << endl;
    for (int i = 0; i < 3; ++i)
    {
        out << p.positionCurrent[i] << "    ";
    }
    out << endl;
    out << "Current Velocity : " << endl;
    for (int i = 0; i < 3; ++i)
    {
        out << p.velocityCurrent[i] << "    ";
    }
    out << endl;
    out << "Time Passed : ";
    out << p.timePassed << endl;
}

bool isItGoal(PhysicalState ball) // checking goal condition
{

    if ((ball.positionCurrent[0] <= -POLE_RADIUS + POLE_LENGTH / 2) &&
        (ball.positionCurrent[0] >= +POLE_RADIUS - POLE_LENGTH / 2) &&
        (ball.positionCurrent[2] <= POLE_HEIGHT) && (ball.positionCurrent[1] > GOAL_POST_Y))
        return true;
    else
        return false;
}

void backgroundMusicPlayer(int _) // plays sound
{
    system("paplay resources/back.wav --volume 30000 &");
    glutTimerFunc(5 * 1000, backgroundMusicPlayer, 0);
}

void initialiseEverything() // reset after every shoot
{
    currentMode = ADJUSTING;

    ground.Type = WALL;
    axes temp;
    temp = {-20, -20, -BALL_RADIUS};
    ground.corners[0] = temp;
    temp = {-20, 20, -BALL_RADIUS};
    ground.corners[1] = temp;
    temp = {20, 20, -BALL_RADIUS};
    ground.corners[2] = temp;
    temp = {20, -20, -BALL_RADIUS};
    ground.corners[3] = temp;
    ground.color[0] = 1 / 255.0;
    ground.color[1] = 142 / 255.0;
    ground.color[2] = 14 / 255.0;

    poles[0].Type = L_POLE;
    poles[0].height = POLE_HEIGHT;
    poles[1].Type = U_POLE;
    poles[1].height = POLE_LENGTH / 2;
    poles[2].Type = R_POLE;
    poles[2].height = POLE_HEIGHT;

    temp = {0, 0, 0};
    aimArrow.start = temp;
    temp = {0, 2, 0};
    aimArrow.finish = temp;

    aimArrow.width = 0.2;

    aimArrow.zAngle = 0.0;
    aimArrow.yAngle = 0.0;
    aimArrow.length = 2.0;

    aimArrow.color[0] = 50 / 255.0;
    aimArrow.color[1] = 50 / 255.0;
    aimArrow.color[2] = 127 / 255.0;
    aimArrow.color[3] = 1.0;

    defender.color[0] = 250 / 255.0;
    defender.color[1] = 100 / 255.0;
    defender.color[2] = 100 / 255.0;
    defender.color[3] = 1.0;

    defender.width = DEFENDER_WIDTH;
    defender.height = 2.3;

    if (currentLevel == EASY) // setting parameters of defender in easy level
    {

        // printf("Init called\n");
        defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_EASY;
        defender.state.velocityInitial[2] = defender.state.velocityCurrent[2] = DEFENDER_SPEED_VERTICAL;
        defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_EASY;
        defender.state.velocityInitial.z = defender.state.velocityCurrent.z = DEFENDER_SPEED_VERTICAL;
        defender.move_random_dist = 50;

        defender.state.accelerationCurrent[0] = 0;
        defender.state.accelerationCurrent[1] = 0;
        defender.state.accelerationCurrent[2] = -9.8;

        defender.state.positionCurrent[0] = 0;
        defender.state.positionCurrent[1] = 0;
        defender.state.positionCurrent[2] = 0;

        poles[0].state.velocityCurrent[0] = 0;
        poles[2].state.velocityCurrent[0] = 0;
        poles[1].state.velocityCurrent[0] = 0;
        poles[0].state.positionCurrent[0] = 0;
        poles[2].state.positionCurrent[0] = 0;
        poles[1].state.positionCurrent[0] = 0;
    }
    if (currentLevel == MEDIUM) // setting parameters of defender in medium level
    {

        defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_MEDIUM;
        defender.state.velocityInitial[2] = defender.state.velocityCurrent[2] = DEFENDER_SPEED_VERTICAL;
        defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_MEDIUM;
        defender.state.velocityInitial.z = defender.state.velocityCurrent.z = DEFENDER_SPEED_VERTICAL;

        defender.state.accelerationCurrent[0] = 0;
        defender.state.accelerationCurrent[1] = 0;
        defender.state.accelerationCurrent[2] = -9.8;

        defender.state.positionCurrent[0] = 0;
        defender.state.positionCurrent[1] = 0;
        defender.state.positionCurrent[2] = 0;

        poles[0].state.velocityCurrent[0] = 0;
        poles[2].state.velocityCurrent[0] = 0;
        poles[1].state.velocityCurrent[0] = 0;
        poles[0].state.positionCurrent[0] = 0;
        poles[2].state.positionCurrent[0] = 0;
        poles[1].state.positionCurrent[0] = 0;
    }
    if (currentLevel == HARD) // setting parameters of defender in hard level
    {

        defender.state.velocityInitial[0] = defender.state.velocityCurrent[0] = DEFENDER_SPEED_HARD;
        defender.state.velocityInitial[2] = defender.state.velocityCurrent[2] = DEFENDER_SPEED_VERTICAL;
        defender.state.velocityInitial.x = defender.state.velocityCurrent.x = DEFENDER_SPEED_HARD;
        defender.state.velocityInitial.z = defender.state.velocityCurrent.z = DEFENDER_SPEED_VERTICAL;

        defender.state.accelerationCurrent[0] = 0;
        defender.state.accelerationCurrent[1] = 0;
        defender.state.accelerationCurrent[2] = -9.8;

        defender.state.positionCurrent[0] = 0;
        defender.state.positionCurrent[1] = 0;
        defender.state.positionCurrent[2] = 0;
        poles[0].state.velocityCurrent[0] = 0;
        poles[2].state.velocityCurrent[0] = 0;
        poles[1].state.velocityCurrent[0] = 0;
        poles[0].state.positionCurrent[0] = 0;
        poles[2].state.positionCurrent[0] = 0;
        poles[1].state.positionCurrent[0] = 0;
    }

    if (currentLevel == HUMAN) // setting parameters of defender in play with human
    {

        defender.state.velocityInitial[2] = 0;
        defender.state.velocityCurrent[2] = 0;
        defender.state.velocityInitial[0] = 0;
        defender.state.velocityCurrent[0] = 0;

        defender.state.accelerationCurrent[2] = 0;
        defender.state.accelerationCurrent[0] = 0;
        defender.state.accelerationCurrent[1] = 0;

        defender.state.positionCurrent[0] = 0;
        defender.state.positionCurrent[1] = 0;
        defender.state.positionCurrent[2] = 0;

        defender.state.velocityInitial.x = 0;
        defender.state.velocityCurrent.x = 0;
        defender.state.velocityInitial.y = 0;
        defender.state.velocityCurrent.y = 0;
        poles[0].state.velocityCurrent[0] = 0;
        poles[2].state.velocityCurrent[0] = 0;
        poles[1].state.velocityCurrent[0] = 0;
        poles[0].state.positionCurrent[0] = 0;
        poles[2].state.positionCurrent[0] = 0;
        poles[1].state.positionCurrent[0] = 0;
    }
    // updating ball position
    sphereCamera.xAngle = -90.0f;
    sphereCamera.zAngle = 15.0f;
    sphereCamera.distance = 5.0;
    sphereCamera.distance = 10;
    for (int i = 0; i < 3; ++i)
    {
        sphere.positionCurrent[i] = sphere.velocityCurrent[i] = 0;
    }
    sphere.positionCurrent[1] = -10;
    powerMeter = 0.0;

    sphere.positionInitial.x = sphere.positionCurrent.x = 0.0;
    sphere.positionInitial.y = -10;
    sphere.velocityCurrent[0] = sphere.velocityInitial[0] = 0;
    sphere.accelerationCurrent[2] = -9.8;
    toLookAt = sphere.positionCurrent;

    sphere.elasticity = BALL_ELASTICITY;
    sphere.friction = BALL_FRICTION;
    stopEverything = false;

    if (determineSphere)
    {
        delete determineSphere;
        determineSphere = NULL;
    }
    system("paplay resources/whistle.wav &");
}

void initialiseEverythingCallback(int _)
{
    stopEverything = true;
    initialiseEverything();
}

void drawGoalPost() // drawing goal post
{

    {
        glPushMatrix();
        glTranslated(GOAL_POST_X - POLE_LENGTH / 2 + POLE_RADIUS + poles[0].state.positionCurrent[0], GOAL_POST_Y + 0, 0 - BALL_RADIUS);
        poles[0].draw();
        glPopMatrix();

        glPushMatrix();
        glTranslated(GOAL_POST_X + 0 + poles[1].state.positionCurrent[0], GOAL_POST_Y + 0, POLE_HEIGHT + POLE_RADIUS - BALL_RADIUS);
        poles[1].draw();
        glPopMatrix();

        glPushMatrix();
        glTranslated(GOAL_POST_X + POLE_LENGTH / 2 - POLE_RADIUS + poles[2].state.positionCurrent[0], GOAL_POST_Y + 0, 0 - BALL_RADIUS);
        poles[2].draw();
        glPopMatrix();
    }
}

void cameraPosition(axes point, double distance, double zAngle, double xAngle)
{
    gluLookAt(point.x + distance * (cos(DEG2GRAD(zAngle)) * cos(DEG2GRAD(xAngle))),
              point.y + distance * (cos(DEG2GRAD(zAngle)) * sin(DEG2GRAD(xAngle))),
              point.z + distance * sin(DEG2GRAD(zAngle)), point.x, point.y, point.z, 0, 0, 1);
}

camera::camera()
{
    zAngle = xAngle = 0.0;
    distance = 5.0;
}

camera sphereCamera;

void rainBox(double alpha = 0.7) // used in making powermeter
{

    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.0f, 0.0, alpha); // RED
    glVertex2f(-10.0, -20.0);
    glVertex2f(10.0, -20.0);

    glColor4f(1.0f, 1.0f, 0.0, alpha); // YELLOW
    glVertex2f(10.0, -10.0);
    glVertex2f(-10.0, -10.0);

    glVertex2f(-10.0, -10.0);
    glVertex2f(10.0, -10.0);

    glColor4f(0.0f, 1.0f, 0.0, alpha); // GREEN
    glVertex2f(10.0, 0.0);
    glVertex2f(-10.0, 0.0);
    glEnd();
}

void myShear()
{
    float m[] = {
        1.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0};
    glMultMatrixf(m);
}

double powerMeter = 0.0;
double powerMeter2 = 0.0;

void drawPowerMeter()
{

    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);

    glTranslatef(-10.0, -20.0 + powerMeter * 40, 0.0);

    glColor4f(0.1, 0.1, 0.1, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-10.0, -0.2);
    glVertex2f(-5.0, -0.2);
    glVertex2f(-5.0, 0.2);
    glVertex2f(-10.0, 0.2);
    glEnd();
    glBegin(GL_TRIANGLES);

    glVertex2f(-5.0, -0.4);
    glVertex2f(-2.0, 0.0);
    glVertex2f(-5.0, 0.4);

    glEnd();
    glPopAttrib();
    glPopMatrix();
}

void drawPowerMeter2()
{

    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);

    glTranslatef(-10.0, -20.0 + powerMeter2 * 40, 0.0);

    glColor4f(0.1, 0.1, 0.1, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-10.0, -0.2);
    glVertex2f(-5.0, -0.2);
    glVertex2f(-5.0, 0.2);
    glVertex2f(-10.0, 0.2);
    glEnd();
    glBegin(GL_TRIANGLES);

    glVertex2f(-5.0, -0.4);
    glVertex2f(-2.0, 0.0);
    glVertex2f(-5.0, 0.4);

    glEnd();
    glPopAttrib();
    glPopMatrix();
}

void drawHUD()
{
    glDisable(GL_LIGHTING);
    if (currentMode == HELP)
    {
        // drawing instruction page
        const char *instructions = R"INSTRUCT(      
        INSTRUCTIONS
        You can use the mouse to look around.
        Use the +/- keys for zooming in/out.
        AIMING
        Press the Enter/Return key to enter Aiming Mode.
        Direct the arrow with the arrow keys to set up the
        direction of the shot.
        SPIN AND POWERING
        Press and Hold 'p' for spin
        Press and Hold Space for speed
        Press the ESC key (when holding down) to cancel
        Press ESC key to return to the previous mode or to exit the
        instructions.
        Press Q at any time to exit the game.
        )INSTRUCT";

        glPushMatrix();
        glRotatef(90 + sphereCamera.xAngle, 0, 0, 1);
        glRotatef(-sphereCamera.zAngle, 1, 0, 0);

        glTranslatef(-0.8, -BALL_RADIUS - 9.8, -BALL_RADIUS - 3);

        glColor4f(0, 0, 0, 0.8);
        glBegin(GL_QUADS);
        glVertex3f(-10, 0, -5);
        glVertex3f(10, 0, -5);
        glVertex3f(10, 0, 6);
        glVertex3f(-10, 0, 6);
        glEnd();
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(0, -0.001, 9.5);

        currentTextColor = {1.0, 1.0, 1.0, 1.0};
        writeMultiLineText(instructions, font, CENTER);
        glPopMatrix();
    }
    else
    {
        if (currentMode == CHOOSE && Tries % 5 == 0) // drawing choose page
        {
            // prevGoals = Goals;
            const char *instructions = R"INSTRUCT(
            Choose:-

            PLAY WITH HUMAN:-
            HUMAN - 1

            PLAY WITH COMPUTER:-
               EASY - 2   
              MEDIUM - 3
            HARD - 4

            )INSTRUCT";
            glPushMatrix();
            glRotatef(90 + sphereCamera.xAngle, 0, 0, 1);
            glRotatef(-sphereCamera.zAngle, 1, 0, 0);

            glTranslatef(-0.8, -BALL_RADIUS - 9.8, -BALL_RADIUS - 3);

            glColor4f(0, 0, 0, 0.8);
            glBegin(GL_QUADS);
            glVertex3f(-10, 0, -5);
            glVertex3f(10, 0, -5);
            glVertex3f(10, 0, 6);
            glVertex3f(-10, 0, 6);
            glEnd();
            glScalef(0.5, 0.5, 0.5);
            glTranslatef(0, -0.001, 9.5);

            currentTextColor = {1.0, 1.0, 1.0, 1.0};
            writeMultiLineText(instructions, font, CENTER);
            glPopMatrix();
        }
        else
        {
            glDisable(GL_LIGHTING);
            glMatrixMode(GL_PROJECTION);
            glPushAttrib(GL_CURRENT_BIT);
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glClear(GL_DEPTH_BUFFER_BIT);

            if (currentMode == POWERING)
            { // Power Bar
                glPushMatrix();

                glTranslatef(90, 0, 0);

                glScalef(0.3, 4.0, 1.0);

                drawPowerMeter();

                rainBox();
                glScalef(1.0, -1.0, 0.0);
                rainBox();

                glPopMatrix();
            }
            if (currentMode == POWERING_ACC)
            { // Power Bar
                glPushMatrix();

                glTranslatef(-84, 0, 0);

                glScalef(0.3, 4.0, 1.0);

                drawPowerMeter2();

                rainBox();
                glScalef(1.0, -1.0, 0.0);
                rainBox();

                glPopMatrix();
            }
        }

        // Making sure we can render 3d again

        glEnable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glPopAttrib();
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    glEnable(GL_LIGHTING);
}

void updateGoalPostPosition(int _) // goal post movement
{

    poles[0].state.timePassed += 1 / 60.0;
    poles[1].state.timePassed += 1 / 60.0;
    poles[2].state.timePassed += 1 / 60.0;
    double t = 1 / 60.0;

    for (int i = 0; i < 3; ++i)
    {
        poles[i].state.positionCurrent[0] += poles[i].state.velocityCurrent[0] * t;
    }

    if (GOAL_POST_X + POLE_LENGTH / 2 - POLE_RADIUS + poles[2].state.positionCurrent[0] >= 15 || GOAL_POST_X - POLE_LENGTH / 2 + POLE_RADIUS + poles[0].state.positionCurrent[0] <= -15)
    {

        for (int i = 0; i < 3; ++i)
        {
            poles[i].state.velocityCurrent[0] *= -1;
        }
    }
    glutTimerFunc(1000 * 1 / 60.0, updateGoalPostPosition, 1 / 60.0);
}

void updateDefenderPosition(int yAngle) // defender motion
{
    static float increment = 2.0f;
    static int done = 0;
    defender.armRot += increment;

    if (defender.armRot > 100.0 || defender.armRot < 0.0)
    {
        increment *= -1;
    }

    defender.state.timePassed += 1 / 60.0;
    double t = 1 / 60.0;
    defender.acceleration();
    // for(int i=0; i<3; i++){
    // printf("Current Defender Velocity on axis %d is: %f\n", i, defender.state.velocityCurrent[i]);}

    if (currentLevel == HUMAN) // updting defender in human level
    {

        if (defender.state.positionCurrent[2] < 0) // if defender goes inside ground
        {
            defender.state.positionCurrent[2] = 0;
            defender.state.velocityCurrent[2] = 0;
            defender.state.accelerationCurrent[2] = 0;
        }
        else
        {
            defender.state.accelerationCurrent[2] = -9.8;
        }
        if (defender.state.velocityCurrent[0] < 0 && defender.state.accelerationCurrent[0] < 0) // negative accelaration
        {
            defender.state.accelerationCurrent[0] = 0;
            defender.state.velocityCurrent[0] = 0;
        }
        if (defender.state.velocityCurrent[0] > 0 && defender.state.accelerationCurrent[0] > 0)
        {
            defender.state.accelerationCurrent[0] = 0;
            defender.state.velocityCurrent[0] = 0;
        }
        for (int i = 0; i < 3; ++i) // updating position and velocity
        {
            defender.state.positionCurrent[i] +=
                defender.state.velocityCurrent[i] * t + 0.5 * defender.state.accelerationCurrent[i] * t * t;

            defender.state.velocityCurrent[i] =
                defender.state.velocityCurrent[i] + defender.state.accelerationCurrent[i] * t;
        }
    }

    if (currentLevel == EASY || currentLevel == MEDIUM || currentLevel == HARD) // updting defender in easy level
    {
        defender.state.accelerationCurrent[0] = 0;
        defender.state.accelerationCurrent[1] = 0;
        defender.state.accelerationCurrent[2] = -9.8;

        if (defender.state.positionCurrent[2] < 0)
        {
            defender.state.positionCurrent[2] = 0;
            defender.state.velocityCurrent[2] = -defender.state.velocityCurrent[2];
        }
        // Sometimes randomly move it in the wrong direction when random move is multiple of 27
        if (defender.move_random_dist % 27 == 0)
        {
            // Reverse the direction of movement
            yAngle *= -1;
        }
        float rand_move = defender.move_random_dist / 100.0;
        // printf("Random move: %f\n", rand_move);
        //  float rand_move= 1;
        float max_distance = POLE_LENGTH / 2 - POLE_RADIUS - defender.width / 2;
        // defender.state.positionCurrent[0]= 0; //resetting the position of defender

        if (yAngle < 0)
        {
            // defender.state.positionCurrent[0] += rand_move*max_distance;
            if (defender.state.positionCurrent[0] > rand_move * max_distance * (-1))
            {
                // Set defender velocity in the correct direction
                if (defender.state.velocityCurrent[0] > 0)
                {
                    defender.state.velocityCurrent[0] *= -1;
                }
                defender.state.positionCurrent[0] += defender.state.velocityCurrent[0] * t + 0.5 * defender.state.accelerationCurrent[0] * t * t;
            }
            else
            {
                defender.state.positionCurrent[0] = rand_move * max_distance * (-1);
                defender.state.velocityCurrent[0] = 0;
                defender.state.accelerationCurrent[0] = 0;
                // update z position
                defender.state.positionCurrent[2] += defender.state.velocityCurrent[2] * t + 0.5 * defender.state.accelerationCurrent[2] * t * t;
                // defender.acceleration();
            }
        }
        else if (yAngle == 0)
        {
            defender.state.positionCurrent[0] = 0;
        }
        else
        {
            if (defender.state.positionCurrent[0] < rand_move * max_distance * (1))
            {
                // Set defender velocity in the correct direction
                if (defender.state.velocityCurrent[0] < 0)
                {
                    defender.state.velocityCurrent[0] *= -1;
                }
                defender.state.positionCurrent[0] += defender.state.velocityCurrent[0] * t + 0.5 * defender.state.accelerationCurrent[0] * t * t;
            }
            else
            {
                defender.state.positionCurrent[0] = rand_move * max_distance * (1);
                defender.state.velocityCurrent[0] = 0;
                defender.state.accelerationCurrent[0] = 0;
                // update z position
                defender.state.positionCurrent[2] += defender.state.velocityCurrent[2] * t + 0.5 * defender.state.accelerationCurrent[2] * t * t;
                // defender.acceleration();
            }
        }
    }
}

int convertToTexture(const char *filename) // for texture making
{

    ifstream textFile(filename);
    string destination(filename);
    destination += ".tx";
    if (!textFile)
    {
        cout << filename << endl;
        cout << "File cannot be opened!" << endl;
        return -1;
    }
    unsigned color;
    vector<unsigned char> text;
    int w, h;
    textFile >> w >> h;
    while (textFile >> color)
    {
        text.push_back((color & 0xff000000) >> 24);
        text.push_back((color & 0x00ff0000) >> 16);
        text.push_back((color & 0x0000ff00) >> 8);
        text.push_back((color & 0x000000ff) >> 0);
    }
    unsigned char *colors = &text[0];
    ofstream dest(destination.c_str(), ios::out | ios::binary);
    unsigned int size = text.size();
    dest.write((char *)colors, text.size());
    dest.write((char *)&w, sizeof(w));
    dest.write((char *)&h, sizeof(h));
    dest.close();
}

GLuint loadTextureFile(const char *filename) // loading texture
{

    GLuint texture;
    glEnable(GL_DEPTH_TEST);
    ifstream textureFile(filename, ios::ate | ios::binary);
    int size = textureFile.tellg();
    size -= 2 * sizeof(int); // widht, height stored at the end.
    textureFile.close();
    int w, h;
    textureFile.open(filename, ios::in | ios::binary);

    unsigned char *texel = new unsigned char[size];
    textureFile.read((char *)texel, size);
    textureFile.read((char *)&w, sizeof(w));
    textureFile.read((char *)&h, sizeof(h));
    assert(w * h == size / 4);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texel[0]);
    delete[] texel;
    return texture;
}

const float chalkwidth = POLE_RADIUS;

void chalkHRectangle(axes start, axes end)
{
    glBegin(GL_QUADS);
    glVertex3f(start.x, start.y - chalkwidth / 2.0, 0);
    glVertex3f(start.x, start.y + chalkwidth / 2.0, 0);
    glVertex3f(end.x, end.y + chalkwidth / 2.0, 0);
    glVertex3f(end.x, end.y - chalkwidth / 2.0, 0);
    glEnd();
}

void chalkVRectangle(axes start, axes end)
{
    glBegin(GL_QUADS);
    glVertex3f(start.x - chalkwidth / 2.0, start.y, 0);
    glVertex3f(start.x + chalkwidth / 2.0, start.y, 0);
    glVertex3f(end.x + chalkwidth / 2.0, end.y, 0);
    glVertex3f(end.x - chalkwidth / 2.0, end.y, 0);
    glEnd();
}

void chalkSemiCircle(axes center, float radius, float start_angle, float end_angle)
{
    float twoPI = 2 * PI;

    glBegin(GL_POINTS);

    for (float i = start_angle; i <= end_angle; i += 0.001)
    {
        for (float disp = 0; disp <= chalkwidth; disp += 0.01)
        {
            glVertex2f(center.x + (sin(i) * radius), center.y + (cos(i) * radius) + disp);
        }
    }

    glEnd();
    glFlush();
}

void drawChalkLines()
{
    glPushAttrib(GL_LINE_WIDTH);
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();

    glColor3f(0.9, 0.9, 0.9);
    glTranslatef(0, 0, 0.0001 - BALL_RADIUS);
    GLUquadric *quad = gluNewQuadric();
    gluDisk(quad, 0, chalkwidth / 1.5, 25, 6);
    gluDeleteQuadric(quad);
    axes start = {ground.corners[1].x + 2, GOAL_POST_Y, 0};
    axes end = {ground.corners[2].x - 2, GOAL_POST_Y, 0};
    chalkHRectangle(start, end);
    start.y = -start.y;
    end.y = -end.y;
    chalkHRectangle(start, end);
    start = {ground.corners[1].x + 2, GOAL_POST_Y + chalkwidth / 2.0, 0};
    end = {ground.corners[0].x + 2, -GOAL_POST_Y - chalkwidth / 2.0, 0};
    chalkVRectangle(start, end);
    start.x = -start.x;
    end.x = -end.x;
    chalkVRectangle(start, end);
    start = {(ground.corners[1].x + ground.corners[2].x) / 2, GOAL_POST_Y};
    chalkSemiCircle(start, (ground.corners[2].x - ground.corners[1].x) / 4, PI / 2, 3 * PI / 2);
    glPopMatrix();
    glPopAttrib();
    glPopAttrib();
}

GLuint convertAndLoadTexture(const char *filename)
{
    convertToTexture(filename);
    string dest(filename);
    dest += ".tx";
    return loadTextureFile(dest.c_str());
}

GLuint groundTexture, defenderTexture, leftArm, rightArm, font, ads;

void start2DTexture(GLuint texture, bool lightingDisabled)
{

    glPushAttrib(GL_CURRENT_BIT);
    if (lightingDisabled)
        glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 1.0); // Replace this alpha for transparency
}

void end2DTexture(bool lightingDisabled)
{

    //    glDisable(GL_BLEND);
    if (lightingDisabled)
        glEnable(GL_LIGHTING);

    //    glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
    //    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopAttrib();
    glDepthMask(GL_TRUE);
}

float writeMultiLineText(string text, int texture, alignment align)
{
    std::stringstream iss(text);

    while (iss.good())
    {
        std::string SingleLine;
        getline(iss, SingleLine, '\n');
        writeText(SingleLine, texture, align);
        glTranslated(0, 0, -1);
    }
}

float writeText(string text, int texture, alignment align)
{
    start2DTexture(texture);
    glColor4fv(&currentTextColor[0]);
    int w, h;
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
    float translatex = (w / 128.0) / h, translatey = 0, translatez = 0;
    if (align == CENTER)
    {
        glTranslated(0 - ((text.size()) * translatex / 2.0), 0, 0);
    }
    else if (align == RIGHT)
    {
        float transx = text.size() * translatex;
        glTranslated(0 - ((text.size()) * translatex / 1.0), 0, 0);
    }
    for (char chr : text)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(chr / 128.0, 1);
        glVertex3f(0, 0, 0);

        glTexCoord2f((chr + 1) / 128.0, 1);
        glVertex3f((translatex), 0, 0);

        glTexCoord2f((chr + 1) / 128.0, 0);
        glVertex3f((translatex), 0, 1);

        glTexCoord2f(chr / 128.0, 0.0);
        glVertex3f(0, 0, 1);

        glEnd();

        glTranslatef(translatex, translatey, translatez);
    }

    glEnd();

    end2DTexture();
    return (2 * w / 128.0) / h;
}

int textRotX;

void rotateMsg(int _)
{

    textRotX = (textRotX + 3) % 360;

    if (textRotX != 0)
    {
        glutTimerFunc(1, rotateMsg, 0);
    }
}

void fun(string msg)
{

    float col[] = {132 / 255.0, 121 / 255.0, 150 / 255.0, 0.7};
    float distance = sphereCamera.distance - 4;

    float colin[] = {1.0, 1.0, 1.0, 0.7};
    glTranslatef(distance * (cos(DEG2GRAD(sphereCamera.zAngle)) * cos(DEG2GRAD(sphereCamera.xAngle))),
                 distance * (cos(DEG2GRAD(sphereCamera.zAngle)) * sin(DEG2GRAD(sphereCamera.xAngle))),
                 distance * sin(DEG2GRAD(sphereCamera.zAngle)));
    glTranslatef(toLookAt.x, toLookAt.y, toLookAt.z);
    glRotatef(90 + sphereCamera.xAngle + textRotX, 0, 0, 1);
    glRotatef(-sphereCamera.zAngle, 1, 0, 0);
    glScalef(0.75, 0.75, 0.75);
    GLUquadric *quad = gluNewQuadric();
    glPushMatrix();
    glColor4fv(col);
    glScalef(2, 0.5, 1);
    glRotatef(90, 1, 0, 0);
    // gluSphere(sphere, radius, slices, stacks);
    gluCylinder(quad, 1, 1, 1, 40, 40);
    // gluDisk(quad, 0.9, 1, 40, 40);
    glColor4fv(colin);
    // gluDisk(quad, 0, 0.9, 40, 40);

    glPopMatrix();

    glPushMatrix();

    glPushMatrix();
    glColor4fv(col);
    glScalef(2, 0.5, 1);
    glTranslatef(0, -1, 0);
    glRotatef(90, 1, 0, 0);
    gluDisk(quad, 0.9, 1, 40, 40);
    glColor4fv(colin);
    gluDisk(quad, 0, 0.9, 40, 40);
    glPopMatrix();
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, .001, -0.5);
    glRotatef(180, 0, 0, 1);

    writeText(msg, font, CENTER);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -0.501, -0.5);
    writeText(msg, font, CENTER);
    glPopMatrix();
}
void resultMsg() // printing results
{
    bool toWrite = true;

    string msg = "MISS!";

    currentTextColor = {1.0, 0.3, 0.3, 1};
    if (determineSphere)
    {
        if (isItGoal(*determineSphere))
        {
            msg = "GOAL!";

            currentTextColor = {0.3, 1.0, 0.3, 1};
        }
    }

    if (!determineSphere)
    {
        msg = "";
        toWrite = false;
    }

    if (toWrite)
    {

        glPopMatrix();
        if (Tries % 5 == 0)
        {

            if (prevGoals > 2)
            {
                msg = "A-WINS!";
            }
            else
            {
                msg = "D-WINS";
            }
        }
        fun(msg);
    }
    glPopMatrix();
}
// This file contains different declarations of functionalities used in our project

#ifndef FOOTBALL_PENALTY_SHOOTER_FUNCTIONALITIES_H
#define FOOTBALL_PENALTY_SHOOTER_FUNCTIONALITIES_H

#include <bits/stdc++.h>
#include <GL/glut.h>
#include "constants.h"
using namespace std;

#define DEG2GRAD(x) ((x) * PI / 180.0)
#define GRAD2DEG(x) ((x) * 180.0 / PI)

extern unsigned int Tries, Goals;

void handleResize(int w, int h);

struct axes
{
    double x, y, z;
    double &operator[](int);
};

double distanceBW(axes axes1, axes axes2);

struct PhysicalState
{
    axes positionInitial;
    axes velocityInitial;

    axes positionCurrent;
    axes velocityCurrent;
    axes accelerationCurrent;

    double elasticity;
    double timePassed;
    double friction;

    PhysicalState();
    friend ostream &operator<<(ostream &out, PhysicalState &p);
};

bool isItGoal(PhysicalState ball);

int LoadGLTexture(char *filename);

void updateDefenderPosition(int);
void updateGoalPostPosition(int);

void initialiseEverything();

void initialiseEverythingCallback(int _);

void drawGoalPost();

void cameraPosition(axes point, double distance, double xAngle, double zAngle);

struct camera
{
    double zAngle, xAngle;
    double distance;

    camera();
};

extern camera sphereCamera;

void myShear();

void drawHUD();
void draw_options();

enum mode
{
    ADJUSTING,
    AIMING,
    POWERING, // speed meter
    SHOOTING,
    HELP,
    NONE,
    CHOOSE,
    POWERING_ACC, // spin meter
    POWERING_IDLE // transition between both meter
};
enum level
{
    HUMAN,
    EASY,
    MEDIUM,
    HARD,
    NIL
};

void backgroundMusicPlayer(int);

extern double powerMeter;
extern double powerMeter2;

extern string message;
extern bool oncePassed;

void drawPowerMeter();

int convertToTexture(const char *filename);
GLuint loadTextureFile(const char *filename);
GLuint convertAndLoadTexture(const char *filename);

void start2DTexture(GLuint texture, bool lightingDisabled = true);
void end2DTexture(bool lightingDisabled = true);

extern mode currentMode;
extern level currentLevel;

extern bool currentlyWaiting;
extern bool stopEverything;

extern bool downKeys[127];

extern PhysicalState sphere, *determineSphere;

extern bool scoredGoal;

extern int prevGoals;

void drawBitmapText(const char *string, float x, float y);

extern int mouseX, mouseY;

extern bool firstTime;
enum alignment
{
    LEFT,
    RIGHT,
    CENTER
};
extern GLuint groundTexture, defenderTexture, leftArm, rightArm, font, ads;
extern vector<float> currentTextColor;
float writeText(string text, int texture, alignment align = LEFT);
float writeMultiLineText(string text, int texture, alignment align = LEFT);
void drawChalkLines();

void resultMsg();
void rotateMsg(int _);

extern axes toLookAt;

#endif // FOOTBALL_PENALTY_SHOOTER_FUNCTIONALITIES_H

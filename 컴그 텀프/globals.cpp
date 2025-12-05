#include "globals.h"

// ========= VAO/VBO =========
GLuint boatVAO, boatVBO;
GLuint rightVAO, rightVBO;
GLuint leftVAO, leftVBO;
GLuint groundVAO, groundVBO;
GLuint shader;

// ========= Mesh =========
std::vector<Vertex> boatMesh;
std::vector<Vertex> rightMesh;
std::vector<Vertex> leftMesh;

// ========= Model Matrices =========
float gModel[16];
float gRightModel[16];
float gLeftModel[16];

// ========= Boat State =========
float boatRotY = 0.0f;
float boatRotZ = 0.0f;
float boatRotX = 0.0f;
float boatScale = 0.025f;

float boatPosX = 0.0f;
float boatPosY = 90.0f;
float boatPosZ = 0.0f;
float boatRotVel = 0.0f;

// ========= Oars =========
// ========= Oars =========
float rightRotZ = 0.0f;
float rightVelZ = 0.0f;
float leftRotZ = 0.0f;
float leftVelZ = 0.0f;

// ÆÐµé Å©±â
float paddleScale = 0.03f;

// ========= Oar Spring Physics =========
float oarRestAngle = 0.0f;     // ±âº» °¢µµ
float oarPullAngle = 10.0f;    // µÚ·Î Á¥Èú ÃÖ´ë °¢µµ
float oarSnapForce = 4.0f;     // ¾ÕÀ¸·Î Æ¨±â´Â Èû
float oarSpringK = 0.15f;      // Åº¼º °è¼ö
float oarDamping = 0.82f;      // °¨¼è

// ========= Movement =========
float velX = 0.0f;
float velZ = 0.0f;

float accel = 0.1f;
float driftFactor = 0.5f;
float turnAngle = 2.0f;

float speed = 0.0f;
float acceleration = 0.03f;
float deceleration = 0.95f;
float maxSpeed = 2.0f;

bool sPressed = false;
bool kPressed = false;

// ========= Camera =========
float camX = 0.0f;
float camY = 5.0f;
float camZ = 15.0f;

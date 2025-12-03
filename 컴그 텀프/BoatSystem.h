#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>

class BoatSystem
{
public:
    void Init();

    void Update(float dt);
    void Render();

    void KeyDown(unsigned char key);
    void KeyUp(unsigned char key);

private:
    void UpdateBoatPhysics(float dt);
    void UpdateOars(float dt);
    void UpdateTransforms();

    // 보트 관련 수치
    float boatPosX = 0, boatPosY, boatPosZ = 0;
    float boatRotY = 0;

    float velX = 0, velZ = 0;
    float speed = 0;
    float deceleration = 0.95f;

    // 오어 관련
    float rightRotZ = 0, rightVelZ = 0;
    float leftRotZ = 0, leftVelZ = 0;

    // 카메라
    float camX = 0, camY = 2.0f, camZ = 8.0f;

    // 행렬 저장
    float gModel[16];
    float gRightModel[16];
    float gLeftModel[16];
    float groundModel[16];

    // VAO
    GLuint boatVAO = 0, leftVAO = 0, rightVAO = 0, groundVAO = 0;

    // 셰이더
    GLuint shader = 0;

    // 메쉬 데이터 크기
    int boatMeshCount = 0;
    int leftMeshCount = 0;
    int rightMeshCount = 0;

private:
    void MatIdentity(float m[16]);
    void MatTranslate(float m[16], float x, float y, float z);
    void MatRotateY(float m[16], float deg);
    void MatRotateZ(float m[16], float deg);
    void MatPerspective(float m[16], float fovy, float aspect, float zNear, float zFar);
    void MatLookAt(float m[16], float eyeX, float eyeY, float eyeZ,
        float cenX, float cenY, float cenZ,
        float upX, float upY, float upZ);

    void BuildModels();
};
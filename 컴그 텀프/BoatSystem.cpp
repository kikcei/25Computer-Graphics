#include "BoatSystem.h"
#include "graphics.h"
#include "globals.h"
#include "boat.h"
#include <cmath>
#include <vector>
#include "Matrix.h"


// ---------------------------------------------------------
// 내부 매트릭스 유틸
// ---------------------------------------------------------
void BoatSystem::MatIdentity(float m[16]) {
    for (int i = 0; i < 16; i++) m[i] = 0;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void BoatSystem::MatTranslate(float m[16], float x, float y, float z) {
    MatIdentity(m);
    m[12] = x;  m[13] = y;  m[14] = z;
}

void BoatSystem::MatRotateY(float m[16], float deg) {
    float rad = deg * M_PI / 180.f;
    float c = cos(rad), s = sin(rad);

    MatIdentity(m);
    m[0] = c;   m[2] = s;
    m[8] = -s;  m[10] = c;
}

void BoatSystem::MatRotateZ(float m[16], float deg) {
    float rad = deg * M_PI / 180.f;
    float c = cos(rad), s = sin(rad);

    MatIdentity(m);
    m[0] = c;   m[1] = -s;
    m[4] = s;   m[5] = c;
}

void BoatSystem::MatMul(float out[16], float a[16], float b[16])
{
    float r[16];
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++) {
            r[row * 4 + col] =
                a[row * 4 + 0] * b[0 * 4 + col] +
                a[row * 4 + 1] * b[1 * 4 + col] +
                a[row * 4 + 2] * b[2 * 4 + col] +
                a[row * 4 + 3] * b[3 * 4 + col];
        }
    for (int i = 0; i < 16; i++) out[i] = r[i];
}


// =========================================================
// 초기화
// =========================================================
void BoatSystem::Init() {
    boatVAO = ::boatVAO;
    leftVAO = ::leftVAO;
    rightVAO = ::rightVAO;
    groundVAO = ::groundVAO;

    boatMeshCount = (int)boatMesh.size();
    rightMeshCount = (int)rightMesh.size();
    leftMeshCount = (int)leftMesh.size();

    shader = ::shader;

    MatIdentity(gModel);
    MatIdentity(gLeftModel);
    MatIdentity(gRightModel);
}


// =========================================================
// 보트 물리 업데이트
// =========================================================
void BoatSystem::UpdateBoatPhysics(float dt)
{
    //  먼저 회전 업데이트
    boatRotY += boatRotVel;
    boatRotVel *= 0.85f;   // 감쇠

    //  회전 후 방향 업데이트
    float baseYaw = 180.0f;
    float rad = (boatRotY + baseYaw) * M_PI / 180.f;

    float dirX = sin(rad);
    float dirZ = cos(rad);

    velX = velX * 0.9f + dirX * speed * 0.1f;
    velZ = velZ * 0.9f + dirZ * speed * 0.1f;

    boatPosX -= velX;
    boatPosZ -= velZ;

    speed *= deceleration;
    if (speed < 0.0001f) speed = 0;
}



// =========================================================
// 노 스프링 애니메이션
// =========================================================
void BoatSystem::UpdateOars(float dt)
{
    // RIGHT OAR
    {
        float spring = (oarRestAngle - rightRotZ) * oarSpringK;
        rightVelZ *= oarDamping;
        rightVelZ += spring;
        rightRotZ += rightVelZ;
    }
    // LEFT OAR
    {
        float spring = (oarRestAngle - leftRotZ) * oarSpringK;
        leftVelZ *= oarDamping;
        leftVelZ += spring;
        leftRotZ += leftVelZ;
    }
}


// =========================================================
// 트랜스폼 업데이트 (회전 후 이동 = 올바른 순서)
// =========================================================
void BoatSystem::UpdateTransforms()
{
    float Tboat[16], Rboat[16], Sboat[16], TR[16];

    // Scale
    MatIdentity(Sboat);
    Sboat[0] = boatScale;
    Sboat[5] = boatScale;
    Sboat[10] = boatScale;

    // Rotation
    MatRotateY(Rboat, boatRotY + 180.0f);

    // Translation
    MatTranslate(Tboat, boatPosX, boatPosY, boatPosZ);

    // TR = T * R
    MatMul(TR, Tboat, Rboat);

    // gModel = (T*R) * S
    MatMul(gModel, TR, Sboat);


    // ======================================================
    // RIGHT OAR
    // ======================================================
    {
        float Rlocal[16], Tlocal[16], local[16];

        MatRotateZ(Rlocal, -rightRotZ);

        float Tpos[3] = { -0.03f, -0.15f, -0.3f };

        float x = Tpos[0], y = Tpos[1], z = Tpos[2];

        float rad = (boatRotY + 180.0f) * M_PI / 180.0f;
        float cosr = cos(rad), sinr = sin(rad);

        float rx = x * cosr - z * sinr;
        float rz = x * sinr + z * cosr;

        MatTranslate(Tlocal, rx, y, rz);

        MatMul(local, Tlocal, Rlocal);
        MatMul(gRightModel, gModel, local);
    }




    // ======================================================
    // LEFT OAR
    // ======================================================
    {
        float Rlocal[16], Tlocal[16], local[16];

        // 노 젓는 애니메이션
        MatRotateZ(Rlocal, leftRotZ);

        // LEFT 노의 로컬 위치
        float Tpos[3] = { 0.5f, 0.0f, -0.2f };

        float x = Tpos[0], y = Tpos[1], z = Tpos[2];

        // 보트 회전에 의해 좌표 회전
        float rad = (boatRotY + 180.0f) * M_PI / 180.0f;
        float cosr = cos(rad), sinr = sin(rad);

        float lx = x * cosr - z * sinr;
        float lz = x * sinr + z * cosr;

        // 변환행렬 적용
        MatTranslate(Tlocal, lx, y, lz);

        MatMul(local, Tlocal, Rlocal);
        MatMul(gLeftModel, gModel, local);
    }





}


// =========================================================
// Update()
// =========================================================
void BoatSystem::Update(float dt)
{
    UpdateBoatPhysics(dt);
    UpdateOars(dt);
    UpdateTransforms();
}


// =========================================================
// Render()
// =========================================================
void BoatSystem::Render()
{
    glUseProgram(shader);

    // Ground
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, groundModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.2f, 0.3f, 0.9f);
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Boat
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.8f, 0.8f, 0.8f);
    glBindVertexArray(boatVAO);
    glDrawArrays(GL_TRIANGLES, 0, boatMeshCount);

    // Right Oar
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gRightModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 1, 0.1, 0.1);
    glBindVertexArray(rightVAO);
    glDrawArrays(GL_TRIANGLES, 0, rightMeshCount);

    // Left Oar
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, gLeftModel);
    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.1, 0.1, 1.0);
    glBindVertexArray(leftVAO);
    glDrawArrays(GL_TRIANGLES, 0, leftMeshCount);
}


// =========================================================
// 입력 처리
// =========================================================
void BoatSystem::KeyDown(unsigned char key)
{
    if (key == 'z') // LEFT stroke
    {
        boatRotVel -= 0.3f;   // 왼쪽 stroke → 보트 오른쪽으로 돌려야 함
        speed += 0.3f;

        leftRotZ = oarPullAngle;
        leftVelZ = -oarSnapForce;
    }

    if (key == 'm') // RIGHT stroke
    {
        boatRotVel += 0.3f;   // 오른쪽 stroke → 보트 왼쪽으로 돌려야 함
        speed += 0.3f;

        rightRotZ = -oarPullAngle;
        rightVelZ = oarSnapForce;
    }

}

void BoatSystem::KeyUp(unsigned char key)
{
}

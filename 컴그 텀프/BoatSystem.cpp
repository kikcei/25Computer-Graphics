#include "BoatSystem.h"
#include "graphics.h"
#include "globals.h" 
#include <cmath>
#include <vector>

// =========================================================
// 내부 수학 유틸
// =========================================================

void BoatSystem::MatIdentity(float m[16])
{
    for (int i = 0; i < 16; i++) m[i] = 0;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void BoatSystem::MatTranslate(float m[16], float x, float y, float z)
{
    MatIdentity(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

void BoatSystem::MatRotateY(float m[16], float deg)
{
    float rad = deg * 3.141592f / 180.f;
    float c = cos(rad), s = sin(rad);

    MatIdentity(m);
    m[0] = c;   m[2] = s;
    m[8] = -s;  m[10] = c;
}

void BoatSystem::MatRotateZ(float m[16], float deg)
{
    float rad = deg * 3.141592f / 180.f;
    float c = cos(rad), s = sin(rad);

    MatIdentity(m);
    m[0] = c;   m[1] = -s;
    m[4] = s;   m[5] = c;
}

void BoatSystem::MatPerspective(float m[16], float fovy, float aspect, float zNear, float zFar)
{
    float t = tan(fovy * 3.141592f / 180.f / 2.f);
    MatIdentity(m);

    m[0] = 1.f / (aspect * t);
    m[5] = 1.f / t;
    m[10] = -(zFar + zNear) / (zFar - zNear);
    m[11] = -1.f;
    m[14] = -(2.f * zFar * zNear) / (zFar - zNear);
    m[15] = 0.f;
}

void BoatSystem::MatLookAt(float m[16], float ex, float ey, float ez,
    float cx, float cy, float cz,
    float ux, float uy, float uz)
{
    float fx = cx - ex, fy = cy - ey, fz = cz - ez;
    float rl = 1.0f / sqrt(fx * fx + fy * fy + fz * fz);
    fx *= rl; fy *= rl; fz *= rl;

    float sx = fy * uz - fz * uy;
    float sy = fz * ux - fx * uz;
    float sz = fx * uy - fy * ux;
    rl = 1.0f / sqrt(sx * sx + sy * sy + sz * sz);
    sx *= rl; sy *= rl; sz *= rl;

    float ux2 = sy * fz - sz * fy;
    float uy2 = sz * fx - sx * fz;
    float uz2 = sx * fy - sy * fx;

    MatIdentity(m);
    m[0] = sx;   m[4] = sy;   m[8] = sz;
    m[1] = ux2;  m[5] = uy2;  m[9] = uz2;
    m[2] = -fx;  m[6] = -fy;  m[10] = -fz;

    m[12] = -(sx * ex + sy * ey + sz * ez);
    m[13] = -(ux2 * ex + uy2 * ey + uz2 * ez);
    m[14] = (fx * ex + fy * ey + fz * ez);
}


// =========================================================
// 모델 빌드
// =========================================================

void BoatSystem::BuildModels()
{
    // 기존 graphics.cpp 기반으로 작성
    // VAO 빌드, 셰이더 로딩, 메쉬 로딩 직접 연결


    

    MatIdentity(gModel);
    MatIdentity(gLeftModel);
    MatIdentity(gRightModel);
    MatIdentity(groundModel);
}


// =========================================================
// 초기화
// =========================================================

void BoatSystem::Init()
{
    // globals.cpp 에서 만든 실제 VAO/Shader 가져오기
    boatVAO = ::boatVAO;
    leftVAO = ::leftVAO;
    rightVAO = ::rightVAO;
    groundVAO = ::groundVAO;

    // OBJ 로더가 채워준 메쉬 개수
    boatMeshCount = static_cast<int>(boatMesh.size());
    rightMeshCount = static_cast<int>(rightMesh.size());
    leftMeshCount = static_cast<int>(leftMesh.size());

    // globals 의 전역 shader 사용
    shader = ::shader;
}


// =========================================================
// Update
// =========================================================

void BoatSystem::UpdateBoatPhysics(float dt)
{
    // 기본 방향을 180도 뒤집어서 사용
    const float baseYaw = 180.0f;

    float rad = (boatRotY + baseYaw) * 3.141592f / 180.f;
    float dirX = -sin(rad);
    float dirZ = -cos(rad);

    velX = velX * 0.9f + dirX * speed * 0.1f;
    velZ = velZ * 0.9f + dirZ * speed * 0.1f;

    boatPosX -= velX;
    boatPosZ -= velZ;

    speed *= deceleration;
    if (speed < 0.0001f) speed = 0;
}

void BoatSystem::UpdateOars(float dt)
{
    rightRotZ += rightVelZ;
    rightVelZ *= 0.85f;
    rightVelZ += (0 - rightRotZ) * 0.02f;

    leftRotZ += leftVelZ;
    leftVelZ *= 0.85f;
    leftVelZ += (0 - leftRotZ) * 0.02f;
}

void BoatSystem::UpdateTransforms()
{
    const float baseYaw = 180.0f;

    // ========== Boat ==========
    // 기본 각도에 180도를 더해서 반대 방향 바라보게
    MatRotateY(gModel, boatRotY + baseYaw);

    const float s = boatScale;
    gModel[0] *= s; gModel[1] *= s; gModel[2] *= s;
    gModel[4] *= s; gModel[5] *= s; gModel[6] *= s;
    gModel[8] *= s; gModel[9] *= s; gModel[10] *= s;

    gModel[12] = boatPosX;
    gModel[13] = boatPosY;
    gModel[14] = boatPosZ;

    const float ps = paddleScale;
    // ========= Left Oar =========
    MatRotateZ(gLeftModel, leftRotZ);
    gLeftModel[0] *= ps; gLeftModel[1] *= ps; gLeftModel[2] *= ps;
    gLeftModel[4] *= ps; gLeftModel[5] *= ps; gLeftModel[6] *= ps;
    gLeftModel[8] *= ps; gLeftModel[9] *= ps; gLeftModel[10] *= ps;

    gLeftModel[12] = boatPosX + 0.45f;
    gLeftModel[13] = boatPosY - 0.05f;
    gLeftModel[14] = boatPosZ + 0.40f;

    // ========= Right Oar =========
    MatRotateZ(gRightModel, rightRotZ);
    gRightModel[0] *= ps; gRightModel[1] *= ps; gRightModel[2] *= ps;
    gRightModel[4] *= ps; gRightModel[5] *= ps; gRightModel[6] *= ps;
    gRightModel[8] *= ps; gRightModel[9] *= ps; gRightModel[10] *= ps;

    gRightModel[12] = boatPosX + 0.08f;
    gRightModel[13] = boatPosY - 0.22f;
    gRightModel[14] = boatPosZ + 0.27f;

   
}

void BoatSystem::Update(float dt)
{
    UpdateBoatPhysics(dt);
    UpdateOars(dt);
    UpdateTransforms();
}


// =========================================================
// 렌더링
// =========================================================

void BoatSystem::Render()
{
    glUseProgram(shader);

   
    // ===== 여기 아래부터는 원래 너 코드 그대로 =====
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
    if (key == 'w') speed += 0.03f;
    if (key == 's') speed -= 0.03f;
    if (key == 'f') boatRotY += 1.f;
    if (key == 'd') boatRotY -= 1.f;

    if (key == 'j') rightVelZ -= 1.0f;
    if (key == 'k') leftVelZ -= 1.0f;
}

void BoatSystem::KeyUp(unsigned char key)
{
    // 키에서 손 뗄 때 처리할 거 있으면 여기에
}

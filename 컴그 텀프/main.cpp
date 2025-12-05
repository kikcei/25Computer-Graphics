#include <gl/glew.h>
#include <gl/freeglut.h>
#include <ctime>

#include "Shader.h"                // shaderProgramID 생성
#include "ShapeBuffer.h"           // Stick/Pyramid VAO 생성
#include "Game_Manager.h"

// ----- 보트 -----
#include "graphics.h"
#include "globals.h"
#include "boat.h"
#include "BoatSystem.h"


// ----- 객체 -----
#include "Skybox.h"
#include "Floor.h"
#include "Stick.h"
#include "Pyramid.h"
#include "PyramidWall.h"
#include "BasicObstacle.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma warning(disable: 4711 4710 4100)


GLuint shaderProgramID;
BoatSystem boat;

float lastTime = 0.0f;

float cameraX = 0.0f;
float cameraZ = 3.0f;
float cameraYaw = 0.0f;
//------------------------------
GameManager gamemanager;
//------------------------------

void drawScene()
{
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --------------------------
    // 카메라 회전(Yaw 적용)
    // --------------------------
    float rad = glm::radians(cameraYaw);

    glm::vec3 forward;
    forward.x = sin(rad);
    forward.z = -cos(rad);
    forward.y = 0.0f;

    glm::vec3 eye = glm::vec3(cameraX, 1.5f, cameraZ);
    glm::vec3 center = eye + forward;

    glm::mat4 view = glm::lookAt(eye, center, glm::vec3(0, 1, 0));

    // --------------------------
    // 투영 행렬 (proj)
    // --------------------------
    glm::mat4 proj = glm::perspective(
        glm::radians(60.0f),
        1280.0f / 960.0f,
        0.1f,
        1000.0f
    );

    // --------------------------
    // 월드 오브젝트 그리기
    // --------------------------
    glUseProgram(shaderProgramID);
    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvp");
    gamemanager.Draw(view, proj, mvpLoc);

    // --------------------------
    // 보트 그리기
    // --------------------------
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, &proj[0][0]);

    boat.Render();

    glutSwapBuffers();
}

void Timer(int)
{
    static float last = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    float now = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    float dt = now - last;
    last = now;

    boat.Update(dt);
    gamemanager.Update(dt);

    glutTimerFunc(16, Timer, 0);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    boat.KeyDown(key);

	switch (key)
	{
    case 'w':
        cameraX += 3.0f;;
        break;
    case 's':
        cameraX -= 3.0f;;
        break;
    case 'a':
        cameraZ -= 3.0f;;
        break;
    case 'd':
        cameraZ += 3.0f;;
        break;
    case 'j':  // 왼쪽 회전
        cameraYaw -= 5.0f;
        break;
    case 'l':  // 오른쪽 회전
        cameraYaw += 5.0f;
        break;
	case 'q':
		exit(0);
		break;
	}
}


void KeyUpAll(unsigned char key, int x, int y)
{
    boat.KeyUp(key);
}

int main(int argc, char** argv)
{
   
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 960);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Rotating Obstacle Test");
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW 초기화 실패!\n");
        return -1;
    }
    srand(time(0));
    //------------------------------------------------------------------
    LoadAllOBJ();
    InitAllVAO();
    InitGroundVAO();

    shader = LoadShader("vertex_boat.glsl", "fragment_boat.glsl");
    boat.Init();
    //------------------------------------------------------------------

    shaderProgramID = make_shaderProgram();
    floorShaderID = loadTextureShader("texture_vertex.glsl", "texture_fragment.glsl");
    floorTexture = LoadTexture("water.png");

    skyShaderID = loadTextureShader("sky_vertex.glsl", "sky_fragment.glsl");
    skyTexture = LoadTexture("sky2.png");   // sky 텍스처 파일

    glUseProgram(shaderProgramID);
    InitStickModel();
    InitPyramidModel();
    InitFloorModel();
    InitSkyboxModel();
    InitMoveWallModel();
    InitTrapModel();

    gamemanager.LoadStage(1);

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboard);
    glutReshapeFunc([](int w, int h) { glViewport(0, 0, w, h); });
    glutTimerFunc(0, Timer, 0);
   
    glutKeyboardUpFunc(KeyUpAll);

    // 메인 루프
    glutMainLoop();
    return 0;
}

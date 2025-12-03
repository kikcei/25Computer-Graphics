#include <gl/glew.h>
#include <gl/freeglut.h>

#include "Shader.h"                // shaderProgramID 생성
#include "ShapeBuffer.h"           // Stick/Pyramid VAO 생성
#include "Game_Manager.h"

// ----- 보트 -----
#include "graphics.h"
#include "globals.h"
#include "boat.h"
#include "BoatSystem.h"


// ----- 객체 -----
#include "Floor.h"
#include "Stick.h"
#include "Pyramid.h"
#include "PyramidWall.h"
#include "BasicObstacle.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma warning(disable: 4711 4710 4100)

GLuint skyShaderID;
GLuint shaderProgramID;
BoatSystem boat;

float lastTime = 0.0f;
float cameraZ = 3.0f;

//------------------------------
GameManager gamemanager;
//------------------------------

void drawScene()
{
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 카메라
//-----------------------------------------------------------------------------------------
    // 1) 카메라: 메인에서 한 번만 계산
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 1.5, cameraZ +3),       // eye
        glm::vec3(0, 1.5, cameraZ),          // center
        glm::vec3(0, 1, 0));               // up

    glm::mat4 proj = glm::perspective(
        glm::radians(60.0f),
        1280.0f / 960.0f,
        0.1f,
        1000.0f);
//-----------------------------------------------------------------------------------------


    // 2) 장애물 + 바닥 그리기 (기존 shaderProgramID 사용)
    glUseProgram(shaderProgramID);
    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvp");
    gamemanager.Draw(view, proj, mvpLoc);

    // 3) 보트 그리기 (보트 shader 사용, 같은 view/proj 셋팅)
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, &proj[0][0]);

    boat.Render();  // ← 이제 여기서는 uModel/uColor/VAO만 사용

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
    case 'a':
        cameraZ -= 0.8f;;
        break;
    case 's':
        cameraZ += 0.8f;;
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

    glUseProgram(shaderProgramID);
    InitStickModel();
    InitPyramidModel();
    InitFloorModel();
    InitSkyboxModel();

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

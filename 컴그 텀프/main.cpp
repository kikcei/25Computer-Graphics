#include <gl/glew.h>
#include <gl/freeglut.h>

#include "Shader.h"                // shaderProgramID 생성
#include "ShapeBuffer.h"           // Stick/Pyramid VAO 생성
#include "Game_Manager.h"

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
float lastTime = 0.0f;

//------------------------------
GameManager gamemanager;
//------------------------------

void drawScene()
{
    
    // 배경색: 흰색
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);     //  흰색 배경
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

	glm::mat4 view = glm::lookAt(glm::vec3(0, 2.0f, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 proj = glm::perspective(glm::radians(60.0f),1280.0f / 960.0f,0.1f,100.0f);
    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvp");

	gamemanager.Draw(view, proj, mvpLoc);
    
    glutSwapBuffers();
}

void Timer(int)
{
    float currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f;  // ms → sec
    float dt = currentTime - lastTime;
    lastTime = currentTime;

	gamemanager.Update(dt);

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(0);
		break;
	}
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
    shaderProgramID = make_shaderProgram();
    glUseProgram(shaderProgramID);


    // Stick, Pyramid VAO 생성
    InitStickModel();
    InitPyramidModel();
	InitFloorModel( );

    gamemanager.LoadStage(1);

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboard);
    glutReshapeFunc([](int w, int h) { glViewport(0, 0, w, h); });
    glutTimerFunc(0, Timer, 0);
    // 메인 루프
    glutMainLoop();
    return 0;
}

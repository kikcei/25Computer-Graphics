#pragma once

#include <vector>
#include "Stick.h"
#include "Pyramid.h"

#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


class BasicObstacle_Right {
public:
    Stick body;
    std::vector<Pyramid> spikes;

    float basic_angle = 0.0f; // <- 도형 기본 회전값 추가
    float angle = 0.0f;   //  <- 회전값 추가!
   
    glm::vec3 basic_Axis = glm::vec3(0, 0, 1); // Z축 기준 회전
    glm::vec3 rotationAxis = glm::vec3(0, 0, 1); // Z축 기준 회전

    BasicObstacle_Right(glm::vec3 pos, float length, float spacing, glm::vec3 basicaxis = glm::vec3(0, 0, 1), glm::vec3 axis = glm::vec3(0, 0, 1), float basicangle = 0.0f, float initialAngle = 0.0f);
    void Draw_Right(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
    void Update_Right(float Time);
}; 

class BasicObstacle_Left {
public:
	Stick body;
	std::vector<Pyramid> spikes;

    float basic_angle = 0.0f; // <- 도형 기본 회전값 추가
    float angle = 0.0f;   //  <- 회전값 추가!
   
    glm::vec3 basic_Axis = glm::vec3(0, 0, 1); // Z축 기준 회전
    glm::vec3 rotationAxis = glm::vec3(0, 0, 1); // Z축 기준 회전

	BasicObstacle_Left(glm::vec3 pos, float length, float spacing, glm::vec3 basicaxis = glm::vec3(0, 0, 1), glm::vec3 axis = glm::vec3(0, 0, 1), float basicangle = 0.0f, float initialAngle = 0.0f);
	void Draw_Left(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc);
	void Update_Left(float Time);
};

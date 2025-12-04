#include "BasicObstacle.h"
#include "Pyramid.h"
#include "ShapeBuffer.h"

                                            // 직사각형 길이 , 가시 간격, 각도
BasicObstacle_Right::BasicObstacle_Right(glm::vec3 pos, float length, float spacing, glm::vec3 basicaxis, glm::vec3 axis, float basicangle, float initialAngle)
    : body(pos, glm::vec3(0.3f, 0.3f, length))   // Stick 클래스의 생성자를 호출한다
{
    basic_Axis = basicaxis;
    basic_angle = basicangle;
    angle = initialAngle;
    rotationAxis = axis;

    int count = length / spacing;

    for (int i = 0; i < count; i++)
    {
        float offset = (i - (count / 2.0f)) * spacing;        // 2.0f로 나누는 이유? -> 가운데 즉 중심 인덱스를 찾기 위해서 가운데를 기준으로 음수 -> 0 -> 양수 
  

        spikes.emplace_back(Pyramid(glm::vec3(pos.x , pos.y + 0.1f, pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f)));

        Pyramid right(glm::vec3(pos.x + 0.1f, pos.y , pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f));
        right.rotation = glm::vec3(0.0f, 0.0f, -90.0f);
        spikes.emplace_back(right);

        Pyramid bottom(glm::vec3(pos.x , pos.y - 0.1f, pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f));
        bottom.rotation = glm::vec3(180.0f, 0.0f, 0.0f);
        spikes.emplace_back(bottom);

        Pyramid left(glm::vec3(pos.x - 0.1f, pos.y , pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f));
        left.rotation = glm::vec3(0.0f, 0.0f, 90.0f);
        spikes.emplace_back(left);

    }
}

void BasicObstacle_Right::Draw_Right(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
    // --- 공통 회전 모델 행렬 ---
    glm::mat4 model = glm::mat4(1.0f);

    // 벽의 중심을 기준으로 회전
    model = glm::translate(model, body.pos);              // 1) 중심으로 이동
    model = glm::rotate(model, glm::radians(basic_angle), basic_Axis); // 기본 도형 회전
    model = glm::rotate(model, glm::radians(angle), rotationAxis); // 2) 회전
    model = glm::translate(model, -body.pos);             // 3) 다시 원래자리로 이동

    // ===== Stick 그리기 =====
    {
        glm::mat4 m = model;
        m = glm::translate(m, body.pos);
        m = glm::scale(m, body.size);
        glm::mat4 mvp = proj * view * m;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        glBindVertexArray(vao_stick);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // ===== Spikes(뿔) 그리기 =====
    for (auto& spike : spikes)
    {
        glm::mat4 m = model;
        m = glm::translate(m, spike.pos);

        // spike의 회전 (예: 아래쪽 뿔 180도)
        m = glm::rotate(m, glm::radians(spike.rotation.x), glm::vec3(1, 0, 0));
        m = glm::rotate(m, glm::radians(spike.rotation.y), glm::vec3(0, 1, 0));
        m = glm::rotate(m, glm::radians(spike.rotation.z), glm::vec3(0, 0, 1));

        m = glm::scale(m, spike.size);

        glm::mat4 mvp = proj * view * m;
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        glBindVertexArray(vao_pyramid);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }
}

void BasicObstacle_Right::Update_Right(float dt)
{
    angle += 60.0f * dt;   // 초당 60도 회전
}

//=============================================================================================================================================================

// 직사각형 길이 , 가시 간격, 각도
BasicObstacle_Left::BasicObstacle_Left(glm::vec3 pos, float length, float spacing, glm::vec3 basicaxis, glm::vec3 axis, float basicangle, float initialAngle)
	: body(pos, glm::vec3(0.3f, 0.3f, length))   // Stick 클래스의 생성자를 호출한다
{
    basic_Axis = basicaxis;
    basic_angle = basicangle;
    angle = initialAngle;
    rotationAxis = axis;

	int count = length / spacing;

	for (int i = 0; i < count; i++)
	{
		float offset = (i - (count / 2.0f)) * spacing;        // 2.0f로 나누는 이유? -> 가운데 즉 중심 인덱스를 찾기 위해서 가운데를 기준으로 음수 -> 0 -> 양수 


		spikes.emplace_back(Pyramid(glm::vec3(pos.x, pos.y + 0.1f, pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f)));

		Pyramid right(glm::vec3(pos.x + 0.1f, pos.y, pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f));
		right.rotation = glm::vec3(0.0f, 0.0f, -90.0f);
		spikes.emplace_back(right);

		Pyramid bottom(glm::vec3(pos.x, pos.y - 0.1f, pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f));
		bottom.rotation = glm::vec3(180.0f, 0.0f, 0.0f);
		spikes.emplace_back(bottom);

		Pyramid left(glm::vec3(pos.x - 0.1f, pos.y, pos.z + offset + spacing / 2.f), glm::vec3(0.2f, 0.25f, 0.2f));
		left.rotation = glm::vec3(0.0f, 0.0f, 90.0f);
		spikes.emplace_back(left);

	}
}

void BasicObstacle_Left::Draw_Left(const glm::mat4& view, const glm::mat4& proj, GLuint mvpLoc)
{
	// --- 공통 회전 모델 행렬 ---
	glm::mat4 model = glm::mat4(1.0f);

	// 벽의 중심을 기준으로 회전
	model = glm::translate(model, body.pos);              // 1) 중심으로 이동
    model = glm::rotate(model, glm::radians(basic_angle), basic_Axis); // 기본 도형 회전
    model = glm::rotate(model, glm::radians(angle), rotationAxis); // 2) 회전
	model = glm::translate(model, -body.pos);             // 3) 다시 원래자리로 이동

	// ===== Stick 그리기 =====
	{
		glm::mat4 m = model;
		m = glm::translate(m, body.pos);
		m = glm::scale(m, body.size);
		glm::mat4 mvp = proj * view * m;
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[ 0 ][ 0 ]);

		glBindVertexArray(vao_stick);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	// ===== Spikes(뿔) 그리기 =====
	for (auto& spike : spikes)
	{
		glm::mat4 m = model;
		m = glm::translate(m, spike.pos);

		// spike의 회전 (예: 아래쪽 뿔 180도)
		m = glm::rotate(m, glm::radians(spike.rotation.x), glm::vec3(1, 0, 0));
		m = glm::rotate(m, glm::radians(spike.rotation.y), glm::vec3(0, 1, 0));
		m = glm::rotate(m, glm::radians(spike.rotation.z), glm::vec3(0, 0, 1));

		m = glm::scale(m, spike.size);

		glm::mat4 mvp = proj * view * m;
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[ 0 ][ 0 ]);

		glBindVertexArray(vao_pyramid);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	}
}

void BasicObstacle_Left::Update_Left(float dt)
{
	angle -= 60.0f * dt;   // 초당 60도 회전
}
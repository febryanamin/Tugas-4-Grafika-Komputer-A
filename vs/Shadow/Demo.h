#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint depthmapShader, shadowmapShader, depthMapFBO, depthMap;
	GLuint VBO, VAO, EBO, texture, stexture,
		VBO1, VAO1, EBO1, texture1, stexture1,
		VBO2, VAO2, EBO2, texture2, stexture2,
		VBO3, VAO3, EBO3, texture3, stexture3,
		VBO4, VAO4, EBO4, texture4, stexture4,
		VBO5, VAO5, EBO5, texture5, stexture5,
		VBO6, VAO6, EBO6, texture6, stexture6,
		VBO7, VAO7, EBO7, texture7, stexture7,
		VBO8, VAO8, EBO8, texture8, stexture8;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildTexturedAtas();
	void BuildTexturedLantai();
	void BuildTexturedKakiKananBelakang();
	void BuildTexturedKakiKiriBelakang();
	void BuildTexturedKakiKiriDepan();
	void BuildTexturedKakiKananDepan();
	void BuildTexturedBelakangBawah();
	void BuildTexturedKananBawah();
	void BuildTexturedKiriBawah();
	void DrawTexturedAtas(GLuint shader);
	void DrawTexturedKakiKananBelakang(GLuint shader);
	void DrawTexturedKakiKiriBelakang(GLuint shader);
	void DrawTexturedKakiKiriDepan(GLuint shader);
	void DrawTexturedKakiKananDepan(GLuint shader);
	void DrawTexturedBelakangBawah(GLuint shader);
	void DrawTexturedKananBawah(GLuint shader);
	void DrawTexturedKiriBawah(GLuint shader);
	void DrawTexturedLantai(GLuint shader);
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
	void BuildDepthMap();
	void BuildShaders();
};


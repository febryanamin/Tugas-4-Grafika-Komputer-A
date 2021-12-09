#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	BuildShaders();

	BuildDepthMap();
	
	BuildTexturedAtas();
	
	BuildTexturedKakiKananBelakang();

	BuildTexturedKakiKiriBelakang();

	BuildTexturedKakiKiriDepan();

	BuildTexturedKakiKananDepan();

	BuildTexturedBelakangBawah();

	BuildTexturedKananBawah();
	
	BuildTexturedKiriBawah();
	
	BuildTexturedLantai();
	
	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);

	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);

	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);

	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &EBO5);

	glDeleteVertexArrays(1, &VAO6);
	glDeleteBuffers(1, &VBO6);
	glDeleteBuffers(1, &EBO6);

	glDeleteVertexArrays(1, &VAO7);
	glDeleteBuffers(1, &VBO7);
	glDeleteBuffers(1, &EBO7);

	glDeleteVertexArrays(1, &VAO8);
	glDeleteBuffers(1, &VBO8);
	glDeleteBuffers(1, &EBO8);

	glDeleteBuffers(1, &depthMapFBO);

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);

}

void Demo::Update(double deltaTime) {
	
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Step 1 Render depth of scene to texture
	// ----------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(-2.0f, 6.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 2.0,0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	UseShader(this->depthmapShader);
	glUniformMatrix4fv(glGetUniformLocation(this->depthmapShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glViewport(0, 0, this->SHADOW_WIDTH, this->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawTexturedAtas(this->depthmapShader);	
	DrawTexturedKakiKananBelakang(this->depthmapShader);
	DrawTexturedKakiKiriBelakang(this->depthmapShader);
	DrawTexturedKakiKiriDepan(this->depthmapShader);
	DrawTexturedKakiKananDepan(this->depthmapShader);
	DrawTexturedBelakangBawah(this->depthmapShader);
	DrawTexturedKananBawah(this->depthmapShader);
	DrawTexturedKiriBawah(this->depthmapShader);
	DrawTexturedLantai(this->depthmapShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Step 2 Render scene normally using generated depth map
	// ------------------------------------------------------
	glViewport(0, 0, this->screenWidth, this->screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pass perspective projection matrix
	UseShader(this->shadowmapShader);
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// LookAt camera (position, target/direction, up)
	glm::vec3 cameraPos = glm::vec3(posCamX, posCamY, posCamZ);
	glm::vec3 cameraFront = glm::vec3(viewCamX, viewCamY, viewCamZ);
	glm::mat4 view = glm::lookAt(cameraPos, cameraFront, glm::vec3(0, 3, 0));
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Setting Light Attributes
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "lightPos"), -2.0f, 10.0f, -1.0f);

	// Configure Shaders
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "shadowMap"), 1);
	
	// Render Meja
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedAtas(this->shadowmapShader);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedKakiKananBelakang(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedKakiKiriBelakang(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedKakiKiriDepan(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedKakiKananDepan(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedBelakangBawah(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedKananBawah(this->shadowmapShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedKiriBawah(this->shadowmapShader);
	
	// Render Lantai
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedLantai(this->shadowmapShader);

	glDisable(GL_DEPTH_TEST);

}

void Demo::BuildTexturedAtas() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja_diffusemap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture);
	glBindTexture(GL_TEXTURE_2D, stexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("meja_specularmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		// front
		-1.9, 2.0, 3.5, 0, 0, 0.0f,  0.0f,  1.0f, // 0
		1.9,  2.0, 3.5, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		1.9,  2.3, 3.5, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-1.9, 2.3, 3.5, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		1.9,  2.0,  3.5,  0, 0, 1.0f,  0.0f,  0.0f, // 4
		1.9,  2.3,  3.5,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		1.9,  2.3,  -3.0, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		1.9,  2.0,  -3.0, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-1.9, 2.3, -3.0, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		1.9,  2.3, -3.0, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		1.9,  2.0, -3.0, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-1.9, 2.0, -3.0, 0, 1, 0.0f,  0.0f,  -1.0f, // 11
		
		// left
		-1.9, 2.0, 3.5,  0, 0, -1.0f,  0.0f,  0.0f, // 12
		-1.9, 2.3, 3.5,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		-1.9, 2.3, -3.0, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-1.9, 2.0, -3.0, 0, 1, -1.0f,  0.0f,  0.0f, // 15
		
		// upper
		-1.9, 2.3, 3.5,  0, 0, 0.0f,  1.0f,  0.0f, // 16
		1.9,  2.3, 3.5,  1, 0, 0.0f,  1.0f,  0.0f, // 17
		1.9,  2.3, -3.0, 1, 1, 0.0f,  1.0f,  0.0f, // 18
		-1.9, 2.3, -3.0, 0, 1, 0.0f,  1.0f,  0.0f, // 19

		// bottom
		-1.9, 2.0, -3.0, 0, 0, 0.0f,  -1.0f,  0.0f, // 20
		1.9,  2.0, -3.0, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		1.9,  2.0, 3.5,  1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-1.9, 2.0, 3.5,  0, 1, 0.0f,  -1.0f,  0.0f, // 23

	};

	unsigned int indices[] = {
	    0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedAtas(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedKakiKananBelakang() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		1.4, -1.0, 3.4, 0, 0,  0.0f,  0.0f,  1.0f, // 0
		1.7,  -1.0, 3.4, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		1.7,  2.1, 3.4, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		1.4, 2.1, 3.4, 0, 1,  0.0f,  0.0f,  1.0f, // 3
		
		// right
		1.7,  -1.0,  3.4,  0, 0, 1.0f,  0.0f,  0.0f, // 4
		1.7,  2.1,  3.4,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		1.7,  2.1,  3.1, 1, 1,  1.0f,  0.0f,  0.0f, // 6
		1.7,  -1.0,  3.1, 0, 1,  1.0f,  0.0f,  0.0f, // 7
		
		// back
		1.4, 2.1, 3.1, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		1.7,  2.1, 3.1, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		1.7,  -1.0, 3.1, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		1.4, -1.0, 3.1, 0, 1,  0.0f,  0.0f,  -1.0f, // 11
		
		// left
		1.4, -1.0, 3.4,  0, 0, -1.0f,  0.0f,  0.0f, // 12
		1.4, 2.1, 3.4,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		1.4, 2.1, 3.1, 1, 1,  -1.0f,  0.0f,  0.0f, // 14
		1.4, -1.0, 3.1, 0, 1,  -1.0f,  0.0f,  0.0f, // 15

		// upper
		1.4, 2.1, 3.4,  0, 0,  0.0f,  1.0f,  0.0f, // 16
		1.7,  2.1, 3.4,  1, 0, 0.0f,  1.0f,  0.0f, // 17
		1.7,  2.1, 3.1, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		1.4, 2.1, 3.1, 0, 1,   0.0f,  1.0f,  0.0f, // 19

		// bottom
		1.4, -1.0, 3.1, 0, 0,   0.0f,  -1.0f,  0.0f, // 20
		1.7,  -1.0, 3.1, 1, 0,  0.0f,  -1.0f,  0.0f, // 21
		1.7,  -1.0, 3.4,  1, 1, 0.0f,  -1.0f,  0.0f, // 22
		1.4, -1.0, 3.4,  0, 1,  0.0f,  -1.0f,  0.0f, // 23
		
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedKakiKananBelakang(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedKakiKiriBelakang() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		1.4, -1.0, -2.6, 0, 0,  0.0f,  0.0f,  1.0f, // 0
		1.7,  -1.0, -2.6, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		1.7,  2.1, -2.6, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		1.4, 2.1, -2.6, 0, 1,  0.0f,  0.0f,  1.0f, // 3

		// right
		1.7,  -1.0,  -2.6,  0, 0, 1.0f,  0.0f,  0.0f, // 4
		1.7,  2.1,  -2.6,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		1.7,  2.1,  -2.9, 1, 1,  1.0f,  0.0f,  0.0f, // 6
		1.7,  -1.0,  -2.9, 0, 1,  1.0f,  0.0f,  0.0f, // 7

		// back
		1.4, 2.1, -2.9, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		1.7,  2.1, -2.9, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		1.7,  -1.0,-2.9, 1, 1,  0.0f,  0.0f,  -1.0f, // 10
		1.4, -1.0, -2.9, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left
		1.4, -1.0, -2.6,  0, 0, -1.0f,  0.0f,  0.0f, // 12
		1.4, 2.1, -2.6,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		1.4, 2.1, -2.9, 1, 1,  -1.0f,  0.0f,  0.0f, // 14
		1.4, -1.0, -2.9, 0, 1,  -1.0f,  0.0f,  0.0f, // 15

		// upper
		1.4, 2.1, -2.6,  0, 0,  0.0f,  1.0f,  0.0f, // 16
		1.7,  2.1, -2.6,  1, 0, 0.0f,  1.0f,  0.0f, // 17
		1.7,  2.1, -2.9, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		1.4, 2.1, -2.9, 0, 1,   0.0f,  1.0f,  0.0f, // 19

		// bottom
		1.4, -1.0, -2.9, 0, 0,  0.0f,  -1.0f,  0.0f, // 20
		1.7,  -1.0, -2.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 21
		1.7,  -1.0, -2.6, 1, 1, 0.0f,  -1.0f,  0.0f, // 22
		1.4, -1.0, -2.6,  0, 1, 0.0f,  -1.0f,  0.0f, // 23

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedKakiKiriBelakang(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedKakiKiriDepan() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1.8, -1.0, -2.6, 0, 0,  0.0f,  0.0f,  1.0f, // 0
		-1.5,  -1.0, -2.6, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		-1.5,  2.1, -2.6, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-1.8, 2.1, -2.6, 0, 1,  0.0f,  0.0f,  1.0f, // 3

		// right
		-1.5,  -1.0,  -2.6,  0, 0, 1.0f,  0.0f,  0.0f, // 4
		-1.5,  2.1,  -2.6,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		-1.5,  2.1,  -2.9, 1, 1,  1.0f,  0.0f,  0.0f, // 6
		-1.5,  -1.0,  -2.9, 0, 1,  1.0f,  0.0f,  0.0f, // 7

		// back
		-1.8, 2.1, -2.9, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		-1.5,  2.1, -2.9, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		-1.5,  -1.0, -2.9, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-1.8, -1.0, -2.9, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left
		-1.8, -1.0, -2.6,  0, 0, -1.0f,  0.0f,  0.0f, // 12
		-1.8, 2.1, -2.6,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		-1.8, 2.1, -2.9, 1, 1,  -1.0f,  0.0f,  0.0f, // 14
		-1.8, -1.0, -2.9, 0, 1,  -1.0f,  0.0f,  0.0f, // 15

		// upper
		-1.8, 2.1, -2.6,  0, 0,  0.0f,  1.0f,  0.0f, // 16
		-1.5,  2.1, -2.6,  1, 0, 0.0f,  1.0f,  0.0f, // 17
		-1.5,  2.1, -2.9, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-1.8, 2.1, -2.9, 0, 1,   0.0f,  1.0f,  0.0f, // 19

		// bottom
		-1.8, -1.0, -2.9, 0, 0,   0.0f,  -1.0f,  0.0f, // 20
		-1.5,  -1.0, -2.9, 1, 0,  0.0f,  -1.0f,  0.0f, // 21
		-1.5,  -1.0, -2.6,  1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-1.8, -1.0, -2.6,  0, 1,  0.0f,  -1.0f,  0.0f, // 23

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedKakiKiriDepan(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedKakiKananDepan() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1.8, -1.0, 3.4, 0, 0,  0.0f,  0.0f,  1.0f, // 0
		-1.5,  -1.0, 3.4, 1, 0, 0.0f,  0.0f,  1.0f, // 1
		-1.5,  2.1, 3.4, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-1.8, 2.1, 3.4, 0, 1,  0.0f,  0.0f,  1.0f, // 3

		// right
		-1.5,  -1.0,  3.4,  0, 0, 1.0f,  0.0f,  0.0f, // 4
		-1.5,  2.1,  3.4,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		-1.5,  2.1,  3.1, 1, 1,  1.0f,  0.0f,  0.0f, // 6
		-1.5,  -1.0,  3.1, 0, 1,  1.0f,  0.0f,  0.0f, // 7

		// back
		-1.8, 2.1, 3.1, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		-1.5,  2.1, 3.1, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		-1.5,  -1.0, 3.1, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-1.8, -1.0, 3.1, 0, 1,  0.0f,  0.0f,  -1.0f, // 11

		// left
		-1.8, -1.0, 3.4,  0, 0, -1.0f,  0.0f,  0.0f, // 12
		-1.8, 2.1, 3.4,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		-1.8, 2.1, 3.1, 1, 1,  -1.0f,  0.0f,  0.0f, // 14
		-1.8, -1.0, 3.1, 0, 1,  -1.0f,  0.0f,  0.0f, // 15

		// upper
		-1.8, 2.1, 3.4,  0, 0,  0.0f,  1.0f,  0.0f, // 16
		-1.5,  2.1, 3.4,  1, 0, 0.0f,  1.0f,  0.0f, // 17
		-1.5,  2.1, 3.1, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-1.8, 2.1, 3.1, 0, 1,   0.0f,  1.0f,  0.0f, // 19

		// bottom
		-1.8, -1.0, 3.1, 0, 0,   0.0f,  -1.0f,  0.0f, // 20
		-1.5,  -1.0, 3.1, 1, 0,  0.0f,  -1.0f,  0.0f, // 21
		-1.5,  -1.0, 3.4,  1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-1.8, -1.0, 3.4,  0, 1,  0.0f,  -1.0f,  0.0f, // 23

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedKakiKananDepan(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedBelakangBawah() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		1.5, -0.5, 3.1, 0, 0,   0.0f,  0.0f,  1.0f, // 0
		1.7, -0.5, 3.1, 1, 0,   0.0f,  0.0f,  1.0f, // 1
		1.7, 0.1, 3.1, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		1.5, 0.1, 3.1, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		1.7,  -0.5,  3.1,  0, 0,   1.0f,  0.0f,  0.0f, // 4
		1.7,  0.1,  3.1,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		1.7,  0.1,  -2.6, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		1.7,  -0.5,  -2.6, 0, 1,   1.0f,  0.0f,  0.0f, // 7

		// back
		1.5, 0.1, -2.8, 0, 0,  0.0f,  0.0f,  -1.0f, // 8 
		1.7,  0.1, -2.8, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		1.7,  -0.5, -2.8, 1, 1,   0.0f,  0.0f,  -1.0f, // 10
		1.5, -0.5, -2.8, 0, 1,    0.0f,  0.0f,  -1.0f, // 11

		// left
		1.5, -0.5, 3.1,  0, 0,   -1.0f,  0.0f,  0.0f, // 12
		1.5, 0.1, 3.1,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		1.5, 0.1, -2.8, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		1.5, -0.5, -2.8, 0, 1,   -1.0f,  0.0f,  0.0f, // 15

		// upper
		1.5, 0.1, 3.1,  0, 0,  0.0f,  1.0f,  0.0f, // 16
		1.7,  0.1, 3.1,  1, 0, 0.0f,  1.0f,  0.0f, // 17
		1.7,  0.1, -2.8, 1, 1, 0.0f,  1.0f,  0.0f, // 18
		1.5, 0.1, -2.8, 0, 1,  0.0f,  1.0f,  0.0f,	 // 19

		// bottom
		1.5, -0.5, -2.8, 0, 0,  0.0f,  -1.0f,  0.0f, // 20
		1.7,  -0.5, -2.8, 2, 0, 0.0f,  -1.0f,  0.0f, // 21
		1.7,  -0.5, 3.1,  2, 2, 0.0f,  -1.0f,  0.0f, // 22
		1.5, -0.5, 3.1,  0, 2,  0.0f,  -1.0f,  0.0f, // 23

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedBelakangBawah(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedKananBawah() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1.5, -0.5, 3.4, 0, 0,   0.0f,  0.0f,  1.0f, // 0
		1.4,  -0.5, 3.4, 1, 0,   0.0f,  0.0f,  1.0f, // 1
		1.4,  0.1, 3.4, 1, 1, 0.0f,  0.0f,  1.0f, // 2
		-1.5, 0.1, 3.4, 0, 1, 0.0f,  0.0f,  1.0f, // 3

		// right
		1.4,  -0.5,  3.4,  0, 0,   1.0f,  0.0f,  0.0f, // 4
		1.4,  0.1,  3.4,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		1.4,  0.1,  3.2, 1, 1,  1.0f,  0.0f,  0.0f, // 6
		1.4,  -0.5,  3.2, 0, 1,    1.0f,  0.0f,  0.0f, // 7

		// back
		-1.5, 0.1, 3.2, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		1.4,  0.1, 3.2, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		1.4,  -0.5, 3.2, 1, 1,   0.0f,  0.0f,  -1.0f, // 10
		-1.5, -0.5, 3.2, 0, 1,   0.0f,  0.0f,  -1.0f, // 11

		// left
		-1.5, -0.5, 3.4,  0, 0,   -1.0f,  0.0f,  0.0f, // 12
		-1.5, 0.1, 3.4,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		-1.5, 0.1, 3.2, 1, 1,  -1.0f,  0.0f,  0.0f, // 14
		-1.5, -0.5, 3.2, 0, 1,    -1.0f,  0.0f,  0.0f, // 15

		// upper
		-1.5, 0.1, 3.4,  0, 0,  0.0f,  1.0f,  0.0f, // 16
		1.4,  0.1, 3.4,  1, 0,  0.0f,  1.0f,  0.0f, // 17
		1.4,  0.1, 3.2, 1, 1,   0.0f,  1.0f,  0.0f, // 18
		-1.5, 0.1, 3.2, 0, 1,   0.0f,  1.0f,  0.0f, // 19

		// bottom
		-1.5, -0.5, 3.2, 0, 0,  0.0f,  -1.0f,  0.0f, // 20
		1.4,  -0.5, 3.2, 1, 0,  0.0f,  -1.0f,  0.0f, // 21
		1.4,  -0.5, 3.4,  1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-1.5, -0.5, 3.4,  0, 1, 0.0f,  -1.0f,  0.0f, // 23

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedKananBawah(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedKiriBawah() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("meja.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1.5, -0.5, -2.8, 0, 0,   0.0f,  0.0f,  -1.0f, // 0
		1.4,  -0.5, -2.8, 1, 0,   0.0f,  0.0f,  -1.0f, // 1
		1.4,  0.1, -2.8, 1, 1, 0.0f,  0.0f,  -1.0f, // 2
		-1.5, 0.1, -2.8, 0, 1, 0.0f,  0.0f,  -1.0f, // 3

		// right
		1.4,  -0.5,  -2.8,  0, 0,   1.0f,  0.0f,  0.0f, // 4
		1.4,  0.1,  -2.8,  1, 0, 1.0f,  0.0f,  0.0f, // 5
		1.4,  0.1,  -2.6, 1, 1,  1.0f,  0.0f,  0.0f, // 6
		1.4,  -0.5,  -2.6, 0, 1,    1.0f,  0.0f,  0.0f, // 7

		// back
		-1.5, 0.1, -2.6, 0, 0, 0.0f,  0.0f,  1.0f, // 8 
		1.4,  0.1, -2.6, 1, 0, 0.0f,  0.0f,  1.0f, // 9
		1.4,  -0.5, -2.6, 1, 1,   0.0f,  0.0f,  1.0f, // 10
		-1.5, -0.5, -2.6, 0, 1,   0.0f,  0.0f,  1.0f, // 11

		// left
		-1.5, -0.5, -2.6,  0, 0,   -1.0f,  0.0f,  0.0f, // 12
		-1.5, 0.1, -2.6,  1, 0, -1.0f,  0.0f,  0.0f, // 13
		-1.5, 0.1, -2.6, 1, 1,  -1.0f,  0.0f,  0.0f, // 14
		-1.5, -0.5, -2.6, 0, 1,    -1.0f,  0.0f,  0.0f, // 15

		// upper
		-1.5, 0.1, -2.8,  0, 0, 0.0f,  1.0f,  0.0f, // 16
		1.4,  0.1, -2.8,  1, 0, 0.0f,  1.0f,  0.0f, // 17
		1.4,  0.1, -2.6, 1, 1,  0.0f,  1.0f,  0.0f, // 18
		-1.5, 0.1, -2.6, 0, 1,  0.0f,  1.0f,  0.0f, // 19

		// bottom
		-1.5, -0.5, -2.6, 0, 0,  0.0f,  -1.0f,  0.0f, // 20
		1.4,  -0.5, -2.6, 1, 0,  0.0f,  -1.0f,  0.0f, // 21
		1.4,  -0.5, -2.8,  1, 1, 0.0f,  -1.0f,  0.0f, // 22
		-1.5, -0.5, -2.8,  0, 1, 0.0f,  -1.0f,  0.0f, // 23

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  10,  9, 8,  11, 10,  // back
		12, 13, 14, 12, 14, 15,  // left
		16, 17, 18, 16, 18, 19,  // upper
		20, 22, 21, 20, 23, 22   // bottom 
	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTexturedKiriBawah(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedLantai()
{
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lantai_diffusemap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &stexture8);
	glBindTexture(GL_TEXTURE_2D, stexture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("lantai_specularmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0, 0.0f,  1.0f,  0.0f,
		 50.0, -0.5, -50.0, 50,  0, 0.0f,  1.0f,  0.0f,
		 50.0, -0.5,  50.0, 50, 50, 0.0f,  1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0, 50, 0.0f,  1.0f,  0.0f,

	};
	 
	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);

	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawTexturedLantai(GLuint shader)
{
	UseShader(shader);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 7.0f;
	posCamZ = 10.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

void Demo::BuildDepthMap() {
	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->SHADOW_WIDTH, this->SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Demo::BuildShaders()
{
	// build and compile our shader program
	// ------------------------------------
	shadowmapShader = BuildShader("shadowMapping.vert", "shadowMapping.frag", nullptr);
	depthmapShader = BuildShader("depthMap.vert", "depthMap.frag", nullptr);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Meja - Shadow Mapping", 800, 600, false, false);
}
#include "ProceduralCity.h"
#include "ProceduralTools.h"
#include "CityObjects.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

ProceduralCity::ProceduralCity()
{
}

ProceduralCity::~ProceduralCity()
{
}

void ProceduralCity::Init()
{
	{
		renderCameraTarget = false;
		fovStep = 60;
		zNear = 0.08f;
		zFar = 200.0f;
		orthoStep = 10;
	}
	{
		gridWidth = 32.0f;
		gridHeight = 32.0f;
	}
	{
		camera = new Camera();
		camera->Set(glm::vec3(-2.52f, 3.0f, -1.5f), glm::vec3(1.8f, 1, 3), glm::vec3(0, 1, 0));
		camera->SetProjectionMatrix(fovStep, window->props.aspectRatio, zNear, zFar);
	}
	{
		lightPosition = glm::vec3(gridWidth/2, 15.0f, gridHeight/2);
		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 32;
		materialKa = 0.75f;
		materialKd = 0.5f;
		materialKs = 0.7f;
		attenuation_factor = 0.006f;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema3/Textures/grass.jpg", GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema3/Textures/concrete.jpg", GL_REPEAT);
		mapTextures["concrete"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema3/Textures/windows.jpg", GL_REPEAT);
		mapTextures["windows"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema3/Textures/sidewalk.jpg", GL_REPEAT);
		mapTextures["sidewalk"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema3/Textures/roadH.jpg", GL_REPEAT);
		mapTextures["roadH"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema3/Textures/wood.jpg", GL_REPEAT);
		mapTextures["wood"] = texture;
	}

	{
		Shader *shader = new Shader("CityShader");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}


	Mesh* CityPlane = CreatePlane("CityPlane", gridWidth, gridHeight);
	AddMeshToList(CityPlane);
	CityMap = generateStreets(0, gridHeight, unit);
	roadNetwork = CityMap.first;
	usableRectangles = CityMap.second;
	
	for (int i = 0; i < usableRectangles.size(); i++) {
		std::vector <glm::vec3> points = usableRectangles[i];
		float area = length(points[0] - points[1]) * length(points[2] - points[0]);
		if(area > 4)
			recursiveFootprintSearch(points[0], points[3], buildingZones);
		else
			rectangles.push_back(CreateRectangle("GreenZone", points[0], points[1], points[2], points[3], unit));
	}

	for (int i = 0; i < buildingZones.size(); i++) {
		float area = length(buildingZones[i].first - glm::vec3(buildingZones[i].first.x, buildingZones[i].first.y, buildingZones[i].second.z)) * 
			length(buildingZones[i].second - glm::vec3(buildingZones[i].second.x, buildingZones[i].second.y, buildingZones[i].first.z));
		int randomHeight = randomRange(1, 4);
		glm::vec3 center = glm::vec3((buildingZones[i].first.x + buildingZones[i].second.x) / 2, buildingZones[i].first.y, (buildingZones[i].first.z + buildingZones[i].second.z) / 2);
		
		if (area > 4) {
			Building b = Building("Building", glm::vec3(buildingZones[i].first.x + 0.05f, buildingZones[i].first.y, buildingZones[i].first.z + 0.05f), 
				glm::vec3(buildingZones[i].second.x - 0.05f, buildingZones[i].second.y, buildingZones[i].second.z - 0.05f), randomHeight, buildingType::Cuboid, 
				CreateCuboid("Building", glm::vec3(buildingZones[i].first.x + 0.05f, buildingZones[i].first.y, buildingZones[i].first.z + 0.05f),
					glm::vec3(buildingZones[i].second.x - 0.05f, buildingZones[i].second.y, buildingZones[i].second.z - 0.05f),
					randomHeight));
			buildings.push_back(b);
			rooftops.push_back(CreateCuboidRoofs("Roof", glm::vec3(buildingZones[i].first.x + 0.05f, buildingZones[i].first.y, buildingZones[i].first.z + 0.05f),
				glm::vec3(buildingZones[i].second.x - 0.05f, buildingZones[i].second.y, buildingZones[i].second.z - 0.05f),
				randomHeight));
		}
		else {
			int randBuild = randomRange(1, 2);
			if (randBuild == 1) {
				Building b = Building("Building", glm::vec3(buildingZones[i].first.x + 0.05f, buildingZones[i].first.y, buildingZones[i].first.z + 0.05f),
					glm::vec3(buildingZones[i].second.x - 0.05f, buildingZones[i].second.y, buildingZones[i].second.z - 0.05f), randomHeight, buildingType::Cylinder,
					CreateCylinder("Building", center, (buildingZones[i].second.z - buildingZones[i].first.z) / 2 - 0.1f, randomHeight));
				buildings.push_back(b);
				rooftops.push_back(CreateCylinderTop("Roof", center, (buildingZones[i].second.z - buildingZones[i].first.z) / 2 - 0.1f, randomHeight));
			}
			else {
				Building b = Building("Building", glm::vec3(buildingZones[i].first.x + 0.05f, buildingZones[i].first.y, buildingZones[i].first.z + 0.05f),
					glm::vec3(buildingZones[i].second.x - 0.05f, buildingZones[i].second.y, buildingZones[i].second.z - 0.05f), randomHeight, buildingType::Cuboid,
					CreateCuboid("Building", glm::vec3(buildingZones[i].first.x + 0.05f, buildingZones[i].first.y, buildingZones[i].first.z + 0.05f),
						glm::vec3(buildingZones[i].second.x - 0.05f, buildingZones[i].second.y, buildingZones[i].second.z - 0.05f),
						randomHeight));
				buildings.push_back(b);
				rooftops.push_back(CreateCuboidRoofs("Roof", glm::vec3(buildingZones[i].first.x + 0.05f, buildingZones[i].first.y, buildingZones[i].first.z + 0.05f),
					glm::vec3(buildingZones[i].second.x - 0.05f, buildingZones[i].second.y, buildingZones[i].second.z - 0.05f),
					randomHeight));
			}
		}
	}
}

void ProceduralCity::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void ProceduralCity::Update(float deltaTimeSeconds)
{

	glm::mat4 modelMatrix = glm::mat4(1);
	RenderMesh(meshes["CityPlane"], shaders["CityShader"], modelMatrix, mapTextures["sidewalk"]);

	for (int i = 0; i < roadNetwork.size(); i++) {
		RenderMesh(roadNetwork[i], shaders["CityShader"], modelMatrix, mapTextures["roadH"]);
	}

	for (int i = 0; i < rectangles.size(); i++) {
		RenderMesh(rectangles[i], shaders["CityShader"], modelMatrix, mapTextures["grass"]);
	}

	for (int i = 0; i < buildings.size(); i++) {
		RenderMesh(buildings[i].getPrimitives(), shaders["CityShader"], modelMatrix, mapTextures["windows"]);
		RenderMesh(rooftops[i], shaders["CityShader"], modelMatrix, mapTextures["concrete"]);
	}

	

}

void ProceduralCity::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), camera->getProjectionMatrix());
}

void ProceduralCity::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, Texture2D *texture)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();

	glUniform3f(glGetUniformLocation(shader->program, "light_position"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(glGetUniformLocation(shader->program, "light_direction"), lightDirection.x, lightDirection.y, lightDirection.z);
	glm::vec3 eyePosition = camera->getPosition();
	glUniform3f(glGetUniformLocation(shader->program, "eye_position"), eyePosition.x, eyePosition.y, eyePosition.z);
	glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
	glUniform1f(glGetUniformLocation(shader->program, "material_ka"), materialKa);
	glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
	glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
	glUniform1f(glGetUniformLocation(shader->program, "attenuation_factor"), attenuation_factor);

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	texture->BindToTextureUnit(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void ProceduralCity::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 1.5f;

		if (window->KeyHold(GLFW_KEY_W)){
			camera->TranslateForward(deltaTime * 4 * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-deltaTime * 4 * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-deltaTime * 4 * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(deltaTime * 4 * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(deltaTime * 4 * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(-deltaTime * 4 * cameraSpeed);
		}
	}
}

void ProceduralCity::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}

}

void ProceduralCity::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void ProceduralCity::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
		}
	}
}

void ProceduralCity::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void ProceduralCity::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}

void ProceduralCity::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void ProceduralCity::OnWindowResize(int width, int height)
{
}

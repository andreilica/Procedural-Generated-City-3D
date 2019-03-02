#pragma once
#include <Component/SimpleScene.h>
#include "CityCamera.h"
#include "ProceduralTools.h"
#include "3DTransform.h"
#include <Core/Engine.h>

struct SpotLight {
	glm::vec3 light_direction;
	glm::vec3 light_position;
	float material_kd;
	float material_ka;
	float material_ks;
	int material_shininess;
	float attenuation_factor;
	float cutoff_angle;
};

class ProceduralCity : public SimpleScene
{
	public:
		ProceduralCity();
		~ProceduralCity();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, Texture2D *texture = NULL);
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Camera *camera;
		std::unordered_map<std::string, Texture2D*> mapTextures;
		bool renderCameraTarget;
		bool perspective, ortho;

		float fovStep, zNear, zFar;
		double orthoStep;
		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		float materialKd, materialKs, materialKa;
		float attenuation_factor;

		const float unit = 0.25f;
		float gridWidth, gridHeight;
		std::pair < std::vector <Mesh*>, std::vector < std::vector<glm::vec3>> > CityMap;
		std::vector<Mesh*> roadNetwork;
		std::vector< std::vector <glm::vec3> > usableRectangles;
		std::vector <Mesh*> rectangles;
		std::vector<std::pair <glm::vec3, glm::vec3>> buildingZones;
		std::vector <Building> buildings;
		std::vector <Mesh*> rooftops;
		
};

#pragma once
#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <time.h>
#include "CityObjects.h"

enum class buildingType { Cube, Cylinder, Cuboid };

int randomRange(int min, int max);
std::pair < std::vector <Mesh*>, std::vector < std::vector<glm::vec3>> > generateStreets(int lowBound, int highBound, float unit);
std::vector< std::vector<glm::vec3> > generateGrid(int lowBound, int highBound);
void recursiveFootprintSearch(glm::vec3 start, glm::vec3 end, std::vector<std::pair <glm::vec3, glm::vec3>> &buildingZones);


class Building {
public:
	Building();
	Building(std::string name, glm::vec3 l, glm::vec3 r, int h, buildingType t, Mesh* m);
	~Building();
	Mesh* getPrimitives();
	std::tuple<glm::vec3, glm::vec3, int> getPosition();
	void setPosition(glm::vec3 leftCorner, glm::vec3 rightCorner);
protected:
	int height;
	std::string name;
	glm::vec3 leftCorner;
	glm::vec3 rightCorner;
	Mesh* mesh;
	buildingType type;
};
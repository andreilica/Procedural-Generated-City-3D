#pragma once
#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

Mesh* CreatePlane(std::string name, float length, float width);
Mesh* CreateRoad(std::string name, glm::vec3 firstPoint, glm::vec3 secondPoint, float unit);
Mesh* CreateRectangle(std::string name, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float unit);
Mesh* CreateCuboid(std::string name, glm::vec3 start, glm::vec3 end, float height);
Mesh* CreateCuboidRoofs(std::string name, glm::vec3 startCorner, glm::vec3 endCorner, float height);
Mesh* CreateCylinder(std::string name, glm::vec3 center, float radius, float length);
Mesh* CreateCylinderTop(std::string name, glm::vec3 center, float radius, float height);
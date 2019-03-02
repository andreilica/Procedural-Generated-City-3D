#include "CityObjects.h"
#include <iostream>
#include <Core/Engine.h>

Mesh* CreatePlane(std::string name, float length, float width)
{
	glm::vec3 startCorner = glm::vec3(0, 0, 0);

	std::vector<glm::vec3> positions
	{
		startCorner + glm::vec3(0, 0, 0),
		startCorner + glm::vec3(length, 0, 0),
		startCorner + glm::vec3(0, 0, width),
		startCorner + glm::vec3(length, 0, width),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(length, 0),
		glm::vec2(0, length),
		glm::vec2(length, length),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		0, 3, 2
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* CreateRoad(std::string name, glm::vec3 firstPoint, glm::vec3 secondPoint, float unit)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textureCoords;

	if (firstPoint.x == secondPoint.x) {
		//horizontalStreet
		glm::vec3 leftCorner = glm::vec3(firstPoint.x - unit, firstPoint.y, firstPoint.z);
		glm::vec3 rightCorner = glm::vec3(secondPoint.x + unit, secondPoint.y, secondPoint.z);

		positions.push_back(leftCorner);
		positions.push_back(glm::vec3(rightCorner.x - 2 * unit, rightCorner.y, rightCorner.z));
		positions.push_back(glm::vec3(leftCorner.x + 2 * unit, leftCorner.y, leftCorner.z));
		positions.push_back(rightCorner);

		textureCoords.push_back(glm::vec2(0, 0));
		textureCoords.push_back(glm::vec2(secondPoint.z - firstPoint.z, 0));
		textureCoords.push_back(glm::vec2(0, 1));
		textureCoords.push_back(glm::vec2(secondPoint.z - firstPoint.z, 1));
	}
	else if (firstPoint.z == secondPoint.z) {
		//verticalStreet
		glm::vec3 leftCorner = glm::vec3(firstPoint.x, firstPoint.y, firstPoint.z - unit);
		glm::vec3 rightCorner = glm::vec3(secondPoint.x, secondPoint.y, secondPoint.z + unit);

		positions.push_back(leftCorner);
		positions.push_back(glm::vec3(rightCorner.x, rightCorner.y, rightCorner.z - 2 * unit));
		positions.push_back(glm::vec3(leftCorner.x, leftCorner.y, leftCorner.z + 2 * unit));
		positions.push_back(rightCorner);

		textureCoords.push_back(glm::vec2(0, 0));
		textureCoords.push_back(glm::vec2(secondPoint.x - firstPoint.x, 0));
		textureCoords.push_back(glm::vec2(0, 1));
		textureCoords.push_back(glm::vec2(secondPoint.x - firstPoint.x, 1));
	}
	

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
	};


	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		0, 3, 2
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* CreateRectangle(std::string name, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float unit) {
	std::vector<glm::vec3> positions
	{
		p1,
		p2,
		p3,
		p4
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		0, 3, 2
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}


Mesh* CreateCuboid(std::string name, glm::vec3 startCorner, glm::vec3 endCorner, float height)
{

	std::vector<glm::vec3> positions
	{
		startCorner,
		startCorner + glm::vec3(0, 0, endCorner.z - startCorner.z),
		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(0, height, endCorner.z - startCorner.z),

		startCorner,
		startCorner + glm::vec3(endCorner.x - startCorner.x, 0, 0),
		startCorner + glm::vec3(endCorner.x - startCorner.x, height, 0),
		startCorner + glm::vec3(0, height, 0),

		startCorner,
		startCorner + glm::vec3(0, 0, endCorner.z - startCorner.z),
		startCorner + glm::vec3(endCorner.x - startCorner.x, 0,  endCorner.z - startCorner.z),
		startCorner + glm::vec3(endCorner.x - startCorner.x, 0, 0),

		startCorner + glm::vec3(0, 0,  endCorner.z - startCorner.z),
		startCorner + glm::vec3(endCorner.x - startCorner.x, 0, endCorner.z - startCorner.z),
		startCorner + glm::vec3(endCorner.x - startCorner.x , height, endCorner.z - startCorner.z),
		startCorner + glm::vec3(0, height, endCorner.z - startCorner.z),

		startCorner + glm::vec3(endCorner.x - startCorner.x, 0, endCorner.z - startCorner.z),
		startCorner + glm::vec3(endCorner.x - startCorner.x, 0, 0),
		startCorner + glm::vec3(endCorner.x - startCorner.x, height, 0),
		startCorner + glm::vec3(endCorner.x - startCorner.x, height, endCorner.z - startCorner.z),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),

		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),

		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),

		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),

		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0)
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(endCorner.z - startCorner.z, 0),
		glm::vec2(0, height),
		glm::vec2(endCorner.z - startCorner.z, height),

		glm::vec2(endCorner.x - startCorner.x, 0),
		glm::vec2(0, 0),
		glm::vec2(0, height),
		glm::vec2(endCorner.x - startCorner.x, height),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(endCorner.x - startCorner.x, 0),
		glm::vec2(endCorner.x - startCorner.x, height),
		glm::vec2(0, height),

		glm::vec2(0, 0),
		glm::vec2(endCorner.z - startCorner.z, 0),
		glm::vec2(endCorner.z - startCorner.z, height),
		glm::vec2(0, height)
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		1, 3, 2,

		7, 5, 4,
		7, 6, 5,

		8, 11, 9,
		11, 10, 9,

		12, 14, 15,
		12, 13, 14,

		16, 17, 19,
		17, 18, 19
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* CreateCuboidRoofs(std::string name, glm::vec3 startCorner, glm::vec3 endCorner, float height)
{

	std::vector<glm::vec3> positions
	{
		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(0, height, endCorner.z - startCorner.z),
		startCorner + glm::vec3(endCorner.x - startCorner.x, height, 0),
		startCorner + glm::vec3(endCorner.x - startCorner.x, height, endCorner.z - startCorner.z),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),

	};

	std::vector<glm::vec2> textureCoords
	{

		glm::vec2(0, 0),
		glm::vec2(endCorner.z - startCorner.z, 0),
		glm::vec2(0, endCorner.x - startCorner.x),
		glm::vec2(endCorner.x - startCorner.x, endCorner.z - startCorner.z),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		0, 3, 2,
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* CreateCylinder(std::string name, glm::vec3 center, float radius, float length)
{
	int triangles = 50;
	int k = 0;
	float circleX = center.x;
	float circleZ = center.z;
	float circumference = (float)(2 * M_PI);

	float tx = 0, ty = length;

	Mesh* circle = new Mesh(name);
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned short> indices;

	//positions.push_back(glm::vec3(circleX, 0, circleZ));
	for (double i = 0; i <= triangles; i++) {
		positions.push_back(glm::vec3(circleX + (radius * cos(i * circumference / triangles)), center.y, circleZ + (radius * sin(i * circumference / triangles))));
		positions.push_back(glm::vec3(circleX + (radius * cos(i * circumference / triangles)), center.y + length, circleZ + (radius * sin(i * circumference / triangles))));
		indices.push_back(k++);
		indices.push_back(k++);
		normals.push_back(glm::vec3(( cos(i * circumference / triangles), 0, ( sin(i * circumference / triangles)))));
		normals.push_back(glm::vec3((cos(i * circumference / triangles), 0, ( sin(i * circumference / triangles)))));
		textureCoords.push_back(glm::vec2(tx, 0));
		textureCoords.push_back(glm::vec2(tx, ty));
		tx += (radius * circumference / triangles);
	}

	circle->SetDrawMode(GL_QUAD_STRIP);
	circle->InitFromData(positions, normals, textureCoords, indices);

	return circle;
}


Mesh* CreateCylinderTop(std::string name, glm::vec3 center, float radius, float height) {
	int triangles = 50;
	double i;
	double k = 0;
	float circleX = center.x;
	float circleZ = center.z;
	float circumference = (float)(2 * M_PI);

	Mesh* circle = new Mesh(name);
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned short> indices;

	positions.push_back(glm::vec3(circleX, center.y + height, circleZ));
	normals.push_back(glm::vec3(0, 1, 0));
	textureCoords.push_back(glm::vec2(0.5f, 0.5f));
	indices.push_back(k++);

	for (i = 0; i <= triangles; i++) {
		positions.push_back(glm::vec3(circleX + (radius * cos(i * circumference / triangles)), center.y + height, circleZ + (radius * sin(i * circumference / triangles))));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2((1.0f + cos(i * circumference / triangles)) * 0.5f, (1.0f + sin(i * circumference / triangles)) * 0.5f ));
		indices.push_back(k++);
	}

	indices.push_back(1);


	circle->SetDrawMode(GL_TRIANGLE_FAN);
	circle->InitFromData(positions, normals, textureCoords, indices);

	return circle;
}

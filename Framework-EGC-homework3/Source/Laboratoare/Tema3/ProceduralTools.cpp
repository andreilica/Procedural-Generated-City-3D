#include "ProceduralTools.h"
#include <iostream>
#include <Core/Engine.h>

int randomRange(int min, int max){
	static bool first = true;
	if (first)
	{
		srand((unsigned int)time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}


std::vector< std::vector<glm::vec3> > generateGrid(int lowBound, int highBound) {

	static float cityHeight = 0.002f;
	glm::vec3 leftCorner = glm::vec3(0, cityHeight, 0);
	glm::vec3 rightCorner = glm::vec3(highBound, cityHeight, highBound);
	std::vector< std::vector<glm::vec3> > streetSeeds;
	
	const int minDistanceBetween = 2;
	const int maxDistanceBetween = 5;
	int currentSeedPoint = 0, currentIntersection;


	std::vector <glm::vec3> intersections;
	intersections.push_back(leftCorner);
	intersections.push_back(glm::vec3(leftCorner.x, cityHeight, rightCorner.z));
	streetSeeds.push_back(intersections);

	while (rightCorner.x - currentSeedPoint > minDistanceBetween) {
		std::vector <glm::vec3> intersections;
		currentSeedPoint += randomRange(minDistanceBetween, maxDistanceBetween);
		glm::vec3 currentPoint = glm::vec3(currentSeedPoint, cityHeight, leftCorner.z);
		currentIntersection = 0;
		if (rightCorner.x - currentSeedPoint >= minDistanceBetween) {
			intersections.push_back(currentPoint);
			while (rightCorner.z - currentIntersection > minDistanceBetween) {
				currentIntersection += randomRange(minDistanceBetween, maxDistanceBetween);
				glm::vec3 currentIntersectionP = glm::vec3(currentSeedPoint, cityHeight, currentIntersection);
				if (rightCorner.x - currentIntersection >= minDistanceBetween)
					intersections.push_back(currentIntersectionP);
			}
			if (intersections.back() != glm::vec3(currentSeedPoint, cityHeight, rightCorner.z)) {
				intersections.push_back(glm::vec3(currentSeedPoint, cityHeight, rightCorner.z));
				streetSeeds.push_back(intersections);
			}
		}
	}

	if (streetSeeds.back().front() != glm::vec3(rightCorner.x, cityHeight, leftCorner.z)) {
		std::vector <glm::vec3> intersections;
		currentIntersection = 0;
		currentSeedPoint = (int)rightCorner.x;
		intersections.push_back(glm::vec3(rightCorner.x, cityHeight, leftCorner.z));
		while (rightCorner.z - currentIntersection > minDistanceBetween) {
			currentIntersection += randomRange(minDistanceBetween, maxDistanceBetween);
			glm::vec3 currentIntersectionP = glm::vec3(currentSeedPoint, cityHeight, currentIntersection);
			if (rightCorner.x - currentIntersection >= minDistanceBetween)
				intersections.push_back(currentIntersectionP);
		}
		if (intersections.back() != glm::vec3(currentSeedPoint, cityHeight, rightCorner.z)) {
			intersections.push_back(glm::vec3(currentSeedPoint, cityHeight, rightCorner.z));
			streetSeeds.push_back(intersections);
		}
	}
	return streetSeeds;
}

std::pair< std::vector <Mesh*>, std::vector < std::vector<glm::vec3>> > generateStreets(int lowBound, int highBound, float unit) {
	std::vector< std::vector<glm::vec3> > grid = generateGrid(lowBound, highBound);
	std::pair< std::vector <Mesh*>, std::vector < std::vector<glm::vec3>> > streetsAndRectangles;

	int nameIndex = 0;
	auto name = "Road" + std::to_string(nameIndex);
	streetsAndRectangles.first.push_back( CreateRoad(name,
		glm::vec3(grid.front().front().x, grid.front().front().y, grid.front().front().z - unit), 
		glm::vec3(grid.front().back().x, grid.front().back().y, grid.front().back().z + unit), 
		unit));
	for (int i = 0; i < grid.size() - 1; i++) {
		std::vector<glm::vec3> currentLine = grid[i];
		std::vector<glm::vec3> futureLine = grid[i + 1];
		
		nameIndex++;
		name = "Road" + std::to_string(nameIndex);
		streetsAndRectangles.first.push_back(CreateRoad(name, futureLine.front() - glm::vec3(0, 0, unit), futureLine.back() + glm::vec3(0, 0, unit), unit));

		for (int j = 0; j < futureLine.size(); j++) {
			glm::vec3 intersectionPoint = glm::vec3(futureLine[j].x - unit, futureLine[j].y, futureLine[j].z);
			glm::vec3 connectionPoint = glm::vec3(currentLine.front().x + unit, futureLine[j].y, futureLine[j].z);
			nameIndex++;
			name = "Road" + std::to_string(nameIndex);
			streetsAndRectangles.first.push_back(CreateRoad(name, intersectionPoint, connectionPoint, unit));
		}

		for (int j = 0; j < futureLine.size() - 1; j++) {
			std::vector <glm::vec3> rectanglePoints;

			glm::vec3 intersectionPoint1 = glm::vec3(futureLine[j].x - 2 * unit, futureLine[j].y, futureLine[j].z + 2 * unit);
			glm::vec3 connectionPoint1 = glm::vec3(currentLine.front().x + 2 * unit, futureLine[j].y, futureLine[j].z + 2 * unit);
			glm::vec3 intersectionPoint2 = glm::vec3(futureLine[j + 1].x - 2 * unit, futureLine[j + 1].y, futureLine[j + 1].z - 2 * unit);
			glm::vec3 connectionPoint2 = glm::vec3(currentLine.front().x + 2 * unit, futureLine[j + 1].y, futureLine[j + 1].z - 2 * unit);
			
			rectanglePoints.push_back(connectionPoint1);
			rectanglePoints.push_back(connectionPoint2);
			rectanglePoints.push_back(intersectionPoint1);
			rectanglePoints.push_back(intersectionPoint2);

			streetsAndRectangles.second.push_back(rectanglePoints);
		}
	}

	return streetsAndRectangles;
}


void recursiveFootprintSearch(glm::vec3 start, glm::vec3 end, std::vector<std::pair <glm::vec3, glm::vec3>> &buildingZones) {
	int foundSide, firstSide, secondSide;
	const int minBuildingSize = 1, maxBuildingSize = 3;
	glm::vec3 newEnd;

	firstSide = (int)length(glm::vec3(end.x, end.y, start.z) - start);
	secondSide = (int)length(glm::vec3(start.x, start.y, end.z) - start);
	foundSide = randomRange(minBuildingSize, min(maxBuildingSize, min(firstSide, secondSide)));
	newEnd = glm::vec3(start.x + foundSide, start.y, start.z + foundSide);
	buildingZones.push_back(std::make_pair(start, newEnd));

	if (end.x > newEnd.x) {
		//Zone left above
		recursiveFootprintSearch(glm::vec3(newEnd.x, newEnd.y, start.z), end, buildingZones);
	}

	if (end.z > newEnd.z) {
		//Zone left right
		recursiveFootprintSearch(glm::vec3(start.x, start.y, newEnd.z), glm::vec3(newEnd.x, newEnd.y, end.z), buildingZones);
	}

	return;
}

Building::Building()
{
}

Building::Building(std::string name, glm::vec3 l, glm::vec3 r, int h, buildingType t, Mesh* m)
{
	name = name;
	leftCorner = l;
	rightCorner = r;
	type = t;
	height = h;
	mesh = m;
}

Building::~Building()
{
}

Mesh* Building::getPrimitives()
{
	return mesh;
}

std::tuple<glm::vec3, glm::vec3, int> Building::getPosition()
{
	return std::make_tuple(leftCorner, rightCorner, height);
}

void Building::setPosition(glm::vec3 leftCorner, glm::vec3 rightCorner)
{
	leftCorner = leftCorner;
	rightCorner = rightCorner;
}

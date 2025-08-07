#pragma once
#include "glm/glm.hpp"
#include "Dot.h"
#include "vector"
#include "Game.h"

class DotRenderer;

class QuadTree
{

public:
	QuadTree(glm::vec2 newPosition, glm::vec2 newSize, DotRenderer* renderer);
	void Insert(Dot* newDot);
	void ClearTree();
	void Query(glm::vec2 comparePos, glm::vec2 compareSize, std::vector<Dot*> &foundDots);

private: 

	int capacity = 32;
	glm::vec2 position;
	glm::vec2 size;
	std::vector<Dot*> dotsContained;
	bool subdivided = false;

	DotRenderer* renderer;

	QuadTree* neQuad;
	QuadTree* nwQuad;
	QuadTree* seQuad;
	QuadTree* swQuad;


	void DebugDraw(glm::vec2 position, glm::vec2 size, DotRenderer* renderer);
	void Subdivide();
	bool Contains(Dot* newDot);
	bool Overlap(glm::vec2 comparePos, glm::vec2 compareSize);

};


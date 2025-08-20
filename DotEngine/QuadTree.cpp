#include "QuadTree.h"
#include "DotRenderer.h"

QuadTree::QuadTree(glm::vec2 newPosition, glm::vec2 newSize, DotRenderer* newRenderer)
{
	position = newPosition;
	size = newSize;
	renderer = newRenderer;

}

void QuadTree::DebugDraw(glm::vec2 position, glm::vec2 size, DotRenderer* renderer)
{
	renderer->SetDrawColor(255, 255, 255, 255);

	for (int i = -size.x/2; i < size.x/2; i++)
	{
		renderer->DrawPoint(position.x + i, position.y);
	}

	for (int i = -size.y/2; i < size.y/2; i++)
	{
		renderer->DrawPoint(position.x, position.y + i);
	}
}

void QuadTree::Insert(Dot* newDot)
{
	if (!Contains(newDot))
	{
		return;
	}

	if (dotsContained.size() < capacity)
	{
		dotsContained.push_back(newDot);
	}
	else
	{
		if (!subdivided)
		{
			Subdivide();
		}

		neQuad->Insert(newDot);
		nwQuad->Insert(newDot);
		seQuad->Insert(newDot);
		swQuad->Insert(newDot);

		for (int i = 0; i < dotsContained.size(); i++)
		{
			neQuad->Insert(dotsContained[i]);
			nwQuad->Insert(dotsContained[i]);
			seQuad->Insert(dotsContained[i]);
			swQuad->Insert(dotsContained[i]);
		}
		dotsContained.clear();
	}
}

void QuadTree::ClearTree()
{
	if (subdivided)
	{
		neQuad->ClearTree();
		delete neQuad;
		nwQuad->ClearTree();
		delete nwQuad;
		seQuad->ClearTree();
		delete seQuad;
		swQuad->ClearTree();
		delete swQuad;
	}

	dotsContained.clear();
	subdivided = false;
}

void QuadTree::Query(glm::vec2 comparePos, glm::vec2 compareSize, std::vector<Dot*> &foundDots)
{
	if (!Overlap(comparePos, compareSize))
	{
		return;
	}

	if (subdivided)
	{
		neQuad->Query(comparePos, compareSize, foundDots);

		nwQuad->Query(comparePos, compareSize, foundDots);
		
		seQuad->Query(comparePos, compareSize, foundDots);

		swQuad->Query(comparePos, compareSize, foundDots);
	}
	else
	{
		for (int i = 0; i < dotsContained.size(); i++)
		{
			foundDots.push_back(dotsContained[i]);
		}
	}
}
void QuadTree::Subdivide()
{

	//DebugDraw(position, size, renderer);

	neQuad = new QuadTree(glm::vec2(position.x + size.x / 4, position.y + size.y / 4), glm::vec2(size.x / 2, size.y / 2), renderer);
	nwQuad = new QuadTree(glm::vec2(position.x - size.x / 4, position.y + size.y / 4), glm::vec2(size.x / 2, size.y / 2), renderer);
	seQuad = new QuadTree(glm::vec2(position.x + size.x / 4, position.y - size.y / 4), glm::vec2(size.x / 2, size.y / 2), renderer);
	swQuad = new QuadTree(glm::vec2(position.x - size.x / 4, position.y - size.y / 4), glm::vec2(size.x / 2, size.y / 2), renderer);

	subdivided = true;
}

bool QuadTree::Overlap(glm::vec2 comparePos, glm::vec2 compareSize)
{
	return 	(comparePos.x - compareSize.x < position.x + size.x / 2 &&
			 comparePos.x + compareSize.x > position.x - size.x / 2 &&
			 comparePos.y - compareSize.y < position.y + size.y / 2 &&
			 comparePos.y + compareSize.y > position.y - size.y / 2);
}


bool QuadTree::Contains(Dot* newDot)
{
	if (newDot->Position.x > position.x - size.x / 2 && 
		newDot->Position.x < position.x + size.x / 2 && 
		newDot->Position.y > position.y - size.y / 2 && 
		newDot->Position.y < position.y + size.y / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}
	
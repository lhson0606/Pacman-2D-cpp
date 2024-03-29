#pragma once

#include "Polyline2D/Polyline2D.h"
#include <vector>
#include <glm/glm.hpp>

#define DEBUB_PATH_THICKNESS 0.3f

class DebugPathComponent
{
	using Vec2 = crushedpixel::Vec2;
public:
	DebugPathComponent() = default;

	DebugPathComponent(std::vector<Vec2> points, float id)
	{
		SetPath(points);
		this->id = id;
	}

	DebugPathComponent(float id)
	{
		this->id = id;
	}

	void SetPath(std::vector<Vec2> points)
	{
		//I change numTriagles in Polyline2D.h to 1, so the JointStyle will always use only 1 triangle
		this->vertices = crushedpixel::Polyline2D::create(points, DEBUB_PATH_THICKNESS,
			crushedpixel::Polyline2D::JointStyle::BEVEL,
			crushedpixel::Polyline2D::EndCapStyle::SQUARE);
	}

	void ClearPath()
	{
		vertices.clear();
	}

	~DebugPathComponent() = default;

	std::vector<Vec2> vertices;
	glm::vec3 targetPositions = glm::vec3(0);
	float id = 0;
};

#pragma once

#include "Polyline2D/Polyline2D.h"
#include <vector>

#define DEBUB_PATH_THICKNESS 0.7f

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
		this->vertices = crushedpixel::Polyline2D::create(points, DEBUB_PATH_THICKNESS,
			crushedpixel::Polyline2D::JointStyle::ROUND,
			crushedpixel::Polyline2D::EndCapStyle::SQUARE);
	}

	~DebugPathComponent() = default;

	std::vector<Vec2> vertices;
	float id = 0;
};


#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "animorph/BodySettings.h"

template<int rows, int cols>
struct Grid
{
	float maxValue;
	float cellRatio;
	
	// from left-bottom point to right-up point
	std::array<glm::ivec2, rows * cols> points;
	
	void calcPoints(glm::ivec2 size)
	{
		for(int y = 0; y < rows; y++) {
			for(int x = 0; x < cols; x++) {
				glm::ivec2 tmp(x * size.x / (cols - 1),
				               y * size.y / (rows - 1));
				
				points[y * cols + x] = tmp;
			}
		}
		
		float cellWidth  = size.x / (cols - 1);
		float cellHeight = size.y / (rows - 1);
		
		cellRatio = cellWidth / cellHeight;
		maxValue  = glm::min(cellWidth, cellHeight * cellRatio);
	}
	
	std::vector<float> calculateDists(glm::ivec2 cursorPos) const
	{
		std::vector<float> ret;
		
		for(const auto & vp_it : points) {
			const glm::ivec2 & tmp(vp_it);
			
			float dist  = sqrt(pow(tmp.x - cursorPos.x, 2) +
                              pow((tmp.y - cursorPos.y) * cellRatio, 2));
			float value = 1 - (dist / maxValue);
			if(value > 0) {
				ret.push_back(value);
			} else {
				ret.push_back(0.0f);
			}
		}
		
		return ret;
	}
	
	std::vector<float> dists;
};

struct Grids {
	glm::ivec2 agePos;
	glm::ivec2 muscleSizePos;
	glm::ivec2 breastPos;
	glm::ivec2 shapePos;
	
	Grid<2, 5> ageGrid;
	Grid<2, 2> muscleSizeGrid;
	Grid<2, 2> breastGrid;
	Grid<2, 2> shapeGrid;
	
	Grids()
	{
		ageGrid.calcPoints(glm::ivec2(192, 104));
		muscleSizeGrid.calcPoints(glm::ivec2(192, 104));
		breastGrid.calcPoints(glm::ivec2(192, 104));
		shapeGrid.calcPoints(glm::ivec2(192, 104));
	}
	
	void clearDists()
	{
		ageGrid.dists.clear();
		muscleSizeGrid.dists.clear();
		breastGrid.dists.clear();
		shapeGrid.dists.clear();
	}
	
	void calcDists();
	void applyCompositeMorphTargets() const;
	
	
	
	void fromSavedPositions(const Animorph::SavedCompSetting & saved);
};

#include "CompositeMorph.h"

#include <array>
#include "Global.h"

using std::string;

Grids g_grids;

static std::array<string, 10> ageLabels = {
    "female_10",
    "female_30",
    "female_50",
    "female_70",
    "female_90",
    "male_10",
    "male_30",
    "male_50",
    "male_70",
    "male_90"
};

static std::array<string, 4> muscleSizeLabels = {
    "skinny_nomuscle",
    "big_nomuscle",
    "skinny_muscle",
    "big_muscle"
};

static std::array<string, 4> breastLabels = {
    "cone_little",
    "cone_big",
    "sphere_little",
    "sphere_big"
};

static std::array<string, 4> shapeLabels = {
    "brevilinear_vshape",
    "brevilinear_peershape",
    "longilinear_vshape",
    "longilinear_peershape"
};



void Grids::calcDists()
{
	ageGrid.calculateDists(agePos);
	breastGrid.calculateDists(breastPos);
	muscleSizeGrid.calculateDists(muscleSizePos);
	shapeGrid.calculateDists(shapePos);
}

void Grids::applyCompositeMorphTargets() const
{
	static_assert(std::tuple_size<decltype(ageGrid.dists)>::value ==
	              std::tuple_size<decltype(ageLabels)>::value,
	              "");
	
	static_assert(std::tuple_size<decltype(muscleSizeGrid.dists)>::value ==
	              std::tuple_size<decltype(muscleSizeLabels)>::value,
	              "");
	
	static_assert(std::tuple_size<decltype(breastGrid.dists)>::value ==
	              std::tuple_size<decltype(breastLabels)>::value,
	              "");
	
	static_assert(std::tuple_size<decltype(shapeGrid.dists)>::value ==
	              std::tuple_size<decltype(shapeLabels)>::value,
	              "");
	
	
	// std::cout << "--------------------------" << std::endl;
	for(size_t i = 0; i < ageGrid.dists.size(); ++i) {
		
		string tmpTargetName("ages/" + ageLabels[i] + ".target");
		g_mesh.setMorphTarget(tmpTargetName, ageGrid.dists[i]);
	}
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	
	for(const float & ms_it : muscleSizeGrid.dists) {
		
		i = 0;
		
		for(const float & di_it : ageGrid.dists) {
			if(j < muscleSizeLabels.size() && i < ageLabels.size()) {
				string tmpTargetName("muscleSize/" + ageLabels[i] + "_" +
				                     muscleSizeLabels[j] + ".target");
				float  tmpTargetValue = di_it * ms_it;
				
				g_mesh.setMorphTarget(tmpTargetName, tmpTargetValue);
				
				// breast widget
				
				k = 0;
				if(i <= 4) {
					
					for(const float & br_it : breastGrid.dists) {
						
						if(k < breastLabels.size()) {
							string tmpTargetName(
							    "breast/" + ageLabels[i] + "_" +
							    muscleSizeLabels[j] + "_" +
							    breastLabels[k] + ".target");
							float tmpTargetValue =
							    di_it * ms_it * br_it;
							
							if(tmpTargetValue > 0) {
								//log_info("{} {}", tmpTargetName, tmpTargetValue);
							}
							
							g_mesh.setMorphTarget(tmpTargetName, tmpTargetValue);
						}
						k++;
					}
				}
			}
			i++;
		}
		j++;
	}
	
	for(size_t i = 0; i < shapeGrid.dists.size(); ++i) {
	
		string tmpTargetName("shapes/" + shapeLabels[i] + ".target");
		g_mesh.setMorphTarget(tmpTargetName, shapeGrid.dists[i]);
	}
}



void Grids::fromSavedPositions(const Animorph::SavedCompSetting & saved)
{
	using glm::ivec2;
	using glm::clamp;
	
	int yOff = 16;
	auto size = ivec2(192, 104);
	auto defPos = ivec2(96, 52);
	
	if(saved.m_kAge != InvalidPoint){
		const int bottom = yOff + 10 + size.y;
		
		agePos = clamp(ivec2(saved.m_kAge.x, bottom - saved.m_kAge.y),
		               ivec2(0), size);
	} else {
		agePos = defPos;
	}
	
	if(saved.m_kMuscleSize != InvalidPoint){
		const int bottom = yOff + 124 + size.y;
		
		muscleSizePos = clamp(ivec2(saved.m_kMuscleSize.x, bottom - saved.m_kMuscleSize.y),
		                      ivec2(0), size);
	} else {
		muscleSizePos = defPos;
	}
	
	if(saved.m_kBreast != InvalidPoint){
		const int bottom = yOff + 238 + size.y;
		
		breastPos = clamp(ivec2(saved.m_kBreast.x, bottom - saved.m_kBreast.y),
		                  ivec2(0), size);
	} else {
		breastPos = defPos;
	}
	
	if(saved.m_kShape != InvalidPoint){
		const int bottom = yOff + 352 + size.y;
		
		shapePos = clamp(ivec2(saved.m_kShape.x, bottom - saved.m_kShape.y),
		                 ivec2(0), size);
	} else {
		shapePos = defPos;
	}
}

void Grids::toSavedPositions(Animorph::SavedCompSetting & saved)
{
	using glm::ivec2;
	
	int yOff = 16;
	auto size = ivec2(192, 104);
	
	saved.m_kAge = ivec2(agePos.x, yOff + 10 + size.y - agePos.y);
	saved.m_kMuscleSize = ivec2(muscleSizePos.x, yOff + 124 + size.y - muscleSizePos.y);
	saved.m_kBreast = ivec2(breastPos.x, yOff + 238 + size.y - breastPos.y);
	saved.m_kShape = ivec2(shapePos.x, yOff + 352 + size.y - shapePos.y);
}

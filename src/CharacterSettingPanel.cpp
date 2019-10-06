/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005-2007  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  3  of
 *  the License, or (at your option) any later version.
 *
 *  This  program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the  implied  warranty  of
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foun-
 *  dation, Inc., 59 Temple Place, Suite 330, Boston,  MA  02111-1307
 *  USA
 *
 *  File   : CharacterSettingPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "CharacterSettingPanel.h"
#include "ComponentID.h"

#include <algorithm>
#include <assert.h>
#include <stdio.h>

#include <gui/Rect.h>
#include <gui/Selector.h>
#include <gui/Window.h>

#include "ComponentID.h"
#include "Global.h"
#include "log/log.h"

using mhgui::Rect;
using mhgui::Image;
using mhgui::Selector;


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

// ================================================================================================

void Grid::calcPoints(glm::ivec2 size, int inRows, int inCols)
{
	rows = inRows;
	cols = inCols;
	
	points.clear();
	
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			glm::ivec2 tmp(j * size.x / (cols - 1),
			               i * size.y / (rows - 1));
			points.push_back(tmp);
		}
	}
	
	float cellWidth  = size.x / (cols - 1);
	float cellHeight = size.y / (rows - 1);
	
	cellRatio = cellWidth / cellHeight;
	maxValue  = glm::min(cellWidth, cellHeight * cellRatio);
}

std::vector<float> Grid::calculateDists(glm::ivec2 cursorPos) const
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


void Grids::applyCompositeMorphTargets() const
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	
	// std::cout << "--------------------------" << std::endl;
	for(const float & di_it : ageDists) {
		if(i < ageLabels.size()) {
			string tmpTargetName("ages/" + ageLabels[i++] + ".target");
			
			g_mesh.setMorphTarget(tmpTargetName, di_it);
		}
	}
	
	for(const float & ms_it : muscleSizeDists) {
		
		i = 0;
		
		for(const float & di_it : ageDists) {
			if(j < muscleSizeLabels.size() && i < ageLabels.size()) {
				string tmpTargetName("muscleSize/" + ageLabels[i] + "_" +
				                     muscleSizeLabels[j] + ".target");
				float  tmpTargetValue = di_it * ms_it;
				
				g_mesh.setMorphTarget(tmpTargetName, tmpTargetValue);
				
				// breast widget
				
				k = 0;
				if(i <= 4) {
					
					for(const float & br_it : breastDists) {
						
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
	
	i = 0;
	
	for(const float & sh_it : shapeDists) {
		if(i < shapeLabels.size()) {
			string tmpTargetName("shapes/" + shapeLabels[i++] + ".target");
			
			g_mesh.setMorphTarget(tmpTargetName, sh_it);
		}
	}
}

// ================================================================================================

SelectorListener::SelectorListener()
    : AbstractListener()
    , oldPos(0, 0)
{
}

SelectorListener::~SelectorListener()
{
}

bool SelectorListener::mouseDragged(const glm::ivec2 & inMousePos, mhgui::Component * source)
{
	Selector * selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	selectorSource->setCursorPosFromMousePoint(inMousePos);
	
	int xDist = abs(oldPos.x - inMousePos.x);
	int yDist = abs(oldPos.y - inMousePos.y);
	
	
	//g_global.setFuzzyValue(selectorSource->getID(), inMousePos);
	
	if(xDist > 3 || yDist > 3) {
		oldPos = inMousePos;
		calcWidgetTargets(*selectorSource, inMousePos);
	}
	
	return true;
}

bool SelectorListener::mousePressed(const glm::ivec2 & inMousePos, int button, mhgui::Component * source)
{
	Selector * selectorSource = dynamic_cast<Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	selectorSource->setCursorPosFromMousePoint(inMousePos);
	
	selectorSource->setActive(true);
	selectorSource->setClickConsumed(false);
	
	return true;
}

bool SelectorListener::mouseReleased(const glm::ivec2 & inMousePos, int button, mhgui::Component * source)
{
	mhgui::Selector * selectorSource = dynamic_cast<mhgui::Selector *>(source); // req. RTTI!
	assert(selectorSource); // Check if this is really an Image object?
	
	selectorSource->setActive(false);
	selectorSource->setClickConsumed(false);
	
	oldPos = inMousePos;
	
	//g_global.setFuzzyValue(selectorSource->getID(), inMousePos);
	calcWidgetTargets(*selectorSource, inMousePos);
	
	g_mesh.calcNormals();
	
	return true;
}

void SelectorListener::calcWidgetTargets(mhgui::Selector & sel, glm::ivec2 inMousePos)
{
	
	switch(sel.getID()) {
	case kAge:
		g_global.m_comp.m_kAge = inMousePos;
		grids.agePos = sel.cursorPos;
		grids.ageDists = grids.ageGrid.calculateDists(grids.agePos);
		break;
	case kMuscleSize:
		g_global.m_comp.m_kMuscleSize = inMousePos;
		grids.muscleSizePos = sel.cursorPos;
		grids.muscleSizeDists = grids.muscleSizeGrid.calculateDists(grids.muscleSizePos);
		break;
	case kBreast:
		g_global.m_comp.m_kBreast = inMousePos;
		grids.breastPos = sel.cursorPos;
		grids.breastDists = grids.breastGrid.calculateDists(grids.breastPos);
		break;
	case kShape:
		g_global.m_comp.m_kShape = inMousePos;
		grids.shapePos = sel.cursorPos;
		grids.shapeDists = grids.shapeGrid.calculateDists(grids.shapePos);
		break;
	}
	
	grids.applyCompositeMorphTargets();
}


// ================================================================================================
// ================================================================================================


CharacterSettingPanel::CharacterSettingPanel()
        : Panel(kComponentID_CharacterSettingPanel, Rect(0, 16, 192, 580))
{
	setAlignment(VERTICAL);
}

CharacterSettingPanel::~CharacterSettingPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
	
	delete m_age;
	delete m_muscleSize;
	delete m_breast;
	delete m_shape;
}

void CharacterSettingPanel::createWidgets()
{
	const Color c(1.0, 0.0, 0.0);
	// --------------------------------------------------------------------------
	Image * image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                          Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// --------------------------------------------------------------------------
	
	mhgui::Selector *         selector;
	
	selector = new Selector(kAge,
	                        searchPixmapFile("ui/age_selector.png"), Rect(0, 0, 192, 104));
	
	lis.grids.ageGrid.calcPoints(selector->getSize(), 2, 5);
	
	selector->setListener(&lis);
	m_age = selector;
	addWidget(selector);
	
	{
		auto & r = selector->getRect();
		log_info("kAge: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
	}
	
	// --------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                  Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// -------------------------------------------------------------------------
	selector =
	        new Selector(kMuscleSize,
	                     searchPixmapFile("ui/muscle_size_selector.png"), Rect(0, 0, 192, 104));
	
	lis.grids.muscleSizeGrid.calcPoints(selector->getSize(), 2, 2);
	
	selector->setListener(&lis);
	m_muscleSize = selector;
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kMuscleSize: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
	}
	
	// --------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                  Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// -------------------------------------------------------------------------
	selector = new Selector(kBreast,
	                        searchPixmapFile("ui/breast_selector.png"), Rect(0, 0, 192, 104));
	
	lis.grids.breastGrid.calcPoints(selector->getSize(), 2, 2);
	
	selector->setListener(&lis);
	m_breast = selector;
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kBreast: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
	}
	
	// --------------------------------------------------------------------------
	image = new Image(kComponentID_Dummy, searchPixmapFile("ui/dummy_image.png"),
	                  Rect(0, 0, 192, 10));
	image->setKillMouseDrag(true);
	imageVector.push_back(image);
	addWidget(image);
	// -------------------------------------------------------------------------
	selector = new Selector(kShape,
	                        searchPixmapFile("ui/shape_selector.png"), Rect(0, 0, 192, 104));
	
	lis.grids.shapeGrid.calcPoints(selector->getSize(), 2, 2);
	
	selector->setListener(&lis);
	m_shape = selector;
	addWidget(selector);

	{
		auto & r = selector->getRect();
		log_info("kShape: {} {} {} {}", r.pos.x, r.pos.y, r.size.x, r.size.y);
	}
	
	updateUi();
}

void CharacterSettingPanel::resetSlidersValues()
{
	m_age->cursorPos = glm::ivec2(96, 52);
	m_muscleSize->cursorPos = glm::ivec2(96, 52);
	m_breast->cursorPos = glm::ivec2(96, 52);
	m_shape->cursorPos = glm::ivec2(96, 52);
	
	lis.grids.ageDists.clear();
	lis.grids.muscleSizeDists.clear();
	lis.grids.breastDists.clear();
	lis.grids.shapeDists.clear();
}

void CharacterSettingPanel::calcSelectorValues(Selector * sel)
{
	glm::ivec2 foo = InvalidPoint;

	switch(sel->getID()) {
	case kAge:
		foo = g_global.m_comp.m_kAge;
		break;
	case kBreast:
		foo = g_global.m_comp.m_kBreast;
		break;
	case kMuscleSize:
		foo = g_global.m_comp.m_kMuscleSize;
		break;
	case kShape:
		foo = g_global.m_comp.m_kShape;
		break;
	}
	
	if(foo == InvalidPoint) {
		sel->cursorPos = glm::ivec2(96, 52);
	} else {
		sel->setCursorPosFromMousePoint(foo);
	}
	
	switch(sel->getID()) {
	case kAge:
		lis.grids.agePos = sel->cursorPos;
		lis.grids.ageDists = lis.grids.ageGrid.calculateDists(lis.grids.agePos);
		break;
	case kBreast:
		lis.grids.breastPos = sel->cursorPos;
		lis.grids.breastDists = lis.grids.breastGrid.calculateDists(lis.grids.breastPos);
		break;
	case kMuscleSize:
		lis.grids.muscleSizePos = sel->cursorPos;
		lis.grids.muscleSizeDists = lis.grids.muscleSizeGrid.calculateDists(lis.grids.muscleSizePos);
		break;
	case kShape:
		lis.grids.shapePos = sel->cursorPos;
		lis.grids.shapeDists = lis.grids.shapeGrid.calculateDists(lis.grids.shapePos);
		break;
	}
	
}

void CharacterSettingPanel::updateUi()
{
	calcSelectorValues(m_age);
	calcSelectorValues(m_breast);
	calcSelectorValues(m_muscleSize);
	calcSelectorValues(m_shape);
}

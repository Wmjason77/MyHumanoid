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
 *  File   : Global.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Animation.h"
#include <animorph/Mesh.h>
#include <gui/Camera.h>
#include <gui/Point.h>
#include <gui/Texture.h>

using namespace Animorph;
using namespace mhgui;

enum Modes {
	BODY_DETAILS,
	POSES,
	CHARACTER_SETTING,
	CLOTHES,
	ANIMATIONS,
	BODY_SETTINGS,
	POSES_BODY_SETTINGS,
};

struct GlobalPaths {
	
};

struct Global
{
	GlobalPaths paths;
	
	Mesh *mesh = NULL;
	Camera *camera = NULL;
	Animation *animation = NULL;
	Autozoom *autozoom = NULL;
	bool drawGrid = false;
	bool quotedBox = false;
	bool m_enableTexture = false;
	bool m_canTexture = false;
	bool subdivision = false;
	bool lightMesh = false;
	Modes appMode = CHARACTER_SETTING;
	
	std::map<string, Texture *> textures;
	std::map<const uint32_t, Point *> fuzzyValues;

public:
	void setMesh(Mesh *mesh) { this->mesh = mesh; }
	Mesh *getMesh() { return mesh; }

	void setCamera(Camera *camera) { this->camera = camera; }
	Camera *getCamera() { return camera; }

	void setTexture(string name, Texture *texture) { textures[name] = texture; }
	Texture *getTexture(string name) { return textures[name]; }

	void setAnimation(Animation *animation) { this->animation = animation; }
	Animation *getAnimation() { return animation; }

	void setAutozoom(Autozoom *autozoom) { this->autozoom = autozoom; }
	Autozoom *getAutozoom() { return autozoom; }

	void setDrawGrid(bool grid) { drawGrid = grid; }
	bool getDrawGrid() { return drawGrid; }

	void setQuotedBox(bool qbox) { quotedBox = qbox; }
	bool getQuotedBox() { return quotedBox; }

	void setAppMode(Modes mode) { appMode = mode; }
	Modes getAppMode() { return appMode; }

	void setTexture(bool texture) { this->m_enableTexture = texture; }
	bool getTexture() { return m_enableTexture; }

	void setCanTexture(bool canTexture) { this->m_canTexture = canTexture; }
	bool getCanTexture() { return m_canTexture; }

	void setFuzzyValue(uint32_t index, const Point &inPoint)
	{
		this->fuzzyValues[index] = new Point(inPoint);
	}
	void resetFuzzyValues() { fuzzyValues.clear(); }
	Point *getFuzzyValue(const uint32_t index)
	{
		return this->fuzzyValues[index];
	}

	void setSubdivision(bool subd) { subdivision = subd; }
	bool getSubdivision() { return subdivision; }

	void setLightMesh(bool lmesh) { lightMesh = lmesh; }
	bool getLightMesh() { return lightMesh; }
};

extern Global g_global;

#endif // GLOBAL_H

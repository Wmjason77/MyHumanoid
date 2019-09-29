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
 *  File   : TargetSliderListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <animorph/Mesh.h>

#include <gui/CGUtilities.h>
#include <gui/GLUTWrapper.h>
#include <gui/ImageSlider.h>

#include <assert.h>
#include <stdio.h>

#include "Global.h"
#include "TargetSlider.h"
#include "TargetSliderListener.h"

using namespace Animorph;
using namespace std;

TargetSliderListener::TargetSliderListener()
        : AbstractListener()
{
}

TargetSliderListener::~TargetSliderListener() {}

bool TargetSliderListener::mouseOver(const Point & inMousePos, Component * source) { return false; }

bool TargetSliderListener::mouseWheel(const Point & inMousePos, int inButton, Component * source)
{
#ifdef GLUT_WHEEL_UP
	return mouseDragged(inMousePos, source);
#else
	return false;
#endif
}

bool TargetSliderListener::mouseOut(const Point & inMousePos, Component * source)
{
	// TODO: the mouseOut event is perhaps a little wrong for this.
	// better is mouseReleased, but this should generate an event also
	// if released outside the widget (but only for slider!!)

	return false;
}

bool TargetSliderListener::mouseDragged(const Point & inMousePos, Component * source)
{
	TargetSlider * imgSliderSource = dynamic_cast<TargetSlider *>(source); // req. RTTI!
	assert(imgSliderSource); // Check if this is really a TargetSlider object?

	Mesh * mesh = g_global.getMesh();

	mesh->doMorph(imgSliderSource->getTargetName(), imgSliderSource->getSliderValue());

	/* TODO:
	 * It's a little slow to calc normals in realtime while morphing.
	 * A better way is to calc it once after a drag operation ends. This
	 * needs some changes in listener implementation.
	 *
	 * Another solution is to speed up normals calculation in animorph.
	 * I've some good ideas how to do this...
	 */

	return true;
}

bool TargetSliderListener::mousePressed(const Point & inMousePos, int button, Component * source)
{
	return false;
}

bool TargetSliderListener::mouseReleased(const Point & inMousePos, int button, Component * source)
{
	TargetSlider * imgSliderSource = dynamic_cast<TargetSlider *>(source); // req. RTTI!
	assert(imgSliderSource); // Check if this is really a TargetSlider object?

	Mesh * mesh = g_global.getMesh();

	mesh->doMorph(imgSliderSource->getTargetName(), imgSliderSource->getSliderValue());

	mesh->calcNormals();

	return false;
}

bool TargetSliderListener::keyType(unsigned char key, Component * source) { return false; }

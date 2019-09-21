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
 *  File   : UtilitybarPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "UtilitybarPanel.h"
#include "ComponentID.h"

#include <algorithm>

#include <gui/Rect.h>
#include <gui/Tooltip.h>
#include <gui/Window.h>
//#include "Global.h"

using namespace std;
using namespace Animorph;

UtilitybarPanel::UtilitybarPanel()
    : Panel(kComponentID_UtilitybarPanel, Rect(0, 16, 192, 16))
{
}

UtilitybarPanel::~UtilitybarPanel()
{
	for_each(imageVector.begin(), imageVector.end(), deleteFunctor<Image *>());
}

void UtilitybarPanel::createWidgets()
{
	Window &mainWindow = *g_mainWindow;
	Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
	assert(tooltipPanel);

	const Color color_red(1.0, 0.0, 0.0);
	float alpha = 1.0;
	const Point kTooltipPos(70, 12);

	Image *utilitybar;
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	/*  utilitybar= new Image(kComponentID_ImageUtilitybar_RenderPreview,
	                        searchPixmapFile ("ui/utilitybar_04.png"),
	                        Rect(0,0,16,16));
	  utilitybar->setListener(&imgListener1);
	  utilitybar->setTooltip(Tooltip("Rendering preview", kTooltipPos, color_red,
	  tooltipPanel)); utilitybar->setAlpha (alpha); imageVector.push_back
	  (utilitybar); addWidget (utilitybar);
	*/
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	utilitybar =
	    new Image(kComponentID_ImageUtilitybar_About,
	              searchPixmapFile("ui/utilitybar_05.png"), Rect(0, 0, 16, 16));
	utilitybar->setListener(&imgListener1);
	utilitybar->setTooltip(
	    Tooltip("About MakeHuman", kTooltipPos, color_red, tooltipPanel));
	utilitybar->setAlpha(alpha);
	imageVector.push_back(utilitybar);
	addWidget(utilitybar);

	// -------------------------------------------------------------------------
	utilitybar = new Image(kComponentID_ImageUtilitybar_Frames_01,
	                       searchPixmapFile("ui/frames_01.png"),
	                       Rect(0, 0, 96, 16)); // w = 1024 - 32x17
	utilitybar->setAlpha(alpha);
	imageVector.push_back(utilitybar);
	addWidget(utilitybar);
}

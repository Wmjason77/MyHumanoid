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
 *  File   : ConsoleListener.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

/** client listener header **/

#ifndef CONSOLELISTENER_H
#define CONSOLELISTENER_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <iostream>
#include <mhgui/AbstractListener.h>
#include <mhgui/Point.h>

class mhgui::Component;

class ConsoleListener : public AbstractListener
{
protected:
  void loadWindowBackground(Console& console, const string& filename);
  void loadBodySettings(Console& console, const string& filename);
  void loadPoses(Console& console, const string& filename);
  void loadAqsisPath(Console& console, const string& path);
  void loadPixiePath(Console& console, const string& path);
  void parseSetParameter(Console& console, const string& path);
  void saveBodySettings(Console& console, const string& filename);
  void savePoses(Console& console, const string& filename);
  void saveAutozoom(Console& console, const string& filename);
  void exportBodySettings(Console& console, string& filename, bool full = false);
  void exportCollada(Console& console, string& filename);

  void loadAnimation(Console& console, const string& path);
  //void saveAnimation(Console& console, const string& path);
  //bool loadSelectorsPositions(const std::string& filename);
  //bool saveSelectorsPositions(const std::string& filename, std::ios_base::openmode mode = std::ios::app);

public:
           ConsoleListener();
  virtual ~ConsoleListener();

  void parseCommand (Console& console);
  void commandNotFound (Console& console);

  virtual bool mouseOver            (const Point& inMousePos,               Component *source );
  virtual bool mouseOut             (const Point& inMousePos,               Component *source );
  virtual bool mousePressed         (const Point& inMousePos, int inButton, Component *source );
  virtual bool mouseReleased        (const Point& inMousePos, int inButton, Component *source );
  virtual bool mouseDragged         (const Point& inMousePos,               Component *source );
  virtual bool mouseWheel           (const Point& inMousePos, int inButton, Component *source );
  virtual bool keyType              (unsigned char inKey,                   Component *source );
};

#endif

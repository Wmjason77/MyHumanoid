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
 *  File   : Rect.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include <iostream>
#include <stdint.h>
#include <glm/glm.hpp>

namespace mhgui
{

/*!
 * \brief Represents a rectangle by its position and size
 */
class Rect
{
public:
	Rect(int32_t inX, int32_t inY, int32_t inWidth, int32_t inHeight);
	/// Copy constructor
	Rect(const Rect & inRect);

	~Rect();

	/// Assigns another Rect to this one
	Rect & operator=(const Rect & inPoint);

	bool isEmpty() const { return size == glm::ivec2(0); }

	/// Checks whether two rectangles are identical
	bool operator==(const Rect & inRect) const;

	/// Sets the position
	void moveTo(const glm::ivec2 & inPos);
	/** \brief Moves the rectangle
	 *
	 * Wrapper for Point::moveBy().
	 */
	void moveBy(const glm::ivec2 & inDeltaPos);

	/// Sets the size
	void resizeTo(const glm::ivec2 & inSize);
	/** \brief Changes the size
	 *
	 * Wrapper for Size::resizeBy().
	 */
	void resizeBy(const glm::ivec2 & inDeltaSize);

	/** \brief Makes this rectangle smaller by removing an outer border
	 *
	 * @param inInsetX vertical border size
	 * @param inInsetY horizontal border size
	 */
	void inset(int32_t inInsetX, int32_t inInsetY);

	/// Checks whether a particular Point is located in this rectangle
	bool isHitBy(const glm::ivec2 & inPoint) const;

	glm::ivec2 pos;
	glm::ivec2  size;

}; // class Rect

inline Rect::Rect(int32_t inX, int32_t inY, int32_t inWidth, int32_t inHeight)
        : pos(inX, inY)
        , size(inWidth, inHeight)
{
}

inline Rect::Rect(const Rect & inRect)
        : pos(inRect.pos)
        , size(inRect.size)
{
}

inline Rect::~Rect()
{
}

inline Rect & Rect::operator=(const Rect & inPoint)
{
	pos  = inPoint.pos;
	size = inPoint.size;
	return *this;
}

inline bool Rect::operator==(const Rect & inRect) const
{
	if(this == &inRect) // The same object?
		return true;

	return (pos == inRect.pos && size == inRect.size);
}

inline void Rect::moveTo(const glm::ivec2 & inPos)
{
	pos = inPos;
}

inline void Rect::moveBy(const glm::ivec2 & inDeltaPos)
{
	pos += inDeltaPos;
}

inline void Rect::resizeTo(const glm::ivec2 & inSize)
{
	size = inSize;
}

inline void Rect::resizeBy(const glm::ivec2 & inDeltaSize)
{
	size += inDeltaSize;
}

inline void Rect::inset(int32_t inInsetX, int32_t inInsetY)
{
	moveBy(glm::ivec2(inInsetX, inInsetY));
	resizeBy(glm::ivec2(-2 * inInsetX, -2 * inInsetY));
}

inline bool Rect::isHitBy(const glm::ivec2 & inPoint) const
{
	return (((inPoint.x >= pos.x) && (inPoint.y >= pos.y)) &&
	        ((inPoint.x < pos.x + size.x) &&
	         (inPoint.y < pos.y + size.y)));
}

} // namespace mhgui

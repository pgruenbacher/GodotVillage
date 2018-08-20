/***************************************************************************
 *   Copyright (C) 2007  Antony Martin                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   as published by the Free Software Foundation; either version 2        *
 *   of the License, or (at your option) any later version.                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA          *
 ***************************************************************************/


/*!*****************************************************************************
*
*       Edited by     :      Arnaud Emilien
*
*******************************************************************************/


#ifndef __loadprogram_h__
#define __loadprogram_h__

#include <iostream>

GLuint LoadShader(GLenum type, const char *filename);

GLuint LoadProgram(const char *vsname, const char *psname);


#endif

/*
 *  Utilities_ACS.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-20.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;

namespace Utilities_ACS
{
	static void setTextureWithinShader(const char* name, gl::Texture& tex, gl::GlslProg shader, int textureLocation)
	{
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		glEnable(tex.getTarget());
		glBindTexture(tex.getTarget(), tex.getTextureId());
		glDisable(tex.getTarget());
		shader.uniform( name, textureLocation);
		glActiveTexture(GL_TEXTURE0);
	}
	
	static void tokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ")
	{
		// Skip delimiters at beginning.
		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos     = str.find_first_of(delimiters, lastPos);
		
		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}
}


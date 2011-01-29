/*
 *  Ribbon.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-24.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#pragma once

#include "Resources.h"
#include "DefinesAndConstants.h"

#include "cinder/ObjLoader.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/DisplayList.h"
#include "cinder/gl/Material.h"

//static const int NUM_QUESTIONS = 12;

using namespace ci;
using namespace ci::app;

class Ribbon
{
public:
	Ribbon();
	~Ribbon();
	
	void setup( int type );
	void update();
	void draw();
	
	bool isAnimating;
	
private:
	
	TriMesh			mMesh;	
	gl::VboMesh		mVBO;
	int mRibbonType; 
};

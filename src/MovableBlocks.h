/*
 *  MovableBlocks.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-28.
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

#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "Utilities_ACS.h"

using namespace ci;
using namespace ci::app;

class MovableBlocks
{
public:
	MovableBlocks(){};
	~MovableBlocks(){};
	
	void setup();
	void update();
	void draw( gl::GlslProg* shader );
	void drawMask( gl::GlslProg* shader );
	
	void activate( int blockNum );
	void activate( int xCoord, int yCoord );
	void deactivate( int blockNum );
	void deactivate( int xCoord, int yCoord );
	void activateAnswerState();
	void deactivateAnswerState();
	void activateRibbon();
	void deactivateRibbon();
	void activateStepAnimation();
	void activateBottomAnimation();
	int getArrIndex( int xCoord, int yCoord );
	bool answerStateActivated;
	
private:
	gl::GlslProg	mBasicShader;
	gl::DisplayList	mColArrow;
	TriMesh			mMesh;	
	gl::Texture		mWallTex;
	gl::Texture		mTwitterMessage;
	gl::Texture		mBlackTex;
	gl::VboMesh		mVBOs[NUM_WALL_BLOCKS];
	bool			isAnimatingOut[NUM_WALL_BLOCKS];
	bool			isAnimatingIn[NUM_WALL_BLOCKS];
	float			currDepth[NUM_WALL_BLOCKS];
	float			minDepth[NUM_WALL_BLOCKS];
	
	Vec3f			mArrowPos;
	Vec3f			mArrowScale;
	float			mArrowScaleRate;
	bool			mArrowAnimOut;
	bool			mArrowAnimIn;
	bool			mArrowVisible;
	
	bool			stepActivated;
	bool			bottomAnimated;
};

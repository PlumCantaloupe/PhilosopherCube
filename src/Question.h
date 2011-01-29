/*
 *  Question.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-24.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#pragma once

#include "DefinesAndConstants.h"

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/DisplayList.h"
#include "cinder/gl/Material.h"
#include "cinder/Rand.h"

#include <FTGL/ftgl.h>

using namespace ci;
using namespace ci::app;

class Question
{
	
public:
	Question();
	~Question();
	
	void update();
	void draw();
	
	void init3DFont();
	bool isAnimating();
	void startNewQuestion();
	void stopQuestion();
	void startAnswer();
	void stopAnswer();
	void setupAnswerIntro();
	
	std::string questionString;
	bool visible;
	bool mIsAnswerIntro;
	
private:
	
	const char* getNewQuestion();
	
	int questCounter;
	FTFont* font;
	FTFont* fontTop;
	FTSimpleLayout textLayout;
	FTSimpleLayout textLayoutTop;
	bool isAnimatingOut;
	bool isAnimatingIn;
	bool mFirstTime;
	float currDepth;
};
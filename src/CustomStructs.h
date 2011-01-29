/*
 *  CustomStructs.h
 *  emptyExample
 *
 *  Created by anthonyScavarelli on 10-10-02.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#pragma once
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

#include <FTGL/ftgl.h>

struct quad 
{
	ci::Vec3f pos;
	ci::Vec3f verts[4];
};

struct tweet 
{
	std::string	id;
	std::string	message;
	std::string	location;
	std::string	time;
	std::string	name;
};

struct tweetDataRender
{	
	//data
	tweet			twitterData;
	
	//render data
//	FTSimpleLayout	messageCon;
//	FTSimpleLayout	nameCon;
	ci::gl::Texture		questionTex;
	
	float			heightMessage;
	float			scrollSpeed;
	bool			animating;
	Vec3f			posTweet;
};

/*
 *  DefinesAndConstants.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-11-01.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Vector.h"

#define NUM_CUBE_FRAMES		24
#define NUM_TYPES_RIBBONS	3
#define NUM_WALL_BLOCKS		15
#define TRAIL_LENGTH		15 

using namespace ci;
using namespace ci::app;

enum PhiloCubeStates 
{ 
	WANDER_PHASE, 
	ASKING_PHASE, 
	TWITTER_PHASE,
	ANSWER_PHASE
};

static const int	WANDER_PHASE_LENGTH			= 15;
static const int	ASKING_PHASE_LENGTH			= 000;
static const int	TWITTER_PHASE_LENGTH		= 300;
static const int	ANSWER_PHASE_LENGTH			= 60;
static const int	AUTO_PHASE_LENGTH			= 100;

static const float	SPEED_CUBE					= 0.003f;

static const int	NUM_HALO_PARTICLES			= 100;
static const int	NUM_BLOCKS_WALL_WIDE		= 5;
static const int	SHADOW_MAP_RESOLUTION		= 2048;
static const int	QUAD_TEXTURE_SIZE			= 256;
static const int	WALL_SIZE					= 2048;

static const float	EXTRUDE_SPEED_BLOCK			= 0.008f;
static const float	MAX_DEPTH_BLOCK				= 0.3f;
static const float	MIN_DEPTH_BLOCK				= 0.7f;

static const float	EXTRACT_SPEED_ARROW			= 0.035f;
static const float	MAX_ARROW_RIGHT				= -3.9f;
static const float	MIN_ARROW_RIGHT				= -8.2f;
static const float	MIN_ARROW_SCALEX			= 0.3f;

static const int	NUM_QUESTIONS				= 14;
static const float	EXTRUDE_SPEED_QUESTION		= 0.007f;
static const float	MAX_DEPTH_QUESTION			= -2.0f;
static const float	MIN_DEPTH_QUESTION			= -1.5f;

static const Color	BLUE( 0.25f, 0.25f, 1.0f );
static const Color	RED( 1.0f, 0.55f, 0.55f );
static const Color	STREET_LIGHT(0.48f, 0.43f, 0.42f);

static const Vec3f	CAM_POSITION_INIT( 0, 0, 6.6);
static const Vec3f	LIGHT_POSITION_INIT( -1.9, -1.0, 6 );

static const Vec3f	BASE_CUBE_POS(-0.30f, 0.0f, 0.1f);
static const Vec3f	MID_CUBE_POS(-1.16f, 0.26f, 0.1f);
static const Vec3f	SIDE_CUBE_POS(-2.3f, 1.36f, 0.1f);

static const Vec3f	TWEET_INIT_POS(1.15f, -8.0f, 0.0f);
static const float	TWEET_SCALE					= 0.3f;
static const float	TWEET_SIG_SCALE				= 0.3f;

static const std::string SWEAR_STRING = "";
static const std::string IGNORE_STRING ="the,a,as,an,of,@PhiloCubeTest,@efcube";
static const std::string HTML_ENTITIES ="";

static std::vector<std::string> SWEAR_STRING_VECTOR;
static std::vector<std::string> IGNORE_STRING_VECTOR;
static std::vector<std::string> HTML_ENTITIES_VECTOR;

//Hackey simple solution
static std::vector<int> WORDS_COUNTS;
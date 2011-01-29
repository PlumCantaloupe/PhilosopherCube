#pragma once

#include "Resources.h"
#include "DefinesAndConstants.h"

#include "cinder/ObjLoader.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/DisplayList.h"
#include "cinder/gl/Material.h"
#include "cinder/Perlin.h"

#include "CustomStructs.h"
#include "Particle.h"

using namespace ci;
using namespace ci::app;

class AlienCube
{
	public:
		AlienCube(){ /*nothing happening here*/ };
		~AlienCube(){ /*nothing happening or here*/ };
	
		void setup( float size, ci::Vec3f pos, int subdivisions, gl::Texture * cubeTexture );
		void update();
		void draw();
		void wander( bool stop );
		void goToPosition( Vec3f targetPos );
		bool reachedTarget();
	
		float size_cube;
		ci::Vec3f pos_cube;
		ci::Vec3f rotation;
		//Matrix44f	mCubeRotation;
		float scaleFactor;
	
		std::vector<Particle> particles;
		
	private:
		void createAlienList( int subdivisions, float size );								//function that saves alienCube render calls
		void drawPlaneWithinBegin( int subWidth, int subHeight, ci::Vec3f bottomLeft, ci::Vec3f bottomRight, ci::Vec3f topRight, ci::Vec3f topLeft, 
								  ci::Vec2f bottomLeft_tex, ci::Vec2f bottomRight_tex, ci::Vec2f topRight_tex, ci::Vec2f topLeft_tex );
	
		gl::Texture * texture;
		int quadTexSize;
		gl::DisplayList alienCubeList;								//var that saves cube render calls for future reference
		bool isAnimating;
		int randomAnimStart;
		int currFrame;
		int frameConversion;
	
		Vec3f		currentTarget;

		TriMesh			mMesh;	
		gl::VboMesh		mVBOs[NUM_CUBE_FRAMES];
		int animCounter;
	
		int						mSeed;
		int						mOctaves;
		Perlin					mPerlin;
		float mCounter;
};

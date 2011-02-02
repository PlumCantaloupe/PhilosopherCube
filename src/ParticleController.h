/*
 *  ParticleController.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-20.
 *
 */

#pragma once

#include "Resources.h"

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"

#include "Particle.h"
#include "Utilities_ACS.h"


using namespace ci;
using namespace ci::app;

class ParticleController
{
public:
	ParticleController(){};
	~ParticleController(){};	
	
	ParticleController(Vec3f initPos, float initSpeed, int lifeExpect, float initSize, Boolean keepSeparate, Vec3f constantForce, int numParticles);
	
	void update( Vec3f newPos );
	void draw( Vec3f& billRight, Vec3f& billUp, gl::Texture& mPartText );
	
	Vec3f mSpawnPosition;
	std::vector<Particle> mParticles;
	int mNumParticles;
	
private:
	
	gl::GlslProg mBasicShader;
	Vec3f mInitPos;
	float mInitSpeed;
	float mInitSize;
	int mInitLife;
	float mInitRadius;
	
};

/*
 *  ParticleController.cpp
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-20.
 *
 */

#include "ParticleController.h"

ParticleController::ParticleController(Vec3f initPos, float initSpeed, int lifeExpect, float initSize, Boolean keepSeparate, Vec3f constantForce, int numParticles)
{
	mNumParticles = numParticles;
	mInitPos = initPos;
	mInitSpeed = initSpeed;
	mInitSize = initSize;
	mInitLife = lifeExpect;
	mInitRadius = initSize;
	mBasicShader = gl::GlslProg( loadResource( BASIC_TEXTURE_VERT ), loadResource( BASIC_TEXTURE_FRAG ) );

	Vec3f dirVec;
	
	for (int i = 0; i < mNumParticles; i++)
	{
		//std::cout << initPos.x << " " << initPos.y << " " << initPos.z << std::endl;
		
		dirVec = Vec3f( Rand::randFloat( -1.0f, 1.0f ), -1.0f, Rand::randFloat( -1.0f, 1.0f ) );
		initPos.x += Rand::randFloat( -0.3, 0.3 );
		
		dirVec.normalize();
		dirVec *= initSpeed;
		Particle newPart = Particle( initPos, dirVec, (int)Rand::randFloat(mInitLife * 0.1f, mInitLife * 1.0f), Rand::randFloat(mInitSize * 0.8f, mInitSize * 2.2f), false, Vec3f(0, Rand::randFloat(-3.1, -1.8), 0) );
		mParticles.push_back( newPart );
	}
}

void ParticleController::update( Vec3f newPos )
{
	for( int i = 0; i < mParticles.size(); i++)
	{
		if(mParticles[i].isDead)
		{
			//halo stuff
			mParticles[i].loc[0] = newPos;
			mParticles[i].lifespan = (int)Rand::randFloat(mInitLife * 0.1f, mInitLife * 1.0f);
			mParticles[i].loc[0].x += Rand::randFloat( -0.3, 0.3 );
			mParticles[i].radius = Rand::randFloat(mInitSize * 0.8f, mInitSize * 2.2f);
			//halo stuff
			
			mParticles[i].reset();
		}
		
		mParticles[i].update();
	}
}

void ParticleController::draw( Vec3f& billRight, Vec3f& billUp, gl::Texture& mPartText )
{
	mBasicShader.bind();
	Utilities_ACS::setTextureWithinShader( "texture", mPartText, mBasicShader, 4 );
	float alphaVal;
	for( int i = 0; i < mParticles.size(); i++)
	{
		alphaVal = 1.0f - ((float)mParticles[i].currAge/(float)mParticles[i].lifespan);
		mBasicShader.uniform("alpha", alphaVal );
		mParticles[i].draw( billRight, billUp );
	}
	mBasicShader.unbind();
}
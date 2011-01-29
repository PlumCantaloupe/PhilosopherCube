#pragma once

#include "DefinesAndConstants.h"

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderMath.h"
#include "cinder/Perlin.h"

#include "cinder/gl/Material.h"
#include "cinder/gl/DisplayList.h"

using namespace ci;
using namespace ci::app;

class Particle
{
    public:
        Vec3f vel;
        Vec3f frc;			// frc is also know as acceleration (newton says "f=ma")
		Vec3f constantFrc;   // frc that is always present even after reset "forces" - specifically call "resetConstantForce" to get rid of it
		Vec3f loc[ TRAIL_LENGTH ];	// array of position vectors
		float radius;			//size of particel considering volume input
	
		float alphaDiff;
		float colour[4];
		bool alphaAge;
		bool sizeAge;
		bool decreaseConstantFrc;
	
		Particle(){};
		Particle(Vec3f initPos, Vec3f initVel, int lifeExpect, float initSize, Boolean keepSeparate, Vec3f constantForce );
		virtual ~Particle(){};

		void reset();
        void resetForce();
		void addForce( Vec3f newFrc );
		void addConstantForce( Vec3f newFrc );
		void resetConstantForce();
		void setCloudColour();
	
		void setColour(int rx, int gx, int bx, int alpha);
		void setColour(int rx, int gx, int bx);
		void setSize(float diameter);
	
		void addRepulsionForce(float x, float y, float radius, float scale);
		void addAttractionForce(float x, float y, float radius, float scale);
		void addRepulsionForce(Particle &p, float radius, float scale);
		void addAttractionForce(Particle &p, float radius, float scale);
		
		void addDampingForce();
        
		void update();
		void draw( Vec3f billRight, Vec3f billUp );
		void renderTrails();
	
		void bounceOffWalls();
	
		float damping;
	
		bool isDead;
		bool separate;
		int lifespan;
		int currAge;
	
		//DEBUG
		gl::DisplayList mTestObject; 

    protected:
    private:
	
		int						mSeed;
		int						mOctaves;
		Perlin					mPerlin;
	
		float  noiseOff;
		float noiseInc;
		float decay;
	
		bool infiniteLife;
		bool taperTrail;
};

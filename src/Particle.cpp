#include "particle.h"

//------------------------------------------------------------
Particle::Particle(Vec3f initPos, Vec3f initVel, int lifeExpect, float initSize, Boolean keepSeparate, Vec3f constantForce )
{
	//perlin noise
	//mSeed = clock() & 65535;
	mSeed = clock() & 655;
	mOctaves = 4;
	mPerlin = Perlin( mOctaves, mSeed );
	
	damping = 0.2f;
	
	noiseOff = 0.0;
	noiseInc = 0.005;
	//noise = new ofxPerlin();
	decay = 0.002;
	
	constantFrc = constantForce;		//add some randomness to force
	separate = keepSeparate;
	taperTrail = 0.1;
	alphaAge = false;
	sizeAge = false;
	alphaDiff = 255.0f;
	
	if ( lifeExpect < 0 )
	{
		infiniteLife = true;
	}
	else 
	{
		infiniteLife = false;
		lifespan = lifeExpect; 
		decreaseConstantFrc = true;
	}
	
	currAge = 0;
	isDead = false;
	
	for( int i=0; i < TRAIL_LENGTH; i++ )
	{
		loc[i] = initPos;
    }
	
    loc[0] = initPos;
	vel = initVel;
	
	colour[0] = 255;
	colour[1] = 255;
	colour[2] = 255;
	colour[3] = 255;
	
	radius = initSize;
	
	resetConstantForce();
	addConstantForce( constantForce );
	
	//DEBUG
	//create materials
	gl::Material torusMaterial;
	torusMaterial.setSpecular( Color( 0.25f, 0.25f, 1.0f ) );
	torusMaterial.setDiffuse( Color( 0.25f, 0.25f, 1.0f ) );
	torusMaterial.setAmbient( Color( 0.1f, 0.1f, 0.1f ) );
	torusMaterial.setShininess( 25.0f );
	
	mTestObject = gl::DisplayList( GL_COMPILE );
	mTestObject.newList();
	gl::drawCube( Vec3f( 0.0f, 0.0f, 0.0f ), Vec3f( 0.2f, 0.2f, 0.2f ) );
	mTestObject.endList();
	mTestObject.setMaterial( torusMaterial );
}

//------------------------------------------------------------
void Particle::resetForce()
{
    // we reset the forces every frame
    frc = Vec3f::zero();
}

//------------------------------------------------------------
void Particle::addForce( Vec3f newFrc )
{
    frc += newFrc;
}

void Particle::addConstantForce( Vec3f newFrc )
{
    constantFrc += newFrc;
}

void Particle::resetConstantForce()
{
    // we reset the forces every frame
    constantFrc = Vec3f::zero();
}

void Particle::setColour(int rx, int gx, int bx, int alpha)
{
	if (rx > 255) rx = 255;
	if (rx < 0 ) rx = 0;
	
	if (gx > 255) rx = 255;
	if (gx < 0 ) rx = 0;
	
	if (bx > 255) rx = 255;
	if (bx < 0 ) rx = 0;
	
	if (alpha > 255) rx = 255;
	if (alpha < 0 ) rx = 0;
	
	colour[0] = rx;
	colour[1] = gx;
	colour[2] = bx;
	colour[3] = alpha;
}

void Particle::setColour(int rx, int gx, int bx)
{
	if (rx > 255) rx = 255;
	if (rx < 0 ) rx = 0;
	
	if (gx > 255) rx = 255;
	if (gx < 0 ) rx = 0;
	
	if (bx > 255) rx = 255;
	if (bx < 0 ) rx = 0;
	
	colour[0] = rx;
	colour[1] = gx;
	colour[2] = bx;
}

void Particle::setSize(float diameter)
{
	radius = diameter;
}

//------------------------------------------------------------
void Particle::addRepulsionForce(float x, float y, float radius, float scale)
{
    
	// ----------- (1) make a vector of where this position is: 
	
	Vec3f posOfForce;
	posOfForce = Vec3f(x,y,0);
	
	// ----------- (2) calculate the difference & length 
	
	Vec3f diff	= loc[0] - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0)
	{
        if (length > radius)
		{
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true)
	{
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
		frc.x = frc.x + diff.x * scale * pct;
        frc.y = frc.y + diff.y * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(float x, float y, float radius, float scale){
    
	// ----------- (1) make a vector of where this position is: 
	
	Vec3f posOfForce;
	posOfForce = Vec3f(x,y,0);
	
	// ----------- (2) calculate the difference & length 
	
	Vec3f diff	= loc[0] - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0)
	{
        if (length > radius)
		{
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true)
	{
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x - diff.x * scale * pct;
        frc.y = frc.y - diff.y * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addRepulsionForce(Particle &p, float radius, float scale)
{
	
	// ----------- (1) make a vector of where this particle p is: 
	Vec3f posOfForce;
	posOfForce = p.loc[0];
	
	// ----------- (2) calculate the difference & length 
	
	Vec3f diff	= loc[0] - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0)
	{
        if (length > radius)
		{
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true)
	{
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x + diff.x * scale * pct;
        frc.y = frc.y + diff.y * scale * pct;
		p.frc.x = p.frc.x - diff.x * scale * pct;
        p.frc.y = p.frc.y - diff.y * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(Particle & p, float radius, float scale)
{
	
	// ----------- (1) make a vector of where this particle p is: 
	Vec3f posOfForce;
	posOfForce = p.loc[0];
	
	// ----------- (2) calculate the difference & length 
	
	Vec3f diff	= loc[0] - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0)
	{
        if (length > radius)
		{
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true)
	{
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x - diff.x * scale * pct;
        frc.y = frc.y - diff.y * scale * pct;
		p.frc.x = p.frc.x + diff.x * scale * pct;
        p.frc.y = p.frc.y + diff.y * scale * pct;
    }
	
}


//------------------------------------------------------------
void Particle::addDampingForce(){
	
	// the usual way to write this is  vel *= 0.99
	// basically, subtract some part of the velocity 
	// damping is a force operating in the oposite direction of the 
	// velocity vector
	
    frc.x = frc.x - vel.x * damping;
    frc.y = frc.y - vel.y * damping;
}

void Particle::reset()
{
	currAge = 0;
	isDead = false;
}

//------------------------------------------------------------
void Particle::update()
{	
	if ( !infiniteLife )
	{
		++currAge;
		if ( currAge > lifespan )
		{
			isDead = true;
		}
		
		//if (alphaAge) alphaDiff		-= (255.0f/lifespan);
		if (decreaseConstantFrc)	constantFrc *= decay;
		if (true)			setColour(alphaDiff, alphaDiff + 40, alphaDiff + 100);
	}
		
//	alphaDiff += 10.0f; 
//	noiseOff += noiseInc;
//	float offset = noise->noise(  amplitude * noiseOff ) * amplitude/10.0;
	
	vel = vel + constantFrc;
	
	if( separate ) 
	{
		vel = vel + frc;
	}
	
	for (int i = TRAIL_LENGTH - 1; i > 0; i--)
	{
		loc[i] = loc[i - 1];
		//printf(" ( %f %f ) ", loc[i].x, loc[i].y);
	}
	loc[0] = loc[0] + vel; /* + offset */;
	loc[0].x = loc[0].x + (mPerlin.noise( loc[0].y ) * 0.009);
	
	//printf("\n");
}

//---------------------------------

void Particle::draw( Vec3f billRight, Vec3f billUp )
{
	//DEBUG
//	glPushMatrix();
//	glTranslatef(loc[0].x, loc[0].y, loc[0].z);
//	mTestObject.draw();
//	glPopMatrix();

	//gets smaller as ages
	//float modifier = (float)currAge/(float)lifespan;
	//modRadius *= ( 1.0f - modifier );

	glPushMatrix();
	glNormal3f( 0, 0, 1); //were not rendering properly for some reason ....
	gl::drawBillboard(Vec3f(loc[0].x, loc[0].y, loc[0].z) , Vec2f(radius, radius), 0.0, billRight, billUp);
	glPopMatrix();
		
	glClearColor(1, 1, 1, 1);
}

void Particle::renderTrails()
{	
	Vec3f off1;
	Vec3f off2;
	Vec3f perp0;
	Vec3f perp1;
	Vec3f perp2;
	float per;
	
	glBegin( GL_QUAD_STRIP );
		for( int i=0; i<TRAIL_LENGTH-2; i++ )
		{
			per				= i / (float)(TRAIL_LENGTH-3);
			
			perp0			= loc[i] - loc[i+1];
			perp1			= perp0.cross( Vec3f( 0, 0, 1 ) );
			
			perp2			= perp0 *  -1.0;
			perp1.normalize();
			perp2.normalize();
			
			float taperFactor;
			if ( taperTrail )
			{
				taperFactor = ( 1.0f - per );
			}
			else 
			{
				taperFactor = 1.0f;
			}

			off1			= perp1 * radius/3.5f * taperFactor * 0.5  + loc[i];
			off2			= perp2 * radius/3.5f * taperFactor * 0.5  + loc[i];
			
			glColor4f( colour[0]/255.0f, colour[1]/255.0f, colour[2]/255.0f, ((alphaDiff/255.0f)/pow(i + 1, 0.5)));		
			
			//bound check
			if ( math<float>::abs(loc[i].y - loc[i - 1].y) < getWindowHeight() )
			{
				glVertex3f( off1.x, off1.y, off1.z );
				glVertex3f( off2.x, off2.y, off2.z );
			}
			else 
			{
				break;
			}

		}
	glEnd();
}

//------------------------------------------------------------
void Particle::bounceOffWalls(){
	
	// sometimes it makes sense to damped, when we hit
	bool bDampedOnCollision = true;
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = getWindowWidth();
	float maxy = getWindowHeight();
	
	if (loc[0].x > maxx){
		loc[0].x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (loc[0].x < minx){
		loc[0].x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
	
	if (loc[0].y > maxy){
		loc[0].y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (loc[0].y < miny){
		loc[0].y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.3;
	}
	
}

void Particle::setCloudColour()
{
	//setColour(amplitude, 0, 0, amplitude);
}

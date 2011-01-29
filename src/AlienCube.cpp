#include "AlienCube.h"

//--------------------------------------------------------------
void AlienCube::setup( float size, ci::Vec3f pos, int subdivisions, gl::Texture * cubeTexture )
{
	animCounter = 0;
	randomAnimStart = 0;
	currFrame = 0;
	isAnimating = false;
	frameConversion = int((getFrameRate()/24.0f) * 3.0f);
	
	//loading frames for "animation"
	ObjLoader loader1( loadResource( CUBE_OBJ_1 )->createStream() );
	loader1.load( &mMesh );
	mVBOs[0] = gl::VboMesh( mMesh );
	
	ObjLoader loader2( loadResource( CUBE_OBJ_2 )->createStream() );
	loader2.load( &mMesh );
	mVBOs[1] = gl::VboMesh( mMesh );
	
	ObjLoader loader3( loadResource( CUBE_OBJ_3 )->createStream() );
	loader3.load( &mMesh );
	mVBOs[2] = gl::VboMesh( mMesh );
	
	ObjLoader loader4( loadResource( CUBE_OBJ_4 )->createStream() );
	loader4.load( &mMesh );
	mVBOs[3] = gl::VboMesh( mMesh );
	
	ObjLoader loader5( loadResource( CUBE_OBJ_5 )->createStream() );
	loader5.load( &mMesh );
	mVBOs[4] = gl::VboMesh( mMesh );
	
	ObjLoader loader6( loadResource( CUBE_OBJ_6 )->createStream() );
	loader6.load( &mMesh );
	mVBOs[5] = gl::VboMesh( mMesh );
	
	ObjLoader loader7( loadResource( CUBE_OBJ_7 )->createStream() );
	loader7.load( &mMesh );
	mVBOs[6] = gl::VboMesh( mMesh );
	
	ObjLoader loader8( loadResource( CUBE_OBJ_8 )->createStream() );
	loader8.load( &mMesh );
	mVBOs[7] = gl::VboMesh( mMesh );
	
	ObjLoader loader9( loadResource( CUBE_OBJ_9 )->createStream() );
	loader9.load( &mMesh );
	mVBOs[8] = gl::VboMesh( mMesh );
	
	ObjLoader loader10( loadResource( CUBE_OBJ_10 )->createStream() );
	loader10.load( &mMesh );
	mVBOs[9] = gl::VboMesh( mMesh );
	
	ObjLoader loader11( loadResource( CUBE_OBJ_11 )->createStream() );
	loader11.load( &mMesh );
	mVBOs[10] = gl::VboMesh( mMesh );
	
	ObjLoader loader12( loadResource( CUBE_OBJ_12 )->createStream() );
	loader12.load( &mMesh );
	mVBOs[11] = gl::VboMesh( mMesh );
	
	ObjLoader loader13( loadResource( CUBE_OBJ_13 )->createStream() );
	loader13.load( &mMesh );
	mVBOs[12] = gl::VboMesh( mMesh );
	
	ObjLoader loader14( loadResource( CUBE_OBJ_14 )->createStream() );
	loader14.load( &mMesh );
	mVBOs[13] = gl::VboMesh( mMesh );
	
	ObjLoader loader15( loadResource( CUBE_OBJ_15 )->createStream() );
	loader15.load( &mMesh );
	mVBOs[14] = gl::VboMesh( mMesh );
	
	ObjLoader loader16( loadResource( CUBE_OBJ_16 )->createStream() );
	loader16.load( &mMesh );
	mVBOs[15] = gl::VboMesh( mMesh );
	
	ObjLoader loader17( loadResource( CUBE_OBJ_17 )->createStream() );
	loader17.load( &mMesh );
	mVBOs[16] = gl::VboMesh( mMesh );
	
	ObjLoader loader18( loadResource( CUBE_OBJ_18 )->createStream() );
	loader18.load( &mMesh );
	mVBOs[17] = gl::VboMesh( mMesh );
	
	ObjLoader loader19( loadResource( CUBE_OBJ_19 )->createStream() );
	loader19.load( &mMesh );
	mVBOs[18] = gl::VboMesh( mMesh );
	
	ObjLoader loader20( loadResource( CUBE_OBJ_20 )->createStream() );
	loader20.load( &mMesh );
	mVBOs[19] = gl::VboMesh( mMesh );
	
	ObjLoader loader21( loadResource( CUBE_OBJ_21 )->createStream() );
	loader21.load( &mMesh );
	mVBOs[20] = gl::VboMesh( mMesh );
	
	ObjLoader loader22( loadResource( CUBE_OBJ_22 )->createStream() );
	loader22.load( &mMesh );
	mVBOs[21] = gl::VboMesh( mMesh );
	
	ObjLoader loader23( loadResource( CUBE_OBJ_23 )->createStream() );
	loader23.load( &mMesh );
	mVBOs[22] = gl::VboMesh( mMesh );
	
	ObjLoader loader24( loadResource( CUBE_OBJ_24 )->createStream() );
	loader24.load( &mMesh );
	mVBOs[23] = gl::VboMesh( mMesh );

	
	size_cube = size;
	pos_cube = pos;
	texture = cubeTexture;
	quadTexSize = (int)cubeTexture->getWidth()/3;
	
	scaleFactor = size;
	
	//create material
	gl::Material cubeMaterial;
	cubeMaterial.setSpecular( Color( 0.0, 0.0, 0.0 ) );
	cubeMaterial.setDiffuse( Color( 0.59, 0.84, 0.71 ) );
	cubeMaterial.setAmbient( Color( 0.1f, 0.1f, 0.1f ) );
	cubeMaterial.setShininess( 2.0f );
	
	//create alien cube list
	alienCubeList = gl::DisplayList( GL_COMPILE );
	alienCubeList.newList();
	createAlienList( subdivisions, size );
	alienCubeList.endList();
	alienCubeList.setMaterial( cubeMaterial );
	
	rotation = ci::Vec3f(0,0,0);
	
	//perlin noise
	mSeed = clock() & 65535;
	mOctaves = 4;
	mPerlin = Perlin( mOctaves, mSeed );
	mCounter = 0.0f;
	
	currentTarget = Vec3f(0.0f, 0.0f, -1001.0f); //means that there is no target
}

void AlienCube::update()
{
	rotation.x += 0.08f;
	rotation.y += 0.05f;
	rotation.z += 0.12f;
	
	mCounter += 0.05;
	
	if (mCounter > 360.0f)
	{
		mCounter == 0.0f;
	}
	
	pos_cube.x += (0.0005f * sin(mCounter));
	pos_cube.y += (0.001f * cos(mCounter + 0.005));
	
	//then there is a target
	if ( currentTarget.z > -1000.0f  ) 
	{
		Vec3f dirVec = currentTarget - pos_cube;
		
		if ( dirVec.length() <= SPEED_CUBE)
		{
			printf("Cube reached destination! \n");
			
			//has reached target
			currentTarget = Vec3f(0.0f, 0.0f, -1001.0f); //means that there is no target
		}
		else 
		{
			//hasn't reached target yet
			dirVec.normalize();
			dirVec = dirVec * SPEED_CUBE;
			
			pos_cube.x += dirVec.x;
			pos_cube.y += dirVec.y;
		}
	}
	
}

void AlienCube::draw()
{
	glPushMatrix();
	//glTranslatef(pos_cube.x + (mPerlin.fBm( pos_cube.x * mCounter  ) * 0.09), pos_cube.y + (mPerlin.fBm( pos_cube.y * mCounter ) * 0.09), pos_cube.z);
	glTranslatef(pos_cube.x, pos_cube.y, pos_cube.z);
	//printf("%f\n", (mPerlin.noise( pos_cube.x * mCounter  ) * 0.9));
	
	glRotatef(rotation.x,1.0f,0.0f,0.0f);									// Rotate On The X Axis
	glRotatef(rotation.y,0.0f,1.0f,0.0f);									// Rotate On The Y Axis
	glRotatef(rotation.z,0.0f,0.0f,1.0f);									// Rotate On The Z Axis
	
	glScalef(scaleFactor, scaleFactor, scaleFactor);

	//animated OBJ
	if ( isAnimating )
	{
		gl::draw( mVBOs[ animCounter ] );
		if( (getElapsedFrames() % frameConversion) == 0 ) ++animCounter;
		if( animCounter > (NUM_CUBE_FRAMES - 1) ) 
		{
			animCounter = 0;
			isAnimating = false;
			randomAnimStart = Rand::randInt( 1000 );
			currFrame = getElapsedFrames();
		}
	}
	else 
	{
		gl::draw( mVBOs[ 0 ] ); //just draw first "frame"
		
		if( (getElapsedFrames() - currFrame) > randomAnimStart )
		{
			isAnimating = true;
		}
	}
	
	//alienCubeList.draw();
	glPopMatrix();
}

void AlienCube::wander( bool stop )
{
}

void AlienCube::goToPosition( Vec3f targetPos )
{	
	currentTarget = targetPos; //means that there is no target
}

bool AlienCube::reachedTarget()
{
	return ( currentTarget.z <= -1000.0f );
}

void AlienCube::createAlienList( int subdivisions, float size )
{
	float halfSize = size/2.0f;
	
	//	if(!(bool)glIsEnabled(GL_NORMALIZE)) glEnable(GL_NORMALIZE);
	glBegin(GL_QUADS);
	drawPlaneWithinBegin( subdivisions, subdivisions, 
						 ci::Vec3f(	-halfSize, -halfSize,  halfSize), 
						 ci::Vec3f( halfSize, -halfSize,  halfSize), 
						 ci::Vec3f( halfSize,  halfSize,  halfSize), 
						 ci::Vec3f(	-halfSize,  halfSize,  halfSize), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				(quadTexSize * 2.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize*2.0f)/(quadTexSize*3.0f),		(quadTexSize * 2.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	quadTexSize/(quadTexSize*4.0f)			), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				quadTexSize/(quadTexSize*4.0f)			));
	
						 drawPlaneWithinBegin( subdivisions, subdivisions, 
						 ci::Vec3f(	-halfSize, -halfSize, -halfSize), 
						 ci::Vec3f( -halfSize,  halfSize, -halfSize), 
						 ci::Vec3f(	halfSize,  halfSize, -halfSize), 
						 ci::Vec3f(	halfSize, -halfSize, -halfSize), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				(quadTexSize * 3.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				(quadTexSize * 4.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	(quadTexSize * 4.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	(quadTexSize * 3.0f)/(quadTexSize*4.0f) ));	
						 
						 drawPlaneWithinBegin( subdivisions, subdivisions, 
						 ci::Vec3f(	-halfSize,  halfSize, -halfSize), 
						 ci::Vec3f(	-halfSize,  halfSize,  halfSize), 
						 ci::Vec3f(	halfSize,  halfSize,  halfSize), 
						 ci::Vec3f(	halfSize,  halfSize, -halfSize), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				0.0f									), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				quadTexSize/(quadTexSize*4.0f)			), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	quadTexSize/(quadTexSize*4.0f)			), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	0.0f									));
						 
						 drawPlaneWithinBegin( subdivisions, subdivisions, 
						 ci::Vec3f(	-halfSize, -halfSize, -halfSize), 
						 ci::Vec3f(	halfSize, -halfSize, -halfSize), 
						 ci::Vec3f(	halfSize, -halfSize,  halfSize), 
						 ci::Vec3f(	-halfSize, -halfSize,  halfSize), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				(quadTexSize * 3.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	(quadTexSize * 3.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	(quadTexSize * 2.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				(quadTexSize * 2.0f)/(quadTexSize*4.0f)	));
						 
						 drawPlaneWithinBegin( subdivisions, subdivisions, 
						 ci::Vec3f(	halfSize, -halfSize, -halfSize), 
						 ci::Vec3f(	halfSize,  halfSize, -halfSize), 
						 ci::Vec3f(	halfSize,  halfSize,  halfSize), 
						 ci::Vec3f(	halfSize, -halfSize,  halfSize), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	(quadTexSize * 3.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 3.0f)/(quadTexSize*3.0f),	(quadTexSize * 3.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 3.0f)/(quadTexSize*3.0f),	(quadTexSize * 2.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	(quadTexSize * 2.0f)/(quadTexSize*3.0f),	(quadTexSize * 2.0f)/(quadTexSize*3.0f) ));
						 
						 drawPlaneWithinBegin( subdivisions, subdivisions, 
						 ci::Vec3f(	-halfSize, -halfSize, -halfSize), 
						 ci::Vec3f(	-halfSize, -halfSize,  halfSize), 
						 ci::Vec3f(	-halfSize,  halfSize,  halfSize), 
						 ci::Vec3f(	-halfSize,  halfSize, -halfSize), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				(quadTexSize * 3.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	quadTexSize/(quadTexSize*3.0f),				(quadTexSize * 2.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	0.0f,										(quadTexSize * 2.0f)/(quadTexSize*4.0f)	), 
						 ci::Vec2f(	0.0f,										(quadTexSize * 3.0f)/(quadTexSize*4.0f)	));
	 
	glEnd();
	glEndList();
	
}


//to draw a quad/plane within a opengl render call glBegin(GL_QUADS)
void AlienCube::drawPlaneWithinBegin( int subWidth, int subHeight, ci::Vec3f bottomLeft, ci::Vec3f bottomRight, ci::Vec3f topRight, ci::Vec3f topLeft, 
									 ci::Vec2f bottomLeft_tex, ci::Vec2f bottomRight_tex, ci::Vec2f topRight_tex, ci::Vec2f topLeft_tex )
{
	//references to vertices of each quad
	ci::Vec3f vert0;
	ci::Vec3f vert1;
	ci::Vec3f vert2;
	ci::Vec3f vert3;
	ci::Vec3f quadPos;
	
	//vertex calculations and variable initializations
	ci::Vec3f botLToBotR = bottomRight - bottomLeft;
	ci::Vec3f topRToBotR = topRight - bottomRight;
	ci::Vec3f normalVec = ci::Vec3f( botLToBotR.x, botLToBotR.y, botLToBotR.z ).cross( topRToBotR );
	float quantWidth = botLToBotR.length()/(float)subWidth;
	float quantHeight = topRToBotR.length()/(float)subHeight;
	
	botLToBotR.normalize();
	botLToBotR *= quantWidth;
	
	topRToBotR.normalize();
	topRToBotR *= quantHeight;
	
	normalVec.normalize();
	
	//texture calculations and coordinate inits
	ci::Vec2f botLToBotR_tex = bottomRight_tex - bottomLeft_tex;
	ci::Vec2f topRToBotR_tex = topRight_tex - bottomRight_tex;
	float quantWidth_tex = botLToBotR_tex.length()/(float)subWidth;
	float quantHeight_tex = topRToBotR_tex.length()/(float)subHeight;
	
	botLToBotR_tex.normalize();
	botLToBotR_tex *= quantWidth_tex;
	
	topRToBotR_tex.normalize();
	topRToBotR_tex *= quantHeight_tex;
	
	ci::Vec3f currVec;
	ci::Vec2f currVec_tex;
	
	glNormal3f( normalVec.x, normalVec.y, normalVec.z); //default normal vec - need to change later
	
	for( int i = 0; i < subHeight; i++ )
	{
		
		for( int j = 0; j < subWidth; j++ )
		{
			currVec		= ci::Vec3f(bottomLeft + (botLToBotR * j) + (topRToBotR * i));
			currVec_tex = ci::Vec2f(bottomLeft_tex	+ (botLToBotR_tex * j) + (topRToBotR_tex * i));
			
			quad tempQuad = quad();
			tempQuad.verts[0]	= ci::Vec3f( currVec.x, currVec.y, currVec.z );																								// bottom left
			tempQuad.verts[1]	= ci::Vec3f( currVec.x + botLToBotR.x, currVec.y + botLToBotR.y, currVec.z + botLToBotR.z );													// bottom right
			tempQuad.verts[2]	= ci::Vec3f( currVec.x + topRToBotR.x + botLToBotR.x, currVec.y + topRToBotR.y + botLToBotR.y, currVec.z + topRToBotR.z + botLToBotR.z );	// top right
			tempQuad.verts[3]	= ci::Vec3f( currVec.x + topRToBotR.x, currVec.y + topRToBotR.y, currVec.z + topRToBotR.z );													// top left
			tempQuad.pos		= vert0 + (vert2 - vert0)/2.0f; //(topRight - bottomLeft)/2 + bottomleft
			
			//allow for 3 layers of textures for later use
			glMultiTexCoord2f(GL_TEXTURE0, currVec_tex.x, currVec_tex.y);
			glMultiTexCoord2f(GL_TEXTURE1, currVec_tex.x, currVec_tex.y);
			glMultiTexCoord2f(GL_TEXTURE2, currVec_tex.x, currVec_tex.y);
			glVertex3f( tempQuad.verts[0].x, tempQuad.verts[0].y, tempQuad.verts[0].z );	
			
			glMultiTexCoord2f(GL_TEXTURE0, currVec_tex.x + botLToBotR_tex.x, currVec_tex.y + botLToBotR_tex.y);
			glMultiTexCoord2f(GL_TEXTURE1, currVec_tex.x + botLToBotR_tex.x, currVec_tex.y + botLToBotR_tex.y);
			glMultiTexCoord2f(GL_TEXTURE2, currVec_tex.x + botLToBotR_tex.x, currVec_tex.y + botLToBotR_tex.y);
			glVertex3f( tempQuad.verts[1].x, tempQuad.verts[1].y, tempQuad.verts[1].z );
			
			glMultiTexCoord2f(GL_TEXTURE0, currVec_tex.x + topRToBotR_tex.x + botLToBotR_tex.x, currVec_tex.y + topRToBotR_tex.y + botLToBotR_tex.y);
			glMultiTexCoord2f(GL_TEXTURE1, currVec_tex.x + topRToBotR_tex.x + botLToBotR_tex.x, currVec_tex.y + topRToBotR_tex.y + botLToBotR_tex.y);
			glMultiTexCoord2f(GL_TEXTURE3, currVec_tex.x + topRToBotR_tex.x + botLToBotR_tex.x, currVec_tex.y + topRToBotR_tex.y + botLToBotR_tex.y);
			glVertex3f( tempQuad.verts[2].x, tempQuad.verts[2].y, tempQuad.verts[2].z );
			
			glMultiTexCoord2f(GL_TEXTURE0, currVec_tex.x + topRToBotR_tex.x, currVec_tex.y + topRToBotR_tex.y);
			glMultiTexCoord2f(GL_TEXTURE1, currVec_tex.x + topRToBotR_tex.x, currVec_tex.y + topRToBotR_tex.y);
			glMultiTexCoord2f(GL_TEXTURE2, currVec_tex.x + topRToBotR_tex.x, currVec_tex.y + topRToBotR_tex.y);
			glVertex3f( tempQuad.verts[3].x, tempQuad.verts[3].y, tempQuad.verts[3].z );
			
		}
	}
}

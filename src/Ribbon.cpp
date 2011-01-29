/*
 *  Ribbon.cpp
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-24.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#include "Ribbon.h"

Ribbon::Ribbon()
{}

Ribbon::~Ribbon()
{}

void Ribbon::setup( int type )
{
	//type 0, 1, 2, 3 ...
	
	mRibbonType = type;
	
	//loading 3D models
	switch ( type )
	{
		case 0:
		{
			ObjLoader loader1( loadResource( RIBBON_ONE )->createStream() );
			loader1.load( &mMesh );
			mVBO = gl::VboMesh( mMesh );
		}
			break;
		case 1:
		{
			ObjLoader loader2( loadResource( RIBBON_TWO )->createStream() );
			loader2.load( &mMesh );
			mVBO = gl::VboMesh( mMesh );
		}
			break;
		case 2:
		{
			ObjLoader loader3( loadResource( RIBBON_THREE )->createStream() );
			loader3.load( &mMesh );
			mVBO = gl::VboMesh( mMesh );
		}
			break;	
		default:
			printf("That type of ribbon doesn't exist");
			break;
	}
}

void Ribbon::update()
{
}

void Ribbon::draw()
{
	//glPushMatrix();
	//glRotatef(90, 1, 0, 0);
	gl::draw( mVBO );
	//glPopMatrix();
	
//	int segments = 5;
//	float halfWidth = 0.25f;
//	Vec3f dirLines[] = { Vec3f(0.25f, 0.05f, 1.0f), Vec3f(0.75f, 0.1f, 1.5f), Vec3f(1.25f, 0.15f, 1.0f), Vec3f(1.75f, 0.1f, 0), Vec3f(2.0f, 0.0f, 0) };
//	Vec3f lineVec;
//	Vec3f perp1;
//	Vec3f perp2;
//	Vec3f currVertVec;
//	Vec3f currNormal;
//	
//	glBegin(GL_QUAD_STRIP);
//		for( int i = 0; i < segments; i++ )
//		{
//			lineVec = dirLines[i];
//			perp1 = lineVec.cross( Vec3f(0, 0, 1) );
//			perp1.normalize();
//			perp2 = perp1 * -1;
//			currVertVec = lineVec + (perp1 * halfWidth);
//			currNormal = Vec3f(0, 0, 1);
//			
//			glNormal3f(currNormal.x, currNormal.y, currNormal.z);
//			glMultiTexCoord2f(GL_TEXTURE10, lineVec.length()/1.0f, 0);  
//			glVertex3f(currVertVec.x, currVertVec.y, currVertVec.z);
//			
//			currVertVec = dirLines[i] + (perp2 * halfWidth);
//			
//			glMultiTexCoord2f(GL_TEXTURE10, lineVec.length()/1.0f, 1);  
//			glVertex3f(currVertVec.x, currVertVec.y, currVertVec.z);
//		}
//	
//	glEnd();
}
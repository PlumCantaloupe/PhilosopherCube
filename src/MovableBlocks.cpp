/*
 *  MovableBlocks.cpp
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-28.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#include "MovableBlocks.h"

void MovableBlocks::setup()
{
	//load all block Pieces
	
	//first row
	ObjLoader loader1( loadResource( BLOCK_0_0 )->createStream() );
	loader1.load( &mMesh );
	mVBOs[0] = gl::VboMesh( mMesh );
	
	ObjLoader loader2( loadResource( BLOCK_1_0 )->createStream() );
	loader2.load( &mMesh );
	mVBOs[1] = gl::VboMesh( mMesh );
	
	ObjLoader loader3( loadResource( BLOCK_2_0 )->createStream() );
	loader3.load( &mMesh );
	mVBOs[2] = gl::VboMesh( mMesh );
	
	ObjLoader loader4( loadResource( BLOCK_3_0 )->createStream() );
	loader4.load( &mMesh );
	mVBOs[3] = gl::VboMesh( mMesh );
	
	ObjLoader loader5( loadResource( BLOCK_4_0 )->createStream() );
	loader5.load( &mMesh );
	mVBOs[4] = gl::VboMesh( mMesh );
	
	//second row
	ObjLoader loader6( loadResource( BLOCK_0_1 )->createStream() );
	loader6.load( &mMesh );
	mVBOs[5] = gl::VboMesh( mMesh );
	
	ObjLoader loader7( loadResource( BLOCK_1_1 )->createStream() );
	loader7.load( &mMesh );
	mVBOs[6] = gl::VboMesh( mMesh );
	
	ObjLoader loader8( loadResource( BLOCK_2_1 )->createStream() );
	loader8.load( &mMesh );
	mVBOs[7] = gl::VboMesh( mMesh );
	
	ObjLoader loader9( loadResource( BLOCK_3_1 )->createStream() );
	loader9.load( &mMesh );
	mVBOs[8] = gl::VboMesh( mMesh );
	
	ObjLoader loader10( loadResource( BLOCK_4_1 )->createStream() );
	loader10.load( &mMesh );
	mVBOs[9] = gl::VboMesh( mMesh );
	
	//third row
	ObjLoader loader11( loadResource( BLOCK_0_2 )->createStream() );
	loader11.load( &mMesh );
	mVBOs[10] = gl::VboMesh( mMesh );
	
	ObjLoader loader12( loadResource( BLOCK_1_2 )->createStream() );
	loader12.load( &mMesh );
	mVBOs[11] = gl::VboMesh( mMesh );
	
	ObjLoader loader13( loadResource( BLOCK_2_2 )->createStream() );
	loader13.load( &mMesh );
	mVBOs[12] = gl::VboMesh( mMesh );
	
	ObjLoader loader14( loadResource( BLOCK_3_2 )->createStream() );
	loader14.load( &mMesh );
	mVBOs[13] = gl::VboMesh( mMesh );
	
	ObjLoader loader15( loadResource( BLOCK_4_2 )->createStream() );
	loader15.load( &mMesh );
	mVBOs[14] = gl::VboMesh( mMesh );
	
	for( int i = 0; i < NUM_WALL_BLOCKS; i++ )
	{
		isAnimatingOut[i]	= false;
		isAnimatingIn[i]	= false;
		currDepth[i]		= MAX_DEPTH_BLOCK;
		minDepth[i]			= MIN_DEPTH_BLOCK;
		
		if ( i == 14 )
		{
			minDepth[i] = MIN_DEPTH_BLOCK * 1.02f;
		}
		else if (i == 13)
		{
			minDepth[i] = MIN_DEPTH_BLOCK * 0.92f;
		}
		else if (i == 12)
		{
			minDepth[i] = MIN_DEPTH_BLOCK * 0.82f;
		}
		else if (i == 11)
		{
			minDepth[i] = MIN_DEPTH_BLOCK * 0.72f;
		}
		else if (i == 10)
		{
			minDepth[i] = MIN_DEPTH_BLOCK * 0.62f;
		}
	}
	
	mWallTex			= gl::Texture( loadImage( loadResource( WALL_BASE_TEX ) ) );
	mTwitterMessage		= gl::Texture( loadImage( loadResource( TWITTER_TEX ) ) );
	mBlackTex			= gl::Texture( loadImage( loadResource( BLACK_TEX ) ) );
	
	mColArrow = gl::DisplayList( GL_COMPILE );
	mColArrow.newList();
	//gl::drawCube( Vec3f( 0.0f, -2.5f, 0.0f ), Vec3f( 10.0f, 0.1f, 10.0f ) );
	glBegin(GL_QUAD_STRIP);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.91f, 1.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.5f);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glTexCoord2f(1.0f, 0.5f);
	glVertex3f(3.3f, 0.4f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.0f);
	glTexCoord2f(0.91f, 0.0f);
	glVertex3f(3.0f, 0.8f, 0.0f);
	
	glEnd();
	
	mColArrow.endList();
	
	mArrowPos = Vec3f( MIN_ARROW_RIGHT, -2.01f, -1.3f);
	mArrowScale = Vec3f( 0.95f, 0.95f, 0.95f );
	mArrowAnimIn = false;
	mArrowAnimOut = false;
	mArrowVisible = false;
	mArrowScaleRate = 0.02; //EXTRUDE_SPEED_BLOCK/(MAX_ARROW_RIGHT - MIN_ARROW_RIGHT);
	
	mBasicShader		= gl::GlslProg( loadResource( BASIC_TEXTURE_VERT ), loadResource( BASIC_TEXTURE_FRAG ) );
	//mColArrow.setMaterial( rectMat );
	
	answerStateActivated = false;
	stepActivated = false;
	bottomAnimated = false;
}

void MovableBlocks::update()
{
	for( int i = 0; i < NUM_WALL_BLOCKS; i++ )
	{
		if (isAnimatingOut[i])
		{
			currDepth[i] += EXTRUDE_SPEED_BLOCK;
			
			if (stepActivated || bottomAnimated) 
			{
				//want movement in other blocks before animation is complete
				if ( currDepth[i] >  minDepth[i])
				{
					isAnimatingOut[i] = false;
					deactivate( i );
				}
				
				if ( currDepth[i] >  (minDepth[i] * 0.7f))
				{
					if ( stepActivated )
					{
						if ( i == (NUM_WALL_BLOCKS - 6) )
						{
							stepActivated = false;
						}
						else
						{
							activate(i + 1);
						}
					}
					else 
					{
						//TODO: concentric animation ( may have no time)
					}

				}
			}
			else 
			{
				if ( currDepth[i] >  minDepth[i])
				{
					isAnimatingOut[i] = false;
					
					if ( answerStateActivated )
					{
						if (i == 13)
						{
							activateRibbon();
							mArrowVisible = true;
						}
						else if ( i < (NUM_WALL_BLOCKS - 6) )
						{
							deactivate( i ); 
						}
					}
					else 
					{
						deactivate( i );
					}
				}
			}
		}
		else if ( isAnimatingIn[i] )
		{
			currDepth[i] -= EXTRUDE_SPEED_BLOCK;
			
			if ( currDepth[i] <  MAX_DEPTH_BLOCK)
			{
				isAnimatingIn[i] = false;
			}
		}
	}
	
	//animation of ribbon
	if (mArrowAnimOut)
	{
		mArrowPos.x		+= EXTRACT_SPEED_ARROW;
		//mArrowScale.x	+= mArrowScaleRate;
		
		if ( mArrowPos.x > MAX_ARROW_RIGHT)
		{
			mArrowAnimOut = false;
			//mArrowScale.x = 0.8f;
			activate(13);
			activate(14);
			//deactivate( i ); //temporary!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
	}
	else if ( mArrowAnimIn )
	{
		mArrowPos.x		-= EXTRACT_SPEED_ARROW;
		//mArrowScale.x	-= mArrowScaleRate;
		
		if ( mArrowPos.x < MIN_ARROW_RIGHT)
		{
			mArrowAnimIn = false;
			mArrowVisible = false;
			//mArrowScale.x = MIN_ARROW_SCALEX;
			deactivate(10);
			deactivate(11);
			deactivate(12);
			deactivate(13);
			deactivate(14);
		}
	}
}

void MovableBlocks::draw( gl::GlslProg* shader )
{
	//printf("%f", currDepth[1]);
	
	//draw all blocks ( at various depths )
	for( int i = 0; i < NUM_WALL_BLOCKS; i++ )
	{
		if((i != 13) && (i != 14))
		{
			glPushMatrix();
			glTranslatef( -1.10f, 1.68f, -1.60f + currDepth[i] );
			glScalef(2.29f, 2.29f, 2.29f);
			glRotatef(90, 1, 0, 0);
			gl::draw( mVBOs[i] );
			glPopMatrix();
		}
	}
	
	//HACK so that these blocks does not get coloured by FBO in main
	Utilities_ACS::setTextureWithinShader( "texture", mWallTex, *shader, 3 );
	glPushMatrix();
	glTranslatef( -1.10f, 1.68f, -1.60f + currDepth[13] );
	glScalef(2.29f, 2.29f, 2.29f);
	glRotatef(90, 1, 0, 0);
	gl::draw( mVBOs[13] );
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( -1.10f, 1.68f, -1.60f + currDepth[14] );
	glScalef(2.29f, 2.29f, 2.29f);
	glRotatef(90, 1, 0, 0);
	gl::draw( mVBOs[14] );
	glPopMatrix();
	
	if ( mArrowVisible )
	{
		
		mBasicShader.bind();
		mBasicShader.uniform("alpha", 1.0f );
		
		Utilities_ACS::setTextureWithinShader( "texture", mTwitterMessage, mBasicShader, 13 );
			glPushMatrix();
			//glTranslatef(-1.1f, -3.0f, -1.4f);
			glTranslatef(mArrowPos.x, mArrowPos.y, mArrowPos.z);
			glScalef(mArrowScale.x, mArrowScale.y, mArrowScale.z);
			mColArrow.draw();
			glPopMatrix();
		
		mBasicShader.unbind();
	}
}

void MovableBlocks::drawMask( gl::GlslProg* shader )
{
	if ( mArrowVisible )
	{
		mBasicShader.bind();
		mBasicShader.uniform("alpha", 1.0f );
		
		Utilities_ACS::setTextureWithinShader( "texture", mBlackTex, mBasicShader, 13 );
		glPushMatrix();
		glTranslatef(-5.87f, 0.0f, mArrowPos.z + 0.001f);
		glRotatef(-90.0f, 0, 0, 1);
		glScalef(10.0f, 3.0f, 1.0f);
		mColArrow.draw();
		glPopMatrix();
		
		mBasicShader.unbind();
	}
}

void MovableBlocks::activate( int xCoord, int yCoord )
{
	int location = getArrIndex( xCoord, yCoord );
	
	if( currDepth[location] <= MAX_DEPTH_BLOCK)
	{
		isAnimatingIn[location] = false;
		isAnimatingOut[location] = true;
	}
}

void MovableBlocks::activate( int blockNum )
{
	if( currDepth[blockNum] <= MAX_DEPTH_BLOCK)
	{
		isAnimatingIn[blockNum] = false;
		isAnimatingOut[blockNum] = true;
	}
}

void MovableBlocks::deactivate( int xCoord, int yCoord )
{
	int location = getArrIndex( xCoord, yCoord ); 
	
	if( currDepth[location] >= minDepth[location])
	{
		isAnimatingIn[location] = true;
		isAnimatingOut[location] = false;
	}
}

void MovableBlocks::deactivate( int blockNum )
{
	if( currDepth[blockNum] >= minDepth[blockNum])
	{
		isAnimatingIn[blockNum] = true;
		isAnimatingOut[blockNum] = false;
	}
}

void MovableBlocks::activateAnswerState()
{
	answerStateActivated = true;
	
	//activateRibbon();
	activate(10);
	activate(11);
	activate(12);
	activate(13);
	activate(14);
}

void MovableBlocks::deactivateAnswerState()
{
	answerStateActivated = false;
	
	deactivateRibbon();
	//deactivate(11);
	//deactivate(12);
}

void MovableBlocks::activateRibbon()
{
	if( mArrowPos.x <= MAX_ARROW_RIGHT)
	{
		mArrowAnimIn = false;
		mArrowAnimOut = true;
	}
}

void MovableBlocks::deactivateRibbon()
{
	if( mArrowPos.x >= MIN_ARROW_RIGHT)
	{
		mArrowAnimIn = true;
		mArrowAnimOut = false;
	}
}

void MovableBlocks::activateStepAnimation()
{
	stepActivated = true;
	activate( 0 );
}

void MovableBlocks::activateBottomAnimation()
{
	bottomAnimated = true;
}

int MovableBlocks::getArrIndex( int xCoord, int yCoord )
{
	return (xCoord + yCoord * NUM_BLOCKS_WALL_WIDE);
}
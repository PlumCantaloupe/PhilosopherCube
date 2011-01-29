/*
 *  Answers.cpp
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-31.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#include "AnswerWall.h"

AnswerWall::AnswerWall()
{
	init3DFont();
	
	testStr = "Love is doing things for those you care about even if what you really want to do is sleep...because it will make them happy. It can also be something felt for an animal such as a dog";
	testName = "~PlumCantaloupe";
	
	//shadow answers
	mNextCanGo = true;
	mCurrMessageVis = false;
	mCurrMessageStr = "";
	mCurrMessageAnimOut = false;
}

AnswerWall::~AnswerWall()
{
	//ftglDestroyFont(font);
}

void AnswerWall::init3DFont()
{
	// Create a pixmap font from a TrueType file.
	font	= new FTPolygonFont("/Developer/CinderProjects/FTGL_Font_Test/resources/HelveticaBold.ttf");
	
	// If something went wrong, bail out.
	if(font->Error())
	{
		printf("ERROR loading font");
	}
	else
	{
		font->FaceSize(1, 72);
		font->CharMap(ft_encoding_unicode); //unicode
	}
	messageLayout.SetLineLength(9.0f);
	messageLayout.SetFont( font );
	messageLayout.SetLineSpacing(1.0f);
	messageLayout.SetAlignment(FTGL::ALIGN_LEFT);
	
	nameLayout.SetLineLength(9.0f);
	nameLayout.SetFont( font );
	nameLayout.SetLineSpacing(1.0f);
	nameLayout.SetAlignment(FTGL::ALIGN_RIGHT);
	
	messageLayoutPos = TWEET_INIT_POS;
}

void AnswerWall::setup( CameraPersp *mCam, gl::GlslProg *mShader )
{
	mWallTex	= gl::Texture( loadImage( loadResource( WALL_BASE_TEX ) ) );
	
	mBlackTex	= gl::Texture( loadImage( loadResource( BLACK_TEX ) ) );
	mBlueTex	= gl::Texture( loadImage( loadResource( BLUE_TEX ) ) );
	mLGreenTex	= gl::Texture( loadImage( loadResource( LIGHT_GREEN_TEX ) ) );
	mDGreenTex	= gl::Texture( loadImage( loadResource( DARK_GREEN_TEX ) ) );
	mOrangeTex	= gl::Texture( loadImage( loadResource( ORANGE_TEX ) ) );
	mRedTex		= gl::Texture( loadImage( loadResource( RED_TEX ) ) );
	mYellowTex	= gl::Texture( loadImage( loadResource( YELLOW_TEX ) ) );
	mWhiteTex	= gl::Texture( loadImage( loadResource( WHITE_TEX ) ) );
	
	//init animated wall texture
	gl::Fbo::Format format;
	//	format.setSamples( 4 ); // enable 4x antialiasing
	mAnimatedWallTex = gl::Fbo( mWallTex.getWidth(), mWallTex.getHeight(), format );
	mAnimatedTextTex = gl::Fbo( mWallTex.getWidth(), mWallTex.getHeight(), format );
	
	mCamera			= mCam;
	mBasicShader	= mShader;
	mCamInitPos		= mCamera->getEyePoint();
	
	mColRect = gl::DisplayList( GL_COMPILE );
	mColRect.newList();
	
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0, 0, 1);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(4.0f, 0.0f, 0.2f); //top
	
	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f);
	glTexCoord2f(0.5f, 1.075f);
	glVertex3f(4.3f, 1.15f, 0.3f); //top 
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 2.3f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(4.0f, 2.3f, 0.3f); //top
	
	glEnd();
	
	mColRect.endList();
	//mColRect.setMaterial( rectMat );
}

void AnswerWall::renderWallFBO()
{
	CameraPersp	mCam = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
	
	// bind the framebuffer - now everything we draw will go there
	mAnimatedWallTex.bindFramebuffer();
	
	gl::setMatricesWindow( mAnimatedWallTex.getSize(), false ); // false to prevent vertical flipping
	// setup the viewport to match the dimensions of the FBO
	gl::setViewport( mAnimatedWallTex.getBounds() );
	
	// setup our camera to render the wallTexture scene
	mCam.setPerspective( 60, mAnimatedWallTex.getTexture(0).getAspectRatio(), 0.1f, 100.0f );
	mCam.lookAt( Vec3f( 0.0f, 0.0f, mCamInitPos.z ), Vec3f::zero() );
	gl::setMatrices( mCam );
	
	// clear out the FBO ( set as red so any background showing is obvious )
	gl::clear( Color( 1.0f, 0.0f, 0.0f ) );
	
	mBasicShader->bind();
	Utilities_ACS::setTextureWithinShader( "texture", mWallTex, *mBasicShader, 13 );
	mBasicShader->uniform("alpha", 1.0f );
	
	// render wall texture
	gl::draw( mWallTex, Rectf( -3.85f, -3.82f, 3.85f, 3.82f ) );
	
	mBasicShader->unbind();
	mAnimatedWallTex.unbindFramebuffer();
}

void AnswerWall::renderTextBoxFBO()
{
	CameraPersp	mCam = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
	
	// bind the framebuffer - now everything we draw will go there
	mAnimatedTextTex.bindFramebuffer();
	
	gl::setMatricesWindow( mAnimatedWallTex.getSize(), false ); // false to prevent vertical flipping
	// setup the viewport to match the dimensions of the FBO
	gl::setViewport( mAnimatedTextTex.getBounds() );
	
	// setup our camera to render the wallTexture scene
	mCam.setPerspective( 60, mAnimatedTextTex.getTexture(0).getAspectRatio(), 0.1f, 100.0f );
	mCam.lookAt( Vec3f( 0.0f, 0.0f, mCamInitPos.z ), Vec3f::zero() );
	gl::setMatrices( mCam );
	
	// clear out the FBO ( set as red so any background showing is obvious )
	gl::clear( Color( 0.57f, 0.65f, 0.53f ) );
	
	mBasicShader->bind();
	mBasicShader->uniform("alpha", 1.0f );
	
	Utilities_ACS::setTextureWithinShader( "texture", mWhiteTex, *mBasicShader, 13 );
	
	//render text boxes ( if any )
	glPushMatrix();
	glTranslatef(-3.3f, 3.5f, 0.01f);
	glScalef(1.0f, 0.44, 1.0f); //HACK: weird aspect ration was giving problems .... so using a square ratio and scaling down
	//	glTranslatef(messageLayoutPos.x, messageLayoutPos.y, messageLayoutPos.z);
	glScalef(0.75f, 0.75f, 0.75f);
	messageLayout.Render(std::string("\"" + testStr + "\"").c_str(), -1,  FTPoint(), FTGL::RENDER_ALL);
	
	Utilities_ACS::setTextureWithinShader( "texture", mLGreenTex, *mBasicShader, 13 );
	glTranslatef(1.4f, -getTextBoxHeight( &messageLayout, testStr) - 0.9f, 0.0);
	glScalef(0.8f, 0.8, 0.8f);
	nameLayout.Render(testName.c_str(), -1,  FTPoint(), FTGL::RENDER_ALL);
	
	glPopMatrix();
	
	mBasicShader->unbind();
	
	mAnimatedTextTex.unbindFramebuffer();
}

void AnswerWall::update()
{	
	Vec3f currCamPos = mCamera->getEyePoint();
	
	renderWallFBO();
	//renderTextBoxFBO();
	
	//reset camera and viewport back
	gl::setViewport( getWindowBounds() );
	mCamera->lookAt( currCamPos, Vec3f::zero() );
	mCamera->setPerspective( 45.0f, getWindowAspectRatio(), 0.1f, 100.0f );
	
	//tweetDataRender tempTweetStruct;
	bool anyAnimating = false;
	for (int j = 0; j < currTweets.size(); j++) 
	{
		if ( currTweets[j].animating )
		{
			anyAnimating = true; 
			break;
		}
	}
	
	//now update "tweet positions"
	for (int i = (currTweets.size()-1); i >= 0; i--)
	{
		//tweetDataRender tempTweetStruct = currTweets[i];
		
		if ((!anyAnimating) && (i==0))
		{
			currTweets[0].animating = true;
		}
		
		if ( currTweets[i].animating )
		{
			currTweets[i].posTweet.y += currTweets[i].scrollSpeed;
			//printf("%i \n", i);
			
			//delete any finished animations ( moved past top boundaries )
			if ( currTweets[i].posTweet.y > 3.5f ) 
			{
				//
				currTweets.erase( currTweets.begin() + i );
			}
		}
		else
		{
			float heightDiff = (currTweets[i-1].posTweet.y - currTweets[i].posTweet.y);
			
			//if(i == 1)printf("%f %f \n", heightDiff, (currTweets[i-1].heightMessage));
			
			if (heightDiff > (currTweets[i-1].heightMessage) ) 
			{
				currTweets[i].animating = true;
				//printf("hjdsgfhjaksdgfhjkdsga \n");
			}
		}
	}
	
	if ( mCurrMessageVis )
	{
		if ( mCurrMessageAnimOut )
		{
			mCurrMessageScale += 0.001f;
			
			if ( mCurrMessageScale > 0.8f )
			{
				mCurrMessageScale = 0.8f;
			}
		}
		else 
		{
			mCurrMessageScale -= 0.01f;
			
			if ( mCurrMessageScale < 0.0f )
			{
				mCurrMessageScale = 0.0f;
				mCurrMessageVis = false;
			}
		}
		
	}
}

void AnswerWall::drawText()
{	
	//test works
	glPushMatrix();
	glTranslatef(1.15f, -3.0f, 0.0f);
	glRotatef(90.0f, 0, 0, 1);
	//	glTranslatef(-1.05f, 0.0f, 0.0f);
	mColRect.draw();
	glPopMatrix();	
}

void AnswerWall::activateShadowStr(  std::string str  )
{
	mCurrMessageStr = str;
	mCurrMessageVis = true;
	mCurrMessageAnimOut = true;
	mCurrMessageScale = 0.0f;
}
	
void AnswerWall::deactivateShadowStr()
{
	mCurrMessageAnimOut = false;
}	

void AnswerWall::drawShadowStr()
{	
	if ( mCurrMessageVis )
	{
		//test works
		glPushMatrix();
		glScalef(mCurrMessageScale, mCurrMessageScale, mCurrMessageScale);
			messageLayout.Render(mCurrMessageStr.c_str(), -1,  FTPoint(), FTGL::RENDER_ALL);
		glPopMatrix();	
	}
}

void AnswerWall::drawTweets( gl::GlslProg*	mTexAndShadowShader )
{
	//render tweets
	//glDisable(GL_DEPTH_TEST);
	
	for( int i = 0; i < currTweets.size(); i++ )
	{
		if ( currTweets[i].animating )
		{
			Utilities_ACS::setTextureWithinShader( "texture", currTweets[i].questionTex, *mTexAndShadowShader, 3 );
			
			glPushMatrix();
			glTranslatef(currTweets[i].posTweet.x, currTweets[i].posTweet.y, currTweets[i].posTweet.z);
			glRotatef(90.0f, 0, 0, 1);
			mColRect.draw();
			glPopMatrix();
		}
	}

	//DEBUG
//	if (!currTweets.empty())
//	{
//		Utilities_ACS::setTextureWithinShader( "texture", currTweets[1].questionTex, *mTexAndShadowShader, 3 );
//		
//		glPushMatrix();
//		glTranslatef(1.15f, -3.0f, 0.0f);
//		glRotatef(90.0f, 0, 0, 1);
//		mColRect.draw();
//		glPopMatrix();
//	}
	
	//glEnable(GL_DEPTH_TEST);
}

float AnswerWall::getTextBoxHeight( FTLayout *layout, std::string text)
{
	FTBBox textBox = layout->BBox(text.c_str(), -1,  FTPoint());
	FTPoint lowPoint = textBox.Lower();
	return Vec3f( Vec3f(lowPoint.X(), lowPoint.Y(), lowPoint.Z()) ).length();
}

void AnswerWall::passNewAnswers( std::vector<tweet> newTweets )
{
	for ( int i = 0; i < newTweets.size(); i++ )
	{
		tweetDataRender newTweeter;
		gl::Texture		questTex;
		
		newTweeter.twitterData		=	newTweets[i];
		newTweeter.heightMessage	=	getTextBoxHeight( &messageLayout, newTweeter.twitterData.message);
		createQuestionTex( newTweeter.twitterData, &questTex, newTweeter.heightMessage );
		newTweeter.questionTex		=	questTex;
		newTweeter.posTweet			=	Vec3f(TWEET_INIT_POS.x, TWEET_INIT_POS.y, TWEET_INIT_POS.z);
		newTweeter.scrollSpeed		=	0.008f + Rand::randFloat(-0.0008,0.0008);
		newTweeter.animating		=	false;
		
		//do in main function now when gettiong tweets ....
		//if ( i == 0 ) lastTwitterID = newTweeter.twitterData.id; //the first in list is the "newest" so this id is required
		
		currTweets.push_back( newTweeter );
	}
	
	printf("FINISHED PASSING NEWTWEETS!!!!!!!!!!!!!!!!! \n");
}

void AnswerWall::createQuestionTex( tweet twitterStuff, gl::Texture* questionTex, float messageHeight )
{
	gl::Fbo::Format format;
	//	format.setSamples( 4 ); // enable 4x antialiasing
	gl::Fbo mTempFBO = gl::Fbo( mWallTex.getWidth(), mWallTex.getHeight(), format );
	
	// bind the framebuffer - now everything we draw will go there
	mTempFBO.bindFramebuffer();
	
	// setup the viewport to match the dimensions of the FBO
	gl::setViewport( mTempFBO.getBounds() );
	
	CameraPersp	mCam = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
	
	// setup our camera to render the wallTexture scene
	mCam.setPerspective( 60, mTempFBO.getTexture(0).getAspectRatio(), 0.1f, 100.0f );
	mCam.lookAt( Vec3f( 0.0f, 0.0f, mCamInitPos.z ), Vec3f::zero() );
	gl::setMatrices( mCam );
	
	// clear out the FBO ( set as red so any background showing is obvious )
	gl::clear( Color( 0.57f, 0.65f, 0.53f ) );
	
	mBasicShader->bind();
	mBasicShader->uniform("alpha", 1.0f );
	
	Utilities_ACS::setTextureWithinShader( "texture", mWhiteTex, *mBasicShader, 13 );
	
	//render text boxes ( if any )
	glPushMatrix();
	glTranslatef(-3.3f, 3.5f, 0.01f);
	glScalef(1.0f, 0.48, 1.0f); //HACK: weird aspect ration was giving problems .... so using a square ratio and scaling down
	//	glTranslatef(messageLayoutPos.x, messageLayoutPos.y, messageLayoutPos.z);
	glScalef(0.75f, 0.75f, 0.75f);
	messageLayout.Render(std::string("\"" + twitterStuff.message + "\"").c_str(), -1,  FTPoint(), FTGL::RENDER_ALL);
	
	Utilities_ACS::setTextureWithinShader( "texture", mLGreenTex, *mBasicShader, 13 );
	glTranslatef(1.4f, -messageHeight - 0.9f, 0.0);
	glScalef(0.8f, 0.8, 0.8f);
	nameLayout.Render(std::string("~" + twitterStuff.name).c_str(), -1,  FTPoint(), FTGL::RENDER_ALL);
	
	glPopMatrix();
	
	mBasicShader->unbind();
	
	mTempFBO.unbindFramebuffer();
	
	*questionTex = mTempFBO.getTexture(0);
}

FTSimpleLayout* AnswerWall::createNewTwitterContainer( bool messContainer )
{
	FTSimpleLayout* newLayout = new FTSimpleLayout;
	
	if ( messContainer )
	{
		//message container
		newLayout->SetLineLength(8.0f);
		newLayout->SetFont( font );
		newLayout->SetLineSpacing(0.8f);
		newLayout->SetAlignment(FTGL::ALIGN_LEFT);
	}
	else 
	{
		//name container
		newLayout->SetLineLength(8.0f);
		newLayout->SetFont( font );
		newLayout->SetLineSpacing(0.8f);
		newLayout->SetAlignment(FTGL::ALIGN_RIGHT);
	}

	return newLayout;
}

int AnswerWall::getArrIndex( int xCoord, int yCoord )
{
	return (xCoord + yCoord * NUM_BLOCKS_WALL_WIDE);
}

gl::Fbo* AnswerWall::getWallFBO()
{
	return &mAnimatedWallTex;
}

gl::Fbo* AnswerWall::getTextBoxFBO()
{
	return &mAnimatedTextTex;
}
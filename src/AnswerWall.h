/*
 *  Answers.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-31.
 *
 */

#pragma once

#include "Resources.h"
#include "DefinesAndConstants.h"
#include "CustomStructs.h"

#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/ImageIo.h"
#include "cinder/Vector.h"
#include "cinder/gl/DisplayList.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"

#include <FTGL/ftgl.h>

#include "Utilities_ACS.h"

using namespace ci;
using namespace ci::app;

class AnswerWall
{
public:
	AnswerWall();
	~AnswerWall();
	
	void init3DFont();
	void setup( CameraPersp *mCam, gl::GlslProg *mShader );
	void update();
	void drawText();
	void drawTweets( gl::GlslProg*	mTexAndShadowShader );
	void passNewAnswers( std::vector<tweet> newTweets );
	
	void activateShadowStr(  std::string str  );
	void deactivateShadowStr();
	
	void drawShadowStr();
	
	gl::Fbo* getWallFBO();
	gl::Fbo* getTextBoxFBO();
	int getArrIndex( int xCoord, int yCoord );
	std::vector<tweetDataRender>		currTweets;
	std::vector<gl::Texture>			tweetTextures;
	
	std::string		lastTwitterID;
	
	float		mCurrMessageScale;
	bool		mCurrMessageVis;
	bool		mCurrMessageAnimOut;
	std::string	mCurrMessageStr;
	
private:
	void renderWallFBO();
	void renderTextBoxFBO();
	
	float getTextBoxHeight( FTLayout *layout, std::string text);
	FTSimpleLayout* createNewTwitterContainer( bool messContainer );
	void createQuestionTex( tweet twitterStuff, gl::Texture* questionTex, float messageHeight );
	
	bool				mNextCanGo;
	
	gl::Texture			mBlackTex;
	gl::Texture			mBlueTex;
	gl::Texture			mLGreenTex;
	gl::Texture			mDGreenTex;
	gl::Texture			mOrangeTex;
	gl::Texture			mRedTex;
	gl::Texture			mYellowTex;
	gl::Texture			mWhiteTex;
	
	gl::Texture			mWallTex;
	
	gl::DisplayList		mColRect;
	
	FTFont*				font;
	FTSimpleLayout		messageLayout;
	FTSimpleLayout		nameLayout;
	Vec3f				messageLayoutPos;
	gl::Fbo				mAnimatedWallTex;
	gl::Fbo				mAnimatedTextTex;
	float				mTextBoxWidth;
	float				mTextBoxHeight;
	Vec3f				mCamInitPos;
	CameraPersp			*mCamera; 
	gl::GlslProg		*mBasicShader;
	
	std::string			testStr;
	std::string			testName;
};
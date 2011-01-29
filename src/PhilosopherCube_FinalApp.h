/*
 *  PhilosopherCube_82App.h
 *  PhilosopherCube
 *
 *  Created by anthonyScavarelli on 10-10-13.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#include <iostream>

#include "Resources.h"
#include "DefinesAndConstants.h"

#include "cinder/Cinder.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/DisplayList.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/Text.h"
#include "cinder/Thread.h"

#include "CustomStructs.h"
#include "Utilities_ACS.h"
#include "AlienCube.h"
#include "ParticleController.h"
#include "TwitterXML.h"
#include "Question.h"
#include "MovableBlocks.h"
#include "AnswerWall.h"

using namespace ci;
using namespace ci::app;

class PhilosopherCube_FinalApp : public AppBasic 
{
	public:
	
		void prepareSettings( Settings *settings );
		void setup();
		void update();
		void draw();
		void keyDown( app::KeyEvent event );
	
		void drawAnswers( bool doTexture );			//draw "shadow" 4 word answers
		void drawAnswerStrings( bool doTexture );	//
		void drawFrameRate( bool doTexture );		//draw the framerate box and texture
		void drawObjects(  bool doTexture );		//draw "objects" = alien cube
		void drawQuestion( bool doTexture );		//draw question 3D extruded text
		void drawTestObjects();						//draws vector direction of light
		void drawWall( bool doTexture );
	
		void createBackupTwitters();
		
		void createShadowMap();
		void initFBOs();
		void initShaders();
	
		void createStringAnswers();
		int getRandomTweetIndex();
	
		void triggerStage( int state );
		void setTimer( int numSeconds );
		bool isTimerUp();
	
		int					mAutoAddTweetTime;
		int					currentTimeBetween;
	
		std::string			mWordAnswers;
	
		bool				mWander;
		bool				mTwitter;
		bool				mAnswer;
		bool				mFirstTime;
		bool				answerReady;			//attempt to determine when there is an answer ready
	
		int					mNumSecondsTimer;
		int					mNumSecondsStart;
		int					Current_State;
	
		//textures used on objects
		gl::Texture			mCubeTexture;
		gl::Texture			mPartTex;
		gl::Texture			mQuestionTex;
		gl::Texture			mWhiteTex;
	
		CameraPersp			*mCamera;				//the main camera
		gl::Light			*mLight;				//one light ( mainly used for shadow mapping calculations )
	
		//FBO that holds depth information for shadow mapping
		gl::Fbo				mDepthFbo;
	
		gl::GlslProg		mShadowMapShader;
		gl::GlslProg		mTexAndShadowShader;
		gl::GlslProg		mBasicShader;
	
		//TESTING STUFF
		std::string			mTestString;
		Vec3f				mObjMove;
		float				mTestScale;
		bool				mGotTwitterData;
		std::string			twitterUpdateSuccess;
		Vec3f mWallMove;
		//TESTING STUFF
	
		std::string			mStrAnswerFormatted;
		std::string			mStrAnswer;
	
		vector<std::string> mNewWords;				//holds any new words that are returned by twitter call to be added to allWords
		vector<std::string> mAllWords;				//holds all words of current question that can be used for final answer
	
		Font				helveticaBoldFont;		//font used in this project for all text
	
		//TEXT
		gl::DisplayList mFrameRect;					//frameRate object
		gl::Texture mFrameRTex;						//framerate texture
	
		//Custom Object
		AlienCube			mAlienCube;
		AnswerWall			mAnswerWallTex;
		MovableBlocks		mWallBlocks;
		ParticleController	mCubeHalo;
		Question			question;
	
		//twitter thread stuff
		mutex				twitterMutex;			//seems similar to semaphores I have used before ... TODO:Research some more
		vector<tweet>		twitterXMLreturn;
		vector<tweet>		fakeXMLreturn;
		mutex				twitterMentionsLock;
		thread				twitterThread;
	
		//3D wall ( used in conjunctoin with ObjLoader
		TriMesh			mMesh;	
		gl::VboMesh		mWallVBO;
};
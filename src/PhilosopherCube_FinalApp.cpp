#include "PhilosopherCube_FinalApp.h"

void PhilosopherCube_FinalApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1024, 768 );		//not an HD projector ...
	settings->setFrameRate( 60.0f );			//the more the merrier!
	settings->setResizable( false );			//this isn't going to be resizable
	
	//make sure secondary screen isn't blacked out as well when in fullscreen mode ( do wish it could accept keyboard focus though :(
	settings->enableSecondaryDisplayBlanking( false );		
}

void PhilosopherCube_FinalApp::setup()
{
	//setFullScreen(true);
	//hideCursor();								
	
	//twitterThread =  thread(&TwitterXML::newStatusUpdate, "Thanks for all the answers humans, perhaps I'll visit again sometime ...", &twitterUpdateSuccess, &twitterMentionsLock);
	
	mAutoAddTweetTime = getElapsedSeconds();
	mStrAnswerFormatted = "";
	mStrAnswer = "";
	
	mWander = true;
	mTwitter = false;
	mAnswer = false;
	mGotTwitterData = false;
	mFirstTime = true;
	
	//create utility search vectors
	TwitterXML::createSearchStringVectors();
	createBackupTwitters();
	
	mWallMove = Vec3f(-0.8f, -1.2f, 0.0f);
	mTestScale = 1.0f;
		
	glPolygonOffset( 1.0f, 1.0f );
	glEnable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_POLYGON_SMOOTH);
	
	//3D wall
	ObjLoader loader( loadResource( WALL_OBJ )->createStream() );
	loader.load( &mMesh );
	mWallVBO = gl::VboMesh( mMesh );
	
	mObjMove = Vec3f::zero();
	
	//create and load texture of alien cube	
	mCubeTexture = gl::Texture( loadImage( loadResource( ALIEN_CUBE_BASE_TEX ) ) );
	
	mWhiteTex	= gl::Texture( loadImage( loadResource( WHITE_TEX ) ) );
	
	//load textures of particle
	mPartTex	= gl::Texture( loadImage( loadResource( PARTICLE_TEX ) ) );
	
	//load textures of questions
	mQuestionTex	= gl::Texture( loadImage( loadResource( QUESTION_TEX ) ) );
	mFrameRTex = mQuestionTex; //just make it equal to something initially so it doesn't throw an error later
	
	//create camera
	mCamera = new CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
	mCamera->lookAt( CAM_POSITION_INIT, Vec3f::zero() );
	mCamera->setPerspective( 45.0f, getWindowAspectRatio(), 0.1f, 100.0f );
	
	//create light
	mLight = new gl::Light( gl::Light::POINT, 0 );
	mLight->lookAt( LIGHT_POSITION_INIT, Vec3f( 0, 0, 0 ) );
	mLight->setAmbient( Color( 0.1f, 0.1f, 0.1f ) );
	mLight->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight->setSpecular( Color( 0.1f, 0.1f, 0.1f ) );
	mLight->setShadowParams( 80.0f, 0.05f, 10.0f );
	mLight->update( *mCamera );
	mLight->enable();
	 
	//create materials
	gl::Material backboardMaterial;
	backboardMaterial.setAmbient( STREET_LIGHT );
	backboardMaterial.setDiffuse( STREET_LIGHT );	
	backboardMaterial.setShininess( 1.0f );
	
	initFBOs();
	
	mFrameRect = gl::DisplayList( GL_COMPILE );
	mFrameRect.newList();
	glPushMatrix();
	glTranslatef(4.0, 3.0, -2.0);
	glRotatef(90, 1, 0, 0);
	gl::drawCube( Vec3f( 0.0f, 0.0f, 0.0f ), Vec3f( 1.0f, 0.01f, 0.5f ) );
	glPopMatrix();
	mFrameRect.endList();
	mFrameRect.setMaterial( backboardMaterial );
	
	//-1.16f, 1.26f, -0.5f
	mAlienCube.setup( 1.15f, BASE_CUBE_POS, 1, &mCubeTexture);
	
	//set up shader
	initShaders();
	
	//font setup
	helveticaBoldFont = Font( loadResource( RES_CUSTOM_FONT ), 72 );
	
	mCubeHalo = ParticleController( mAlienCube.pos_cube , 0.001f, 1100, 0.7f, false, Vec3f::zero(), NUM_HALO_PARTICLES );
	mWallBlocks.setup();

	mWallMove = Vec3f(-1.10f, 1.68f, -1.60f);
	mTestScale = 2.29f;
	
	mAnswerWallTex.setup( mCamera, &mBasicShader );
	
	//now to get initial Twitter Update Time
	mAnswerWallTex.lastTwitterID = "29082710838";
	
	//KICK OFF STATE
	Current_State = WANDER_PHASE;
	
	mNumSecondsTimer = -1;
	mNumSecondsStart = -1;
	twitterUpdateSuccess = "";
	
	answerReady = false;
	
	currentTimeBetween = getElapsedSeconds();
	
	mWordAnswers = "";
}

void PhilosopherCube_FinalApp::update()
{	
	if ( mWander )
	{
		if ( (getElapsedSeconds() - currentTimeBetween) > WANDER_PHASE_LENGTH  )
		{
//			triggerStage(ASKING_PHASE);
			mWander = false;
			mTwitter = true;
			mAnswer = false;
			currentTimeBetween = getElapsedSeconds();
		}
	}
	else if ( mTwitter ) 
	{
		if ( (getElapsedSeconds() - currentTimeBetween) > TWITTER_PHASE_LENGTH  )
		{
			if ( mAnswerWallTex.currTweets.empty() )
			{
//				triggerStage(ANSWER_PHASE);
				mWander = false;
				mTwitter = false;
				mAnswer = true;
				currentTimeBetween = getElapsedSeconds();
			}
		}
		
	}
	else if ( mAnswer )
	{
		if ( (getElapsedSeconds() - currentTimeBetween) > ANSWER_PHASE_LENGTH  )
		{
//			triggerStage(WANDER_PHASE);
			mWander = true;
			mTwitter = false;
			mAnswer = false;
			currentTimeBetween = getElapsedSeconds();
		}
	}
	
	//then update time
	if ( mNumSecondsStart > -1 )
	{
		if ( ( getElapsedSeconds() - mNumSecondsStart ) > mNumSecondsTimer )
		{
			mNumSecondsStart = -1;
			mNumSecondsTimer = -1;
		}
	}
	
	//DEBUG
	//update frameRate texture
	TextLayout frameR;
	frameR.setFont( helveticaBoldFont );
	frameR.setColor( Color( 1, 0, 0.1f ) );
	frameR.clear( ColorA(0,0,0,0) );
	frameR.addLine( boost::lexical_cast<std::string>(getAverageFps()) );
	mFrameRTex = gl::Texture( frameR.render( true, true ) );
	//DEBUG
	
	mAnswerWallTex.update(); //must update first as texture binds later depend on it
	mAlienCube.update();
	mCubeHalo.update( mAlienCube.pos_cube );
	
	question.update();
	mWallBlocks.update();
	
	switch (Current_State) 
	{
		case WANDER_PHASE:
		{
			answerReady = false; //buggy reset
			
			//make sure nothing else is on stage
			//blocks randomly go in an out
			//cube wanders around area
			
			if ( isTimerUp() )
			{
				int randomTime = Rand::randInt(10, 13);
				int randomNum = Rand::randInt(0, NUM_WALL_BLOCKS);
				
				if ( randomNum > (NUM_WALL_BLOCKS - 6))
				{
					mWallBlocks.activateStepAnimation();
				}
				else 
				{
					mWallBlocks.activate( randomNum );
				}

				
				setTimer(randomTime);
			}
		}
			break;
		case ASKING_PHASE:
		{
			//cube goes to side of screen
			//a question comes out of wall
			//twitter status update after question comes up to Twitter
			//question block is activated
			
				if ( !question.visible )
				{
					//question.startNewQuestion();
					//setTimer(6);
				}
				
				if ( isTimerUp() )
				{
					if ( !question.visible )
					{
						question.startNewQuestion();
						setTimer(3);
					}
					else 
					{
						if ( !mWallBlocks.answerStateActivated )
						{
							mWallBlocks.activateAnswerState();
							twitterThread =  thread(&TwitterXML::newStatusUpdate, question.questionString, &twitterUpdateSuccess, &twitterMentionsLock);
						}
						
						//check if new twitter data has been populated ( just to get recent ID for next phase )
						if (twitterMentionsLock.try_lock())
						{
							//want comm with twitter to cease before moving on
							if ( twitterUpdateSuccess.length() > 1 )
							{
								printf("checking for twitter update result\n");
								
								twitterThread =  thread(&TwitterXML::getMentions, mAnswerWallTex.lastTwitterID, &twitterXMLreturn, &mAnswerWallTex.lastTwitterID, &mNewWords, &twitterMentionsLock, true);
								app::console() << "Update: "<< twitterUpdateSuccess << std::endl;	//DEBUG
								twitterUpdateSuccess = "";
								//mGotTwitterData = true;
							}
							
							if ( mFirstTime )
							{
								if ( !twitterXMLreturn.empty() )
								{
									printf("ready to move onto next phase ..\n");
									
									//mGotTwitterData = false;
									twitterXMLreturn.clear();
									mFirstTime = false;
									triggerStage(TWITTER_PHASE);
								}
							}
							else 
							{
								twitterXMLreturn.clear();
								triggerStage(TWITTER_PHASE);
							}
							
							//if ( !twitterXMLreturn.empty() )
							//{
							//twitterXMLreturn.clear();
							//mGotTwitterData = false;
							//triggerStage(TWITTER_PHASE);
							//}
							
							twitterMentionsLock.unlock();
						}
					}
				}
		
		}
			break;
		case TWITTER_PHASE:
		{
			//check for twitters
			//animate twitters
			
			if ( isTimerUp() )
			{
				setTimer(20); //poll every 20 seconds
				twitterThread =  thread(&TwitterXML::getMentions, mAnswerWallTex.lastTwitterID, &twitterXMLreturn, &mAnswerWallTex.lastTwitterID, &mNewWords, &twitterMentionsLock, false);
			}
			
			//check if new twitter data has been populated
			if (twitterMentionsLock.try_lock())
			{
				if ( !twitterXMLreturn.empty() )
				{
					for( int i = 0; i < twitterXMLreturn.size(); i++)
					{
						app::console() << "Id: "<< twitterXMLreturn[i].id << std::endl;
						app::console() << "Message: "<< twitterXMLreturn[i].message << std::endl;
						app::console() << "Location: " << twitterXMLreturn[i].location << std::endl;
						app::console() << "Time: " << twitterXMLreturn[i].time << std::endl;
						app::console() << "Name: " << twitterXMLreturn[i].name << std::endl;
					}
					
					//add "new words" to word list
					for (int i = 0; i < mNewWords.size(); i++)
					{
						mAllWords.push_back( mNewWords[i] );
					}
					
//					for (int j = 0; j < mAllWords.size(); j++)
//					{
//						printf("%s \n", mAllWords[j].c_str());
//					}
					
					//now give to answer wall to process
					mAnswerWallTex.passNewAnswers( twitterXMLreturn );
					twitterXMLreturn.clear();
					//mGotTwitterData = false;
					
					mAutoAddTweetTime = getElapsedSeconds();
				}
				
				if ( (getElapsedSeconds() - mAutoAddTweetTime) > Rand::randInt(30, 40) )
				{
					//randomly seeding answers hard-coded in from preious answers to try to capture interest
					int randInt = 4; //Rand::randInt(10);
					std::vector<tweet> fakePasser;
					std::vector<std::string> fakeWords;
					
					if ( randInt > 3 )
					{
						//select a random answer
						Rand::randSeed( getElapsedFrames() );
						int randIndex = getRandomTweetIndex(); //Rand::randInt( fakeXMLreturn.size() );
						
						//create and add words to list
						boost::ireplace_first(fakeXMLreturn[randIndex].message, "&quot;", "\"");
						boost::ireplace_first(fakeXMLreturn[randIndex].message, "&apos;", "'");
						
						//split up string into words
						boost::split(fakeWords, fakeXMLreturn[randIndex].message, boost::is_any_of("\t "));
						
						//go through and remove any "filler words"
						for (int m = (fakeWords.size() - 1); m >= 0; m--)
						{
							for(int n = (IGNORE_STRING_VECTOR.size() - 1); n >= 0; n--)
							{
								// iterator_range is convertible to bool
								if(boost::ifind_first(fakeWords.at(m), IGNORE_STRING_VECTOR.at(n)))
								{
									fakeWords.erase( fakeWords.begin() + m );
									break;
								}
							}
						}
						
						//add "new words" to word list
						for (int z = 0; z < fakeWords.size(); z++)
						{
							mAllWords.push_back( fakeWords[z] );
						}
						
						//pass answer on ...
						fakePasser.push_back( fakeXMLreturn[randIndex] );
						mAnswerWallTex.passNewAnswers( fakePasser );
						
						mAutoAddTweetTime = getElapsedSeconds(); //OR ELSE LOOP CRAZINEESS
					}
				}

				
				twitterMentionsLock.unlock();
			}
		}
			break;
		case ANSWER_PHASE:
		{
			//Stop accepting twitters
			//Finish Twitter animations
			//Show "answer" text i.e. "Love is ..."
			//show selected 8 words underneath 
			
			if ( isTimerUp() )
			{
				if ( !answerReady )
				{
					if ( !question.isAnimating() )
					{
						//question.setupAnswerIntro();
						question.startAnswer();
						answerReady = true;
						
						std::string questionStr = question.questionString;
						//update to twitter
						/*std::string ansString =*/ createStringAnswers();
						boost::ireplace_first(questionStr, "\n", "");
						twitterThread =  thread(&TwitterXML::newStatusUpdate, (questionStr + mStrAnswer), &twitterUpdateSuccess, &twitterMentionsLock);
						
						mAnswerWallTex.activateShadowStr( mStrAnswerFormatted );
					}
				}
				else 
				{
					int randomTime = Rand::randInt(10, 13);
					int randomNum = Rand::randInt(0, NUM_WALL_BLOCKS);
					
					if ( randomNum > (NUM_WALL_BLOCKS - 6))
					{
						mWallBlocks.activateStepAnimation();
					}
					else 
					{
						mWallBlocks.activate( randomNum );
					}
					
					setTimer(randomTime);
				}
			}
		}
			break;
		default:
			break;
	}
}

void PhilosopherCube_FinalApp::draw()
{	
	gl::setMatrices( *mCamera );
	gl::setViewport( getWindowBounds() );
	gl::clear( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) );
	glClearColor( 0.0025f, 0.0025f, 0.0025f, 1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glLoadIdentity();
	
	glDisable(GL_LIGHTING);
	glColor3f( 1.0f, 1.0f, 0.1f );
	gl::drawFrustum( mLight->getShadowCamera() );
	glColor3f( 1.0f, 1.0f, 1.0f );
	glEnable(GL_LIGHTING);
	
	//primary pass to create shadowmap ( draw objects that will cast shadows in here )
	createShadowMap();
	
	drawFrameRate( false ); //frame rate
	drawWall( true );
	drawQuestion( true );
	drawAnswers(true);
	
	//start drawing glow particles
	gl::enableAlphaBlending();
	gl::enableAdditiveBlending();
	glDisable(GL_DEPTH_TEST);	//need to disable or the translucent parts will occlude polygons behind (looks like masking of some sort )
	
	//get billboard vectors
	Vec3f br, bup;
	mCamera->getBillboardVectors(&br, &bup);
	mCubeHalo.draw(br, bup, mPartTex);
	
	glEnable(GL_DEPTH_TEST);	//put this back or the scene will be messed up )
	glDisable(GL_BLEND);
	gl::disableAlphaBlending();
	//end drawing glow particles
	
	drawObjects( true ); //draw after particles as not using depth buffering here for glow particles
	mWallBlocks.drawMask( &mBasicShader );
	
	glFlush();
}

void PhilosopherCube_FinalApp::setTimer( int numSeconds )
{
	mNumSecondsStart = getElapsedSeconds();
	mNumSecondsTimer = numSeconds;
}

bool PhilosopherCube_FinalApp::isTimerUp()
{
	return ( mNumSecondsStart == -1 );
}

void PhilosopherCube_FinalApp::createStringAnswers()
{
	std::string	words[4];
	
	if ( !mAllWords.empty() )
	{
		//choose 4 words
		
		int randInt;
		
		Rand::randSeed( getElapsedFrames() );
		randInt= Rand::randInt( 0,  (mAllWords.size() - 1));
		words[0] = mAllWords[randInt];
		
		randInt= Rand::randInt( 0,  (mAllWords.size() - 1));
		words[1] = mAllWords[randInt];
		
		Rand::randSeed( getElapsedSeconds() );
		randInt= Rand::randInt( 0,  (mAllWords.size() - 1));
		words[2] = mAllWords[randInt];
		
		randInt= Rand::randInt( 0,  (mAllWords.size() - 1));
		words[3] = mAllWords[randInt];
		
	}
	else 
	{
		//if for some reason I get nothing these 4 words are generic enough to send out I think ...
		words[0] = "nothing";
		words[1] = "hopeful";
		words[2] = "anonymous";
		words[3] = "information";
	}
	
	//reset
	mStrAnswer = "";
	mStrAnswerFormatted = "";
	
	mStrAnswer = words[0] + " " + words[1] + " " + words[2] + " " + words[3];
	
	mStrAnswerFormatted += "        " + words[0] + "\n";		//1
	mStrAnswerFormatted += "       " + words[1] + "\n";			//2
	mStrAnswerFormatted += "    " + words[2] + "\n";			//3
	mStrAnswerFormatted += "     " + words[3] + "\n";			//4
}

void PhilosopherCube_FinalApp::triggerStage( int state )
{
	Current_State = state;
	
	switch (Current_State) 
	{
		case WANDER_PHASE:
		{
			mAnswerWallTex.deactivateShadowStr();
			mWordAnswers = "";
			mAllWords.clear(); //clear for next iteration
			mAlienCube.goToPosition( MID_CUBE_POS );
			question.stopAnswer();
			
			//make sure nothing else is on stage
			//blocks randomly go in an out
			//cube wanders around area
			
			printf("ENTERING WANDER_PHASE \n");
		}
			break;
		case ASKING_PHASE:
		{
			//cube goes to side of screen
			//a question comes out of wall
			//twitter status update after question comes up to Twitter
			//question block is activated
			
			setTimer(6);
			mAlienCube.goToPosition( SIDE_CUBE_POS );
			printf("ENTERING ASKING_PHASE \n");
		}
			break;
		case TWITTER_PHASE:
		{
			//check for twitters
			//animate twitters
			mAutoAddTweetTime = getElapsedSeconds();
			printf("ENTERING TWITTER_PHASE \n");
		}
			break;
		case ANSWER_PHASE:
		{
			//Stop accepting twitters
			//Finish Twitter animations
			//Show "answer" text i.e. "Love is ..."
			//show selected 8 words underneath 
			setTimer(4);
			//mWordAnswers = createStringAnswers();
			mWallBlocks.deactivateAnswerState();
			question.stopQuestion();
			mAlienCube.goToPosition( SIDE_CUBE_POS );
			printf("ENTERING ANSWER_PHASE \n");
		}
			break;
		default:
			break;
	}
	
}

void PhilosopherCube_FinalApp::keyDown( app::KeyEvent event ) 
{
	//printf("keycode: %i \n", event.getCode());
	Vec3f newPosVec;
	int key = event.getCode();
	
	switch ( key )
	{
		case 49:
		{
			//1
			//WANDER_PHASE
			triggerStage( WANDER_PHASE );
		}
			break;
		case 50:
		{
			//2
			//ASKING_PHASE
			triggerStage( ASKING_PHASE );
		}
			break;	
		case 51:
		{
			//3
			//TWITTER_PHASE
			triggerStage( TWITTER_PHASE );
		}
			break;
		case 52:
		{
			//4
			//ANSWER_PHASE
			triggerStage( ANSWER_PHASE );
		}
			break;
		case 53:
		{
			//5
			//AUTO_PHASE
			printf("Avg. Framerate: %f \n", getAverageFps());
		}
			break;
		case 54:
		{
			//6
			mWallBlocks.activateStepAnimation();
		}
			break;
		case 55:
		{
			//7
			mWallBlocks.activate(10);
		}
			break;
		case 56:
		{
			//8
			mWallBlocks.activateAnswerState();
		}
			break;
		case 57:
		{
			//9
			mWallBlocks.deactivateAnswerState();
		}
			break;	
			
		case 32:
		//space bar
			{
				if ( question.visible )
				{
					if ( !question.isAnimating() )
					{
						question.stopQuestion();
					}
				}
				else 
				{
					question.startNewQuestion();
				}			
			}	
			break;
		case 13:
			//enter key
			{
				//twitterThread =  thread(&TwitterXML::getMentions, mAnswerWallTex.lastTwitterID, &twitterXMLreturn, &mAnswerWallTex.lastTwitterID, &mIgnoreWords, &twitterMentionsLock, false);

				//twitterThread =  thread(&TwitterXML::getMentions, mAnswerWallTex.lastTwitterID, &twitterXMLreturn, &mAnswerWallTex.lastTwitterID, &twitterMentionsLock);
				//mGotTwitterData = true;
			}
			break;
		case 93:
			//[
			{
//				mTestScale += -0.01f;
				Vec3f eyeVec = mCamera->getEyePoint() + Vec3f(0, 0, 0.1);
				mCamera->lookAt( eyeVec, Vec3f::zero() );
				mLight->update( *mCamera );
			}
			break;
		case 91:
			//]
			{
//				mTestScale += 0.01f;
				Vec3f eyeVec = mCamera->getEyePoint() + Vec3f(0, 0, -0.1);
				mCamera->lookAt( eyeVec, Vec3f::zero() );
				mLight->update( *mCamera );
			}
			break;
		case 61:
			//=
			//mObjMove.z += 0.01;
			//mWallMove.z += 0.01f;
			//mAlienCube.scaleFactor += 0.1;
			break;
		case 269:
			//-
			//mObjMove.z += -0.01;
			//mWallMove.z += -0.01f;
			//mAlienCube.scaleFactor += -0.1;
			break;
		case 8:
			//delete ( reset camera and light )
			{
				mCamera->lookAt( CAM_POSITION_INIT, Vec3f::zero() );
				mLight->update( *mCamera );
				
				mLight->lookAt( LIGHT_POSITION_INIT, Vec3f::zero() );
				mLight->update( *mCamera );
			}
			break;
		case 273:
			// up
			{
				if ( event.isShiftDown() )
				{
					Vec3f eyeVec = mCamera->getEyePoint();
					eyeVec = Quatf( Vec3f(1, 0, 0), -0.03f ) * eyeVec;
					mCamera->lookAt( eyeVec, Vec3f::zero() );
					mLight->update( *mCamera );
				}
				else 
				{
					//mWallMove += Vec3f(0.0f, 0.01f, 0.0f);
				}

			}
			break;
		case 274:
			// down
			{
				if ( event.isShiftDown() )
				{
					Vec3f eyeVec = mCamera->getEyePoint();
					eyeVec = Quatf( Vec3f(1, 0, 0), 0.03f ) * eyeVec;
					mCamera->lookAt( eyeVec, Vec3f::zero() );
					mLight->update( *mCamera );
				}
				else 
				{
					//mWallMove += Vec3f(0.0f, -0.01f, 0.0f);
				}

			}
			break;
		case 276:
			// left
			{
				if ( event.isShiftDown() )
				{
					Vec3f eyeVec = mCamera->getEyePoint();
					eyeVec = Quatf( Vec3f(0, 1, 0), 0.03f ) * eyeVec;
					mCamera->lookAt( eyeVec, Vec3f::zero() );
					mLight->update( *mCamera );
				}
				else 
				{
					//mWallMove += Vec3f(-0.01f, 0.0f, 0.0f);
				}
			}
			break;
		case 275:
			// right
			{
				if ( event.isShiftDown() )
				{
					Vec3f eyeVec = mCamera->getEyePoint();
					eyeVec = Quatf( Vec3f(0, 1, 0), -0.03f ) * eyeVec;
					mCamera->lookAt( eyeVec, Vec3f::zero() );
					mLight->update( *mCamera );
				}
				else 
				{
					//mWallMove += Vec3f(0.01f, 0.0f, 0.0f);
				}

			}
			break;
		case 122:
			//Z
			{
				Vec3f lightPos = mLight->getPosition() + Vec3f(0, 0, 0.1);		
				mLight->lookAt( lightPos, Vec3f::zero() );
				mLight->update( *mCamera );
			}
			break;
		case 120:
			//X
			{
				Vec3f lightPos = mLight->getPosition() + Vec3f(0, 0, -0.1);		
				mLight->lookAt( lightPos, Vec3f::zero() );
				mLight->update( *mCamera );
			}
			break;	
	}
	
	if ( key == 119 ) //W
	{
		if ( event.isShiftDown() )
		{
		}
		else 
		{
			Vec3f lightPos = mLight->getPosition() + Vec3f(0, 0.1, 0);		
			mLight->lookAt( lightPos, Vec3f::zero() );
			mLight->update( *mCamera );
		}

	}
	
	if ( key == 97 ) //A
	{
		if ( event.isShiftDown() )
		{
			
		}
		else 
		{
			Vec3f lightPos = mLight->getPosition() + Vec3f(0.1, 0, 0);		
			mLight->lookAt( lightPos, Vec3f::zero() );
			mLight->update( *mCamera );
		}
	}

	
	if ( key == 115) //S
	{
		if ( event.isShiftDown() )
		{
			
		}
		else 
		{
			Vec3f lightPos = mLight->getPosition() + Vec3f(0, -0.1, 0);		
			mLight->lookAt( lightPos, Vec3f::zero() );
			mLight->update( *mCamera );
		}
	}

	
	if ( key == 100 ) //D
	{
		if ( event.isShiftDown() )
		{
			
		}
		else 
		{
			Vec3f lightPos = mLight->getPosition() + Vec3f(-0.1, 0, 0);		
			mLight->lookAt( lightPos, Vec3f::zero() );
			mLight->update( *mCamera );
		}
	}


	if ( key == 116) //T
	{
		if ( event.isShiftDown() )
		{
			mObjMove.y += 0.01;
		}
		else 
		{
			mAlienCube.pos_cube.y += 0.01;
		}
		
		//mWallMove.y += 0.03;
		//mObjMove.y += 0.03;
	}

	if ( key == 102) //F
	{
		//mWallMove.x += -0.03;
		//mObjMove.x += -0.03;
		if ( event.isShiftDown() )
		{
			mObjMove.x += -0.01;
		}
		else 
		{
			mAlienCube.pos_cube.x += -0.01;
		}
	}

	if ( key == 103) //G
	{
		if ( event.isShiftDown() )
		{
			mObjMove.y += -0.01;
		}
		else 
		{
			mAlienCube.pos_cube.y += -0.01;
		}
		
		//mWallMove.y += -0.03;
		//mObjMove.y += -0.03;
	}

	if ( key == 104) //H
	{
		if ( event.isShiftDown() )
		{
			mObjMove.x += 0.01;
		}
		else 
		{
			mAlienCube.pos_cube.x += 0.01;
		}
		
		//mWallMove.x += 0.03;
	}
	
	//printf("Cube Position: %f %f %f \n", mAlienCube.pos_cube.x, mAlienCube.pos_cube.y, mAlienCube.pos_cube.z);
	//printf("Wall Scale: %f \n", mTestScale);
	
}

//////////////////////////////////////
/////// Drawing Functions ////////////
//////////////////////////////////////

void PhilosopherCube_FinalApp::drawFrameRate( bool doTexture )
{
	if( doTexture )
	{
		mTexAndShadowShader.bind();
		mTexAndShadowShader.uniform("LightPosition", mLight->getPosition() );
		Utilities_ACS::setTextureWithinShader( "texture", mFrameRTex, mTexAndShadowShader, 3 );
		Utilities_ACS::setTextureWithinShader( "depthTexture", mDepthFbo.getDepthTexture(), mTexAndShadowShader, 0 );
		mTexAndShadowShader.uniform( "shadowTransMatrix", mLight->getShadowTransformationMatrix( *mCamera ) );
	}
	
	mFrameRect.draw();
	
	if( doTexture )
	{
		mTexAndShadowShader.unbind();
	}
}

void PhilosopherCube_FinalApp::drawObjects(  bool doTexture )
{
	if( doTexture )
	{
		mTexAndShadowShader.bind();
		mTexAndShadowShader.uniform("LightPosition", mLight->getPosition() );
		Utilities_ACS::setTextureWithinShader( "texture", mCubeTexture, mTexAndShadowShader, 3 );
		Utilities_ACS::setTextureWithinShader( "depthTexture", mDepthFbo.getDepthTexture(), mTexAndShadowShader, 0 );
		mTexAndShadowShader.uniform( "shadowTransMatrix", mLight->getShadowTransformationMatrix( *mCamera ) );
	}
	
	mAlienCube.draw();
	
	if( doTexture )
	{
		mTexAndShadowShader.unbind();
	}
}

void PhilosopherCube_FinalApp::drawAnswers( bool doTexture )
{
	if( doTexture )
	{
		mTexAndShadowShader.bind();
		mTexAndShadowShader.uniform("LightPosition", mLight->getPosition() );
		//Utilities_ACS::setTextureWithinShader( "texture", mAnswerWallTex.getTextBoxFBO()->getTexture(), mTexAndShadowShader, 3 );
		Utilities_ACS::setTextureWithinShader( "depthTexture", mDepthFbo.getDepthTexture(), mTexAndShadowShader, 0 );
		mTexAndShadowShader.uniform( "shadowTransMatrix", mLight->getShadowTransformationMatrix( *mCamera ) );		
	}	
	
	glPushMatrix();
	glTranslatef(1.4f, 0.175f, -1.55f);
	//mAnswerWallTex.drawText();
	
	mAnswerWallTex.drawTweets( &mTexAndShadowShader );
	
	glPopMatrix();
	
	if( doTexture )
	{
		mTexAndShadowShader.unbind();
	}
}

void PhilosopherCube_FinalApp::drawAnswerStrings( bool doTexture )
{
	if ( mAnswerWallTex.mCurrMessageVis )
	{
		if( doTexture )
		{
			mTexAndShadowShader.bind();
			mTexAndShadowShader.uniform("LightPosition", mLight->getPosition() );
			Utilities_ACS::setTextureWithinShader( "texture", mWhiteTex, mTexAndShadowShader, 3 );
			//Utilities_ACS::setTextureWithinShader( "texture", mAnswerWallTex.getTextBoxFBO()->getTexture(), mTexAndShadowShader, 3 );
			Utilities_ACS::setTextureWithinShader( "depthTexture", mDepthFbo.getDepthTexture(), mTexAndShadowShader, 0 );
			mTexAndShadowShader.uniform( "shadowTransMatrix", mLight->getShadowTransformationMatrix( *mCamera ) );		
		}	
		
		glPushMatrix();
		glTranslatef(-3.0f, 1.7f, -0.6f);
		//glScalef(0.9f, 0.9f, 0.9f);
		
		mAnswerWallTex.drawShadowStr();
		
		glPopMatrix();
		
		if( doTexture )
		{
			mTexAndShadowShader.unbind();
		}
	}
}

void PhilosopherCube_FinalApp::drawQuestion( bool doTexture )
{
	if( doTexture )
	{
		mTexAndShadowShader.bind();
		mTexAndShadowShader.uniform("LightPosition", mLight->getPosition() );
		Utilities_ACS::setTextureWithinShader( "texture", mQuestionTex, mTexAndShadowShader, 3 );
		Utilities_ACS::setTextureWithinShader( "depthTexture", mDepthFbo.getDepthTexture(), mTexAndShadowShader, 0 );
		mTexAndShadowShader.uniform( "shadowTransMatrix", mLight->getShadowTransformationMatrix( *mCamera ) );		
	}	
	
	gl::pushMatrices();
	glTranslatef(-2.7f, -2.0f, 0.0f);
	glScalef(0.68f, 0.68f, 0.68f);
	question.draw();
	gl::popMatrices();
	
	if( doTexture )
	{
		mTexAndShadowShader.unbind();
	}
}

void PhilosopherCube_FinalApp::drawTestObjects()
{
	//draw light vector
	mBasicShader.bind();
		Utilities_ACS::setTextureWithinShader( "texture", mWhiteTex, mBasicShader, 3 );
		CameraPersp tempCam = mLight->getShadowCamera();
		Vec3f dirVec = tempCam.getViewDirection();		
		gl::drawVector( mLight->getPosition(), dirVec, 0.5f, 0.05f );
	mBasicShader.unbind();
}

void PhilosopherCube_FinalApp::drawWall( bool doTexture )
{
	if( doTexture )
	{
		mTexAndShadowShader.bind();
		mTexAndShadowShader.uniform("LightPosition", mLight->getPosition() );
		Utilities_ACS::setTextureWithinShader( "depthTexture", mDepthFbo.getDepthTexture(), mTexAndShadowShader, 0 );
		mTexAndShadowShader.uniform( "shadowTransMatrix", mLight->getShadowTransformationMatrix( *mCamera ) );
	}
	
	if( doTexture )Utilities_ACS::setTextureWithinShader( "texture", mAnswerWallTex.getWallFBO()->getTexture(), mTexAndShadowShader, 3 );
	glPushMatrix();
		glTranslatef( mWallMove.x, mWallMove.y, mWallMove.z );
		glScalef(mTestScale, mTestScale, mTestScale);
		glRotatef(90, 1, 0, 0);
		gl::draw( mWallVBO );
	glPopMatrix();
	
	if( doTexture )Utilities_ACS::setTextureWithinShader( "texture", mAnswerWallTex.getWallFBO()->getTexture(), mTexAndShadowShader, 3 );
	glPushMatrix();
		mWallBlocks.draw( &mTexAndShadowShader );
	glPopMatrix();
	
	if( doTexture )
	{
		mTexAndShadowShader.unbind();
	}
}

void PhilosopherCube_FinalApp::initShaders()
{
	mShadowMapShader	= gl::GlslProg( loadResource( RES_SHADOWMAP_VERT ), loadResource( RES_SHADOWMAP_FRAG ) );
	mBasicShader		= gl::GlslProg( loadResource( BASIC_TEXTURE_VERT ), loadResource( BASIC_TEXTURE_FRAG ) );
	mTexAndShadowShader = gl::GlslProg( loadResource( TEXTURE_AND_SHAD_VERT ), loadResource( TEXTURE_AND_SHAD_FRAG ) );
}

void PhilosopherCube_FinalApp::initFBOs()
{		
	//init shadow map
	mDepthFbo = gl::Fbo( SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, false, false, true );
	//mDepthFbo.bindDepthTexture();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );	
}

void PhilosopherCube_FinalApp::createShadowMap()
{
	//create depth map and bind to a FBO ( from light's perspective )
	mDepthFbo.bindFramebuffer();
	
		glEnable( GL_POLYGON_OFFSET_FILL );
		glClear( GL_DEPTH_BUFFER_BIT );
		
		glPushAttrib( GL_VIEWPORT_BIT );
		glViewport( 0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION );
		
		gl::pushMatrices();
		
		mLight->setShadowRenderMatrices();
		
		
		drawWall( false );
		drawQuestion( false );
		drawAnswers( false );
		drawAnswerStrings(false);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	
		drawObjects( false );
		//drawTestObjects();
	
		glDisable(GL_CULL_FACE);
		
		gl::popMatrices();
		
		glPopAttrib();
		
		glDisable( GL_POLYGON_OFFSET_FILL );
	
	mDepthFbo.unbindFramebuffer();
	
	glEnable( GL_TEXTURE_2D );
	
	gl::setMatrices( *mCamera );
	mLight->update( *mCamera );
}

int PhilosopherCube_FinalApp::getRandomTweetIndex()
{
	int startInt = 0;
	int endInt = 2;
	
	if (question.questionString ==  "what is \n love?") 
	{
		startInt = 0;
		endInt = 2;
	} 	
	else if (question.questionString == "what is \n hate?")
	{
		startInt = 3;
		endInt = 5;
	}
	else if (question.questionString == "what is \n art?")
	{
		startInt = 6;
		endInt = 8;
	}
	else if (question.questionString == "what is \n war?")
	{
		startInt = 9;
		endInt = 11;
	}
	else if (question.questionString == "what is \n peace?")
	{
		startInt = 12;
		endInt = 14;
	}
	else if (question.questionString == "what is \n free?")
	{
		startInt = 15;
		endInt = 17;
	}
	else if (question.questionString == "what is \n home?")
	{
		startInt = 18;
		endInt = 20;
	}
	else if (question.questionString == "what is \n friend?")
	{
		startInt = 21;
		endInt = 23;
	}
	else if (question.questionString == "what is \n life?")
	{
		startInt = 24;
		endInt = 26;
	}
	else if (question.questionString == "what is \n justice?")
	{
		startInt = 27;
		endInt = 29;
	}
	else if (question.questionString == "what is \n beautiful?")
	{
		startInt = 30;
		endInt = 32;
	}
	else if (question.questionString == "what is \n family?")
	{
		startInt = 33;
		endInt = 35;
	}
	else if (question.questionString == "what is \n happiness?")
	{
		startInt = 36;
		endInt = 38;
	}
	else if (question.questionString == "what is \n fear?")
	{
		startInt = 39;
		endInt = 41;
	}
	else 
	{
		startInt = 0;
		endInt = 41;
	}
	
	printf("%i %i \n", startInt, endInt);

	Rand::randSeed( getElapsedSeconds() );
	return Rand::randInt( startInt, endInt);
}

void PhilosopherCube_FinalApp::createBackupTwitters()
{
	//"Love \n is ..."	
	tweet newTweet0;
	newTweet0.id		= "";						
	newTweet0.message	= "don't hurt me, don't hurt me, no more";	
	newTweet0.time		= "";	
	newTweet0.location	= "Winnipeg, Manitoba";	
	newTweet0.name		= "AdamWinterpeg";
	fakeXMLreturn.push_back( newTweet0 );
	
	tweet newTweet1;
	newTweet1.id		= "";						
	newTweet1.message	= "love is evol";	
	newTweet1.time		= "";	
	newTweet1.location	= "Ottawa, Ontario";	
	newTweet1.name		= "allanyong";
	fakeXMLreturn.push_back( newTweet1 );
	
	tweet newTweet2;
	newTweet2.id		= "";						
	newTweet2.message	= "Love is doing things for those you care about even if what you really want to do is sleep...because it will make them happy.";	
	newTweet2.time		= "";	
	newTweet2.location	= "Ottawa, Ontario";	
	newTweet2.name		= "DramaDogLover";
	fakeXMLreturn.push_back( newTweet2 );
	
	//"Hate \n is ..."
	tweet newTweet3;
	newTweet3.id		= "";						
	newTweet3.message	= "haters gon to hate";	
	newTweet3.time		= "";	
	newTweet3.location	= "Ottawa, Ontario";	
	newTweet3.name		= "tetsutakara";
	fakeXMLreturn.push_back( newTweet3 );
	
	tweet newTweet4;
	newTweet4.id		= "";						
	newTweet4.message	= "It may occur in a wide variety of contexts, from hatred of inanimate objects or animals, to hatred of oneself or other people ...";	
	newTweet4.time		= "";	
	newTweet4.location	= "Ottawa, Ontario";	
	newTweet4.name		= "Wikipedia.com";
	fakeXMLreturn.push_back( newTweet4 );
	
	tweet newTweet5;
	newTweet5.id		= "";						
	newTweet5.message	= "—Antonyms: 1.  love.";	
	newTweet5.time		= "";	
	newTweet5.location	= "Ottawa, Ontario";	
	newTweet5.name		= "Dicyionary.com";
	fakeXMLreturn.push_back( newTweet5 );
	
	//"Art \n is ..."
	tweet newTweet6;
	newTweet6.id		= "";						
	newTweet6.message	= "Art is expression without the liability";	
	newTweet6.time		= "";	
	newTweet6.location	= "Ottawa, Ontario";	
	newTweet6.name		= "PlumCantaloupe";
	fakeXMLreturn.push_back( newTweet6 );
	
	tweet newTweet7;
	newTweet7.id		= "";						
	newTweet7.message	= "art is a waste of nobody's time";	
	newTweet7.time		= "";	
	newTweet7.location	= "Ottawa, Ontario";	
	newTweet7.name		= "grantlucas";
	fakeXMLreturn.push_back( newTweet7 );
	
	tweet newTweet8;
	newTweet8.id		= "";						
	newTweet8.message	= "Art is a creative expression of what you feel in music, dance, art, painting etc. It is mainly a right brain response.";	
	newTweet8.time		= "";	
	newTweet8.location	= "Ottawa, Ontario";	
	newTweet8.name		= "painterMom";
	fakeXMLreturn.push_back( newTweet8 );
	
	//"War \n is ..."
	tweet newTweet9;
	newTweet9.id		= "";						
	newTweet9.message	= "what is it good for? Absolutely nothing, say it again, huh!";	
	newTweet9.time		= "";	
	newTweet9.location	= "Winnipeg, Manitoba";	
	newTweet9.name		= "AdamWinterpeg";
	fakeXMLreturn.push_back( newTweet9 );
	
	tweet newTweet10;
	newTweet10.id		= "";						
	newTweet10.message	= "organized violent conflict, typified by extreme aggression, societal disruption and adaptation, and high mortality.";	
	newTweet10.time		= "";	
	newTweet10.location	= "Ottawa, Ontario";	
	newTweet10.name		= "Wikipedia.com";
	fakeXMLreturn.push_back( newTweet10 );
	
	tweet newTweet11;
	newTweet11.id		= "";						
	newTweet11.message	= "active hostility or contention; conflict; contest: a war of words.";	
	newTweet11.time		= "";	
	newTweet11.location	= "Ottawa, Ontario";	
	newTweet11.name		= "Dictionary.com";
	fakeXMLreturn.push_back( newTweet11 );
	
	//"Peace \n is ..."
	tweet newTweet12;
	newTweet12.id		= "";						
	newTweet12.message	= "a cup of coffee and looking out at the lake.";	
	newTweet12.time		= "";	
	newTweet12.location	= "Ottawa, Ontario";	
	newTweet12.name		= "DramaDogLover";
	fakeXMLreturn.push_back( newTweet12 );
	
	tweet newTweet13;
	newTweet13.id		= "";						
	newTweet13.message	= "describes a society or a relationship that is operating harmoniously and without violent conflict.";	
	newTweet13.time		= "";	
	newTweet13.location	= "Ottawa, Ontario";	
	newTweet13.name		= "Wikipedia.com";
	fakeXMLreturn.push_back( newTweet13 );
	
	tweet newTweet14;
	newTweet14.id		= "";						
	newTweet14.message	= "the normal, nonwarring condition of a nation, group of nations, or the world";	
	newTweet14.time		= "";	
	newTweet14.location	= "Ottawa, Ontario";	
	newTweet14.name		= "Dictionary.com";
	fakeXMLreturn.push_back( newTweet14 );
	
	//"Free \n is ... "
	tweet newTweet15;
	newTweet15.id		= "";						
	newTweet15.message	= "Free is having a voice and being able to accomplish what you want to accomplish, without judgment.";	
	newTweet15.time		= "";	
	newTweet15.location	= "Ottawa, Ontario";	
	newTweet15.name		= "DramaDogLover";
	fakeXMLreturn.push_back( newTweet15 );
	
	tweet newTweet16;
	newTweet16.id		= "";						
	newTweet16.message	= "Your freedom cannot be at the expense of other people.";	
	newTweet16.time		= "";	
	newTweet16.location	= "Ottawa, Ontario";	
	newTweet16.name		= "painterMom";
	fakeXMLreturn.push_back( newTweet16 );
	
	tweet newTweet17;
	newTweet17.id		= "";						
	newTweet17.message	= "uncombined chemically: free oxygen.";	
	newTweet17.time		= "";	
	newTweet17.location	= "Ottawa, Ontario";	
	newTweet17.name		= "Dictionary.com";
	fakeXMLreturn.push_back( newTweet17 );
	
	//"Home \n is ..."
	tweet newTweet18;
	newTweet18.id		= "";						
	newTweet18.message	= "Home is where you feel safe, happy, loved, free and where the food is best.";	
	newTweet18.time		= "";	
	newTweet18.location	= "Ottawa, Ontario";	
	newTweet18.name		= "DramaDogLover";
	fakeXMLreturn.push_back( newTweet18 );
	
	tweet newTweet19;
	newTweet19.id		= "";						
	newTweet19.message	= "A home is a place of residence or refuge.";	
	newTweet19.time		= "";	
	newTweet19.location	= "Ottawa, Ontario";	
	newTweet19.name		= "Wikipedia.com";
	fakeXMLreturn.push_back( newTweet19 );
	
	tweet newTweet20;
	newTweet20.id		= "";						
	newTweet20.message	= "the place in which one's domestic affections are centered.";	
	newTweet20.time		= "";	
	newTweet20.location	= "Ottawa, Ontario";	
	newTweet20.name		= "Dictionary.com";
	fakeXMLreturn.push_back( newTweet20 );
	
	//"Friend \n is ..."
	tweet newTweet21;
	newTweet21.id		= "";						
	newTweet21.message	= "Friends are people that you can call up anytime and cry, laugh, vent and be silent, without judgment. They and family make life worth living";	
	newTweet21.time		= "";	
	newTweet21.location	= "Ottawa, Ontario";	
	newTweet21.name		= "DramaDogLover";
	fakeXMLreturn.push_back( newTweet21 );
	
	tweet newTweet22;
	newTweet22.id		= "";						
	newTweet22.message	= "A friend is someone who supports you in good times and bad and one you feel comfortable in confiding your feelings with";	
	newTweet22.time		= "";	
	newTweet22.location	= "Ottawa, Ontario";	
	newTweet22.name		= "painterMom";
	fakeXMLreturn.push_back( newTweet22 );
	
	tweet newTweet23;
	newTweet23.id		= "";						
	newTweet23.message	= "a person attached to another by feelings of affection or personal regard.";	
	newTweet23.time		= "";	
	newTweet23.location	= "Ottawa, Ontario";	
	newTweet23.name		= "Dictionary.com";
	fakeXMLreturn.push_back( newTweet23 );
	
	//"Life \n is ..."
	tweet newTweet24;
	newTweet24.id		= "";						
	newTweet24.message	= "yeah, seriously, what IS life. dunno but I likes it.";	
	newTweet24.time		= "";	
	newTweet24.location	= "ou-ta-whee, Ontario";	
	newTweet24.name		= "sackoo";
	fakeXMLreturn.push_back( newTweet24 );
	
	tweet newTweet25;
	newTweet25.id		= "";						
	newTweet25.message	= "Your chance to make the universe a little bit better or a little bit worse - depending on how you use it.";	
	newTweet25.time		= "";	
	newTweet25.location	= "Ottawa, Ontario";	
	newTweet25.name		= "AdamWinterpeg";
	fakeXMLreturn.push_back( newTweet25 );
	
	tweet newTweet26;
	newTweet26.id		= "";						
	newTweet26.message	= "Life is a characteristic that distinguishes objects that have signaling and self-sustaining processes from those that do not";	
	newTweet26.time		= "";	
	newTweet26.location	= "Ottawa, Ontario";	
	newTweet26.name		= "Wikipedia.com";
	fakeXMLreturn.push_back( newTweet26 );
	
	//"Justice \n is ..."
	tweet newTweet27;
	newTweet27.id		= "";						
	newTweet27.message	= "\"what is justice?\" A cosmic magnification mirror upon which you cast your actions.";	
	newTweet27.time		= "";	
	newTweet27.location	= "Winnipeg, Manitoba";	
	newTweet27.name		= "WinnerStar";
	fakeXMLreturn.push_back( newTweet27 );
	
	tweet newTweet28;
	newTweet28.id		= "";						
	newTweet28.message	= "Justice in today's society generally goes to those who can pay for it. It is not found in the law but in the the arguments used to get one's way.";	
	newTweet28.time		= "";	
	newTweet28.location	= "Ottawa, Ontario";	
	newTweet28.name		= "painterMom";
	fakeXMLreturn.push_back( newTweet28 );
	
	tweet newTweet29;
	newTweet29.id		= "";						
	newTweet29.message	= "your mom... <3";	
	newTweet29.time		= "";	
	newTweet29.location	= "Ottawa, Ontario";	
	newTweet29.name		= "JeremyS94";
	fakeXMLreturn.push_back( newTweet29 );
	
	//"Beautiful \n is ..."
	tweet newTweet30;
	newTweet30.id		= "";						
	newTweet30.message	= "Beautiful is someone loving you for all your faults.";	
	newTweet30.time		= "";	
	newTweet30.location	= "Ottawa, Ontario";	
	newTweet30.name		= "molsen_cu";
	fakeXMLreturn.push_back( newTweet30 );
	
	tweet newTweet31;
	newTweet31.id		= "";						
	newTweet31.message	= "I'd say watching someone grow up, definitely.";	
	newTweet31.time		= "";	
	newTweet31.location	= "Ottawa, Ontario";	
	newTweet31.name		= "Yahoo Answers";
	fakeXMLreturn.push_back( newTweet31 );
	
	tweet newTweet32;
	newTweet32.id		= "";						
	newTweet32.message	= "Beautiful is as you see it. It truly is in the eyes of the beholder and varies greatly according to ones set of values and ones taste.";	
	newTweet32.time		= "";	
	newTweet32.location	= "Ottawa, Ontario";	
	newTweet32.name		= "painterMom";
	fakeXMLreturn.push_back( newTweet32 );
	
	//"Family \n is ..."
	tweet newTweet33;
	newTweet33.id		= "";						
	newTweet33.message	= "the people you think about on your darkest days and sunniest of all.";	
	newTweet33.time		= "";	
	newTweet33.location	= "Ottawa, Ontario";	
	newTweet33.name		= "generaltao";
	fakeXMLreturn.push_back( newTweet33 );
	
	tweet newTweet34;
	newTweet34.id		= "";						
	newTweet34.message	= "a basic social unit consisting of parents and their children, considered as a group, whether dwelling together or not";	
	newTweet34.time		= "";	
	newTweet34.location	= "Ottawa, Ontario";	
	newTweet34.name		= "Dictionary.com";
	fakeXMLreturn.push_back( newTweet34 );
	
	tweet newTweet35;
	newTweet35.id		= "";						
	newTweet35.message	= "Genetics has very little to do with whom one considers to be family unless you are very lucky.";	
	newTweet35.time		= "";	
	newTweet35.location	= "Ottawa, Ontario";	
	newTweet35.name		= "painterMom";
	fakeXMLreturn.push_back( newTweet35 );
	
	//"Happiness \n is ..."
	tweet newTweet36;
	newTweet36.id		= "";						
	newTweet36.message	= "a moment of absolute calm";	
	newTweet36.time		= "";	
	newTweet36.location	= "Ottawa, Ontario";	
	newTweet36.name		= "lifethelemon";
	fakeXMLreturn.push_back( newTweet36 );
	
	tweet newTweet37;
	newTweet37.id		= "";						
	newTweet37.message	= "Happiness is seeing my dog scoot when I come home and do his happy prance when he catches the ball ...";	
	newTweet37.time		= "";	
	newTweet37.location	= "Ottawa, Ontario";	
	newTweet37.name		= "DramaDogLover";
	fakeXMLreturn.push_back( newTweet37 );
	
	tweet newTweet38;
	newTweet38.id		= "";						
	newTweet38.message	= "eat then sleep. In that order.";	
	newTweet38.time		= "";	
	newTweet38.location	= "Ottawa, Ontario";	
	newTweet38.name		= "effestival";
	fakeXMLreturn.push_back( newTweet38 );
	
	//"Fear \n is ..."
	tweet newTweet39;
	newTweet39.id		= "";						
	newTweet39.message	= "Fear is my middle name!";	
	newTweet39.time		= "";	
	newTweet39.location	= "Ottawa, Ontario";	
	newTweet39.name		= "tetsutakara";
	fakeXMLreturn.push_back( newTweet39 );
	
	tweet newTweet40;
	newTweet40.id		= "";						
	newTweet40.message	= "I am fear...I mean scared.";	
	newTweet40.time		= "";	
	newTweet40.location	= "Ottawa, Ontario";	
	newTweet40.name		= "championdumonde";
	fakeXMLreturn.push_back( newTweet40 );
	
	tweet newTweet41;
	newTweet41.id		= "";						
	newTweet41.message	= "4th year fall semester IMD is fear";	
	newTweet41.time		= "";	
	newTweet41.location	= "Ottawa, Ontario";	
	newTweet41.name		= "allanyong";
	fakeXMLreturn.push_back( newTweet41 );
	
	/////////////////////////////////////////////////////////////////////////////////
}


// This line tells Cinder to actually create the application
CINDER_APP_BASIC( PhilosopherCube_FinalApp, RendererGl )

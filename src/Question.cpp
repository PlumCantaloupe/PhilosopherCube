/*
 *  Question.cpp
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-24.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#include "Question.h"

Question::Question()
{
	init3DFont();
	visible = false;
	isAnimatingIn = false;
	isAnimatingOut = false;
	currDepth = MAX_DEPTH_QUESTION;
	mIsAnswerIntro = false;
	mFirstTime = false;
}

Question::~Question()
{
	delete font;
}

void Question::init3DFont()
{
	// Create a pixmap font from a TrueType file.
	font	= new FTExtrudeFont("/Developer/CinderProjects/FTGL_Font_Test/resources/HelveticaBold.ttf");
	fontTop = new FTExtrudeFont("/Developer/CinderProjects/FTGL_Font_Test/resources/HelveticaBold.ttf");
	
	// If something went wrong, bail out.
	if(font->Error())
	{
		printf("ERROR loading font");
	}
	else
	{
		font->Depth(1.5);
		font->FaceSize(2, 72);
		font->CharMap(ft_encoding_unicode); //unicode
		
		fontTop->Depth(1.5);
		fontTop->FaceSize(1, 72);
		fontTop->CharMap(ft_encoding_unicode); //unicode
	}
	
	textLayout.SetLineLength(12.0f);
	textLayout.SetFont( font );
	textLayout.SetLineSpacing(0.9f);
	textLayout.SetAlignment(FTGL::ALIGN_RIGHT);
	
	textLayoutTop.SetLineLength(12.0f);
	textLayoutTop.SetFont( fontTop );
	textLayoutTop.SetLineSpacing(0.8f);
	textLayoutTop.SetAlignment(FTGL::ALIGN_RIGHT);
}

void Question::update()
{
	if (isAnimatingOut) 
	{
		currDepth += EXTRUDE_SPEED_QUESTION;
		
		if ( currDepth >  MIN_DEPTH_QUESTION)
		{
			isAnimatingOut = false;
		}
	}
	else 
	if (isAnimatingIn)
	{
		currDepth -= EXTRUDE_SPEED_QUESTION;
		
		if ( currDepth <  MAX_DEPTH_QUESTION)
		{
			isAnimatingIn = false;
			visible = false;
		}
	}
}

void Question::draw()
{
	if( visible )
	{
		glPushMatrix();
		if ( mIsAnswerIntro )
		{
			glTranslatef(0.0, 0.0, currDepth);
		}
		else
		{
			glTranslatef(0.0, 0.0, currDepth);
		}
		glScalef(0.6f, 0.6f, 0.6f);
		//int renderMode = FTGL::RENDER_FRONT | FTGL::RENDER_BACK;
		
//		glPushMatrix();
//		glTranslatef(0.0, 1.8f, 0.0);
//		textLayoutTop.Render("@efcube", -1,  FTPoint(), FTGL::RENDER_ALL);
//		glPopMatrix();
		
		textLayout.Render(questionString.c_str(), -1,  FTPoint(), FTGL::RENDER_ALL);
		glPopMatrix();
	}
}


const char* Question::getNewQuestion()
{
	//currDepth = MAX_DEPTH_QUESTION;
	
	mIsAnswerIntro = false;
	
	//update question counter
	questCounter++;
	if( questCounter > ( NUM_QUESTIONS - 1) )
	{
		questCounter = 0;
	}
	
	const char* newQuestion = "";
	switch( questCounter )
	{
		case 0:		newQuestion = "what is \n love?";		break;
		case 1:		newQuestion = "what is \n hate?";		break;
		case 2:		newQuestion = "what is \n art?";		break;
		case 3:		newQuestion = "what is \n war?";		break;
		case 4:		newQuestion = "what is \n peace?";		break;
		case 5:		newQuestion = "what is \n free?";		break;
		case 6:		newQuestion = "what is \n home?";		break;
		case 7:		newQuestion = "what is \n friend?";		break;
		case 8:		newQuestion = "what is \n life?";		break;
		case 9:		newQuestion = "what is \n justice?";	break;
		case 10:	newQuestion = "what is \n beautiful?";	break;
		case 11:	newQuestion = "what is \n family?";		break;
		case 12:	newQuestion = "what is \n happiness?";	break;
		case 13:	newQuestion = "what is \n fear?";		break;
	}

	return newQuestion;
}

void Question::setupAnswerIntro()
{
	//currDepth = MAX_DEPTH_QUESTION;
	
	mIsAnswerIntro = true;
	
	switch( questCounter )
	{
		case 0:		questionString = "Love \n is ...";			break;
		case 1:		questionString = "Hate \n is ...";			break;
		case 2:		questionString = "Art \n is ...";			break;
		case 3:		questionString = "War \n is ...";			break;
		case 4:		questionString = "Peace \n is ...";			break;
		case 5:		questionString = "Free \n is ... ";			break;
		case 6:		questionString = "Home \n is ...";			break;
		case 7:		questionString = "Friend \n is ...";		break;
		case 8:		questionString = "Life \n is ...";			break;
		case 9:		questionString = "Justice \n is ...";		break;
		case 10:	questionString = "Beautiful \n is ...";		break;
		case 11:	questionString = "Family \n is ...";		break;
		case 12:	questionString = "Happiness \n is ...";		break;
		case 13:	questionString = "Fear \n is ...";			break;
	}
}

bool Question::isAnimating()
{
	return (isAnimatingIn || isAnimatingOut);
}

void Question::startNewQuestion()
{
	if ( mFirstTime )
	{
		Rand::randSeed( getElapsedFrames() );
		questCounter = Rand::randInt(13); //when from 0 -> num questions 0 was disproportionately picked .. :/
		mFirstTime = false;
	}
	
	//currDepth = MAX_DEPTH_QUESTION;
	questionString = getNewQuestion();
	visible = true;
	isAnimatingOut = true;
}

void Question::stopQuestion()
{
	isAnimatingIn = true;
}

void Question::startAnswer()
{
	setupAnswerIntro();
	visible = true;
	isAnimatingOut = true;
}

void Question::stopAnswer()
{
	isAnimatingIn = true;
}

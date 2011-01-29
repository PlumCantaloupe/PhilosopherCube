/*
 *  TwitterXML.h
 *  PhilosopherCube_82_5
 *
 *  Created by anthonyScavarelli on 10-10-23.
 *  Copyright 2010 Carleton University. All rights reserved.
 *
 */

#pragma once

#include <string>

#include "DefinesAndConstants.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Thread.h"
#include "cinder/Xml.h"

#include "CustomStructs.h"
#include "Utilities_ACS.h"

//not included by default
#include <boost/algorithm/string.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;

namespace TwitterXML
{
	static void createSearchStringVectors()
	{
		boost::split(HTML_ENTITIES_VECTOR, HTML_ENTITIES, boost::is_any_of(","));
		boost::split(SWEAR_STRING_VECTOR, SWEAR_STRING, boost::is_any_of(","));
		boost::split(IGNORE_STRING_VECTOR, IGNORE_STRING, boost::is_any_of(","));
	}
	
	static void getMentions( std::string sinceTime, vector<tweet>* result, std::string* lastUpdateID, 
							vector<std::string>* newWords,
							mutex* varLock, bool justLastID )
	{
		varLock->lock();
		
		printf("calling twitter ... last id: %s\n", lastUpdateID->c_str());
		
		std::string recentUpdateID = "";
		bool problemStr = false;
		
		std::string command = "/Developer/CinderProjects/PhilosopherCube_82_7/resources/shellScripts/philoTwitterMentions.sh PhiloCubeTest " + sinceTime;
		const char * partCommand = command.c_str();
		
		FILE *fp;
		fp = popen(partCommand, "r");
		
		if (fp == NULL) 
		{
			printf("ERROR opening POpen bash command file");
			return;
		}
		
		char path[50];
		const char * fgetsStr = fgets(path, 50, fp);
		string xml = "";
		while (fgetsStr != NULL)
		{
			std::string str( fgetsStr );
			//str.resize( str.size() - 1, '+' );	//don't do this here - messes up output and leads to xml parsing errors
			if( ( str.size() > 0 ) )
			{
				xml += str;
			}
			
			fgetsStr = fgets(path, 50, fp);
		}
		
		pclose(fp);
				
		std::vector<tweet> mentions;			//make a vector I can return later with all "tweet structs"
		
		//convert string to an istream
		std::istringstream xmlStream(xml);
		XmlDocument xmlObj( xmlStream );
		
		//go through all "status" elements
		if (xmlObj.rootNode().hasChildren())
		{
			std::vector<XmlElement> children = xmlObj.rootNode().children();
			
			for (int i = 0; i < children.size(); i++)
			{
				tweet newMention;
				newMention.id		= children[i].findChild("id").value();						
				newMention.message	= children[i].findChild("text").value();	
				newMention.time		= children[i].findChild("created_at").value();	
				newMention.location	= children[i].findChild("user").findChild("location").value();	
				newMention.name		= children[i].findChild("user").findChild("screen_name").value();	
				
				if ( i == 0 ) recentUpdateID = newMention.id; //the first in list is the "newest" so this id is required
				
				//check for swears
				for (int j = 0; j < SWEAR_STRING_VECTOR.size(); j++) 
				{
					boost::ireplace_first(newMention.message, SWEAR_STRING_VECTOR.at(j), "[bleep]");
					
					// iterator_range is convertible to bool
//					if(boost::ifind_first(newMention.message, SWEAR_STRING_VECTOR.at(j)))
//					{
//						printf("SWEAR IN THIS MESSAGE _ IGNOREDD! \n");
//						problemStr = true;
//						break;
//					}
				}

				
				//if no problems push into vector
				if ( !problemStr ) 
				{
					//then replace any HTML entities
					boost::ireplace_first(newMention.message, "&quot;", "\"");
					boost::ireplace_first(newMention.message, "&apos;", "'");
					
					//split up string into words
					boost::split(*newWords, newMention.message, boost::is_any_of("\t "));
					
					//go through and remove any "filler words"
					for (int k = (newWords->size() - 1); k >= 0; k--)
					{
						for(int l = (IGNORE_STRING_VECTOR.size() - 1); l >= 0; l--)
						{
							// iterator_range is convertible to bool
							if(boost::ifind_first(newWords->at(k), IGNORE_STRING_VECTOR.at(l)))
							{
								newWords->erase( newWords->begin() + k );
								break;
							}
							
							//TODO: also have to check for html entities and convert
							
						}
					}
					
					mentions.push_back( newMention );
				}
				
//				app::console() << "Id: "<< newMention.id << std::endl;
//				app::console() << "Message: "<< newMention.message << std::endl;
//				app::console() << "Location: " << newMention.location << std::endl;
//				app::console() << "Time: " << newMention.time << std::endl;
//				app::console() << "Name: " << newMention.name << std::endl;
			}
		}
		
		//boost::split(strs, "string to split", boost::is_any_of("\t "));
		
		//then set vector of elements 
		if (!lastUpdateID) mentions.clear();
		
		*result = mentions;
		if (recentUpdateID != "") *lastUpdateID = recentUpdateID;
		
		printf("finished calling twitter ... last id: %s\n", lastUpdateID->c_str());
		
		varLock->unlock();
	}
	
	static void newStatusUpdate( std::string update, string* result, mutex* varLock )
	{
		varLock->lock();
		
		printf("updating twitter ... update: %s\n", update.c_str());
		
		const char * partCommand = "/Developer/CinderProjects/PhilosopherCube_82_7/resources/shellScripts/philoTwitterUpdate.sh ";
		char entireCommand[BUFSIZ];
		sprintf(entireCommand, "%s \"%s\"", partCommand, update.c_str());
		
		FILE *fp;
		fp = popen(entireCommand, "r");
		
		if (fp == NULL) 
		{
			*result = "ERROR";
			return;
		}
		
		char path[50];
		const char * fgetsStr = fgets(path, 50, fp);
		string xml = "";
		while (fgetsStr != NULL)
		{
			std::string str( fgetsStr );
			str.resize( str.size() - 1, '+' );	//get rid of carriage return at end
			if( ( str.size() > 0 ) )
			{
				xml += str;
			}
			
			fgetsStr = fgets(path, 50, fp);
		}
		
		pclose(fp);
		
		*result = xml;
		
		printf("finished updating twitter ... update: %s\n", update.c_str());
		
		varLock->unlock();
	}
};
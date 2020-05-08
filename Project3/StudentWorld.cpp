#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
	aliensDestroyed = 0; //the number of aliens the user destroyed
	aliensRemaining = 0; //the number of aliens the user has left to kill to beat the level
	currentAliens = 0; //current aliens on the screen
	maxAliens = 0; //the maximum number of aliens that can be on a screen at a time
}

void StudentWorld::display()
{
	ostringstream oss;
	double health = (y[0]->getHitPoints() / 50.0) * 100; //gets the health of the user based on the hitpoints
	int cabbagePoints = (y[0]->getCabbagePoints() / 30.0) * 100; //gets the percent of cabbage points the user has out of 30
	int torpedoes = y[0]->getTorpedoes(); //number of torpedoes the user has
	oss << "Lives: " << getLives() << "  Health: " << health << "%  Score: " << getScore() << "  Level: " << getLevel() << "  Cabbages: " << cabbagePoints << "%  Torpedoes: " << torpedoes;
	setGameStatText(oss.str());
}

void StudentWorld::addCabbage(double blasterX, double blasterY)
{
	x.push_back(new Cabbage(IID_CABBAGE, blasterX + 12, blasterY, 0, 0.5, 1, this)); //adds a new cabbage to the vector everytime the user presses spacebar
}

void StudentWorld::addExplosion(double shipX, double shipY)
{
	x.push_back(new Explosion(IID_EXPLOSION, shipX, shipY, 0, 1, 0, this)); //adds a new explosion to the vector when an alien dies
}

void StudentWorld::addTurnip(double shipX, double shipY)
{
	x.push_back(new Turnip(IID_TURNIP, shipX - 14, shipY, 0, 0.5, 1, this)); //adds a new turnip everytime the alien shoots it
}

void StudentWorld::addExtraLifeGoodie(double xCoord, double yCoord)
{
	x.push_back(new ExtraLifeGoodie(IID_LIFE_GOODIE, xCoord, yCoord, 0, 0.5, 1, this)); //adds this goodie everytime the alien drops it
}

void StudentWorld::addRepairGoodie(double xCoord, double yCoord)
{
	x.push_back(new RepairLifeGoodie(IID_REPAIR_GOODIE, xCoord, yCoord, 0, 0.5, 1, this)); //adds this goodie everytime the alien drops it
}

void StudentWorld::addTorpedoGoodie(double xCoord, double yCoord)
{
	x.push_back(new TorpedoGoodie(IID_TORPEDO_GOODIE, xCoord, yCoord, 0, 0.5, 1, this)); //adds this goodie everytime the alien drops it
	y[0]->addTorpedos(5); //tells the nachenblaster is has five more torpedoes
}

void StudentWorld::pushTorpedo(int xCoord, int yCoord)
{
	x.push_back(new FlatulanTorpedo(IID_TORPEDO, xCoord + 12, yCoord, 0, 0.5, 1, this, 'n')); //adds a torpedo to the vector that belongs to the nachenblaster
}

void StudentWorld::pushTorpedo2(int xCoord, int yCoord)
{
	x.push_back(new FlatulanTorpedo(IID_TORPEDO, xCoord - 14, yCoord, 180, 0.5, 1, this, 'a')); //adds a torpedo to the vector that belongs to the nachenblaster
}

bool StudentWorld::isEuclidian(double xCoord, double yCoord, double radius, int damage)
{
	for (int i = 0; i != x.size(); i++) //goes through the entire vector that contains the actors
	{
		if (sqrt((pow((x[i]->getX() - xCoord), 2.0)) + (pow((x[i]->getY() - yCoord), 2.0))) < (0.75 * (radius + x[i]->getRadius())) && x[i]->isHittable()) //if the two objects are close enough and its an alien
		{
			x[i]->sufferDamage(damage); //the alien suffers the damage in the parameter
			x[i]->setDamaged(true); //alien was hit
			return true;
		}
	}
	return false;
}

bool StudentWorld::isEuclidian2(double xCoord, double yCoord, double radius, int damage)
{
	for (int i = 0; i != y.size(); i++) //goes through the nachenblaster vector
	{
		if (sqrt((pow((y[i]->getX() - xCoord), 2.0)) + (pow((y[i]->getY() - yCoord), 2.0))) < (0.75 * (radius + y[i]->getRadius())) && y[i]->isHittable()) //if the nachenblaster is hit with a turnip or collides with an alien
		{
			y[i] -> sufferDamage(damage); //the nachenblaster suffers the corresponding damage 
			playSound(SOUND_BLAST);
			return true;
		}
	}
	return false;
}

bool StudentWorld::isEuclidian3(double xCoord, double yCoord, double radius, int benefit)
{
	for (int i = 0; i != y.size(); i++)
	{
		if (sqrt((pow((y[i]->getX() - xCoord), 2.0)) + (pow((y[i]->getY() - yCoord), 2.0))) < (0.75 * (radius + y[i]->getRadius())) && y[i]->isHittable()) //if the nachenblaster collides with a goodie
		{
			y[i]->addPoints(benefit); //the nachenblaster increases its hitpoints by the benefit parameter
			return true;
		}
	}
	return false;
}

bool StudentWorld::toTheLeft(int xCoord, int yCoord)
{
	double NachenX = y[0]->getX(); //gets the y coord of the nachenblaster
	double NachenY = y[0]->getY(); //gets the x coord of the nachenblaster
	if (NachenX < xCoord && (yCoord + 4 == NachenY || yCoord - 4 == NachenY)) //if the nachenblaster is to the left and within 8 pixels of the alien ship
		return true;
	return false;
}

void StudentWorld::killedAlien()
{
	aliensDestroyed++; //how many the user destroyed is increased by 1
	aliensRemaining--; //the user has on less alien to destroy to beat the level
}

int StudentWorld::init()
{
	aliensDestroyed = 0; //at the beginning of the level, the user has destroyed 0 aliens
	aliensRemaining = 6 + (4 * getLevel()); //how many the user has to beat depends on the level
	maxAliens = 4 + (0.5 * getLevel()); //the maximimum number of aliens on screen increases every level
	
	for (int i = 0; i < 30; i++) //adds and initializes 30 stars for the game
	{
		int randomDepth = randInt(5, 50); //each star has a randon depth and size
		double z = randomDepth / 100.0;
		x.push_back(new Star(IID_STAR, randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), 0, z, 3, this));
	}

	y.push_back(new NachenBlaster(IID_NACHENBLASTER, 0, 128, 0, 1, 0, this)); //adds one nachenblaster to the game

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	currentAliens = 0;
	
	for (int i = 0; i != x.size(); i++) //loops through all the actors
	{
		if (x[i]->isAlive()) //if the actor is alive
		{
			if (x[i]->isAlien())
				currentAliens++; //current aliens on screen is incremented
			x[i]->doSomething(); //each alien needs to do something if they are alive each tick
		}
	}
	if (y[0]->getHitPoints() > 0) //if the nachenblaster is alive
		y[0]->doSomething(); //the nachenblaster does something
	else
	{
		decLives(); //the player lost one life
		return GWSTATUS_PLAYER_DIED;
	}
	if (aliensRemaining == 0) //if the user killed the number of aliens needed to beat the level
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL; //the user goes to the next level
	}

	for (int i = 0; i != x.size(); i++)
	{
		if (!(x[i]->isAlive())) //deletes all the dead actors from the vector
		{
			delete x[i];
			x.erase(x.begin() + i); //points to the next element so i needs to be decremented  
			i--; //so some values aren't skipped over
		}
	}

	double smaller = 0;
	if (aliensRemaining <= maxAliens) //smaller is set to the minimum of the number of aliensRemaining and maxAliens
		smaller = aliensRemaining;
	else
		smaller = maxAliens;

	if (currentAliens < smaller) //if the current aliens on the screen is less than smaller you add an alien
	{
		int s1 = 60;
		int s2 = 20 + (getLevel() * 5); //there are more smoregons and snagglegons on higher levels
		int s3 = 5 + (getLevel() * 10);
		int s = s1 + s2 + s3;
		int rand = randInt(1, s);
		if (rand >= 1 && rand < s1) //for the probability s1/s a smallgon is added
			x.push_back(new Smallgon(IID_SMALLGON, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, 1.5, 1, this));
		else if (rand >= s1 && rand < s1 + s2) //for the probability s2/s a smoregon is added
			x.push_back(new Smoregon(IID_SMOREGON, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, 1.5, 1, this));
		else if(rand >= (s1+s2) && rand < s) //for the probability s3/s a snagglegon is added
			x.push_back(new Snagglegon(IID_SNAGGLEGON, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, 1.5, 1, this));
	}


	int c = randInt(1, 15); //there is a one in 15 chance that a new star will be added
	if (c == 1)
	{
		int randomDepth = randInt(5, 50);
		double z = randomDepth / 100.0;
		x.push_back(new Star(IID_STAR, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, z, 3, this));
	}

	display();

	return GWSTATUS_CONTINUE_GAME;//GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i != x.size(); i++) //at the end of each level or the end of the game
	{
		delete x[i]; //every dynamically allocated item is manually deleted
		x.erase(x.begin() + i);
		i--;
	}

	for (int i = 0; i != y.size(); i++)//the nachenblaster is also deleted
	{
		delete y[i];
		y.erase(y.begin() + i);
		i--;
	}

}

StudentWorld::~StudentWorld()
{
	cleanUp(); //cleanup already destructs all the dynamically allocated items
}

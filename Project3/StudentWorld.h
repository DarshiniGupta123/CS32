#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <cmath>
using namespace std;

class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);//these are pure virtual functions inherited from the game world class and need to be overriden so student world is not abstract
    virtual int init(); //intializes the thirty stars and the nachenblaster
    virtual int move(); //allows each actor to do something ever tick
    virtual void cleanUp();

	~StudentWorld();

	void display(); //changes the game stats every tick and displays them on the game 

	void addCabbage(double blasterX, double blasterY); //adds a cabbage to the vector of actor pointers everytime the user presses spacebar

	void addExplosion(double shipX, double shipY); //adds an explosion to the vector of actor pointers everytime the user kills an alien

	void addTurnip(double shipX, double shipY); //adds a turnip to the vector of actor pointers everytime the alien shoots a turnip
	
	void addExtraLifeGoodie(double xCoord, double yCoord); //adds an extra life goodie to the vector of actor pointers everytime the snagglegon dies and drops and extra life goodie

	void addRepairGoodie(double xCoord, double yCoord); //adds an repair life goodie to the vector of actor pointers everytime the smoregon dies and drops and repair life goodie

	void addTorpedoGoodie(double xCoord, double yCoord); //adds an torpedo goodie to the vector of actor pointers everytime the smoregon dies and drops and torpedo goodie

	void pushTorpedo(int xCoord, int yCoord); //adds a torpedo to the vector of actor pointer that belongs to the nachenblaster

	void pushTorpedo2(int xCoord, int yCoord); //adds a torpedo to the vector of actor pointer that belongs to the snagglegon

	bool isEuclidian(double xCoord, double yCoord, double radius, int damage); //checks if a cabbage hits an alien ship

	bool isEuclidian2(double xCoord, double yCoord, double radius, int damage); //chanck is the torpedo or turnip or alien ship hits the nachenblaster

	bool isEuclidian3(double xCoord, double yCoord, double radius, int benefit); //checks if the nachenblaster gets a goodie

	bool toTheLeft(int xCoord, int yCoord); //checks if the nachenblaster is to the left of the alien ship and within 8 pixels in the y direction

	void killedAlien(); //decrements the number of aliens the user has to kill the beat the level


private:
	vector<Actor*> x; //vector that contains all of the actors except the nachen blaster
	vector<Actor*> y; //nachenblaster
	int aliensDestroyed; //how may aliens the user kills
	int aliensRemaining; //how many aliens remaining for the user to finish the level
	int maxAliens; //maximum number of aliens that can be on the screen
	int currentAliens; //current number of aliens on the screen
};

#endif // STUDENTWORLD_H_

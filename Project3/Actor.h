#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething() = 0; //every actor has a doSomething function that needs to be overriden and this function makes actor abstract

    bool isAlive(); //returns whether an actor is alive or not

	void setAliveStatus(bool a); //changes the status of whether the actor is alive or not

	StudentWorld* getStudentWorld(); //returns a pointer to the student world so this class can access its functions

	virtual bool isHittable() = 0; //whether the actor can be hit by projectiles or not

	virtual bool isAlien();//if the actor is an alien or not

	virtual void setDamaged(bool status); //if the actor was hit by a projectile and set to true if it were hit

	virtual void sufferDamage(double points); //reduces the hitpoints of an actor, but only valid for aliens and nachenblaster

	virtual void addPoints(double points); // add the hitpoints of an actor, but only valid for aliens and nachenblaster

	virtual double getHitPoints(); //returns the hitpoints of an actor, but only valid for aliens and nachenblaster , however used for the display in student world cpp

	virtual double getCabbagePoints(); //returns the cabbage points of an actor, but only nachenblaster and used for the display/ game stats for the student world

	virtual void addTorpedos(int num); // adds torpedoes to the nachencblaster every time the user gets a goodie

	virtual double getTorpedoes(); //returns the torpedoes of an actor, but only for the nachenblaster and used for the display/ game stats for the student world


private:
	StudentWorld* student_world; //pointer to the student world
	bool is_Alive; //keeps track of whether the actor is alive or not
	int killed_Aliens;

};


class NachenBlaster : public Actor
{
public:
	NachenBlaster(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //gives the ability for the nachenblaster to move every tick and shoot things

	virtual bool isHittable(); //the nachenblaster is hittable by projectiles

	virtual void addPoints(double points); //whenever the nachenblaster gets a repair goodie

	virtual double getHitPoints(); 

	void addTorpedos(int num); //adds the 5 torpedoes to the nachenblaster

	virtual void sufferDamage(double points); //suffers damage when it hits an alien or a turnip or torpedo

	virtual double getCabbagePoints();

	virtual double getTorpedoes();
private:
	int hit_points; //starts out with 50
	int cabbage_points; //starts out with 30
	int torpedos; //starts out with 0
};

class Star : public Actor
{
public:
	Star(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //all the star does is move to the left every tick

	virtual bool isAlive(); //not alive when it leaves the screen

	virtual bool isHittable(); //stars are not hittable by anything

};

class Explosion : public Actor
{
public:
	Explosion(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //explosions only grow and then die

	virtual bool isHittable(); //explosions are not hittable by anything

private:
	int currentTick; //explosions can only be allive for 4 ticks so this keeps track of when it should die
};

class Projectile : public Actor
{
public:
	Projectile(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void isFlown(); //if the projectile is off the screen, it is set to dead

	virtual void doSomething() = 0; //projectiles are abstract and just the layout for all of the other projectiles, cabbages, turnips, and torpedoes

	virtual bool isHittable(); //projectiles hit other things and are not hittable

};

class Cabbage : public Projectile
{
public:
	Cabbage(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //cabbages are created when the user hits a spacebar and moves to the right until it hits an alien or flies off the screen
	
};

class Turnip : public Projectile
{
public:
	Turnip(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //turnips are created when the alien shoots a turnip and they move to the right until it hits the nachenblaster or flies of the screen
};

class FlatulanTorpedo : public Projectile
{
public:
	FlatulanTorpedo(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld, char belongsTo);

	virtual void doSomething(); //torpedos can be fired by both the nachenblaster and the alien
private:
	char belongs_to; //to keep track of who the torpedo belongs to, to know whether to hit the nachenblaster or hit the alien
};
class Alien : public Actor
{
public:
	Alien(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	bool isDamaged(); //returns if the alien was hit by a projectile

	virtual void setDamaged(bool status);

	void setHitPoints(double points); //every type of alien has a different 

	double getHitPoints();

	void setFlightLength(double length); //sets the length of the flight the alien will travel to before changing its flight

	double getFlightLength();

	void setFlightDirection(char direction); //sets the direction the alien travels in either up and left, down and left, or just left

	char getFlightDirection();

	void setTravelSpeed(double speed); //how fast the alien will move, it will move this may pixels to the left, up or down

	double getTravelSpeed();

	void isFlown();

	virtual bool isAlien(); //all of the subclasses of aliens are aliens so this returns true

	virtual bool isHittable(); //aliens are hittable

	virtual void sufferDamage(double points); //their hitpoints decrease when the alien is hit by a cabbage or torpedo or collides with the nachenblaster

	void collsion(double points, double damage); //checks collision with projectiles and the nachenblaster 

	void changingFlight(); //if it hits the top or bottom of the screen or its flight length is 0

	void moving(); //moves the alien accordingly to its direction and speed

	void setDropGoodie(bool yes); //if the alien dies it may drop a goodie

	bool getDropGoodie();


private:
	bool wasHit; //keeps track if the goodie was hit by a projectile
	double hitPoints; 
	double flightLength;
	char flightDirection;
	double travelSpeed;
	bool drop_goodie; //if there is a possibilty of a goodie being dropped

};

class Smallgon : public Alien
{
public:
	Smallgon(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //the alien moves and checks collision with the nachenblaster and if it was damaged then it takes certain actions 
};

class Smoregon : public Alien
{
public:
	Smoregon(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //the alien moves and checks collision with the nachenblaster and if it was damaged then it may drop a goodie
};

class Snagglegon : public Alien
{
public:
	Snagglegon(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //the alien moves and checks collision with the nachenblaster and if it was damaged then it may drop a goodie
};

class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual bool isHittable(); //goodies are not hittable by projectiles

	void isFlown(); //goodies die when they fly off the screen

	bool collides(double benefit); //if the nachenblaster gets the goodie by colliding with it, it gets some benefit based on the type of goodie
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //checks to see if the nachenblaster got it or it moves until it dies
};

class RepairLifeGoodie : public Goodie
{
public:
	RepairLifeGoodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //checks to see if the nachenblaster got it or it moves until it dies

};

class TorpedoGoodie : public Goodie
{
public:
	TorpedoGoodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld);

	virtual void doSomething(); //checks to see if the nachenblaster got it or it moves until it dies
};

#endif // ACTOR_H_
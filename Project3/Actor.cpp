#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;

Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: GraphObject(imageID, startX, startY, dir, size, depth)
{
	student_world = studentWorld; //pointer to the current student world
	is_Alive = true; //each actor initalized starts out with an alive status as true
}

StudentWorld* Actor::getStudentWorld()
{
	return student_world; //returns pointer to the current student world
}

bool Actor::isAlive()
{
	return is_Alive; //returns whether the actor is alive or not
}

void Actor::setAliveStatus(bool a) //parameter sets alive status to true or false
{
	is_Alive = a; //changes whether the actor is alive or not
}

void Actor::setDamaged(bool status) //this function is so the student world can use this function in the alien and nachenblaster classes
{
	return; //returns nothing since this function is for the nachenblaster and alien ships
}

void Actor::sufferDamage(double points) //this function is so the student world can use this function in the alien and nachenblaster classes
{
	return; //returns nothing because nothing should happen unless called in respect to the collisions and the aliens and nachenblaster
}

void Actor::addPoints(double points)//this function is so the student world can use this function in the alien and nachenblaster classes
{
	return;
}

double Actor::getHitPoints() //for the display function in the student world class
{
	return 0.0; //returns nothing because the nachenBlaster will override it
}

double Actor::getCabbagePoints() //for the display function in the student world class
{
	return 0.0;
}

double Actor::getTorpedoes() //for the display function in the student world class
{
	return 0.0;
}

bool Actor::isAlien() //mostly all the of the subclasses are not aliens 
{
	return false; //so it returns false 
}

void Actor::addTorpedos(int num) //the function in nachenblaster will override it 
{
	return; //return nothing, only need to keep track of nachenblaster's torpedoes
}

Star::Star(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

void Star::doSomething()
{
	moveTo(getX() - 1, getY()); //the star moves to the left one pixel every tick
}

bool Star::isAlive()
{
	if (getX() == -1) //if the x coordinate is less than 0
		return false; //the star is not alive anymore
	return true;
}

bool Star::isHittable()
{
	return false; //stars cannot be hit by projectiles or ships, they are just in the background
}

NachenBlaster::NachenBlaster(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, dir, size, depth, studentWorld)
{
	hit_points = 50; //a nachenblaster stars out with 50 hitpoints
	cabbage_points = 30; //it starts out with cabbage points
	torpedos = 0; //and 0 torpedoes
}

void NachenBlaster::addTorpedos(int num)
{
	torpedos = torpedos + num; //the nachenBlaster gets torpedoes everytime it collides with a torpedo goodie
}

double NachenBlaster::getTorpedoes()
{
	return torpedos; //returns the number torpedoes the nachenblaster has 
}

void NachenBlaster::doSomething()
{
	if (!isAlive()) //if the nacheblaster is not alive return immediately
		return;
	else
	{
		int value;
		if ((getStudentWorld())->getKey(value)) //is the user presses a key
		{
			if (value == KEY_PRESS_SPACE && cabbage_points >= 5) //if its a spacebar and there are enough cabbage points
			{
				cabbage_points = cabbage_points - 5;
				getStudentWorld()->addCabbage(getX(), getY()); //a cabbage is added to the stack of actors and cabbage points is decremented
				getStudentWorld()->playSound(SOUND_PLAYER_SHOOT);
			}
			if (value == KEY_PRESS_DOWN && (getY() - 1) >= 0) //if the key is down and the y coordinate is greater than 0
				moveTo(getX(), getY() - 6); //the user moves down by 6 pixels
			if (value == KEY_PRESS_UP && (getY() + 1) < VIEW_HEIGHT)
				moveTo(getX(), getY() + 6); //the user moves up by 6 pixels
			if (value == KEY_PRESS_RIGHT && (getX() + 1) < VIEW_WIDTH)
				moveTo(getX() + 6, getY()); //moves right by 6 pixels
			if (value == KEY_PRESS_LEFT && (getX() - 1) >= 0)
				moveTo(getX() - 6, getY()); //moves left by 6 pixels
			if (value == KEY_PRESS_TAB && torpedos > 0) //if the user presses tab and they have torpedoes
			{
				torpedos--; //torpedoes is decremented by 1
				getStudentWorld()->pushTorpedo(getX(), getY()); // the student world adds a torpedo
				getStudentWorld()->playSound(SOUND_TORPEDO);
			}
		}
		if (cabbage_points < 30)
			cabbage_points++; //increment cabbages by 1 every tick if less than 30

	}
}

double NachenBlaster::getHitPoints()
{
	return hit_points; //returns the hit points the nachenblaster has 
}

double NachenBlaster::getCabbagePoints()
{
	return cabbage_points;
}

void NachenBlaster::sufferDamage(double points)
{
	hit_points = hit_points - points; //subtracts the hitpoints by the points in the parameter
}

bool NachenBlaster::isHittable()
{
	return true; //the nachenblaster is able to be hit by projectiles and ships
}

void NachenBlaster::addPoints(double points)
{
	for (int i = 0; i != points; i++) //if the hitpoints is less than 50
	{
		if (hit_points < 50)
			hit_points++; // then you can add the points to the hit points 
		else
			break;
	}
}

Explosion::Explosion(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, dir, size, depth, studentWorld)
{
	currentTick = 1;
}

bool Explosion::isHittable()
{
	return false; //explosions are not abe to be hit
}

void Explosion::doSomething()
{
	setSize(getSize() * 1.5); //increase the size of the explosion by a factor of 1.5
	currentTick++;
	if (currentTick == 5) //when four ticks pass
		setAliveStatus(false); // the explosion object dies
}


Projectile::Projectile(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

void Projectile::isFlown()
{
	if (getX() >= VIEW_WIDTH || getX() < 0) //if the projectile flows off the screen
		setAliveStatus(false); //its status is set to dead
	else
		setAliveStatus(true);
}

bool Projectile::isHittable() 
{
	return false; //projectiles are not hittable
}

Cabbage::Cabbage(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Projectile(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

void Cabbage::doSomething()
{
	isFlown();
	if (!isAlive()) //don't do anything is the cabbage is not alive
		return;
	else
	{
		if (getStudentWorld()->isEuclidian(getX(), getY(), getRadius(), 2)) //if the cabbage collides with an alien ship
			setAliveStatus(false); // the cabbage is set to dead
		else
		{
			moveTo(getX() + 8, getY()); //move the cabbage 8 pixels to the right
			setDirection(getDirection() + 20);
			if (getStudentWorld()->isEuclidian(getX(), getY(), getRadius(), 2)) //check if the cabbage collides again
				setAliveStatus(false);
		}
	}
}

Turnip::Turnip(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Projectile(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

void Turnip::doSomething()
{
	isFlown();
	if(!isAlive()) //if the turnip is not alive don't do anything
		return;
	else
	{
		if (getStudentWorld()->isEuclidian2(getX(), getY(), getRadius(), 2)) //if the turnip collides with the nachenBlaster
			setAliveStatus(false); // the turnip is now dead
		else
		{
			moveTo(getX() - 6, getY()); //move the turnip 6 pixels to the left
			setDirection(getDirection() + 20);
			if (getStudentWorld()->isEuclidian2(getX(), getY(), getRadius(), 2)) //checks collision again
				setAliveStatus(false);
		}

	}
		
}

FlatulanTorpedo::FlatulanTorpedo(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld, char belongsTo)
	: Projectile(imageID, startX, startY, dir, size, depth, studentWorld)
{
	belongs_to = belongsTo; //variable to keep track of whether the torpedo belongs to a nachenblaster or an alien
}

void FlatulanTorpedo::doSomething()
{
	isFlown();
	if (!isAlive()) //if the flatulan torpedo is not alive don't do anything
		return;
	else
	{
		if (belongs_to == 'n') //if the torpedo belongs to the nachenblaster
		{
			if (getStudentWorld()->isEuclidian(getX(), getY(), getRadius(), 8)) //checks collision with an alien
				setAliveStatus(false);
		}
		else if (belongs_to == 'a') // checks collision with the nachenblaster
		{
			if (getStudentWorld()->isEuclidian2(getX(), getY(), getRadius(), 8))
				setAliveStatus(false);
		}
		if (belongs_to == 'n') //if it belongs to the nachenblaster
		{
			moveTo(getX() + 8, getY()); //move the torpedo 8 pixels to the right and check collsion again
			if (getStudentWorld()->isEuclidian(getX(), getY(), getRadius(), 8))
				setAliveStatus(false);
		}
		else if (belongs_to == 'a') //if it belongs to an alien
		{
			moveTo(getX() - 8, getY()); //move the torpedo 8 to the left and check collision again
			if (getStudentWorld()->isEuclidian2(getX(), getY(), getRadius(), 8))
				setAliveStatus(false);
		}

	}
}

Alien::Alien(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, dir, size, depth, studentWorld)
{
	wasHit = false; //keeps track of whether the alien is hit, intialized to it isn't hit
	flightLength = 0;
	flightDirection = ' ';
	travelSpeed = 0;
	drop_goodie = false;//keeps track of whether the alien should drop a goodie
}

bool Alien::isDamaged()
{
	return wasHit; //retuns whether the alien was hit or not by a projectile
}

void Alien::setDamaged(bool status)
{
	wasHit = status; //changes the wasHit status 
}

void Alien::setHitPoints(double points)
{
	hitPoints = points; //changes the hit points of the alien
}

void Alien::sufferDamage(double points)
{
	hitPoints = hitPoints - points; //decrement hit points when the alien is hit
}

double Alien::getHitPoints()
{
	return hitPoints; //returns the hit points of the alien
}

void Alien::setFlightLength(double length)
{
	flightLength = length; //changes the flight length
}

double Alien::getFlightLength()
{
	return flightLength; //returns the flight length
}

void Alien::setTravelSpeed(double speed)
{
	travelSpeed = speed;
}

double Alien::getTravelSpeed()
{
	return travelSpeed;
}

void Alien::isFlown()
{
	if (getX() >= VIEW_WIDTH || getX() < 0)//if the alien is off the screen
		setAliveStatus(false); // the alien is set to dead
	else
		setAliveStatus(true);
}

bool Alien::isAlien()
{
	return true; //all of the alien subclasses are aliens
}

bool Alien::isHittable()
{
	return true; //all aliens are able to be hit
}

void Alien::setFlightDirection(char direction)
{
	flightDirection = direction; //you can change the direction of the flight the alien is travelling in
}

char Alien::getFlightDirection()
{
	return flightDirection; //returns the direction the alien is travelling in
}

void Alien::setDropGoodie(bool yes)
{
	drop_goodie = yes; //changes whether the alien should drop a goodie
}

bool Alien::getDropGoodie()
{
	return drop_goodie;
}

void Alien::collsion(double points, double damage)
{
	if (isDamaged()) //if the alien has been hit by a projectile
	{
		if (getHitPoints() <= 0) //if the hit points of the alien is 0 or less
		{
			getStudentWorld()->increaseScore(points); //increases the user's points
			setAliveStatus(false); //the alien is dead
			getStudentWorld()->killedAlien(); //increment the number of aliens the user kills
			getStudentWorld()->playSound(SOUND_DEATH);
			getStudentWorld()->addExplosion(getX(), getY()); //create explosion where the alien dies
			setDropGoodie(true); //there is a possibility of a goodie being created
		}
		else
			getStudentWorld()->playSound(SOUND_BLAST);
	}

	if (getStudentWorld()->isEuclidian2(getX(), getY(), getRadius(), damage)) //if the alien collides with the nachenblaster
	{
		setAliveStatus(false); //the alien dies
		getStudentWorld()->killedAlien(); //increment the number of aliens the user kills
		getStudentWorld()->increaseScore(points);
		getStudentWorld()->playSound(SOUND_DEATH);
		getStudentWorld()->addExplosion(getX(), getY());
		setDropGoodie(true);
	}
	setDamaged(false); //damaged is reinitialized to false
}



void Alien::changingFlight()
{
	if (getFlightLength() == 0 || (getY() <= 0) || (getY() >= (VIEW_HEIGHT - 1))) //if the flight lenght is 0 or the alien is at the edge of the screen
	{
		if (getY() >= (VIEW_HEIGHT - 1)) //if at the top of the screen
			setFlightDirection('d'); //the direction is changed to down and left
		else if (getY() <= 0) //if at the bottom of the screen
			setFlightDirection('u'); //the direction is up and left
		else if (getFlightLength() == 0) //if the flight length is 0
		{
			int randNum = randInt(0, 2);
			if (randNum == 0)
				setFlightDirection('l'); // 1 in 3 chance the direction is left
			if (randNum == 1)
				setFlightDirection('u'); //1 in 3 change the direction is up and left
			if (randNum == 2)
				setFlightDirection('d'); //1 in 3 chance the direction is down and left
		}
		setFlightLength(randInt(1, 32)); //choose random flight length
	}
}

void Alien::moving()
{
	setFlightLength(getFlightLength() - 1); //decrement the flight length by 1
	switch (getFlightDirection())
	{
	case 'u':
		moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());//moves the alien up by the travel speed and left by the travel speed 
		break;
	case 'd':
		moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());//moves the alien down by the travel speed and left by the travel speed 
		break;
	case 'l':
		double currentSpeed = getTravelSpeed();
		double currentX = getX();
		double currentY = getY();
		moveTo(currentX - currentSpeed, currentY);//moves the alien left by the travel speed 
		break;

	}
}


Smallgon::Smallgon(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	:Alien(imageID, startX, startY, dir, size, depth, studentWorld)
{
	setHitPoints(5 * (1 + ((getStudentWorld()->getLevel()) - 1) * .1));//hitpoints depends on the level, more hitpoints every level
	setFlightLength(0.0);
	setTravelSpeed(2.0); //how fast the alien moves every tick
}

void Smallgon::doSomething()
{
	isFlown();
	if (!isAlive())//if the alien is not alive don't do anything
		return;
	else
	{
		collsion(250, 5);//checks collision with parameters 250 hitpoints and 5 damage
		changingFlight(); //change the direction of flight and flight length

		if (getStudentWorld()->toTheLeft(getX(), getY())) //checks if the nachenblaster is to the left of the alien
		{
			int randNum2 = randInt(1, ((20 / getStudentWorld()->getLevel()) + 5));
			if (randNum2 == 1) //if the random number is 1
			{
				getStudentWorld()->addTurnip(getX(), getY()); //add a turnip to attack the nachenblaster
				getStudentWorld()->playSound(SOUND_ALIEN_SHOOT);
				return;
			}
		}
		if (isAlive())
		{
			moving(); //move the ship
			collsion(250, 5); //checks the collision again with the same parameters
		}
	}
}

Smoregon::Smoregon(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Alien(imageID, startX, startY, dir, size, depth, studentWorld)
{
	setHitPoints(5 * (1 + ((getStudentWorld()->getLevel()) - 1) * .1));//hitpoints depends on the level, more hitpoints every level
	setFlightLength(0.0);
	setTravelSpeed(2.0); //how fast the alien moves
}


void Smoregon::doSomething()
{
	isFlown();
	if (!isAlive()) //if the alien is dead don't do anything and return
		return;
	else
	{
		collsion(250, 5); //check collision with projectiles and nachenblaster with hit points 250 and damage 5
		if (getDropGoodie())
		{
			int randNum = randInt(1, 3); //there is a one in three chance that the alien will drop a goodie
			if (randNum == 1)
			{
				int random = randInt(1, 2); //50 percent chance it will be a repair goodie and 50 % chance its a torpedo goodie
				if (random == 1)
					getStudentWorld()->addRepairGoodie(getX(), getY());
				else
					getStudentWorld()->addTorpedoGoodie(getX(), getY());
			}
			setDropGoodie(false); //drop goodie is reinitialized to 0
		}
		changingFlight(); //change the flight direction and plan

		if (getStudentWorld()->toTheLeft(getX(), getY()))
		{
			int randNum2 = randInt(1, ((20 / getStudentWorld()->getLevel()) + 5));
			if (randNum2 == 1) //the probablity for the alien to shoot a turnip
			{
				getStudentWorld()->addTurnip(getX(), getY());
				getStudentWorld()->playSound(SOUND_ALIEN_SHOOT);
				return;
			}
			randNum2 = randInt(1, ((20 / getStudentWorld()->getLevel()) + 5));
			if (randNum2 == 1) //if condition met
			{
				setFlightDirection('l'); //changes direction to the left for the rest of the alien's life
				setFlightLength(VIEW_WIDTH);
				setTravelSpeed(5.0); //sets a really fast speed
			}
		}
		if (isAlive())
		{
			moving();
			collsion(250, 5); //check collision with projectiles and nachenblaster with hit points 250 and damage 5
			if (getDropGoodie())
			{
				int randNum = randInt(1, 3); //there is a one in three chance that the alien will drop a goodie
				if (randNum == 1)
				{
					int random = randInt(1, 2); //50 percent chance it will be a repair goodie and 50 % chance its a torpedo goodie
					if (random == 0)
						getStudentWorld()->addRepairGoodie(getX(), getY());
					else
						getStudentWorld()->addTorpedoGoodie(getX(), getY());
				}
				setDropGoodie(false);
			}
		}
	}
}

Snagglegon::Snagglegon(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Alien(imageID, startX, startY, dir, size, depth, studentWorld)
{
	setHitPoints(10 * (1 + ((getStudentWorld()->getLevel()) - 1) * .1));//hitpoints depends on the level, more hitpoints every level
	setTravelSpeed(1.75);
	setFlightDirection('d'); //intial direction of snagglegon's is down and left
}

void Snagglegon::doSomething()
{
	isFlown();
	if (!isAlive()) //if the alien is dead don't do anything and return
		return;
	else
	{
		collsion(1000, 15); //check collision with projectiles and nachenblaster with hit points 1000 and damage 15
		if (getDropGoodie())
		{
			int randNum = randInt(1, 6); //there is a one in six chance that the alien will drop a goodie
			if (randNum == 1) //the snagglegon only drops extra life goodies
				getStudentWorld()->addExtraLifeGoodie(getX(), getY());
			setDropGoodie(false);
		}
		if ((getY() <= 0) || (getY() >= (VIEW_HEIGHT - 1))) //if the alien reaches the top or bottom of the screen
		{
			if (getY() >= (VIEW_HEIGHT - 1)) //if at the top of the screen
				setFlightDirection('d'); //change the direction to down and left
			else if (getY() <= 0)
				setFlightDirection('u'); //changs the direction to up and left
		}

		if (getStudentWorld()->toTheLeft(getX(), getY()))
		{
			int randNum2 = randInt(1, ((15 / getStudentWorld()->getLevel()) + 10));
			if (randNum2 == 1) //if this probablity
			{
				getStudentWorld()->pushTorpedo2(getX(), getY()); //the snagleggon fires a torpedo at the nachenblaster
				getStudentWorld()->playSound(SOUND_TORPEDO);
				return;
			}
		}

		switch (getFlightDirection())
		{
		case 'u':
			moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed()); //moves up and left by the travel speed
			break;
		case 'd':
			moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed()); //moves down and left by the travel speed
			break;
		}
		if (isAlive())
		{
			collsion(1000, 15); //check collision with projectiles and nachenblaster with hit points 1000 and damage 15
			if (getDropGoodie())
			{
				int randNum = randInt(1, 6); //there is a one in six chance that the alien will drop a goodie
				if (randNum == 1)
					getStudentWorld()->addExtraLifeGoodie(getX(), getY());
				setDropGoodie(false);
			}
		}
	}
}

Goodie::Goodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

bool Goodie::isHittable()
{
	return false; //goodies are not hittable by projectiles
}

void Goodie::isFlown()
{
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
		setAliveStatus(false);
	else
		setAliveStatus(true);
}

bool Goodie::collides(double benefit)
{
	if (getStudentWorld()->isEuclidian3(getX(), getY(), getRadius(), benefit)) //if the goodie is in euclidian distance of the nachenblaster
	{
		getStudentWorld()->increaseScore(100); //increase the users score by 100
		setAliveStatus(false); //the goodie is dead
		getStudentWorld()->playSound(SOUND_GOODIE);
		return true;
	}
	return false;
}

ExtraLifeGoodie::ExtraLifeGoodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Goodie(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

void ExtraLifeGoodie::doSomething()
{
	isFlown();
	if (!isAlive())//if the goodie is not alive don't do anything
		return;
	else
	{
		if (collides(0)) //if the user gets the goodie
		{
			getStudentWorld()->incLives(); //increment the lives the user has by 1
			return;
		}
		moveTo(getX() - 0.75, getY() - 0.75); //move the goodie
		if (collides(0)) //check if the user gets the goodie again
		{
			getStudentWorld()->incLives();
			return;
		}
	}

}

RepairLifeGoodie::RepairLifeGoodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Goodie(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

void RepairLifeGoodie::doSomething()
{
	isFlown();
	if (!isAlive())//if the goodie is not alive don't do anything
		return;
	else
	{
		if (collides(10)) //if the nachenblaster and goodie collide, the user gets 10 hitpoints
			return;
		moveTo(getX() - 0.75, getY() - 0.75); //move the goodie and check collision again
		if (collides(10))
			return;
	}
}

TorpedoGoodie::TorpedoGoodie(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld* studentWorld)
	: Goodie(imageID, startX, startY, dir, size, depth, studentWorld)
{

}

void TorpedoGoodie::doSomething()
{
	isFlown();
	if (!isAlive()) //if the goodie is not alive don't do anything
		return;
	if (collides(0)) //if the nachenblaster gets the goodie, the user gets 5 torpedoes
	{
		return;
	}
	moveTo(getX() - 0.75, getY() - 0.75); //move the goodie and check collision again
	if (collides(0))
	{
		return;
	}

}
	// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

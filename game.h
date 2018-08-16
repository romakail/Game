#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"


const int WINDOW_HEIGHT = 900;
const int WINDOW_LENGHT = 900;

const float MAIN_CHARACTER_SPEED = 10;
const float MAIN_CHARACTER_RADIUS = 75;

const float ARROW_SPEED = 20;
const int MAX_ARROWS = 35;
const float ARROW_LENGHT = 40;

const float ZOMBIE_RADIUS = 50;
const int MAX_ZOMBIES = 35;

const float COIN_RADIUS = 5;
const int MAX_COINS = 100;
const int COIN_VALUE = 5;
const int COIN_LINES_IN_LIST_X = 3;
const int COIN_LINES_IN_LIST_Y = 3;

const double SQRT2 = sqrt (0.5);
const float DEGREES_IN_RADIAN = 57.2956;
const float PI = 3.1416;
const float EXTENDED_ANGLE = 180;



sf::RenderWindow* window = nullptr;

//Classes

class gameObject;
class player;
class arrow;
class zombie;
class coin;
class arrows_list;
class castle;

template <class gObject>
class list_T;

template <class gObject>
class coordinateList_T;

//=========Functions================

//Usual
zombie createZombie ();
int game ();
int arrowPhysicsManager (list_T<arrow>* arrowsList, player* mainCharacter);

//Colliders
bool colliderZombieVsArrow (zombie* Zombie, arrow* Arrow);
bool colliderPlayerVsCoin (player* Player, coin* Coin);
bool colliderPlayerVsCastle (player* Player, castle* Castle);
bool colliderCircleVsLine (sf::Vector2f centre, float radius, sf::Vector2f dot1, sf::Vector2f dot2);
bool colliderCircleVsDot (sf::Vector2f centre, float radius, sf::Vector2f dot);

//Managers
int managerZombiesVsArrows (list_T<zombie>* zombieList, list_T<arrow>* arrowsList, coordinateList_T<coin>* coinsList);
int managerPlayerVsCoins (player* collector, coordinateList_T<coin>* coinsList);










class gameObject
{
    protected:

        sf::Vector2f pos;
        sf::Vector2f v;
        sf::Vector2f lenght;

    public:
        gameObject (sf::Vector2f position, sf::Vector2f speed);

        int changePosition ();
        virtual int draw () = 0;
};


class arrow : public gameObject
{
    friend class player;
    friend class list_T<arrow>;

	friend bool colliderZombieVsArrow(zombie* Zombie, arrow* Arrow);
	friend int managerZombiesVsArrows (list_T<zombie>* zombieList, list_T<arrow>* arrowsList, coordinateList_T<coin>* coinsList);

    private:
        float Vmax;
        float lenght;
        sf::RectangleShape shape;

    protected:
        arrow* next;
        arrow* prev;

    public:
        arrow (sf::Vector2f position, sf::Vector2f speed);
        arrow ();
        arrow& operator= (const arrow& right);

        int draw ();

    //friend class arrows_list;
};


class player : public gameObject
{
	friend bool colliderPlayerVsCoin (player* Player, coin* Coin);
	friend int managerPlayerVsCoins (player* collector, coordinateList_T<coin>* coinsList);
	friend bool colliderPlayerVsCastle (player* Player, castle* Castle);

	//friend int coordinateList_T<coin>::playerCollectsCoins1x1 (player* collector);

    private:
        float Vmax;
        float radius;
        sf::CircleShape shape;

    public:
        player (sf::Vector2f position, float mainCharacterSpeed, float mainCharacterRadius);

        int control ();
        int draw ();
        arrow shoot(float shootSpeed);
};

class zombie : public gameObject
{
	friend class list_T<zombie>;

	friend bool colliderZombieVsArrow(zombie* Zombie, arrow* Arrow);
	friend int managerZombiesVsArrows (list_T<zombie>* zombieList, list_T<arrow>* arrowsList, coordinateList_T<coin>* coinsList);

    private:
    	float Vmax;
    	float radius;
        sf::CircleShape shape;

	protected:
        zombie* next;
        zombie* prev;

    public:
    	zombie ();
        zombie (sf::Vector2f position, float zombieSpeed, float zombieRadius);
        zombie& operator= (const zombie& right);

        sf::Vector2f getPosition ();

        int draw();
};

class coin : public gameObject
{
	friend class coordinateList_T<coin>;

	friend bool colliderPlayerVsCoin (player* Player, coin* Coin);
	friend int managerPlayerVsCoins (player* collector, coordinateList_T<coin>* coinsList);

    private:
        int value;
        float radius;
        sf::CircleShape shape;

    protected:
        coin* next;
        coin* prev;

    public:
        coin ();
        coin (sf::Vector2f position, int value, float coinRadius);
        coin& operator= (const coin& right);

        int draw ();
};

//------------------------------------------------------------------------

class castle
{
    friend bool colliderPlayerVsCastle (player* Player, castle* Castle);

    private:
        int nAngles;
        sf::Vector2f* anglesMassive;
		//sf::RectangleShape* shapesMassive;
        sf::ConvexShape walls;

    public:
        castle (int anglesNumber, sf::Vector2f* anglesCoordinates);
        ~castle ();

        int draw ();
};





#include "list.h"
#include "coordinateList.h"






















/*
class arrows_list
{
    private:
        int maxArrows;
        int nArrows;

        arrow* head;
        arrow* tail;
        arrow* freeHead;
        arrow* freeTail;
        arrow* arrowsMassive;

    public:
        arrows_list (int maxArrowsNumber);
        ~arrows_list ();
        arrow* addArrow    (arrow newArrow);
        int deleteArrow (arrow* deletedArrow);
        int deleteUnnecessaryArrows ();
        int moveAllArrows ();
        int draw ();

        //----dump functions----
        int dump ();
        int dumpAllArrows ();
        int calculateNArrows();
};
*/

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "list.h"


const int WINDOW_HEIGHT = 900;
const int WINDOW_LENGHT = 900;

const float MAIN_CHARACTER_SPEED = 10;
const float MAIN_CHARACTER_RADIUS = 75;

const float ARROW_SPEED = 20;
const int MAX_ARROWS = 10;
const float ARROW_LENGHT = 40;

const float ZOMBIE_RADIUS = 50;

const double SQRT2 = sqrt (0.5);
const float DEGREES_IN_RADIAN = 57.2956;
const float PI = 3.1416;
const float EXTENDED_ANGLE = 180;

sf::RenderWindow* window = nullptr;


class gameObject;
class player;
class arrow;
class zombie;
class arrows_list;


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

	friend bool colliderZombieVsArrow(zombie* Zombie, arrow* Arrow);

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

    friend class arrows_list;
};


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

class player : public gameObject
{
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

        int draw();
};



zombie createZombie ();
int game ();
int arrowPhysicsManager (arrows_list* arrowsList, player* mainCharacter);

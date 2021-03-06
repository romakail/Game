
#include "game.h"

int game()
{
    sf::RenderWindow Window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_LENGHT), "SFML works!");
    Window.setFramerateLimit(30);
    window = &Window;

    sf::RectangleShape backGround (sf::Vector2f (WINDOW_HEIGHT, WINDOW_LENGHT));
    backGround.setFillColor(sf::Color::White);

    player mainCharacter (sf::Vector2f(WINDOW_HEIGHT / 2, WINDOW_LENGHT / 2), MAIN_CHARACTER_SPEED, MAIN_CHARACTER_RADIUS);

    //
    arrow theOnlyArrow (sf::Vector2f (0, 0), sf::Vector2f (0, 0));

    list_T<arrow> arrowsList (MAX_ARROWS);
	arrowsList.dumpAllElements();
	arrowsList.dump();
    //

    //
    list_T<zombie> zombieList (MAX_ZOMBIES);
    zombieList.dumpAllElements();
    zombieList.dump();
    //

    //
	coordinateList_T<coin> coinsList (MAX_COINS, COIN_LINES_IN_LIST_X, COIN_LINES_IN_LIST_Y);
    //

    //
    sf::Vector2f Vectors [29] = {sf::Vector2f (100, 100),
    							sf::Vector2f (300, 100),
    							sf::Vector2f (300, 150),
    							sf::Vector2f (425, 150),

    							sf::Vector2f (575, 150),
    							sf::Vector2f (700, 150),
    							sf::Vector2f (700, 100),
    							sf::Vector2f (900, 100),
    							sf::Vector2f (900, 300),
    							sf::Vector2f (850, 300),
    							sf::Vector2f (850, 425),

    							sf::Vector2f (850, 575),
    							sf::Vector2f (850, 700),
    							sf::Vector2f (900, 700),
    							sf::Vector2f (900, 900),
    							sf::Vector2f (700, 900),
    							sf::Vector2f (700, 850),
    							sf::Vector2f (575, 850),

    							sf::Vector2f (425, 850),
    							sf::Vector2f (300, 850),
    							sf::Vector2f (300, 900),
    							sf::Vector2f (100, 900),
    							sf::Vector2f (100, 700),
    							sf::Vector2f (150, 700),
    							sf::Vector2f (150, 575),


    							sf::Vector2f (150, 425),
    							sf::Vector2f (150, 300),
    							sf::Vector2f (100, 300),
    							//sf::Vector2f (100, 500),
    							sf::Vector2f (100, 100)	};


//    							sf::Vector2f (700, 500),

	bool isDoorMassive [28] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};

	castle ziggurat (28, Vectors, isDoorMassive);
    //

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        window->clear();

        //-----moving functions

        mainCharacter.control ();
        mainCharacter.changePosition(&ziggurat);

        //
        //theOnlyArrow.changePosition();
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        //{
        //    theOnlyArrow = mainCharacter.shoot(ARROW_SPEED);
        //}
		/*
        arrowsList.moveAllArrows();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            arrowsList.addArrow(mainCharacter.shoot(ARROW_SPEED));
        }
        arrowsList.deleteUnnecessaryArrows();
		*/
        arrowPhysicsManager(&arrowsList, &mainCharacter);

		//creating zombies
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			zombieList.addElement(createZombie2());
		}

        managerZombiesVsArrows(&zombieList, &arrowsList, &coinsList);

        zombieList.moveAllZombies(&mainCharacter, &ziggurat);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
        	printf ("I am creating a coin\n");
        	printf ("Mouse position: X = %d, Y = %d\n", sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
            coinsList.addElement(coin(sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y), COIN_VALUE, COIN_RADIUS));
        }
        managerPlayerVsCoins(&mainCharacter, &coinsList);
		//

        if (colliderPlayerVsCastle(&mainCharacter, &ziggurat))
			printf (ANSI_COLOR_RED "Player and Castle collided" ANSI_COLOR_RESET "\n");
        //----drawing functions

        window->draw(backGround);
        mainCharacter.draw();

        //
        //theOnlyArrow.draw();
        arrowsList.draw();
        //
		zombieList.draw();

		//printf ("before staring drawing coins\n");
		coinsList.draw();
        //--------------------

        ziggurat.draw();

        //arrowsList.dump();
        printf ("//---------------------------------------//\n");
        window->display();
    }
    printf ("___ZOMBIES___\n");
    zombieList.dump();
    zombieList.dumpAllElements();
    printf ("___ARROWS___\n");
    arrowsList.dump();
    arrowsList.dumpAllElements();
    printf ("___COINS___\n");
    coinsList.dump();
    coinsList.dumpHeadsAndTails();
    printf ("\n\nTHANKS FOR PLAYING\n");

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------

int arrowPhysicsManager (list_T<arrow>* arrowsList, player* mainCharacter)
{
    static int isLeftPressedPrev = 0;
    static int isLeftPressedNow = 0;
    static arrow* prevArrowPtr = NULL;

    isLeftPressedNow = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	arrowsList->moveAllElements();
/*
	if (isLeftPressedNow)
	{
		if (isLeftPressedPrev)
			arrowsList->deleteElement(prevArrowPtr);

		arrow shootedArrow = mainCharacter->shoot(ARROW_SPEED);
		prevArrowPtr = arrowsList->addElement(shootedArrow);
	}
*/
//

	if (isLeftPressedNow)
	{
        if (isLeftPressedPrev)
        {
			*prevArrowPtr = mainCharacter->shoot(ARROW_SPEED);
        }
        else
        {
			arrow shootedArrow = mainCharacter->shoot(ARROW_SPEED);
			prevArrowPtr = arrowsList->addElement(shootedArrow);
        }
	}


//	for shooting when left button is pressed
/*
	if (isLeftPressedNow)
		arrowsList->addElement(mainCharacter->shoot(ARROW_SPEED));
*/
//

	arrowsList->deleteUnnecessaryArrows();

    isLeftPressedPrev = isLeftPressedNow;

	return 0;
}

//===============GAMEOBJECT FUNCTIONS=================================================================================

gameObject::gameObject (sf::Vector2f position, sf::Vector2f speed)
{
    pos = position;
    v = speed;
}

//--------------------------------------------------------------------------------------------------------------------

int gameObject::changePosition ()
{
    //pos.x += v.x;
    //pos.y += v.y;

    pos += v;


    return 0;
}

//--------------------------------------------------------------------------------------------------------------------

sf::Vector2f gameObject::getPosition()
{
    return pos;
}

//===============PLAYER FUNCTIONS=====================================================================================

player::player (sf::Vector2f position, float mainCharacterSpeed, float mainCharacterRadius) : gameObject (position, sf::Vector2f(0, 0))
{
    Vmax = mainCharacterSpeed;
    radius = mainCharacterRadius;

    shape = sf::CircleShape(100.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);

    printf ("I have created a player\n");
}

//------------------------------------------------------------------------------------------------------------------------

int player::control()
{
    char bitDirection = 0;

    bitDirection = bitDirection | sf::Keyboard::isKeyPressed (sf::Keyboard::Right);
    bitDirection = bitDirection << 1;

    bitDirection = bitDirection | sf::Keyboard::isKeyPressed (sf::Keyboard::Left);
    bitDirection = bitDirection << 1;

    bitDirection = bitDirection | sf::Keyboard::isKeyPressed (sf::Keyboard::Up);
    bitDirection = bitDirection << 1;

    bitDirection = bitDirection | sf::Keyboard::isKeyPressed (sf::Keyboard::Down);

    switch (bitDirection)
    {
        case 0: v.x = 0;
                v.y = 0;
        break;

        case 1: v.x = 0;
                v.y = Vmax;
        break;

        case 2: v.x = 0;
                v.y = - Vmax;
        break;

        case 3: v.x = 0;
                v.y = 0;
        break;

        case 4: v.x = - Vmax;
                v.y = 0;
        break;

        case 5: v.x = - Vmax * SQRT2;
                v.y = Vmax * SQRT2;
        break;

        case 6: v.x = - Vmax * SQRT2;
                v.y = - Vmax * SQRT2;
        break;

        case 7: v.x = - Vmax;
                v.y = 0;
        break;

        case 8: v.x = Vmax;
                v.y = 0;
        break;

        case 9: v.x = Vmax * SQRT2;
                v.y = Vmax * SQRT2;
        break;

        case 10: v.x =   Vmax * SQRT2;
                 v.y = - Vmax * SQRT2;
        break;

        case 11: v.x = Vmax;
                 v.y = 0;
        break;

        case 12: v.x = 0;
                 v.y = 0;
        break;

        case 13: v.x = 0;
                 v.y = Vmax;
        break;

        case 14: v.x = 0;
                 v.y = - Vmax;
        break;

        case 15: v.x = 0;
                 v.y = 0;
        break;
    }

    //printf ("Vmax = %lg, ", Vmax);
    //printf ("x = %lg, y = %lg, vx = %lg, vy = %lg, bitDirection = %d\n", pos.x, pos.y, v.x, v.y, bitDirection);
    //printf ("bitDirection = %d\n", bitDirection);

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------

int player::changePosition (castle* castlePtr)
{
    pos += v;

    if (colliderPlayerVsCastle(this, castlePtr))
    	pos -= v;

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------

int player::draw()
{
    shape.setPosition(pos);
    window->draw(shape);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------

arrow player::shoot(float shootSpeed)
{
	//printf ("start of a shoot\n");
    float cursorX = static_cast<float> (sf::Mouse::getPosition(*window).x);
    float cursorY = static_cast<float> (sf::Mouse::getPosition(*window).y);

    float sqrtLenght = sqrt ((cursorX - pos.x) * (cursorX - pos.x) + (cursorY - pos.y) * (cursorY - pos.y));

    float cos = (cursorX - pos.x) / sqrtLenght;
    float sin = (cursorY - pos.y) / sqrtLenght;

    arrow realisedArrow (sf::Vector2f(pos.x + radius * cos, pos.y + radius * sin), sf::Vector2f(shootSpeed * cos, shootSpeed * sin));

    if (cos > 0)
        realisedArrow.shape.setRotation(EXTENDED_ANGLE + asin(sin) * DEGREES_IN_RADIAN);
    else
        realisedArrow.shape.setRotation(- asin(sin) * DEGREES_IN_RADIAN);

	//printf ("end of a shoot\n");
	return realisedArrow;
}

//===============ARROW FUNCTIONS===============================================================================

arrow::arrow (sf::Vector2f position, sf::Vector2f speed) : gameObject (position, speed)
{
    shape = sf::RectangleShape(sf::Vector2f(ARROW_LENGHT, 3));
    shape.setFillColor(sf::Color::Black);

    lenght = ARROW_LENGHT;

    next = NULL;
    prev = NULL;

    shape.setPosition(position);
    //printf ("I have constructed an arrow\n");
}

//---------------------------------------------------------------------------------------------------------------------

arrow::arrow () : gameObject (sf::Vector2f(0, 0), sf::Vector2f(0, 0))
{
    shape =  sf::RectangleShape(sf::Vector2f(ARROW_LENGHT, 3));
    shape.setFillColor(sf::Color::Black);

    lenght = ARROW_LENGHT;

    next = NULL;
    prev = NULL;

    printf ("I have constructed an empty arrow\n");
}

//---------------------------------------------------------------------------------------------------------------------

arrow& arrow::operator= (const arrow& right)
{
	//printf ("I am using operator =\n");
	if (this == &right)
        return *this;

    Vmax = right.Vmax;
	lenght = right.lenght;
    shape = right.shape;

    pos = right.pos;
    v = right.v;

    //this->draw();

	return *this;
}

//------------------------------------------------------------------------------------------------------------------------

int arrow::draw ()
{
	shape.setPosition(pos);
    window->draw(shape);

	return 0;
}

//==========zombies===================================================================================================

zombie::zombie () : gameObject(sf::Vector2f (0, 0), sf::Vector2f(0, 0))
{
    Vmax = 0;
    radius = 0;
    damage = 0;

    shape = sf::CircleShape(100.f);
    shape.setFillColor(sf::Color::Red);  //If something red is drawn it is a mistake
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);

    next = NULL;
    prev = NULL;

    //printf ("I have created an empty zombie\n");
}

//-------------------------------------------------------------------------------------------------------------------------------

zombie::zombie (sf::Vector2f position, float zombieSpeed, float zombieRadius, float zombieDamage) : gameObject(position, sf::Vector2f(0, 0))
{
    Vmax = zombieSpeed;
	radius = zombieRadius;
	damage = zombieDamage;

    shape = sf::CircleShape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);

    next = NULL;
    prev = NULL;

    //printf ("I have created a zombie\n");
}

//------------------------------------------------------------------------------------------------------------------------------

zombie& zombie::operator= (const zombie& right)
{
	//printf ("I am using operator = of zombie\n");
    if (this == &right)
    	return *this;

    Vmax = right.Vmax;
    radius = right.radius;
    damage = right.damage;
    shape = right.shape;

    pos = right.pos;
    v = right.v;

    return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------

int zombie::changePosition (player* mainCharacter, castle* Castle)
{
	float distance = sqrt ((mainCharacter->getPosition().x - pos.x) * (mainCharacter->getPosition().x - pos.x) + (mainCharacter->getPosition().y - pos.y) * (mainCharacter->getPosition().y - pos.y));

	//printf ("Vmax = %lg\n", Vmax);
    //pos.x += (mainCharacter->getPosition().x - pos.x) / distance * Vmax;
    //pos.y += (mainCharacter->getPosition().y - pos.y) / distance * Vmax;
    if (!colliderZombieVsCastle(this, Castle))
    	if (!colliderPlayerVsZombie(mainCharacter, this))
	    	pos += (mainCharacter->getPosition() - pos) / distance * Vmax;

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------

int zombie::draw ()
{
	shape.setPosition(pos);
    window->draw(shape);

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------

zombie createZombie1 ()
{
	static float i = 0;
	i++;

    return zombie (sf::Vector2f(WINDOW_LENGHT/2 + cos (i) * 200, WINDOW_HEIGHT/2 + sin (i) * 200), ZOMBIE_SPEED, ZOMBIE_RADIUS, ZOMBIE_DAMAGE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------

zombie createZombie2 ()
{
    switch (rand() % 4)
	{
		case 0 : return zombie (sf::Vector2f(rand() % 800 + 100,  25), ZOMBIE_SPEED, ZOMBIE_RADIUS, ZOMBIE_DAMAGE);
			break;
		case 1 : return zombie (sf::Vector2f(975, rand() % 800 + 100), ZOMBIE_SPEED, ZOMBIE_RADIUS, ZOMBIE_DAMAGE);
			break;
		case 2 : return zombie (sf::Vector2f(rand() % 800 + 100, 975), ZOMBIE_SPEED, ZOMBIE_RADIUS, ZOMBIE_DAMAGE);
			break;
		case 3 : return zombie (sf::Vector2f(25,  rand() % 800 + 100), ZOMBIE_SPEED, ZOMBIE_RADIUS, ZOMBIE_DAMAGE);
			break;
    }
}


//===============COINS===========================================================

coin::coin () : gameObject(sf::Vector2f(0, 0), sf::Vector2f(0, 0))
{
    value = 0;
    radius = 0;

	shape = sf::CircleShape(100.f);
    shape.setFillColor(sf::Color::Red);  //If something red is drawn it is a mistake
    shape.setRadius(COIN_RADIUS);

    printf ("I have created an empty coin\n");
}

//------------------------------------------------------------------------------------------------------

coin::coin(sf::Vector2f position, int coinValue, float coinRadius) : gameObject (position, sf::Vector2f(0, 0))
{
	value = coinValue;
	radius = coinRadius;

	shape = sf::CircleShape(100.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setRadius(coinRadius);
    shape.setOrigin(coinRadius, coinRadius);

    ("I have created a coin\n");
}

//-----------------------------------------------------------------------------------------------

coin& coin::operator= (const coin& right)
{
	printf ("I am using operator = of a coin\n");
    if (this == &right)
    	return *this;

    value = right.value;
    radius = right.radius;
    shape = right.shape;

    pos = right.pos;
    v = right.v;

    return *this;
}

//------------------------------------------------------------------------------------------------------

int coin::draw ()
{
	shape.setPosition(pos);
    window->draw(shape);

	return 0;
}

//============CASTLE================================================================================

//----------walls-----------------------------------------------------------------------------------

wall::wall (sf::Vector2f End1, sf::Vector2f End2, bool IsDoor, float Width, float hitPoints)
{
	printf ("I am constructing a wall\n");
    assert (Width > 0);

    end1 = End1;
    end2 = End2;
    isDoor = IsDoor;
    isBroken = 0;
	width = Width;
	maxHP = hitPoints;
	curHP = hitPoints;

    lenght = sqrt ((end1.x - end2.x) * (end1.x - end2.x) + (end1.y - end2.y) * (end1.y - end2.y));

    shape = sf::RectangleShape(sf::Vector2f(lenght, width));
    shape.setOrigin(sf::Vector2f(0, width/2));
    shape.setPosition(end1);
    if (isDoor)
    	shape.setFillColor(sf::Color(DOOR_COLOR_R, DOOR_COLOR_G, DOOR_COLOR_B, 255));
	else
        shape.setFillColor(sf::Color::Black);

    sf::Vector2f direction = end2 - end1;

    float cos = direction.x/lenght;
    float sin = direction.y/lenght;

    if (cos > 0)
        shape.setRotation(asin(sin) * DEGREES_IN_RADIAN);
    else
        shape.setRotation(EXTENDED_ANGLE - asin(sin) * DEGREES_IN_RADIAN);


	edge = sf::CircleShape(100.f);
    edge.setFillColor(sf::Color::Black);
    edge.setRadius(EDGE_WIDTH/2);
    edge.setOrigin(EDGE_WIDTH/2, EDGE_WIDTH/2);
    edge.setPosition(end1);
}

//------------------------------------------------------------------------------------------------------

wall::~wall()
{
	printf ("I am destructing a wall");
}

//------------------------------------------------------------------------------------------------------

bool wall::isBrokenIndicator()
{
	return isBroken;
}

//------------------------------------------------------------------------------------------------------

int wall::draw ()
{
    //const_cast<sf::Color&> (shape.getFillColor()).a = 255 * curHP / maxHP;		// can I do that and will it work or a copy is made somewhere


    window->draw(shape);
	window->draw(edge);
    return 0;
}

//-----------------------------------------------------------------------------------------------------

int wall::getDamaged (float damage)
{
    curHP -= damage;
	printf (ANSI_COLOR_RED "I am damaging a wall" ANSI_COLOR_RESET);

    if (curHP < 0)
    {
        printf ("----I am broken((\n");
        isBroken = 1;
	}
	else
	{
        printf ("----I am getting damaged\n");
		if (isDoor)
    		shape.setFillColor(sf::Color(DOOR_COLOR_R, DOOR_COLOR_G, DOOR_COLOR_B, 255 * curHP / maxHP));
		else
     	   shape.setFillColor(sf::Color(WALL_COLOR_R, WALL_COLOR_R, WALL_COLOR_R, 255 * curHP / maxHP));
	}

    return 0;
}

//---------------castle--------------------------------------------------------------------------------

//castle        (int anglesNumber, sf::Vector2f* anglesCoordinates);
castle::castle(int anglesNumber, sf::Vector2f* anglesCoordinates, bool* isDoors)
{
    assert (anglesNumber >= 3);
    assert (anglesCoordinates[0] == anglesCoordinates[anglesNumber]);

    nAngles = anglesNumber;

    void* rawMemory = operator new[] ((nAngles + 1) * sizeof(sf::Vector2f));//sf::Vector2f [nAngles + 1];
    anglesMassive = static_cast<sf::Vector2f*> (rawMemory);

    for (int i = 0; i < nAngles + 1; i++)
    {
        //anglesMassive[i] = anglesCoordinates[i]
        new (&(anglesMassive [i])) sf::Vector2f(anglesCoordinates [i]);
	}
    assert (anglesMassive[0] == anglesMassive[nAngles]);

    //shapesMassive = NULL;
	/*
    void* rawMemory = operator new [] (nAngles * sizeof(sf::RectangleShape));
    shapesMassive = static_cast<sf::RectangleShape*> (rawMemory);

    for (int i = 0; i < nAngles; i++)
    {
        new (&(shapesMassive [i])) sf::RectangleShape()
    }
	*/
	/*
	walls.setPointCount(nAngles);
    walls.setOutlineColor(sf::Color::Black);
    walls.setOutlineThickness(5.0);
    walls.setFillColor(sf::Color::Transparent);

    for (int i = 0; i < nAngles; i++)
    {
        walls.setPoint(i, anglesCoordinates[i]);
    }
    */

    rawMemory = operator new[] ((nAngles) * sizeof(wall));
	walls = static_cast<wall*> (rawMemory);

	for (int i = 0; i < nAngles; i++)
    {
        //anglesMassive[i] = anglesCoordinates[i]
        new (&(walls [i])) wall(anglesCoordinates [i], anglesCoordinates [i + 1], isDoors [i], WALL_WIDTH, DOOR_HITPOINTS);
	}


}

//-----------------------------------------------------------------------------------------------------------------------

castle::~castle()
{
	/*
    for (int i = nAngles; i >= 0; --i)
    {
        anglesMassive[i].sf::~Vector2f();
    }
    */
    operator delete [] (anglesMassive);

    for (int i = nAngles; i >= 0; --i)
    {
        walls[i].~wall();
    }
    operator delete [] (walls);
}

//-----------------------------------------------------------------------------------------------------------------------

int castle::draw ()
{
	//window->draw (walls);
    for (int i = 0; i < nAngles; i++)
        walls[i].draw();
	return 0;
}


//============COLLIDERЫ=============================================================================
// 1 (true)  - collided
// 0 (false) - not collided

bool colliderZombieVsArrow (zombie* Zombie, arrow* Arrow)
{
    assert (Zombie);
    assert (Arrow);
	assert (Zombie->radius > 0);
    assert (Arrow->lenght > 0);

    if (  ((Zombie->pos.x - Arrow->pos.x) * (Zombie->pos.x - Arrow->pos.x) + (Zombie->pos.y - Arrow->pos.y) * (Zombie->pos.y - Arrow->pos.y)) < Zombie->radius * Zombie->radius)
    	return 1;
	else
		return 0;


	assert (0);
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------

bool colliderPlayerVsCoin (player* Player, coin* Coin)
{
    assert (Player);
    assert (Coin);
    assert (Player->radius > 0);
    assert (Coin->radius > 0);

    if ( ((Player->pos.x - Coin->pos.x) * (Player->pos.x - Coin->pos.x) + (Player->pos.y - Coin->pos.y) * (Player->pos.y - Coin->pos.y)) < (Player->radius + Coin->radius) * (Player->radius + Coin->radius))
    	return 1;
	else
        return 0;

	assert (0);
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------

bool colliderPlayerVsZombie (player* Player, zombie* Zombie)
{
    assert (Player);
    assert (Zombie);
    assert (Player->radius > 0);
    assert (Zombie->radius > 0);

    if ( ((Player->pos.x - Zombie->pos.x) * (Player->pos.x - Zombie->pos.x) + (Player->pos.y - Zombie->pos.y) * (Player->pos.y - Zombie->pos.y)) < (Player->radius + Zombie->radius) * (Player->radius + Zombie->radius))
    	return 1;
	else
        return 0;

	assert (0);
	return 0;
}

//---------------------------------------------------------------------------------------------------------------

bool colliderPlayerVsCastle (player* Player, castle* Castle)
{
    assert (Player);
    assert (Castle);
    assert (Player->radius > 0);

    for (int i = 0; i < Castle->nAngles; i++)
    {
        if (((colliderCircleVsDot (Player->pos, Player->radius, Castle->anglesMassive[i])) ||
			 (colliderCircleVsLine(Player->pos, Player->radius, Castle->anglesMassive[i], Castle->anglesMassive[i + 1]))) &&
			 (!Castle->walls[i].isBrokenIndicator()))

            return 1;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------

bool colliderZombieVsCastle (zombie* Zombie, castle* Castle)
{
    assert (Zombie);
    assert (Castle);
    assert (Zombie->radius > 0);

    printf (ANSI_COLOR_RED "I am colliding zombie & castle" ANSI_COLOR_RESET "\n");
    for (int i = 0; i < Castle->nAngles; i++)
    {
        if (((colliderCircleVsDot (Zombie->pos, Zombie->radius, Castle->anglesMassive[i])) ||
			 (colliderCircleVsLine(Zombie->pos, Zombie->radius, Castle->anglesMassive[i], Castle->anglesMassive[i + 1]))) &&
			 (!Castle->walls[i].isBrokenIndicator()))

		{
			printf ( ANSI_COLOR_RED "Zombie collided with a wall" ANSI_COLOR_RESET "\n");
			//Castle->walls[i].curHP -= Zombie->damage;
			Castle->walls[i].getDamaged(Zombie->damage);
			return 1;
		}
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------

bool colliderCircleVsLine (sf::Vector2f centre, float radius, sf::Vector2f dot1, sf::Vector2f dot2)
{
    assert (radius > 0);
    assert (dot1 != dot2);

    float lenght = sqrt ( (dot1.x - dot2.x) * (dot1.x - dot2.x) + (dot1.y - dot2.y) * (dot1.y - dot2.y) );

	sf::Vector2f newAbscissa = dot2 - dot1;
    sf::Vector2f newCentre = centre - dot1;

    float cosA = newAbscissa.x / lenght;
    float sinA = newAbscissa.y / lenght;

    sf::Vector2f newnewCentre (newCentre.x * cosA + newCentre.y * sinA, -newCentre.x * sinA + newCentre.y * cosA);

    if ((newnewCentre.x > 0) &&
    	(newnewCentre.x < lenght) &&
    	(newnewCentre.y < radius) &&
    	(newnewCentre.y > -radius) )
			return 1;
	else
			return 0;
}

//----------------------------------------------------------------------------------------------------------------

bool colliderCircleVsDot (sf::Vector2f centre, float radius, sf::Vector2f dot)
{
    assert (radius > 0);
    if (((centre.x - dot.x) * (centre.x - dot.x) + (centre.y - dot.y) * (centre.y - dot.y)) < radius * radius)
    	return 1;
	else
        return 0;
}

//================MANAGMENT======================================================================================

//returns number of collisions
int managerZombiesVsArrows (list_T<zombie>* zombieList, list_T<arrow>* arrowsList, coordinateList_T<coin>* coinsList)
{
	assert (zombieList);
	assert (arrowsList);

	//printf ("I am in manager\n");

    int nCollisions = 0;

    arrow*  collidedArrow  = arrowsList->head;
	zombie* collidedZombie = zombieList->head;

    while (collidedArrow != NULL)
    {
		collidedZombie = zombieList->head;
		while (collidedZombie != NULL)
		{
            if (colliderZombieVsArrow(collidedZombie, collidedArrow))
            {
                coinsList->addZombieDrop(collidedZombie);
                arrowsList->deleteElement(collidedArrow);
                zombieList->deleteElement(collidedZombie);
                nCollisions++;
            }
			collidedZombie = collidedZombie->next;
		}
        collidedArrow = collidedArrow->next;
    }

    //printf ("nCollisions = %d\n", nCollisions);
    //printf ("I return from manager\n");
	return nCollisions;
}

//-------------------------------------------------------------------------------------------------------------------------

//returns number of collected coins
int managerPlayerVsCoins (player* collector, coordinateList_T<coin>* coinsList)
{
	int nCollectedCoins = 0;

	int xLine = collector->pos.x / WINDOW_LENGHT * coinsList->nLinesX;
	int yLine = collector->pos.y / WINDOW_HEIGHT * coinsList->nLinesY;

    int listIndex = yLine * coinsList->nLinesX + xLine;

	if		(listIndex % coinsList->nLinesX == 0)
	{
        if (listIndex == 0)
        {
            nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
            nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + 1);
            nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX);
            nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX + 1);
        }
        else if (listIndex == coinsList->nLinesX * (coinsList->nLinesY - 1))
        {
        	nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX + 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + 1);
        }
        else
        {
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX + 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX);
            nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX + 1);
        }
	}
	else if ((listIndex + 1) % coinsList->nLinesX == 0)
	{
        if		(listIndex == coinsList->nLinesX - 1)
        {
        	nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX);
        }
        else if (listIndex == coinsList->nLinesX * coinsList->nLinesY - 1)
        {
        	nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX - 1);
        	nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
        }
        else
        {
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX - 1);
        	nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
        	nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX);
        }
	}
	else
	{
		if		((listIndex > 0) && (listIndex < coinsList->nLinesX - 1))
		{
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX + 1);
		}
		else if ((listIndex > coinsList->nLinesX * (coinsList->nLinesY - 1)) && (listIndex < coinsList->nLinesX * coinsList->nLinesY - 1))
		{
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX + 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + 1);
		}
		else
		{
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - coinsList->nLinesX + 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX - 1);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX);
			nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex + coinsList->nLinesX + 1);
		}
	}

    nCollectedCoins += collectCoins1x1(collector, coinsList, listIndex);

	printf ("Collected coins = %d\n", nCollectedCoins);
	return nCollectedCoins;
}

//----------------------------------------------------------------------------------------------

int collectCoins1x1 (player* collector, coordinateList_T<coin>* coinsList, int listIndex)
{
	int nCollectedCoins = 0;
	coin* collidedCoin = NULL;

	if (coinsList->headsMassive[listIndex] != 0)
	{
        collidedCoin = coinsList->headsMassive[listIndex];
        while (collidedCoin != NULL)
        {
            if (colliderPlayerVsCoin(collector, collidedCoin))
            {
                coinsList->deleteElement(collidedCoin);
                nCollectedCoins++;
			}
            collidedCoin = collidedCoin->next;
        }
	}

	printf ("Collected coins = %d\n", nCollectedCoins);
	return nCollectedCoins;
}

//-------------------------------------------------------------------------------------------------------




































/*
sf::Vector2f zombie::getPosition ()
{
    return pos;
}
*/
//-----------------------------------------------------------------------------------------------------------------------------




/*
int arrowPhysicsManager (list_T<arrow>* arrowsList, player* mainCharacter)
{
    static int isLeftPressedPrev = 0;
    static int isLeftPressedNow = 0;
    static arrow* prevArrowPtr = NULL;

    isLeftPressedNow = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	arrowsList->moveAllElements();

	if (isLeftPressedNow)
	{
		if (isLeftPressedPrev)
			arrowsList->deleteElement(prevArrowPtr);

		arrow shootedArrow = mainCharacter->shoot(ARROW_SPEED);
		prevArrowPtr = arrowsList->addElement(shootedArrow);
	}

//	for shooting when left button is pressed

//	if (isLeftPressedNow)
//		arrowsList->addElement(mainCharacter->shoot(ARROW_SPEED));


	arrowsList->deleteUnnecessaryArrows();

    isLeftPressedPrev = isLeftPressedNow;

	return 0;
}
*/











/*
int gameObject::changeLocation()
{
    double sqrtV = v * sqrt (0.5);

    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right))
    {
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))
        {
            x += sqrtV;
            y -= sqrtV;
        }
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))
        {
            x += sqrtV;
            y += sqrtV;
        }
        else
        {
            x = x + v;
        }
    }

    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left))
    {

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))
        {
            x -= sqrtV;
            y -= sqrtV;
        }
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))
        {
            x -= sqrtV;
            y += sqrtV;
        }
        else
        {
            x -= v;
        }
    }
    printf ("x = %lg  y = %lg\n", x, y);

    shape.setPosition(x, y);
    return 0;
}
*/




/*
arrows_list::arrows_list (int maxArrowsNumber)
{
    assert (maxArrowsNumber > 0);
    maxArrows = maxArrowsNumber;

    arrowsMassive = new arrow [maxArrowsNumber] ;

    head = NULL;
    tail = NULL;

    freeHead = arrowsMassive;
    freeTail = & (arrowsMassive [maxArrowsNumber - 1]);

    freeHead->prev = NULL;
    freeTail->next = NULL;

    arrow* arrowPtr = freeHead;
    for(int i = 0; i < (maxArrowsNumber - 1); i++)
    {
        arrowPtr->next = &(arrowPtr[1]);
        arrowPtr->next->prev = arrowPtr;
        arrowPtr = arrowPtr->next;
    }


}

//---------------------------------------------------------------------------------------------------------------------------

arrows_list::~arrows_list ()
{
     delete arrowsMassive;
}

//--------------------------------------------------------------------------------------------------------------------------

arrow* arrows_list::addArrow (arrow newArrow)
{
    printf ("I have started adding an arrow\n");

    if (nArrows == 0)
    {
    	printf ("case nArrows == 0\n");
        assert (head == NULL);
        assert (tail == NULL);

        nArrows++;

        arrow* emptySpace = freeHead;

        freeHead->next->prev = NULL;
        freeHead = emptySpace->next;

        *emptySpace = newArrow;

        emptySpace->next = tail;
        emptySpace->prev = head;

        head = emptySpace;
        tail = emptySpace;

        return emptySpace;
    }
    else if ((nArrows > 0) && (nArrows < maxArrows))
    {
    	printf ("case 0 < nArrows < max\n");
        nArrows++;

		assert (freeHead->prev == NULL);

        arrow* emptySpace = freeHead;

		if (freeHead->next != NULL)
		{
			freeHead->next->prev = NULL;
			freeHead = emptySpace->next;
		}
		else if (freeHead->next == NULL)
		{
			assert (freeHead == freeTail);
            freeHead = NULL;
            freeTail = NULL;
		}
		printf ("___1___");
        *emptySpace = newArrow;

        assert (head->prev == NULL);

        head->prev = emptySpace;
        emptySpace->next = head;
        head = emptySpace;

        return emptySpace;
    }
    else if (nArrows == maxArrows)
    {
    	printf ("case nArrows == max\n");
        printf ("I cannot add anymore arrows\n");
        return 0;
    }
    else
    	assert (0);

    //this->dumpAllArrows();
    //this->dump();

    return NULL;
}

//-------------------------------------------------------------------------------------------------------------------

int arrows_list::deleteArrow (arrow* deletedArrow)
{
	printf ("I have started deleting a cerain arrow\n");
    assert (deletedArrow);
    assert (nArrows > 0);
    assert ((deletedArrow >= &deletedArrow[0]) && (deletedArrow <= &deletedArrow[maxArrows]));

    if      ((deletedArrow->next != NULL) && (deletedArrow->prev != NULL))
    {
        nArrows--;

        deletedArrow->prev->next = deletedArrow->next;
        deletedArrow->next->prev = deletedArrow->prev;
    }
    else if ((deletedArrow->next == NULL) && (deletedArrow->prev != NULL))
    {
    	nArrows--;

        deletedArrow->prev->next = NULL;
        tail = deletedArrow->prev;
    }
    else if ((deletedArrow->next != NULL) && (deletedArrow->prev == NULL))
    {
    	nArrows--;

        deletedArrow->next->prev = NULL;
        head = deletedArrow->next;
    }
    else if ((deletedArrow->next == NULL) && (deletedArrow->prev == NULL))
    {
    	nArrows--;

        head = deletedArrow->next;
        tail = deletedArrow->prev;
	}
	else
	{
		assert (0);
	}

	printf ("---1---\n");
	if      (freeTail != NULL)
	{
		printf ("case freeTail != NULL\n");
		//this->dumpAllArrows();
		//this->dump();
		//printf ("freeTail = %p\n", freeTail);
		//printf ("freeTail->next = %p\n", freeTail->next);
		assert (freeTail->next == NULL);

	    freeTail->next = deletedArrow;
        deletedArrow->prev = freeTail;

        deletedArrow->next = NULL;
        freeTail = deletedArrow;
	}
	else if (freeTail == NULL)
	{
		printf ("case freeTail == NULL");
        assert (freeHead == NULL);
		//assert (freeTail->next == NULL);

        freeHead = deletedArrow;
        freeTail = deletedArrow;

		deletedArrow->next = NULL;
		deletedArrow->prev = NULL;
	}
	else
	{
		printf ("whaaaaat?\n");
	}

	printf ("I have finished deleting a cerain arrow\n");
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int arrows_list::deleteUnnecessaryArrows ()
{
	printf ("I have stared deleting\n");
	int nDeletedArrows = 0;

	if ((head != NULL) && (tail != NULL))
	{
    	arrow* checkedArrow = head;

        do
        {
            if 	((checkedArrow->pos.x < 0 - 10) ||
            	 (checkedArrow->pos.y < 0 - 10) ||
				 (checkedArrow->pos.x > WINDOW_LENGHT + 10) ||
				 (checkedArrow->pos.y > WINDOW_HEIGHT + 10)
				)
			{
                this->deleteArrow(checkedArrow);
				nDeletedArrows++;
			}
			checkedArrow = checkedArrow->next;

        } while(checkedArrow != NULL);
    }

	printf ("I have finished deleting\n");
	return nDeletedArrows;
}

//---------------------------------------------------------------------------------------------------------------------------------

int arrows_list::moveAllArrows ()
{
	printf ("I have started moving\n");
    if ((head != NULL) && (tail != NULL))
    {
    	arrow* movedArrow = head;
    	while (movedArrow != NULL)
    	{
			movedArrow->changePosition();
            movedArrow = movedArrow->next;
    	}
    }

    printf ("I have finished moving\n");
	return 0;
}


//---------------------------------------------------------------------------------------------------------------------------------

int arrows_list::draw()
{
	printf ("I have started drawing\n");
	if ((head != NULL) && (tail != NULL))
	{
		arrow* drawedArrow = head;
        while (drawedArrow != NULL)
        {
            drawedArrow->draw ();
            drawedArrow = drawedArrow->next;
        }
	}

	printf ("I have finished drawing\n");
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------

int arrows_list::dump()
{
    printf ("maxArrows = %d, nArrows = %d\n", maxArrows, nArrows);
    printf ("calculatednArrows = %d\n", this->calculateNArrows());
    printf ("head =     %p\n", head);
    printf ("tail =     %p\n", tail);
    printf ("freeHead = %p\n", freeHead);
    printf ("freeTail = %p\n", freeTail);
    if (freeTail != NULL)
    	printf ("freeTail->next = %p\n", freeTail->next);
    //printf ("//------------------------------------------//\n");
    //printf ("")

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------

int arrows_list::calculateNArrows ()
{
    int nArrows = 0;
    arrow* checkedArrow = head;

    if (head != NULL)
    {
        while (checkedArrow != NULL)
        {
            nArrows++;
            checkedArrow = checkedArrow->next;
		}
    }
    printf ("ActivatedArrows = %d\n", nArrows);

    int nFreeArrows = 0;
    if (freeHead != NULL)
    {
    	checkedArrow = freeHead;
        while (checkedArrow != NULL)
        {
            nArrows++;
            nFreeArrows++;
            checkedArrow = checkedArrow->next;
        }
    }
    printf ("FreeArrows = %d\n", nFreeArrows);
    return nArrows;
}

//---------------------------------------------------------------------------------------------------

int arrows_list::dumpAllArrows()
{
    printf ("ACTIVE ARROWS:\n");

    arrow* dumpedArrow = head;
    while (dumpedArrow != NULL)
    {
        printf ("prev = %p\n", dumpedArrow->prev);
		printf ("this = %p\n", dumpedArrow);
		printf ("next = %p\n\n", dumpedArrow->next);
		dumpedArrow = dumpedArrow->next;
    }
	printf ("FREE ARROWS:\n");
	dumpedArrow = freeHead;

	while (dumpedArrow != NULL)
	{
    	printf ("prev = %p\n", dumpedArrow->prev);
		printf ("this = %p\n", dumpedArrow);
		printf ("next = %p\n\n", dumpedArrow->next);
		dumpedArrow = dumpedArrow->next;
	}
	printf ("================================================================\n");
	return 0;
}
*/


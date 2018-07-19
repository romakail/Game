
#include "game.h"




int game()
{
    sf::RenderWindow Window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_LENGHT), "SFML works!");
    Window.setFramerateLimit(30);
    window = &Window;

    sf::RectangleShape backGround (sf::Vector2f (WINDOW_HEIGHT, WINDOW_LENGHT));
    backGround.setFillColor(sf::Color::White);

    player mainCharacter (sf::Vector2f(10, 10), MAIN_CHARACTER_SPEED, MAIN_CHARACTER_RADIUS);

    //
    arrow theOnlyArrow (sf::Vector2f (0, 0), sf::Vector2f (0, 0));

    arrows_list arrowsList (MAX_ARROWS);
	arrowsList.dumpAllArrows();
	arrowsList.dump();
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
        mainCharacter.changePosition();

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

        //

        //----drawing functions

        window->draw(backGround);
        mainCharacter.draw();

        //
        //theOnlyArrow.draw();
        arrowsList.draw();
        //


        //--------------------

        arrowsList.dump();
        printf ("//---------------------------------------//\n");
        window->display();
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------

int arrowPhysicsManager (arrows_list* arrowsList, player* mainCharacter)
{
    static int isLeftPressedPrev = 0;
    static int isLeftPressedNow = 0;
    static arrow* prevArrowPtr = NULL;

    isLeftPressedNow = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	arrowsList->moveAllArrows();

	if (isLeftPressedNow)
	{
		if (prevArrowPtr != NULL)
			if (isLeftPressedPrev)
				arrowsList->deleteArrow(prevArrowPtr);

		arrow shootedArrow = mainCharacter->shoot(ARROW_SPEED);
		prevArrowPtr = arrowsList->addArrow(shootedArrow);
	}
	else
	{
        //if (isLeftPressedPrev)
		//	arrowsList->deleteArrow(prevArrowPtr);
	}

/*
	if ((!isLeftPressedNow) && (isLeftPressedPrev))
		arrowsList->addArrow(mainCharacter->shoot(ARROW_SPEED));
*/

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


int gameObject::changePosition ()
{
    pos.x += v.x;
    pos.y += v.y;

    return 0;
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

int player::draw()
{
    shape.setPosition(pos);
    window->draw(shape);

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------

arrow player::shoot(float shootSpeed)
{
	printf ("start of a shoot\n");
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

	printf ("end of a shoot\n");
	return realisedArrow;

    //arrow realeasedArrow  = arrow (pos, 0);
    //return theOnlyArrow;
}


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



//===============ARROW FUNCTIONS===============================================================================

arrow::arrow (sf::Vector2f position, sf::Vector2f speed) : gameObject (position, speed)
{
    shape = sf::RectangleShape(sf::Vector2f(40, 3));
    shape.setFillColor(sf::Color::Black);

    shape.setPosition(position);
    printf ("I have constructed an arrow\n");
}

//---------------------------------------------------------------------------------------------------------------------

arrow::arrow () : gameObject (sf::Vector2f(0, 0), sf::Vector2f(0, 0))
{
    shape =  sf::RectangleShape(sf::Vector2f(40, 3));
    shape.setFillColor(sf::Color::Black);

    printf ("I have constructed an empty arrow\n");
}

//---------------------------------------------------------------------------------------------------------------------

arrow& arrow::operator= (const arrow& right)
{
	printf ("I am using operator =\n");
	if (this == &right)
        return *this;

    Vmax = right.Vmax;
	radius = right.radius;
    shape = right.shape;

    pos = right.pos;
    v = right.v;

    this->draw();

	return *this;
}

//------------------------------------------------------------------------------------------------------------------------

int arrow::draw ()
{
	shape.setPosition(pos);
    window->draw(shape);

	return 0;
}

//------------------------------------------------------------------------------------------------------------------------

arrows_list::arrows_list (int maxArrowsNumber)
{
    assert (maxArrowsNumber > 0);
    maxArrows = maxArrowsNumber;

    //arrowsMassive = static_cast<arrow*> (calloc (maxArrowsNumber, sizeof(arrow)));
	arrowsMassive = new arrow [maxArrowsNumber] ;

    head = NULL;
    tail = NULL;

    freeHead = arrowsMassive;
    freeTail = & (arrowsMassive [maxArrowsNumber-1]);

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
    	/*
    	do
    	{
            movedArrow->changePosition();
            movedArrow = movedArrow->next;
    	} while (movedArrow->next != NULL);
    	*/
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
        /*
        do
        {
            drawedArrow->draw();
            drawedArrow = drawedArrow->next;
        } while (drawedArrow->next != NULL);
        */
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

//-----------------------------------------------------------------------------------------------------------------------------------










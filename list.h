//class zombie;
//class arrow;

template <class gObject>
class list_T
{
	friend int managerZombiesVsArrows (list_T<zombie>* zombieList, list_T<arrow>* arrowsList, coordinateList_T<coin>* coinsList);

	private:
		int maxElements;
		int nElements;

		gObject* head;
		gObject* tail;
		gObject* freeHead;
		gObject* freeTail;
		gObject* elementsMassive;

    public:
    	//Fundamental functions
        explicit list_T (int maxElementsNumber);
        ~list_T ();
        gObject* addElement (gObject newElement);
        int deleteElement (gObject* deletedElement);

		//Draw & move
        int draw ();
		int moveAllElements();

		//For dump:
		int dump ();
		int dumpAllElements();
		int calculateNElements();

		//For arrows:
		int deleteUnnecessaryArrows();
};


template <class gObject>
list_T<gObject>::list_T (int maxElementsNumber)
{
    assert (maxElementsNumber > 0);
    maxElements = maxElementsNumber;
    nElements = 0;

    elementsMassive = new gObject [maxElementsNumber];

    head = NULL;
    tail = NULL;

    freeHead = elementsMassive;
    freeTail = & (elementsMassive [maxElementsNumber - 1]);

    gObject* elemPtr = freeHead;
    for (int i = 0; i < (maxElementsNumber - 1); i++)
    {
        elemPtr->next = &(elemPtr[1]);
        elemPtr->next->prev = elemPtr;
        elemPtr = elemPtr->next;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------

template <class gObject>
list_T<gObject>::~list_T ()
{
	delete elementsMassive;
}

//---------------------------------------------------------------------------------------------------------------------------------

template <class gObject>
gObject* list_T<gObject>::addElement (gObject newElement)
{
    //printf ("I have started adding an element\n");

    if (nElements == 0)
    {
    	//printf ("case nElements == 0\n");
        assert (head == NULL);
        assert (tail == NULL);

        nElements++;

        gObject* emptySpace = freeHead;

        freeHead->next->prev = NULL;
        freeHead = emptySpace->next;

        *emptySpace = newElement;

        emptySpace->next = tail;
        emptySpace->prev = head;

        head = emptySpace;
        tail = emptySpace;

        return emptySpace;
    }
    else if ((nElements > 0) && (nElements < maxElements))
    {
    	//printf ("case 0 < nElements < max\n");
        nElements++;

		assert (freeHead->prev == NULL);

        gObject* emptySpace = freeHead;

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
		//printf ("___1___");
        *emptySpace = newElement;

        assert (head->prev == NULL);

        head->prev = emptySpace;
        emptySpace->next = head;
        head = emptySpace;

        return emptySpace;
    }
    else if (nElements == maxElements)
    {
    	assert (freeHead == NULL);
    	assert (freeTail == NULL);

    	printf ("case nElements == max\n");
        printf ("I cannot add anymore Elements\n");
        return 0;
    }
    else
    	assert (0);

    //this->dumpAllArrows();
    //this->dump();

    return NULL;
}

//-----------------------------------------------------------------------------------------------------------------

template <class gObject>
int list_T<gObject>::deleteElement (gObject* deletedElement)
{
	//printf ("I have started deleting a certain gObject from a list\n");
    assert (deletedElement);
    assert (nElements > 0);
    assert ((deletedElement >= &elementsMassive[0]) && (deletedElement <= &elementsMassive[maxElements]));

    if      ((deletedElement->next != NULL) && (deletedElement->prev != NULL))
    {
        nElements--;

        deletedElement->prev->next = deletedElement->next;
        deletedElement->next->prev = deletedElement->prev;
    }
    else if ((deletedElement->next == NULL) && (deletedElement->prev != NULL))
    {
    	nElements--;

        deletedElement->prev->next = NULL;
        tail = deletedElement->prev;
    }
    else if ((deletedElement->next != NULL) && (deletedElement->prev == NULL))
    {
    	nElements--;

        deletedElement->next->prev = NULL;
        head = deletedElement->next;
    }
    else if ((deletedElement->next == NULL) && (deletedElement->prev == NULL))
    {
    	nElements--;

        head = deletedElement->next;
        tail = deletedElement->prev;
	}
	else
	{
		assert (0);
	}




	if      (freeTail != NULL)
	{
		//printf ("case freeTail != NULL\n");
		assert (freeTail->next == NULL);

	    freeTail->next = deletedElement;
        deletedElement->prev = freeTail;

        deletedElement->next = NULL;
        freeTail = deletedElement;
	}
	else if (freeTail == NULL)
	{
		//printf ("case freeTail == NULL");
        assert (freeHead == NULL);

        freeHead = deletedElement;
        freeTail = deletedElement;

		deletedElement->next = NULL;
		deletedElement->prev = NULL;
	}
	else
	{
		printf ("whaaaaat?\n");
	}

	//printf ("I have finished deleting a cerain gObject\n");
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------

template <class gObject>
int list_T<gObject>::draw()
{
	//printf ("I have started drawing all gObjects of list\n");
	if ((head != NULL) && (tail != NULL))
	{
		gObject* drawedElement = head;
        while (drawedElement != NULL)
        {
            drawedElement->draw ();
            drawedElement = drawedElement->next;
        }
	}

	//printf ("I have finished drawing all gObjects of list\n");
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------

template <class gObject>
int list_T<gObject>::dump()
{
    printf ("maxElements = %d, nElements = %d\n", maxElements, nElements);
    printf ("calculatednElements = %d\n", this->calculateNElements());
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

template <class gObject>
int list_T<gObject>::calculateNElements ()
{
    int nElems = 0;
    gObject* checkedElement = head;

    if (head != NULL)
    {
        while (checkedElement != NULL)
        {
            nElems++;
            checkedElement = checkedElement->next;
		}
    }
    printf ("ActivatedElements = %d\n", nElems);

    int nFreeElems = 0;
    if (freeHead != NULL)
    {
    	checkedElement = freeHead;
        while (checkedElement != NULL)
        {
            nElems++;
            nFreeElems++;
            checkedElement = checkedElement->next;
        }
    }
    printf ("FreeElements = %d\n", nFreeElems);
    return nElems;
}

//---------------------------------------------------------------------------------------------------

template <class gObject>
int list_T<gObject>::dumpAllElements()
{
    printf ("ACTIVE ELEMENTS:\n");

    gObject* dumpedElement = head;
    while (dumpedElement != NULL)
    {
        printf ("prev = %p\n", dumpedElement->prev);
		printf ("this = %p\n", dumpedElement);
		printf ("next = %p\n\n", dumpedElement->next);
		dumpedElement = dumpedElement->next;
    }
	printf ("FREE ELEMENTS:\n");
	dumpedElement = freeHead;

	while (dumpedElement != NULL)
	{
    	printf ("prev = %p\n", dumpedElement->prev);
		printf ("this = %p\n", dumpedElement);
		printf ("next = %p\n\n", dumpedElement->next);
		dumpedElement = dumpedElement->next;
	}
	printf ("================================================================\n");
	return 0;
}

//------------------------------------------------------------------------------

template <class gObject>
int list_T<gObject>::moveAllElements ()
{
	//printf ("I have started moving\n");
    if ((head != NULL) && (tail != NULL))
    {
    	gObject* movedElement = head;
    	while (movedElement != NULL)
    	{
			movedElement->changePosition();
            movedElement = movedElement->next;
    	}
    }

    //printf ("I have finished moving\n");
	return 0;
}

//------------------------------------------------------------------------------

template<>
int list_T<arrow>::deleteUnnecessaryArrows ()
{
	//printf ("I have stared deleting\n");
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
                this->deleteElement(checkedArrow);
				nDeletedArrows++;
			}
			checkedArrow = checkedArrow->next;

        } while(checkedArrow != NULL);
    }

	//printf ("I have finished deleting\n");
	return nDeletedArrows;
}

//--------------------------------------------------------------------------------------------
























































/*
template<>
int list_T<zombie>::printfSmth ()
{
    printf ("cheeeeeeee!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
    return 0;
}
*/

class zombie;

template <class gObject>
class list_T
{
	private:
		int maxElements;
		int nElements;

		gObject* head;
		gObject* tail;
		gObject* freeHead;
		gObject* freeTail;
		gObject* elementsMassive;

    public:
        list_T (int maxElementsNumber);
        ~list_T ();
        gObject* addElement (gObject newElement);
        int deleteElement (gObject* deletedElement);

        int draw ();

        int printfSmth();
};


template <class gObject>
list_T<gObject>::list_T (int maxElementsNumber)
{
    assert (maxElementsNumber > 0);
    maxElements = maxElementsNumber;

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
    printf ("I have started adding an element\n");

    if (nElements == 0)
    {
    	printf ("case nElements == 0\n");
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
    	printf ("case 0 < nElements < max\n");
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
		printf ("___1___");
        *emptySpace = newElement;

        assert (head->prev == NULL);

        head->prev = emptySpace;
        emptySpace->next = head;
        head = emptySpace;

        return emptySpace;
    }
    else if (nElements == maxElements)
    {
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
	printf ("I have started deleting a certain gObject\n");
    assert (deletedElement);
    assert (nElements > 0);
    assert ((deletedElement >= &deletedElement[0]) && (deletedElement <= &deletedElement[maxElements]));

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

	printf ("---1---\n");
	if      (freeTail != NULL)
	{
		printf ("case freeTail != NULL\n");
		//this->dumpAllArrows();
		//this->dump();
		//printf ("freeTail = %p\n", freeTail);
		//printf ("freeTail->next = %p\n", freeTail->next);
		assert (freeTail->next == NULL);

	    freeTail->next = deletedElement;
        deletedElement->prev = freeTail;

        deletedElement->next = NULL;
        freeTail = deletedElement;
	}
	else if (freeTail == NULL)
	{
		printf ("case freeTail == NULL");
        assert (freeHead == NULL);
		//assert (freeTail->next == NULL);

        freeHead = deletedElement;
        freeTail = deletedElement;

		deletedElement->next = NULL;
		deletedElement->prev = NULL;
	}
	else
	{
		printf ("whaaaaat?\n");
	}

	printf ("I have finished deleting a cerain gObject\n");
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------

template <class gObject>
int list_T<gObject>::draw()
{
	printf ("I have started drawing all gObjects\n");
	if ((head != NULL) && (tail != NULL))
	{
		gObject* drawedElement = head;
        /*
        do
        {
            drawedArrow->draw();
            drawedArrow = drawedArrow->next;
        } while (drawedArrow->next != NULL);
        */
        while (drawedElement != NULL)
        {
            drawedElement->draw ();
            drawedElement = drawedElement->next;
        }
	}

	printf ("I have finished drawing all gObjects\n");
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------

template <class gObject>
int list_T<::dump()
{
    printf ("maxElements = %d, nElements = %d\n", maxArrows, nArrows);
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
int list_T::calculateNElements ()
{
    int nElems = 0;
    arrow* checkedArrow = head;

    if (head != NULL)
    {
        while (checkedArrow != NULL)
        {
            nElems++;
            checkedArrow = checkedArrow->next;
		}
    }
    printf ("ActivatedArrows = %d\n", nElems);

    int nFreeElems = 0;
    if (freeHead != NULL)
    {
    	checkedArrow = freeHead;
        while (checkedArrow != NULL)
        {
            nElems++;
            nFreeElems++;
            checkedArrow = checkedArrow->next;
        }
    }
    printf ("FreeArrows = %d\n", nFreeElems);
    return nArrows;
}

//---------------------------------------------------------------------------------------------------

template <class gObject>
int list_T::dumpAllArrows()
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


























































/*
template<>
int list_T<zombie>::printfSmth ()
{
    printf ("cheeeeeeee!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
    return 0;
}
*/

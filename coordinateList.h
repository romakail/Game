


template <class gObject>
class coordinateList_T
{
    private:
        int nLinesX;
        int nLinesY;

        int maxElements;
        int nElements;

        gObject* freeHead;
        gObject* freeTail;

        gObject*** headsMassive;
        gObject*** tailsMassive;

        gObject* elementsMassive;

    public:
    	//----Fundamental functions
        coordinateList_T  (int maxElementsNumber, int linesNumberX, int linesNumberY);
		~coordinateList_T ();
		gObject* addElement(gObject newElement);
		int deleteElement(gObject* deletedElement);

		//----Removing&drawing
		int draw();

		//----Dunping functions
        int dump();
        int calculateNElements();
};

//---------------------------------------------------------------------------------------------------------------

template <class gObject>
coordinateList_T<gObject>::coordinateList_T (int maxElementsNumber, int linesNumberX, int linesNumberY)
{
	assert (maxElementsNumber > 0);
    assert (linesNumberX > 1);
    assert (linesNumberY > 1);

    maxElements = maxElementsNumber;
    nElements = 0;

    nLinesX = linesNumberX;
    nLinesY = linesNumberY;

    elementsMassive = new gObject [maxElementsNumber];
    freeHead = elementsMassive;
    freeTail = & (elementsMassive [maxElementsNumber - 1]);

	gObject* elemPtr = freeHead;
    for (int i = 0; i < (maxElementsNumber - 1); i++)
    {
        elemPtr->next = &(elemPtr[1]);
        elemPtr->next->prev = elemPtr;
        elemPtr = elemPtr->next;
    }

    printf ("---1---\n");
    //headsMassive = new gObject** [linesNumberX * linesNumberY];
    //tailsMassive = new gObject** [linesNumberX * linesNumberY];

    headsMassive = static_cast<gObject***> (calloc (linesNumberX * linesNumberY, sizeof(gObject*)));
    assert (headsMassive);
    tailsMassive = static_cast<gObject***> (calloc (linesNumberX * linesNumberY, sizeof(gObject*)));
	assert (tailsMassive);

	printf ("--2--\n");
	/*
    for (int i = 0; i < linesNumberX; i++)
		for (int j = 0; j < linesNumberY; j++)
    	{
    		printf ("---3---\n");
            headsMassive [i][j] = NULL;
            tailsMassive [i][j] = NULL;
    	}
	*/
	printf ("I have finished constructing\n");
}

//---------------------------------------------------------------------------------------------------------------

template <class gObject>
coordinateList_T<gObject>::~coordinateList_T()
{
    //delete headsMassive;
    //delete tailsMassive;
    free (headsMassive);
    free (tailsMassive);
    delete elementsMassive;
}

//----------------------------------------------------------------------------------------------------------------

template <class gObject>
gObject* coordinateList_T<gObject>::addElement (gObject newElement)
{
    printf ("I have started  adding an element into coordinate list\n");

    int xList = newElement.pos.x / WINDOW_LENGHT * nLinesX;
    int yList = newElement.pos.y / WINDOW_HEIGHT * nLinesY;

    this->dump();

    printf ("xList = %d, yList = %d\n", xList, yList);
    printf ("head[x][y] = %p\ntail[x][y] = %p\n", headsMassive[xList][yList], tailsMassive[xList][yList]);

    if (headsMassive[xList][yList] == NULL)
    {
    	printf ("case nElements == 0\n");
        assert (tailsMassive[xList][yList] == NULL);

        nElements++;

        gObject* emptySpace = freeHead;

        freeHead->next->prev = NULL;
        freeHead = emptySpace->next;

        *emptySpace = newElement;

        emptySpace->next = tailsMassive[xList][yList]; // = NULL
        emptySpace->prev = headsMassive[xList][yList]; // = NULL

        headsMassive[xList][yList] = emptySpace;
        tailsMassive[xList][yList] = emptySpace;

        printf ("I have finished adding an element into coordinate list\n");
        return emptySpace;
    }
    else if ((nElements > 0) && (nElements < maxElements))
    {
		printf ("case 0 < nElements < max\n");
        nElements++;

        gObject* emptySpace = freeHead;

        if      (freeHead->next != NULL)
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

        printf ("I have prepared emptySpace\n");

        *emptySpace = newElement;

        assert (headsMassive[xList][yList]->prev == NULL);

        headsMassive[xList][yList]->prev = emptySpace;
        emptySpace->next = headsMassive[xList][yList];
        headsMassive[xList][yList] = emptySpace;

        printf ("I have finished adding an element into coordinate list\n");
        return emptySpace;
    }
    else if (nElements == maxElements)
    {
        assert (freeHead == NULL);
        assert (freeTail == NULL);

        printf ("case nElements == max\n");
        printf ("I cannot add anymore elements in coordinateList\n");

    	printf ("I have finished adding an element into coordinate list\n");
        return 0;
    }
    else
    	assert (0);

	//this->dump();
	return 0;
}

//--------------------------------------------------------------------------------------------------------

template <class gObject>
int coordinateList_T<gObject>::deleteElement(gObject* deletedElement)
{
	printf ("I have started deleteing an element from the coordinateList\n");

    assert (deletedElement);
    assert (nElements > 0);
    assert (nElements <= maxElements);
    assert ((deletedElement >= &elementsMassive[0]) && (deletedElement <= &elementsMassive[maxElements]));

    int xList = deletedElement.pos.x / WINDOW_LENGHT * nLinesX;
    int yList = deletedElement.pos.y / WINDOW_HEIGHT * nLinesY;

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
        tailsMassive[xList][yList] = deletedElement->prev;
    }
    else if ((deletedElement->next != NULL) && (deletedElement->prev == NULL))
    {
    	nElements--;

        deletedElement->next->prev = NULL;
        headsMassive[xList][yList] = deletedElement->next;
    }
    else if ((deletedElement->next == NULL) && (deletedElement->prev == NULL))
    {
    	nElements--;

        headsMassive[xList][yList] = deletedElement->next;
        tailsMassive[xList][yList] = deletedElement->prev;
	}
	else
	{
		assert (0);
	}

	printf ("I have disactivated the element\n");

	if      (freeTail != NULL)
	{
		printf ("case freeTail != NULL\n");
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

        freeHead = deletedElement;
        freeTail = deletedElement;

		deletedElement->next = NULL;
		deletedElement->prev = NULL;
	}
	else
	{
		printf ("whaaaaat?\n");
		assert (0);
	}

	printf ("I have finished deleteing an element from the coordinateList\n");

	return 0;
}

//-------------------------------------------------------------------------------------------------

template <class gObject>
int coordinateList_T<gObject>::draw()
{
	printf ("I have started drawing elements of coordinatelist\n");
	printf ("nLinesX = %d, nLinesY = %d nElements = %d\n", nLinesX, nLinesY, nElements);


	if (nElements > 0)
	{
		for (int i = 0; i < nLinesX; i++)
			for (int j = 0; j < nLinesY; j++)
			{
				printf ("---1---\n");
				if (headsMassive[i][j] != NULL)
				{
					assert (tailsMassive[i][j] != NULL);
					gObject* drawnElement = headsMassive[i][j];
					while (drawnElement != NULL)
					{
						drawnElement->draw ();
						drawnElement = drawnElement->next;
					}
				}
			}
	}
	printf ("I have finished drawing elements of coordinatelist\n");
	return 0;
}

//---------------------------------------------------------------------------------------------------------------

template <class gObject>
int coordinateList_T<gObject>::dump ()
{
	printf ("maxElements = %d, nElements = %d\n", maxElements, nElements);
    printf ("freeHead = %p\n", freeHead);
    printf ("freeTail = %p\n", freeTail);

    printf ("headsMassiveptr = %p\n", headsMassive);
    printf ("tailsMassiveptr = %p\n", tailsMassive);

    printf ("calculatednElements = %d\n", this->calculateNElements());
    if (freeTail != NULL)
    	printf ("freeTail->next = %p\n", freeTail->next);

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------

template <class gObject>
int coordinateList_T<gObject>::calculateNElements ()
{
	int nElems = 0;

	gObject* checkedElement = freeHead;

    if (freeHead != NULL)
    {
        while (checkedElement != NULL)
        {
            nElems++;
            checkedElement = checkedElement->next;
        }
    }

    printf ("free Elements number = %d\n", nElems);

    for (int i = 0; i < nLinesX; i++)
		for (int j = 0; j < nLinesY; j++)
		{
            printf ("i = %d  j = %d\n", i, j);
            printf ("sho blin za hren\n");

			printf ("headsMassive[%d][%d]", i, j);
			printf (" = %p\n", headsMassive[i][j]);
			if (headsMassive[i][j] != NULL)
			{
                checkedElement = headsMassive[i][j];
                while (checkedElement != NULL)
                {
                    nElems++;
                    checkedElement = checkedElement->next;
                }
			}
			else
				assert (tailsMassive[i][j] == NULL);
		}



	return nElems;
}






































































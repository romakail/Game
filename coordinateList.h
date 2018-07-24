


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

        gObject** headsMassive;
        gObject** tailsMassive;

        gObject* elementsMassive;

    public:
        coordinateList_T  (int maxElementsNumber, int linesNumberX, int linesNumberY);
		~coordinateList_T ();
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

    headsMassive = new gObject* [linesNumberX] [linesNumberY];
    tailsMassive = new gObject* [linesNumberX] [linesNumberY];

    for (int i = 0; i < linesNumberX; i++)
		for (int j = 0; j < linesNumberY; j++)
    	{
            headsMassive [i][j] = NULL;
            tailsMassive [i][j] = NULL;
    	}
}

//---------------------------------------------------------------------------------------------------------------

template <class gObject>
coordinateList_T<gObject>::~coordinateList_T()
{
    delete headsMassive;
    delete tailsMassive;
    delete elementsMassive;
}

//----------------------------------------------------------------------------------------------------------------






































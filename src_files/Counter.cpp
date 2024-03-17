#include "/src/headers/Counter.hpp"

Counter::Counter(int xBorder, int yBorder)
{
    this->index[0] = 0;
    this->index[1] = 0;
    this->xBorder = xBorder;
    this->yBorder = yBorder;
    this->isFinished = false;
}

void Counter::getCoordinates()
{
    // makes the scoop  mutex until leaving
    // this->mutex.lock();
    int x = this->index[0];
    int y = this->index[1];
    // this->mutex.unlock();

    if (x == xBorder && y == yBorder)
    {
        this->isFinished = true;
    }
    else
    {
        if (x == this->xBorder)
        {
            // mutex.lock();
            y++;
            x = 0;
            // mutex.unlock();
        }
        else
        {
            // mutex.lock();
            x++;
            // mutex.unlock();
        }
    }

    mutex.lock();
    this->index[0] = x;
    this->index[1] = y;
    mutex.unlock();
}

int *Counter::getIndex()
{
    return this->index;
}

bool Counter::getIsFinished()
{
    return this->isFinished;
}
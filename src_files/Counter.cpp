#include "/src/headers/Counter.hpp"

Counter::Counter(int xBorder, int yBorder)
{
    x = 0;
    y = 0;
    this->xBorder = xBorder;
    this->yBorder = yBorder;
    this->isFinished = false;
}

void Counter::getCoordinates()
{

    std::lock_guard<std::mutex> lock(mutex);

    if (x == xBorder && y == yBorder)
    {
        this->isFinished = true;
    }
    else
    {
        if (x == this->xBorder)
        {
            y++;
            x = 0;
        }
        else
        {
            x++;
        }
    }
}

bool Counter::getIsFinished()
{
    return this->isFinished;
}
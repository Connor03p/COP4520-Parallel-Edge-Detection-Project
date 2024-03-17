#include "/src/headers/Counter.hpp"

Counter::Counter()
{
    this->index[0] = 0;
    this->index[1] = 0;
}

int *Counter::getNextPatch()
{
    // makes the scoop  mutex until leaving
    std::lock_guard<std::mutex> guard(this->index_mutex);

    // TODO: actually make this traverse grid, this is just for testing for now
    this->index[0] += 1;
    this->index[1] += 1;

    return this->index;
}
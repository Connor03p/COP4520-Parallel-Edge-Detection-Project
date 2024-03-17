#include <thread>
#include <mutex>

class Counter
{

public:
    Counter(int xBorder, int yBorder);

    /**
     * @brief increment the current patch index and return the next index
     * to be calculated
     */
    void getCoordinates();
    int *getIndex();
    bool getIsFinished();

private:
    /**
     * The x & y values will be the value each thread grabs to then go complete a
     * patch of work during the sobel process
     *
     * These will need to be thread safe and also insure they don't reach out of bounds of the image
     *
     * x & y values will be stored in a array of size two, so they can be passed to the threads
     * in one package
     *
     * They will be in the following format
     *              [x,y]
     */
    int index[2];

    int xBorder;
    int yBorder;
    bool isFinished;
    std::mutex mutex;
};
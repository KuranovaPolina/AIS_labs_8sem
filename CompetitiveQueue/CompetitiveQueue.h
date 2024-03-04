#include <iostream>
#include <mutex>
#include <queue>

using namespace std;

class CompetitiveQueue
{
    private:
        queue<int> queueBuff;
        mutex queueMutex; 

    public:
        void push(int i);

        bool pop(int *i);
};

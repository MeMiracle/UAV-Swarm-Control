//
// Created by caryzhou on 17-11-15.
//

#ifndef TESTUWBLPS_TIMEOUT_H
#define TESTUWBLPS_TIMEOUT_H

#include <ctime>

class _timeout
{
private:

    clock_t m_time;
    int m_maxTime;
    char m_state;

public:

    _timeout(int time);
    void start(void);
    char end(void);
};

#endif //TESTUWBLPS_TIMEOUT_H

/*
 * Klasa testów, wykonuje wielowątkowo zapytania do serwera
 */
#ifndef _STRESSTEST_H
#define	_STRESSTEST_H
#include <string>
#include <vector>
#include "baseCurl.h"

class stressTest {
public:
    stressTest(std::string url, unsigned int requests_per_second, unsigned int time, unsigned long time_out, bool post, std::string post_parameters);
    virtual ~stressTest();
    void execute();
    std::string getInfo();
    double getAvgTime();
    double getMedianTime();
    double getStatusPercentage();
    std::vector<baseCurl *> requests;
private:
    static void * runThread(void * request_id);
    std::vector<baseCurl *>::iterator requests_iterator;
    std::string url;
    unsigned int requests_per_second;
    unsigned int time;
    unsigned int total_requests;
    unsigned long time_out;
    bool post;
    std::string post_parameters;
};

#endif	/* _STRESSTEST_H */


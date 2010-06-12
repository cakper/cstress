/*
 * Klasa dla zapytań POST dzidzicząca po baseCurl
 *
 */

#ifndef _CURLPOST_H
#define	_CURLPOST_H

#include "baseCurl.h"

class curlPost: public baseCurl {
public:
    curlPost(std::string &c);
    curlPost(const curlPost& orig);
    virtual ~curlPost();
    void setParameters(bool post, std::string post_parameters);
    bool execute();
private:

};

#endif	/* _CURLPOST_H */


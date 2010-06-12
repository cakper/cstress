/* 
 * Klasa dla zapytań GET dzidzicząca po baseCurl
 *
 */

#ifndef _CURLGET_H
#define	_CURLGET_H

#include "baseCurl.h"

class curlGet: public baseCurl {
public:
    curlGet(std::string &c);
    curlGet(const curlGet& orig);
    virtual ~curlGet();
    void setParameters(bool post, std::string post_parameters);
private:

};

#endif	/* _CURLGET_H */


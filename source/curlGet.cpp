/* 
 * File:   curlGet.cpp
 * Author: cakper
 * 
 * Created on 30 maj 2010, 02:38
 */

#include "curlGet.h"

curlGet::curlGet(std::string &c) : baseCurl(c)
{
}

curlGet::curlGet(const curlGet& orig) : baseCurl(orig)
{
}

curlGet::~curlGet()
{
}

void curlGet::setParameters(bool post, std::string post_parameters)
{
  // nic nie robimy
}
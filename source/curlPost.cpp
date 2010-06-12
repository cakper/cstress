/* 
 * File:   curlPost.cpp
 * Author: cakper
 * 
 * Created on 30 maj 2010, 02:56
 */

#include "curlPost.h"

curlPost::curlPost(std::string &c) : baseCurl(c)
{
}

curlPost::curlPost(const curlPost& orig) : baseCurl(orig)
{
}

curlPost::~curlPost()
{
}

void curlPost::setParameters(bool post, std::string post_parameters)
{
  if (post)
  {
    this->post = true;
    this->parameters = post_parameters;
  }
}
/**
 * Ustawia parametry post i wykonuje zapytanie
 *
 * @return ...
 * @author cakper
 */
bool curlPost::execute()
{
  this->preExecute();

  if (this->curl)
  {
    if (this->post)
    {

      curl_easy_setopt(this->curl, CURLOPT_POST, 1);
      curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, this->parameters.c_str());
    }

    this->result = curl_easy_perform(this->curl);

    return this->postExecute();
  }

  return false;
}
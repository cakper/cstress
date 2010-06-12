/* 
 * File:   curl.h
 * Author: cakper
 *
 * Abstrakcyjna klasa bazowa zapytań, dziedziczą po niej dwie klasy - curlGet oraz curlPost
 *
 * Dokładny opis klas przy definicjach
 */

#ifndef _BASECURL_H
#define	_BASECURL_H

#include <curl/curl.h>
#include <map>
#include <string>
#include <iostream>

class baseCurl {
public:
    baseCurl(const std::string &c);
    ~baseCurl();
    virtual bool execute();
    virtual void setParameters(bool post, std::string post_parameters) = 0;
    void setConnectionTimeout(unsigned long time);
    double getRequestTime();
    int getStatus();
    std::string getContent();
    std::string getError();
    unsigned int id;
protected:
    static std::string urlencode(std::string &c);
    static std::string char2hex(char dec);
    static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);
    void preExecute();
    bool postExecute();
    char error_buffer[CURL_ERROR_SIZE];
    CURL *curl;
    CURLcode result;
    bool post;
    std::string parameters;
    unsigned long time_out;
    std::string url;
    std::string buffer;
    int status;
    double request_time;
};

#endif	/* _baseCurl_H */


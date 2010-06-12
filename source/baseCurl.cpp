/* 
 * File:   curl.cpp
 * Author: cakper
 * 
 * Created on 9 maj 2010, 18:36
 */

#include "baseCurl.h"
#include <boost/regex.hpp>

/**
 * Konruktot, jako patametr przekazywany jest poprawny URL
 * (sprawdzany w konstruktorze, w przypadku błędu rzuca wyjątek)
 *
 * @param c
 * @author cakper
 */
baseCurl::baseCurl(const std::string &c)
{
  boost::regex url_pattern(".*((http|https):\\/\\/.+)\"*.*");
  try
  {
    if (boost::regex_match(c, url_pattern))
    {
      this->url = c;
    }
    else
    {
      throw new std::string("Przekazano niepoprawny URL");
    }
  }
  catch (boost::regex_error& e)
  {
      throw new std::string("Wyrażenie regularne jest nieprawidłowe");
  }
}
/**
 * Destruktor
 *
 * @author cakper
 */
baseCurl::~baseCurl()
{
}

/**
 * Enkoduje pojedynczy znak
 *
 * @param dec char to convert
 * @return std::string encoded char
 * @author cakper
 */
std::string baseCurl::char2hex(char dec)
{
  char dig1 = (dec & 0xF0) >> 4;
  char dig2 = (dec & 0x0F);
  if (0 <= dig1 && dig1 <= 9) dig1 += 48;
  if (10 <= dig1 && dig1 <= 15) dig1 += 97 - 10;
  if (0 <= dig2 && dig2 <= 9) dig2 += 48;
  if (10 <= dig2 && dig2 <= 15) dig2 += 97 - 10;

  std::string r;
  r.append(&dig1, 1);
  r.append(&dig2, 1);
  return r;
}


/**
 * Enkoduje cały URL
 *
 * @param c orginal url
 * @return std::string encoded url
 * @author cakper
 */
std::string baseCurl::urlencode(std::string &c)
{

  std::string escaped = "";
  int max = c.length();
  for (int i = 0; i < max; i++)
  {
    if ((48 <= c[i] && c[i] <= 57) ||
        (65 <= c[i] && c[i] <= 90) ||
        (97 <= c[i] && c[i] <= 122) ||
        (c[i] == '~' || c[i] == '!' || c[i] == '*' || c[i] == '(' || c[i] == ')' || c[i] == '\'')
        )
    {
      escaped.append(&c[i], 1);
    }
    else
    {
      escaped.append("%");
      escaped.append(char2hex(c[i]));
    }
  }
  return escaped;
}

/**
 * Zapisuje dane do bufora.
 * Jest to callback wywływany przez CURLa.
 *
 * @param data
 * @param size
 * @param nmemb
 * @param buffer
 * @return ...
 * @author cakper
 */
int baseCurl::writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
  int result = 0;
  if (buffer != NULL)
  {
    buffer->append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
}
/**
 * Przygotowuje połączenie połączenia
 *
 * @author cakper
 */
void baseCurl::preExecute()
{
  buffer = "";
  error_buffer[0] = 0;

  this->curl = curl_easy_init();

  if (this->curl)
  {
    curl_easy_setopt(this->curl, CURLOPT_ERRORBUFFER, error_buffer);
    curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(this->curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYHOST, 2);
    curl_easy_setopt(this->curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
    curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(this->curl, CURLOPT_COOKIEFILE, "cookies.txt");
    curl_easy_setopt(this->curl, CURLOPT_COOKIEJAR, "cookies.txt");
    if (this->time_out > 0)
    {
      curl_easy_setopt(this->curl, CURLOPT_CONNECTTIMEOUT, this->time_out);
    }
  }
  else
  {
    throw std::string("Błąd podczas inicjalizacji CURL-a");
  }
}
/**
 * Sprząta po połączeniu
 *
 * @return bool
 * @author cakper
 */
bool baseCurl::postExecute()
{
  curl_easy_getinfo(this->curl, CURLINFO_HTTP_CODE, &status);
  curl_easy_getinfo(this->curl, CURLINFO_TOTAL_TIME, &request_time);
  curl_easy_cleanup(this->curl);

  if (this->result == CURLE_OK)
  {
    return true;
  }
  else
  {
    std::cerr << "error:" << error_buffer << std::endl;
    return false;
  }
}

/**
 * Wykonuje zapytanie
 *
 * @return ...
 * @author cakper
 */
bool baseCurl::execute()
{
  this->preExecute();

  if (this->curl)
  {

    this->result = curl_easy_perform(this->curl);

    return this->postExecute();
  }

  return false;
}

void baseCurl::setConnectionTimeout(unsigned long time)
{
  this->time_out = time;
}

double baseCurl::getRequestTime()
{
  return this->request_time;
}

int baseCurl::getStatus()
{
  return this->status;
}

std::string baseCurl::getContent()
{
  return this->buffer;
}

std::string baseCurl::getError()
{
  return this->error_buffer;
}

#include "stressTest.h"
#include <pthread.h>
#include "curlGet.h"
#include "curlPost.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include "ctime"
#include "cstdlib"
#include <algorithm>

/**
 * Tworzy obiekt testu
 *
 * @param url
 * @param requests_per_second
 * @param time
 * @param time_out
 * @param post
 * @param post_parameters
 * @author cakper
 */
stressTest::stressTest(std::string url, unsigned int requests_per_second, unsigned int time, unsigned long time_out, bool post, std::string post_parameters = "")
{
  this->url = url;
  this->requests_per_second = requests_per_second;
  this->time = time;
  this->time_out = time_out;
  this->post = post;
  this->post_parameters = post_parameters;
  this->total_requests = this->requests_per_second * this->time;

  if (post)
  {
    for (unsigned int i = 1; i <= this->total_requests; i++)
    {
      this->requests.push_back(new curlPost(this->url));
      this->requests.back()->setParameters(true, this->post_parameters);
      this->requests.back()->setConnectionTimeout(this->time_out);
      this->requests.back()->id = i;
    }
  }
  else
  {
    for (unsigned int i = 1; i <= this->requests_per_second * this->time; i++)
    {
      this->requests.push_back(new curlGet(this->url));
      this->requests.back()->setConnectionTimeout(this->time_out);
      this->requests.back()->id = i;
    }
  }

  this->requests_iterator = this->requests.begin();

}

/**
 * Destruktor, czyści pamięć
 *
 * @author cakper
 */
stressTest::~stressTest()
{
  for (this->requests_iterator = this->requests.begin(); this->requests_iterator < this->requests.end(); this->requests_iterator++)
    delete * (this->requests_iterator);

  this->requests.clear();
}

/**
 * Wykonuje wielowątkowo test
 *
 * @author cakper
 */
void stressTest::execute()
{
  pthread_t threads[this->total_requests];
  void *status[this->total_requests];
  int rc;
  long t;

  clock_t start, end;

  double diff;
  start = clock();

  for (unsigned int i = 1; i <= (this->time); i++)
  {
    for (t = (i * this->requests_per_second - this->requests_per_second); t < i * this->requests_per_second; t++)
    {
      rc = pthread_create(&threads[t], NULL, &stressTest::runThread, (void *) (this->requests.at(t)));

      if (rc)
      {
        exit(-1);
      }
    }

    for (t = (i * this->requests_per_second - this->requests_per_second); t < i * this->requests_per_second; t++)
    {
      rc = pthread_join(threads[t], &status[t]);
    }

    end = clock();
    diff = (double) (double(end - start) / CLOCKS_PER_SEC);
    if (diff < (double) i - 1.0)
    {
      diff = (double) i - diff;
      unsigned long int wait;
      wait = (int) (1000000.0 * (diff - (double) i + 1.0));

      usleep(wait);
    }
    end = clock();
    diff = (double) (double(end - start) / CLOCKS_PER_SEC);
    diff = (double) i - diff;
  }
}

/**
 * Wykonuje pojedynczy wątek
 *
 * @param request_id
 * @return ...
 * @author cakper
 */
void *(stressTest::runThread)(void *request_id)
{
  baseCurl * request;
  request = reinterpret_cast<baseCurl *> (request_id);
  request->execute();
  pthread_exit(NULL);
}

/**
 * Zwraca informacje o teście
 *
 * @return ...
 * @author cakper
 */
std::string stressTest::getInfo()
{
  std::ostringstream retVal;
  retVal << "Url;POST;Ilość zapytań;Czas;Parametry POST" << std::endl << this->url << ";" << this->post << ";" << this->requests_per_second << ";" << this->time << ";" << post_parameters << std::endl;
  return retVal.str();
}

double stressTest::getAvgTime()
{
  double sum = 0.0;
  for (long unsigned int i = 0; i < this->total_requests; i++)
    sum += this->requests.at(i)->getRequestTime();
  return sum/(double)this->total_requests;
}

double stressTest::getMedianTime()
{
  std::vector<double> data;
  for (long unsigned int i = 0; i < this->total_requests; i++)
    data.push_back(this->requests.at(i)->getRequestTime());
  std::sort(data.begin(), data.end());
  long unsigned int center = this->total_requests / 2;
  return data.at(center);
}

double stressTest::getStatusPercentage()
{
  int counter = 0;
  for (long unsigned int i = 0; i < this->total_requests; i++)
    if(this->requests.at(i)->getStatus() == 200)
      counter++;
  return (double)(counter*100)/(double)this->total_requests;
}
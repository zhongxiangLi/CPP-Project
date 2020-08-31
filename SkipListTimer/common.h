#ifndef COMMON_H_
#define COMMON_H_

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h> 
#include <iostream>
using namespace std;
#define SKIPLIST_MAXLEVEL 32
#define ZSKIPLIST_P 0.25      /* Skiplist P = 1/4 */

static uint32_t getcurrent_time(){
	
	struct timeval tv;
	
	gettimeofday(&tv,NULL);
	
	uint32_t t;
	
	t = (uint32_t)tv.tv_sec *1000;
	t += tv.tv_sec /1000;
	return t;
}

#endif
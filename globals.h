#ifndef GLOBALS_H
#define GLOBALS_H
	
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>			/* header file for signal handling */
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>			/* header file for time functions*/
#include <sys/time.h>		/* header file for time functions*/
#include <pthread.h>		/* header file for threads*/	
#include <math.h>			/* header file for mathematical functions*/

#define CONSTANT 1
#define RANDOM 2

#define PARTS 16

#define BARRIER 1
#define COND_VAR 2

/*struct describing cost of a 16-part proccess*/
typedef struct _work_
{
	int part[16];

}work_t;

work_t * work;

/*struct holding secs and microsecs. used by the time functions*/
struct timeval tm;

/*defines the pthread mutex*/
pthread_mutex_t mutex;

/*defines the pthread condition variable*/
pthread_cond_t condition;

/*defines the pthread barrier*/
pthread_barrier_t bar;

typedef struct _stage_
{
	int stage_id;
	int num_of_stages;
	int num_of_items;
	int first;
	int last;
}stage_t;

stage_t * stage_info;

/*turns 1 when last operation is completed*/
//int last_flag=0


#endif

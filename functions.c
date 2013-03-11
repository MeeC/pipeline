
#include "globals.h"

/*produces artificial load of 1 millisecond*/
void one_milli(void)
{
	int i;
	double res = 4.0;

	for(i=0;i<28000;i++)
	{
		res=pow(sqrt(res),2.0);
	}

}

/*produces artificial load of given milliseconds*/
void comp(int millis)
{
	int i;

	for(i=0;i<millis;i++)
	{
		one_milli();
		//printf("%d\n",i);
	}
}

void set_work_load(int numItems, int type_of_load)
{
	int i,k,load;

	work=malloc(numItems * (sizeof(work_t)));
	for(k=0;k<numItems;k++)
	{
		
		for(i=0;i<16;i++)
		{
			if(type_of_load==CONSTANT) load=10;
			else if(type_of_load==RANDOM) load=20+rand()%10;
			work[k].part[i]=load;
		}
	}
}

double return_time(void)
{
	gettimeofday(&tm, NULL);
	double time=tm.tv_sec+(tm.tv_usec/1000000.0);
	
	return time;
}

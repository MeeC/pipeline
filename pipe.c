#include "globals.h"
#include "functions.h"

void sequential_pipe(int num_of_items)
{
	int k,i;

	double t1=return_time();

	for(k=0;k<num_of_items;k++)
	{
		for(i=0;i<PARTS;i++)
		{
			comp(work[k].part[i]);
			work[k].part[i]=0;
		}
	}

	double t2=return_time();

	printf("Computation competed.... \tTime of execution: [%.3lf]\n", t2-t1);

}

void set_up_stages(int num_items, int num_stages)
{
	int i,range,mod_counter,last_parameter;
	//allocating some memory for the array of stage_t structs
	stage_info=malloc(num_stages * (sizeof(stage_t)));

	//range is the number of calculations per thread for each 16-part-work
	range=PARTS/num_stages;
	mod_counter=PARTS%num_stages;
	
	for(i=0;i<num_stages;i++)
	{
		stage_info[i].stage_id=i;
		stage_info[i].num_of_stages=num_stages;
		stage_info[i].num_of_items=num_items;

		/* The first and last variables represent the first and the last work-part
		 * each thread will calculate. If 16%threads=0 means that every thread
		 * will calculate the same number of operations. Otherwise some threads
		 * are going to do more work than others. With the algorithm below we try
		 * to make the fair work-load distribution as possible as it gets.				
		 */
		if(mod_counter==0)last_parameter=1;
		else last_parameter=0;

		if(i==0) stage_info[i].first=0;
		else stage_info[i].first=stage_info[i-1].last+1;

		stage_info[i].last=stage_info[i].first+range-last_parameter;

		if(mod_counter!=0) mod_counter--;
		
	}
}

void pipe_stage(void * ptr_to_stage_info)
{
	int i,j;
	double wait_time=0,beneficial_time=0,t1,t2,t3,t4;
	stage_t * stage_data=(stage_t *)ptr_to_stage_info;

	for(i=0;i<stage_data->num_of_items;i++)
	{	
		t1=return_time();
		pthread_mutex_lock(&mutex);
		while(stage_data->first!=0 && work[i].part[stage_data->first-1]!=0)
		{
			pthread_cond_wait(&condition,&mutex);
		}
		pthread_mutex_unlock(&mutex);
		
		t2=return_time();
		for(j=stage_data->first;j<stage_data->last+1;j++)
		{
			comp(work[i].part[j]);
			work[i].part[j]=0;
		}
		t3=return_time();		

		pthread_mutex_lock(&mutex);
		pthread_cond_broadcast(&condition);
		pthread_mutex_unlock(&mutex);
	
		t4=return_time();

		beneficial_time+=(t3-t2);
		wait_time+=(t4-t1)-(t3-t2);
		//if(stage_data->last==15 && work[i].part[15]==0)printf("Calculation %d Ready\n",i); 
	}
	printf("Stage %d\tBeneficial time: [%.3lf]\t Wait time: [%.3lf]\n",stage_data->stage_id,beneficial_time,wait_time);

}

void pipe_stage_barrier(void * ptr_to_stage_info)
{
	int i,j;
	double t1,t2,t3,t4,beneficial_time=0,wait_time=0;
	stage_t * stage_data=(stage_t *)ptr_to_stage_info;


	for(i=0;i<stage_data->num_of_items;i++)
	{
		pthread_mutex_lock(&mutex);

		t1=return_time();

		while(stage_data->first!=0 && work[i].part[stage_data->first-1]!=0)
		{
			pthread_mutex_unlock(&mutex);
			pthread_barrier_wait(&bar);
			pthread_mutex_lock(&mutex);
		}
		pthread_mutex_unlock(&mutex);

		t2=return_time();

		for(j=stage_data->first;j<stage_data->last+1;j++)
		{
			comp(work[i].part[j]);
			work[i].part[j]=0;
		}

		t3=return_time();
		beneficial_time += (t3-t2);

		pthread_barrier_wait(&bar);
		
		t4=return_time();
		wait_time+= (t4-t1)-(t3-t2);
	}

	t1=return_time();
	//until last operation is over wait in barrier
	while(work[stage_data->num_of_items-1].part[15]!=0)
	{
		pthread_barrier_wait(&bar);
	}

	t2=return_time();
	wait_time+= t2-t1;	

	printf("Stage %d\tBeneficial time: [%.3lf]\t Wait time: [%.3lf]\n",stage_data->stage_id,beneficial_time,wait_time);
}

void pipeline(int num_of_items, int num_of_stages, int sync_type)
{
	int i,j;
	set_up_stages(num_of_items,num_of_stages);

	pthread_t thread[num_of_stages];
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);
	pthread_barrier_init(&bar,NULL,num_of_stages);

	double t1=return_time();

	if(sync_type==BARRIER)
	{
		for(i=0;i<num_of_stages;i++)
		{
			pthread_create(&thread[i],NULL, (void*) &pipe_stage_barrier, (void*)&stage_info[i]);
		}
	}
	else if(sync_type==COND_VAR)
	{
		for(i=0;i<num_of_stages;i++)
		{
			pthread_create(&thread[i],NULL, (void*) &pipe_stage, (void*)&stage_info[i]);
		}

	}
	
	for(j=0;j<num_of_stages;j++)
	{
		pthread_join(thread[j], NULL);
	}

	double t2=return_time();

	printf("Computation competed.... \tTime of execution: [%.3lf]\n", t2-t1);

	pthread_mutex_destroy(&mutex);

	pthread_barrier_destroy(&bar);
}


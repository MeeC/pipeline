#include "globals.h"
#include "functions.h"
#include "pipe.h"

int main(int argc, char **argv)
{
	int mode,threads_num,items_num,sync_type,work_mode;

	if(argc<4)
	{
		mode=2;
		threads_num=4;
		items_num=50;
		sync_type=COND_VAR;
		work_mode=CONSTANT;						
	}
	else if(argc==4)
	{
		items_num = atoi(argv[1]);
		work_mode=atoi(argv[2]);
		mode = atoi(argv[3]);
		if(mode==1)
		{
			threads_num = 1;
		}
		else if(mode==2)
		{
			sync_type=COND_VAR;
			threads_num=4;
		}
	}	
	else if(argc == 5)	
	{
		items_num = atoi(argv[1]);
		work_mode=atoi(argv[2]);	
		mode = atoi(argv[3]);
		threads_num = atoi(argv[4]);
		sync_type=COND_VAR;
	}
	else if(argc == 6)	
	{
		items_num = atoi(argv[1]);
		work_mode=atoi(argv[2]);	
		mode = atoi(argv[3]);
		threads_num = atoi(argv[4]);
		sync_type=atoi(argv[5]);
	}

	system("clear");
	srand ( (unsigned)time(NULL) );

	set_work_load(items_num,work_mode);

	if(mode==1) sequential_pipe(items_num);
	else if(mode==2) pipeline(items_num,threads_num,sync_type);
		
	return 0;
}

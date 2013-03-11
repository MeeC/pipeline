#ifndef PIPE_H
#define PIPE_H

void sequential_pipe(int num_of_items);

void pipeline(int num_of_items, int num_of_stages, int sync_type);

void pipe_stage(void * ptr_to_stage_info);

void pipe_stage_barrier(void * ptr_to_stage_info);

void set_up_stages(int num_items, int num_stages);

#endif

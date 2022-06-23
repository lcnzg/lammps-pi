#ifndef STOP_EARLY_H
#define STOP_EARLY_H

#include <mpi.h>
#include <sys/time.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#define PRINT_INIT 1
#define PRINT_STATS 2
#define PRINT_EXIT 3
#define PRINT_AVG 4
#define PRINT_BETA 5

typedef enum {false, true} bool;

#include "ifstats.h"

struct rusage *my_rusage;

int stop_in = 15;
long total_time;
unsigned int current_iteration;
double init_time;
double begin_time;
double first_begin_time;
double end_time;
double pi_sum;
double pi;
bool early_stop = false;

double get_current_time();
int get_iteration_();
void init_timestep_();
void end_timestep_();
void begin_timestep_();
void exit_timestep_();
void after_timestep_();
void my_exit();
void set_early_stop_(int*);
void debug_();
void print_timestep(uint8_t, double, struct rusage*, IFStats_t*);
void print_resources(int, int, struct rusage*);
void print_network(int, int, IFStats_t*);

#endif
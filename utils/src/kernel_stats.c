#include "kernel_stats.h"

void set_early_stop_(int *number){
  early_stop = true;
  stop_in = *number;
}

double get_current_time() {
    struct timeval tp;
    struct timezone tzp;
    gettimeofday(&tp,&tzp);
    return ((double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

void print_timestep(uint8_t type, double collected_time, struct rusage* resource, IFStats_t* network) {
  int rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  switch(type) {
    case PRINT_INIT:
      printf("[PI-INFO] Init time,%i,%f\n", rank, collected_time - init_time);
      break;
    case PRINT_STATS:
      printf("[PI-INFO] Paramount Iteration,%i,%i,%f,%f\n", rank, current_iteration, collected_time - init_time, pi);
      print_resources(rank, current_iteration, resource);
      print_network(rank, current_iteration, network);
      break;
    case PRINT_EXIT:
      printf("[PI-INFO] Total time,%f\n", collected_time - init_time);
      break;
    case PRINT_AVG:
      printf("[PI-INFO] PI avg,%i,%f,%d\n", rank, pi_sum/current_iteration, current_iteration);
      break;
    case PRINT_BETA:
      printf("[PI-INFO] Beta,%i,%f\n", rank, ((collected_time - end_time) + (begin_time - init_time))/pi_sum);
  }
}

void print_resources(int rank, int current_iteration, struct rusage *stats) {
  printf("[RU-INFO] Resources Stats,%i,", rank);
  printf("%i,", current_iteration);
  printf("%f,", ((double)stats->ru_utime.tv_sec + (double)stats->ru_utime.tv_usec*1.e-6));
  printf("%f,", ((double)stats->ru_stime.tv_sec + (double)stats->ru_stime.tv_usec*1.e-6));
  printf("%ld,", stats->ru_maxrss);   /* maximum resident set size */
  printf("%ld,", stats->ru_ixrss);    /* integral shared memory size */
  printf("%ld,", stats->ru_idrss);    /* integral unshared data size */
  printf("%ld,", stats->ru_isrss);    /* integral unshared stack size */
  printf("%ld,", stats->ru_minflt);   /* page reclaims (soft page faults) */
  printf("%ld,", stats->ru_majflt);   /* page faults (hard page faults) */
  printf("%ld,", stats->ru_nswap);    /* swaps */
  printf("%ld,", stats->ru_inblock);  /* block input operations */
  printf("%ld,", stats->ru_oublock);  /* block output operations */
  printf("%ld,", stats->ru_msgsnd);   /* IPC messages sent */
  printf("%ld,", stats->ru_msgrcv);   /* IPC messages received */
  printf("%ld,", stats->ru_nsignals); /* signals received */
  printf("%ld,", stats->ru_nvcsw);    /* voluntary context switches */
  printf("%ld\n", stats->ru_nivcsw);  /* involuntary context switches */
}

void print_network(int rank, int current_iteration, IFStats_t* stats) {
  while(stats != NULL) {
    printf("[NT-INFO] Network Stats,%i,", rank);
    printf("%i,", current_iteration);
    printf("%s,", stats->device);
    printf("%llu,", stats->rxBytes);
    printf("%llu,", stats->rxPackets);
    printf("%llu,", stats->rxErrors);
    printf("%llu,", stats->rxDrop);
    printf("%llu,", stats->rxFifo);
    printf("%llu,", stats->rxFrame);
    printf("%llu,", stats->rxCompressed);
    printf("%llu,", stats->rxMulticast);
    printf("%llu,", stats->txBytes);
    printf("%llu,", stats->txPackets);
    printf("%llu,", stats->txErrors);
    printf("%llu,", stats->txDrop);
    printf("%llu,", stats->txFifo);
    printf("%llu,", stats->txCollisions);
    printf("%llu,", stats->txCarrier);
    printf("%llu\n", stats->txCompressed);
    stats = stats->next;
  }
}

void init_timestep_() {
  init_time = get_current_time();

  current_iteration = 0;
  total_time = 0;

  my_rusage = (struct rusage*) malloc(sizeof(struct rusage));
}

void end_timestep_() {
  end_time = get_current_time();
}

void after_timestep_() {
  begin_timestep_();
  current_iteration--;
}

void begin_timestep_() {
  double old_begin_time = begin_time;

  begin_time = get_current_time();
  getrusage(RUSAGE_SELF, my_rusage);
  IFStats_t* network_stats = getIfStats();

  if(current_iteration == 0) {
    first_begin_time = begin_time;
    print_timestep(PRINT_INIT, begin_time, NULL, NULL);
  }else {
    pi = end_time - old_begin_time;
    pi += begin_time - end_time;
    pi_sum += pi;

    print_timestep(PRINT_STATS, begin_time, my_rusage, network_stats);
  }

  my_exit();

  current_iteration++;
}

int get_iteration_() {
  return current_iteration;
}

void exit_timestep_() {
  int rank;
  double current_time = get_current_time();

  if(current_iteration > 0) {
    print_timestep(PRINT_AVG, 0, NULL, NULL);
    print_timestep(PRINT_BETA, current_time, NULL, NULL);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0)
    print_timestep(PRINT_EXIT, current_time, NULL, NULL);
}

void my_exit() {
  if(early_stop && current_iteration == stop_in) {
    exit_timestep_();
    MPI_Finalize();
    exit(0);
  }
}
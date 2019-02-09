#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rand.h"

int init() {
  return 0;
}

void cleanup() {
}

/* return a random float in the range 0-1 */
float get_rand(void) {
  static int initialized = 0;
  if (!initialized) {
    srand((unsigned)time(NULL));
    initialized = 1;
  }
  return (float)rand() / RAND_MAX;
}

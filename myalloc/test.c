#include <stdio.h>
#include <stdlib.h>

#include "errno.h"
#include "myalloc.h"
#include "sequence.h"

#define VERBOSE 0


int main(int argc, char *argv[]) {
     init_myalloc();
     myalloc(10);

     return 0;

}

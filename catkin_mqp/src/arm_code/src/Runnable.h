#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//unavilable on windows
#include "Thread.h"

class Runnable : public Thread
{
  public:
    void *run();
};
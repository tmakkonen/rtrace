#include <iostream>
#include <sys/resource.h>
#include "rtrace.h"

void usage() {
  std::cout << "todo help" << std::endl;
  
  exit(0); 
}


int main(int argc, char **argv) {
  rtrace::traceparams *params = new rtrace::traceparams();

  setpriority(PRIO_PROCESS, 0, 10);
  // command line parser
  char ch;
  while ((ch = getopt(argc, argv, "hs:o:t:v")) != -1) {
    switch(ch) {
      case 'h':
        usage();
        break;
      case 's':
        params->scenefile = std::string(optarg);
        break;
      case 'o':
        params->outfile = std::string(optarg);
        break;
      case 't':
        params->threads = atoi(optarg);
        break;
      case 'v':
        params->verbose = true;
        break;
      default:        
        usage();
    }
  }

  // check for necessary params
  if (!params->validate()) usage();

  rtrace tracer(params);

  return tracer.run();
}



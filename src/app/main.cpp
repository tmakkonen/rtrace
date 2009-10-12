#include <iostream>
#include "rtrace.h"

void usage() {
  std::cout << "todo help" << std::endl;
  
  exit(0); 
}


int main(int argc, char **argv) {
  RTrace::Params params;
  
  // command line parser
  char ch;
  while ((ch = getopt(argc, argv, "hs:")) != -1) {
    switch(ch) {
      case 'h':
        usage();
      case 's':
        params.file = std::string();
      default:
        usage();
    }
  }

  // check for necessary params
  if (!params.validate()) {
    return EXIT_FAILURE;
  }

  RTrace tracer(params);

  return tracer.run();
}



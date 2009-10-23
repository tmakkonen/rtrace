#include <iostream>
#include "rtrace.h"

void usage() {
  std::cout << "todo help" << std::endl;
  
  exit(0); 
}


int main(int argc, char **argv) {
  RTrace::Params *params = new RTrace::Params();;
  
  // command line parser
  char ch;
  while ((ch = getopt(argc, argv, "hs:o:")) != -1) {
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
      default:
        usage();
    }
  }

  // check for necessary params
  if (!params->validate()) usage();

  RTrace tracer(params);

  return tracer.run();
}



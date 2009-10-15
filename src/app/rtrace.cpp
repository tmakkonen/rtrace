#include "rtrace.h"
#include <memory>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

bool RTrace::Params::validate() {
  if (file.empty()) {
    cerr << "please define a file name" << endl;
    return false;
  }
  
  return true;
}


RTrace::RTrace(Params *params) : m_params(params) {
}


int RTrace::run() {

  // load the scene file
  Scene *scene = new Scene(m_params->file);
  if (!scene->parse()) {
    cout << "scene file parsing failed" << endl;
    return EXIT_FAILURE;
  } 
  
  return EXIT_SUCCESS;
}

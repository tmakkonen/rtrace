#ifndef RTRACE_H
#define RTRACE_H

#include <string>
using std::string;

class RTrace {
  public:

    //! Parameter structure
    struct Params {
      string file;
      bool validate();
    };
    
    //! ctor
    RTrace(Params &params);

    //! dtor
    ~RTrace() {}

    // execute the tracer
    int run();

  private:

    RTrace() {}

};

#endif
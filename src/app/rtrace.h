#ifndef RTRACE_H
#define RTRACE_H

#include <string>
#include <sceneparser.h>
#include <rgb.h>

#include <boost/thread.hpp>


using std::string;
using std::ofstream;

class RTrace {
public:

  //! output file wrapper class
  class OutFile {
  public: 
    OutFile(const string &fname, const unsigned x, const unsigned y, const int chunks);
    ~OutFile();

    void write() const;
    void add_chunk(int i, std::vector<RGB> &chunk);
    
  private:
    FILE *m_file;
    unsigned int m_x;
    unsigned int m_y;
    std::vector<std::vector<RGB> > m_buf;

    boost::mutex m_mutex;
    OutFile();
  };
  
  
  //! Parameter structure
  struct Params {
    string outfile;
    string scenefile;
    bool validate();
  };
  
  //! ctor
  RTrace(Params *params);
  
  //! dtor
  ~RTrace() {}
  
  // execute the tracer
  int run();
  
private:

  Params *m_params;
  RTrace() {}

};

#endif

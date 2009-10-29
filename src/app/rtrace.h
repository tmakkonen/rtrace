#ifndef RTRACE_H
#define RTRACE_H

#include <string>
#include <sceneparser.h>
#include <rgb.h>

#include <boost/thread.hpp>


using std::string;
using std::ofstream;

class rtrace {
public:

  //! output file wrapper class
  class tgafile {
  public: 
    tgafile(const string &fname, const unsigned x, const unsigned y, const int chunks);
    ~tgafile();

    void write() const;
    void add_chunk(int i, std::vector<RGB> &chunk);
    std::vector<RGB>& get_chunck(const int idx) {
      return m_buf[idx];
    }
    
  private:
    FILE *m_file;
    unsigned int m_x;
    unsigned int m_y;
    std::vector<std::vector<RGB> > m_buf;

    boost::mutex m_mutex;
    tgafile();
  };
  
  
  //! Parameter structure
  struct traceparams {
    string outfile;
    string scenefile;
    int threads;
    bool verbose;
    traceparams() : outfile("out.tga"), scenefile(""), threads(1), verbose(false) {}
    bool validate();
  };
  
  //! ctor
  rtrace(traceparams *params);
  
  //! dtor
  ~rtrace() {}
  
  // execute the tracer
  int run();
  
private:

  traceparams *m_params;
  rtrace() {}

};

#endif

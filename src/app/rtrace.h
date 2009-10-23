#ifndef RTRACE_H
#define RTRACE_H

#include <string>
#include <sceneparser.h>
#include <rgb.h>

using std::string;
using std::ofstream;

class RTrace {
public:

  //! output file wrapper class
  class OutFile {
  public: 
    OutFile(const string &fname, const unsigned x, const unsigned y);
    ~OutFile();

    void Write() const;    
    void setPixel(const RGB &r);
    void Append(const short *buf, size_t bufzz);
    
  private:
    FILE *m_file;
    unsigned int m_x;
    unsigned int m_y;
    std::vector<unsigned char> m_buf;    
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
  RGB traceRay(Ray &r, Scene *s);
  
  Params *m_params;
  //  Scene *m_scene;
  
  RTrace() {}

};

#endif

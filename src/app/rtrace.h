#ifndef RTRACE_H
#define RTRACE_H

#include <string>
#include <sceneparser.h>

using std::string;
using std::ofstream;

class RTrace {
public:

  //! output file wrapper class
  class OutFile {
  public:
    
    OutFile(const string &fname, const unsigned x, const unsigned y);
    ~OutFile();

    void Append(const short *buf, size_t bufzz);
    void Write() const;    

    void setPixel(const float R_coeff, const float G_coeff, const float B_coeff);
  private:
    FILE *m_file;
    unsigned int m_x;
    unsigned int m_y;
    std::vector<unsigned char> m_buf;    
    OutFile();
  };
  
  
  //! Parameter structure
  struct Params {
    string file;
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
  //  Scene *m_scene;
  
  RTrace() {}

};

#endif

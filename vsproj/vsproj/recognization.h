#ifndef __Recognization_H_
#define __Recognization_H_
#include "public.h"

class CRecognization
{
public:
    CRecognization() {};
    ~CRecognization() {};

    void train(std::string data_dir);
    int test(cv::Mat img0);
};

#endif
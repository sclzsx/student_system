#ifndef __PUBLIC_H_
#define __PUBLIC_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdio>

#define DATA_DIR "./data/" 
#define CAP_NUM 10
#define IMG_SIZE 96

inline std::vector<std::string> split_string(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

#endif
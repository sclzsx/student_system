#include <iostream>
#include <opencv2/opencv.hpp>
class Cattendance
{
private:
struct Sstudent
{
    std::string name;
    int id_number;
    std::vector<cv::Mat> photos;
    /* data */
};
std::vector<Sstudent> student_lib;
    /* data */
public:
    Cattendance(/* args */);
    ~Cattendance();

    bool add_student(std::vector<Sstudent> &student_lib, Sstudent student);
    bool del_student(std::vector<Sstudent> &student_lib, Sstudent student);

};

Cattendance::Cattendance(/* args */)
{
}

Cattendance::~Cattendance()
{
}

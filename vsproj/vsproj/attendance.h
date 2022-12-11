#ifndef __RECOGNIZATION_H_
#define __RECOGNIZATION_H_

class CAttendance
{
public:
    CAttendance() {};
    ~CAttendance() {};

    std::vector<std::string> names;

    void add_page();
    void del_page();
    void rol_page();
    void home_page();

    std::vector<std::string> update_names();
    void update_model();
};

#endif

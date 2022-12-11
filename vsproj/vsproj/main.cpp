#include "public.h"
#include "attendance.h"
#include "Recognization.h"

void test_hog_svm()
{
    CAttendance Attendance = CAttendance();//初始化点名类
    CRecognization Recognization = CRecognization();//初始化人脸识别类
    Attendance.names = Attendance.update_names(); // 更新已存在的学生姓名
    Attendance.update_model(); // 更新模型
    std::vector<std::string> img_path;
    cv::glob("data", img_path, false);//读取文件夹下所有图片
    int lab = 0;
    for (int i = 0; i < img_path.size(); i++)
    {
        cv::Mat img = cv::imread(img_path[i], 0);
        int pred = Recognization.test(img);//识别该图
        std::cout << img_path[i] << " pred: " << pred << std::endl;//打印识别值，标签值
    }
    std::string key;
    while (1)
    {
        std::cout << "测试人脸识别完毕，输入exit进入点名系统" << std::endl;
        std::cin >> key;
        if (key == "exit")
        {
            break;
        }
    }
}

int main()
{
    test_hog_svm(); // 单独测试人脸识别

    CAttendance Attendance = CAttendance();//初始化点名类
    CRecognization Recognization = CRecognization();//初始化人脸识别类
    Attendance.home_page(); // 进入主页

    return 0;
}
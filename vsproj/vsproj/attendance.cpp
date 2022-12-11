#include "public.h"
#include "attendance.h"
#include "recognization.h"

std::vector<std::string> CAttendance::update_names()//根据文件夹下的图片更新人名
{
    std::vector<std::string> names_new;
    std::vector<std::string> file_names;
    cv::glob(DATA_DIR, file_names, false);
    for (size_t i = 0; i < file_names.size(); i++) // 遍历所有图片
    {
        std::string name = split_string(file_names[i], '\\')[1];
        name = split_string(name, '_')[0]; // 分离出图片名
        if (i % CAP_NUM == 0) // 每CAP_NUM张图片记录一次学生姓名
        {
            names_new.push_back(name);
        }
    }
    std::cout << "完成名单更新" << std::endl;
    for (size_t i = 0; i < names_new.size(); i++)
    {
        std::cout << names_new[i] << "，";
    }
    std::cout << std::endl;
    return names_new;
}

void CAttendance::update_model() // 根据文件夹下的图片更新模型
{
    CRecognization Recognization = CRecognization();
    Recognization.train(DATA_DIR);
    std::cout << "完成模型更新" << std::endl;
}

void CAttendance::del_page()
{
    std::string name;
    while (1)
    {
        std::cout << std::endl;
        std::cout << "#################删除学生#################" << std::endl;
        std::cout << "请输入学生姓名，输入exit返回主页" << std::endl;
        std::cin >> name;
        if (name == "exit") break;
        size_t i = 0;
        bool exist_flag = false;//该生已经存在的标志位
        for (i = 0; i < names.size(); i++)
        {
            if (name == names[i])
            {
                exist_flag = true;
                break;
            }
        }
        if (exist_flag)//存在的情况下才执行删除操作
        {
            std::vector<std::string> file_names;
            cv::glob(DATA_DIR, file_names, false);
            for (size_t i = 0; i < file_names.size(); i++)
            {
                std::string name_tmp = split_string(file_names[i], '\\')[1];
                name_tmp = split_string(name_tmp, '_')[0];
                if (name_tmp == name)
                {
                    remove(file_names[i].c_str());//删除该名字的所有图片
                }
            }
            std::cout << "该生信息已删除" << std::endl;
        }
        else
        {
            std::cout << "该生信息不存在" << std::endl;
        }
    }
    system("cls");
    home_page();
}
void CAttendance::add_page()
{
    std::string name;
    while (1)
    {
        std::cout << std::endl;
        std::cout << "#################添加学生#################" << std::endl;
        std::cout << "请输入学生姓名，输入exit返回主页" << std::endl;
        std::cin >> name;
        if (name == "exit") break;
        size_t i = 0;
        bool exist_flag = false;//判断学生已存在标志位
        for (i = 0; i < names.size(); i++)
        {
            if (name == names[i])
            {
                std::cout << "该生信息已存在" << std::endl;
                exist_flag = true;
                break;
            }
        }
        if (!exist_flag)
        {
            std::cout << "该生信息为空，准备拍照10张照片，按q拍照" << std::endl;
            cv::VideoCapture cap;
            cap.open(0);
            if (!cap.isOpened())
            {
                std::cout << "启动相机失败" << std::endl;
            }
            int cnt = 0;
            while (1)
            {
                cv::Mat photo;
                cap >> photo;
                if (photo.empty()) continue;
                cv::imshow("photo", photo);
                if (cv::waitKey(10) == 'q')
                {
                    std::string photo_path = DATA_DIR + name + "_" + std::to_string(cnt) + ".jpg";
                    cv::imwrite(photo_path, photo);
                    std::cout << "保存图像至 " << photo_path << std::endl;
                    cnt++;
                }
                if (cnt == CAP_NUM)
                {
                    cap.release();
                    cv::destroyAllWindows();
                    break;
                }
            }
            names.push_back(name);
            std::cout << "该生信息录入完毕" << std::endl;
        }
    }
    system("cls");
    home_page();
}

void CAttendance::rol_page()
{
    CRecognization Recognization = CRecognization();
    std::cout << "################开始点名#################" << std::endl;
    std::cout << "应到学生有" << std::endl;
    for (size_t i = 0; i < names.size(); i++)
    {
        std::cout << names[i] << "，";
    }
    std::vector<std::string> unreach_names;
    int rol_num = names.size(); // 点名人数
    std::string key;
    while (rol_num)
    {
        std::cout << std::endl;
        std::cout << "开始拍照点名，请输入的学生名" << std::endl;
        bool not_valid_name = true; // 无效学生名标志位
        while (not_valid_name)
        {
            std::cin >> key;
            for (size_t i = 0; i < names.size(); i++)
            {
                if (key == names[i])
                {
                    not_valid_name = false;
                    break;
                }
            }
            if (not_valid_name)
                std::cout << "没有该学生信息，请重新输入学生名" << std::endl;
        }
        std::cout << "按q拍照核验" << std::endl;
        cv::VideoCapture cap;
        cap.open(0); // 打开摄像头
        if (!cap.isOpened())
        {
            std::cout << "启动相机失败" << std::endl;
        }
        cv::Mat photo;
        while (1)
        {
            cap >> photo;
            if (photo.empty()) continue;
            cv::imshow("photo", photo);
            if (cv::waitKey(10) == 'q') // 按q拍照
            {
                std::cout << "拍照成功，准备识别" << std::endl;
                cap.release();
                cv::destroyAllWindows();
                break;
            }
        }
        int pred_lab = Recognization.test(photo); // 预测结果
        if (pred_lab < names.size())
        {
            std::cout << "预测为：" << names[pred_lab] << " 实际为：" << key << std::endl;
            if (names[pred_lab] == key) // 预测值对应的人名和输入相同
            {
                std::cout << "该生已到" << std::endl;
            }
            else
            {
                std::cout << "该生未到" << std::endl;
                unreach_names.push_back(key);
            }
        }
        else // 预测值不在范围内
        {
            std::cout << "该生未到" << std::endl;
            unreach_names.push_back(key);
        }
        rol_num--;
    }
    std::cout << std::endl;
    std::cout << "点名完毕，如下学生未到" << std::endl;
    for (int i = 0; i < unreach_names.size(); i++)
    {
        std::cout << unreach_names[i] << "，";
    }
    std::cout << std::endl;
    std::cout << "输入exit返回主页" << std::endl;
    while(1)
    {
        std::cin >> key;
        if (key == "exit")
        {
            break;
        }
    }
    system("cls");
    home_page();
}
void CAttendance::home_page()
{
    system("cls"); // 清屏
    names = update_names(); // 更新已存在的学生姓名
    if (names.size() > 0) update_model(); // 更新模型

    int key = 0;
    while (key != 1 && key != 2 && key != 3)
    {
        std::cout << "#################学生点名系统#################" << std::endl;
        std::cout << "功能选择" << std::endl;
        std::cout << "按1：添加学生" << std::endl;
        std::cout << "按2：删除学生" << std::endl;
        std::cout << "按3：开始点名" << std::endl;
        std::cout << "##############################################" << std::endl;
        std::cin >> key;
        switch (key)
        {
        case 1: system("cls"); add_page(); break;
        case 2: system("cls"); del_page(); break;
        case 3: system("cls"); rol_page(); break;
        }
        system("cls");
    }
}
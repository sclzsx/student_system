#include "public.h"
#include "attendance.h"
#include "recognization.h"

std::vector<std::string> CAttendance::update_names()//�����ļ����µ�ͼƬ��������
{
    std::vector<std::string> names_new;
    std::vector<std::string> file_names;
    cv::glob(DATA_DIR, file_names, false);
    for (size_t i = 0; i < file_names.size(); i++) // ��������ͼƬ
    {
        std::string name = split_string(file_names[i], '\\')[1];
        name = split_string(name, '_')[0]; // �����ͼƬ��
        if (i % CAP_NUM == 0) // ÿCAP_NUM��ͼƬ��¼һ��ѧ������
        {
            names_new.push_back(name);
        }
    }
    std::cout << "�����������" << std::endl;
    for (size_t i = 0; i < names_new.size(); i++)
    {
        std::cout << names_new[i] << "��";
    }
    std::cout << std::endl;
    return names_new;
}

void CAttendance::update_model() // �����ļ����µ�ͼƬ����ģ��
{
    CRecognization Recognization = CRecognization();
    Recognization.train(DATA_DIR);
    std::cout << "���ģ�͸���" << std::endl;
}

void CAttendance::del_page()
{
    std::string name;
    while (1)
    {
        std::cout << std::endl;
        std::cout << "#################ɾ��ѧ��#################" << std::endl;
        std::cout << "������ѧ������������exit������ҳ" << std::endl;
        std::cin >> name;
        if (name == "exit") break;
        size_t i = 0;
        bool exist_flag = false;//�����Ѿ����ڵı�־λ
        for (i = 0; i < names.size(); i++)
        {
            if (name == names[i])
            {
                exist_flag = true;
                break;
            }
        }
        if (exist_flag)//���ڵ�����²�ִ��ɾ������
        {
            std::vector<std::string> file_names;
            cv::glob(DATA_DIR, file_names, false);
            for (size_t i = 0; i < file_names.size(); i++)
            {
                std::string name_tmp = split_string(file_names[i], '\\')[1];
                name_tmp = split_string(name_tmp, '_')[0];
                if (name_tmp == name)
                {
                    remove(file_names[i].c_str());//ɾ�������ֵ�����ͼƬ
                }
            }
            std::cout << "������Ϣ��ɾ��" << std::endl;
        }
        else
        {
            std::cout << "������Ϣ������" << std::endl;
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
        std::cout << "#################���ѧ��#################" << std::endl;
        std::cout << "������ѧ������������exit������ҳ" << std::endl;
        std::cin >> name;
        if (name == "exit") break;
        size_t i = 0;
        bool exist_flag = false;//�ж�ѧ���Ѵ��ڱ�־λ
        for (i = 0; i < names.size(); i++)
        {
            if (name == names[i])
            {
                std::cout << "������Ϣ�Ѵ���" << std::endl;
                exist_flag = true;
                break;
            }
        }
        if (!exist_flag)
        {
            std::cout << "������ϢΪ�գ�׼������10����Ƭ����q����" << std::endl;
            cv::VideoCapture cap;
            cap.open(0);
            if (!cap.isOpened())
            {
                std::cout << "�������ʧ��" << std::endl;
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
                    std::cout << "����ͼ���� " << photo_path << std::endl;
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
            std::cout << "������Ϣ¼�����" << std::endl;
        }
    }
    system("cls");
    home_page();
}

void CAttendance::rol_page()
{
    CRecognization Recognization = CRecognization();
    std::cout << "################��ʼ����#################" << std::endl;
    std::cout << "Ӧ��ѧ����" << std::endl;
    for (size_t i = 0; i < names.size(); i++)
    {
        std::cout << names[i] << "��";
    }
    std::vector<std::string> unreach_names;
    int rol_num = names.size(); // ��������
    std::string key;
    while (rol_num)
    {
        std::cout << std::endl;
        std::cout << "��ʼ���յ������������ѧ����" << std::endl;
        bool not_valid_name = true; // ��Чѧ������־λ
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
                std::cout << "û�и�ѧ����Ϣ������������ѧ����" << std::endl;
        }
        std::cout << "��q���պ���" << std::endl;
        cv::VideoCapture cap;
        cap.open(0); // ������ͷ
        if (!cap.isOpened())
        {
            std::cout << "�������ʧ��" << std::endl;
        }
        cv::Mat photo;
        while (1)
        {
            cap >> photo;
            if (photo.empty()) continue;
            cv::imshow("photo", photo);
            if (cv::waitKey(10) == 'q') // ��q����
            {
                std::cout << "���ճɹ���׼��ʶ��" << std::endl;
                cap.release();
                cv::destroyAllWindows();
                break;
            }
        }
        int pred_lab = Recognization.test(photo); // Ԥ����
        if (pred_lab < names.size())
        {
            std::cout << "Ԥ��Ϊ��" << names[pred_lab] << " ʵ��Ϊ��" << key << std::endl;
            if (names[pred_lab] == key) // Ԥ��ֵ��Ӧ��������������ͬ
            {
                std::cout << "�����ѵ�" << std::endl;
            }
            else
            {
                std::cout << "����δ��" << std::endl;
                unreach_names.push_back(key);
            }
        }
        else // Ԥ��ֵ���ڷ�Χ��
        {
            std::cout << "����δ��" << std::endl;
            unreach_names.push_back(key);
        }
        rol_num--;
    }
    std::cout << std::endl;
    std::cout << "������ϣ�����ѧ��δ��" << std::endl;
    for (int i = 0; i < unreach_names.size(); i++)
    {
        std::cout << unreach_names[i] << "��";
    }
    std::cout << std::endl;
    std::cout << "����exit������ҳ" << std::endl;
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
    system("cls"); // ����
    names = update_names(); // �����Ѵ��ڵ�ѧ������
    if (names.size() > 0) update_model(); // ����ģ��

    int key = 0;
    while (key != 1 && key != 2 && key != 3)
    {
        std::cout << "#################ѧ������ϵͳ#################" << std::endl;
        std::cout << "����ѡ��" << std::endl;
        std::cout << "��1�����ѧ��" << std::endl;
        std::cout << "��2��ɾ��ѧ��" << std::endl;
        std::cout << "��3����ʼ����" << std::endl;
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
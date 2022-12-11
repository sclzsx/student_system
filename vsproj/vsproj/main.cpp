#include "public.h"
#include "attendance.h"
#include "Recognization.h"

void test_hog_svm()
{
    CAttendance Attendance = CAttendance();//��ʼ��������
    CRecognization Recognization = CRecognization();//��ʼ������ʶ����
    Attendance.names = Attendance.update_names(); // �����Ѵ��ڵ�ѧ������
    Attendance.update_model(); // ����ģ��
    std::vector<std::string> img_path;
    cv::glob("data", img_path, false);//��ȡ�ļ���������ͼƬ
    int lab = 0;
    for (int i = 0; i < img_path.size(); i++)
    {
        cv::Mat img = cv::imread(img_path[i], 0);
        int pred = Recognization.test(img);//ʶ���ͼ
        std::cout << img_path[i] << " pred: " << pred << std::endl;//��ӡʶ��ֵ����ǩֵ
    }
    std::string key;
    while (1)
    {
        std::cout << "��������ʶ����ϣ�����exit�������ϵͳ" << std::endl;
        std::cin >> key;
        if (key == "exit")
        {
            break;
        }
    }
}

int main()
{
    test_hog_svm(); // ������������ʶ��

    CAttendance Attendance = CAttendance();//��ʼ��������
    CRecognization Recognization = CRecognization();//��ʼ������ʶ����
    Attendance.home_page(); // ������ҳ

    return 0;
}
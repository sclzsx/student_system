#include "Recognization.h"
#include "attendance.h"

void CRecognization::train(std::string data_dir)
{
    std::vector<int> img_catg;
    std::vector<std::string> img_path;
    cv::glob(data_dir, img_path, false);
    int lab = 0;
    for (int i = 0; i < img_path.size(); i++)
    {
        img_catg.push_back(lab);//����ǩ
        if (i > 0 && i % CAP_NUM == 0)//ÿCAP_NUM����ǩֵ��1
        {
            std::pair<std::string, int> label_pair;
            std::string name = split_string(img_path[i], '\\')[1];
            name = split_string(name, '_')[0];
            lab = lab + 1;
        }
    }
    //�Ե�һ��ͼ��ȡ������Ϊ�˻��ά����Ϣ
    cv::Mat img0 = cv::imread(img_path[0], 0);
    cv::resize(img0, img0, cv::Size(IMG_SIZE, IMG_SIZE), 0, 0, cv::INTER_CUBIC);
    cv::HOGDescriptor hog0(cv::Size(IMG_SIZE, IMG_SIZE), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);//winsize, blocksize, blockstride, cellsize, binnum
    std::vector<float> descriptors0;//����������
    hog0.compute(img0, descriptors0, cv::Size(1, 1), cv::Size(0, 0));//winstride, padding
    int data_h = img_path.size();
    int data_w = descriptors0.size();
    cv::Mat data = cv::Mat::zeros(data_h, data_w, CV_32FC1);//ѵ������
    cv::Mat label = cv::Mat::zeros(data_h, 1, CV_32S);//ѵ����ǩ
    for (int i = 0; i < data_h; i++)
    {
        cv::Mat img = cv::imread(img_path[i], 0);
        cv::resize(img, img, cv::Size(IMG_SIZE, IMG_SIZE), 0, 0, cv::INTER_CUBIC);
        cv::HOGDescriptor hog(cv::Size(IMG_SIZE, IMG_SIZE), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);//winsize, blocksize, blockstride, cellsize, binnum
        std::vector<float> descriptors;
        hog.compute(img, descriptors, cv::Size(1, 1), cv::Size(0, 0));//winstride, padding
        for (int j = 0; j < data_w; j++)
        {
            data.at<float>(i, j) = descriptors[j];
            label.at<int>(i, 0) = img_catg[i];
        }
    }

    auto pTrainData = cv::ml::TrainData::create(data, cv::ml::ROW_SAMPLE, label);//�������ݺͱ�ǩ
    auto pSVM = cv::ml::SVM::create();//��ʼ��SVM
    pSVM->setType(cv::ml::SVM::Types::C_SVC);
    pSVM->setKernel(cv::ml::SVM::KernelTypes::LINEAR);
    pSVM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
    pSVM->train(pTrainData);//��ʼѵ��
    pSVM->save("model.xml");//����ģ��
    for (size_t i = 0; i < data_h; i++)
    {
        std::cout << label.at<int>(i, 0) << "��";//������ӡ��ǩֵ
    }
    std::cout << std::endl;
}

int CRecognization::test(cv::Mat img0)
{
    cv::resize(img0, img0, cv::Size(IMG_SIZE, IMG_SIZE), 0, 0, cv::INTER_CUBIC);
    cv::HOGDescriptor hog0(cv::Size(IMG_SIZE, IMG_SIZE), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);//winsize, blocksize, blockstride, cellsize, binnum
    std::vector<float> descriptors0;
    hog0.compute(img0, descriptors0, cv::Size(1, 1), cv::Size(0, 0));//winstride, padding
    int data_w = descriptors0.size();
    cv::Mat data0 = cv::Mat::zeros(1, data_w, CV_32FC1);
    for (int i = 0; i < data_w; i++)
    {
        data0.at<float>(0, i) = descriptors0[i];//������������תΪMat
    }
    auto pSVM = cv::ml::SVM::load("model.xml");//����ģ��
    int response = (pSVM->predict(data0));//ִ��Ԥ��
    return response;//����Ԥ��ֵ
}
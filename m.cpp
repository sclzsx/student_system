#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <deque>
#include <algorithm>
#include "omp.h"

#define FMax(X, Y) ((X) > (Y) ? (X) : (Y))
#define FMin(X, Y) ((X) < (Y) ? (X) : (Y))

std::vector<std::string> stringSplit(const std::string& str, char delim) {
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

int main()
{
    int max_range = 200;
	int min_target_num = 3;

    std::string input_txt_path = "../input.txt";
    std::string output_txt_path = "../output.txt";

	std::ofstream ofresult(output_txt_path.c_str(), std::ios::ate); // 生成并清空输出文件
	ofresult.close();

	std::ifstream file;
	file.open(input_txt_path.c_str(), std::ios::in);
	if (!file.is_open()) return 0;

    std::vector<std::pair<std::vector<int>, float> > all_src;
	std::string strLine;
	while (std::getline(file, strLine))
	{
		if (strLine.empty()) continue;

		std::vector<std::string> tmpstrs = stringSplit(strLine, ' ');
		float data = std::atof(tmpstrs[0].c_str());
		int case1 = std::atoi(tmpstrs[1].c_str());
		int case2 = std::atoi(tmpstrs[2].c_str());
		int case3 = std::atoi(tmpstrs[3].c_str());
        std::cout << data << " " << case1 << " " << case2 << " " << case3 << std::endl;

        std::vector<int> cases;
        cases.push_back(case1);
        cases.push_back(case2);
        cases.push_back(case3);
        std::pair<std::vector<int>, float> dst;
        dst.first = cases;
        dst.second = data;
        all_src.push_back(dst);
    }
	file.close();

    int num = omp_get_num_procs();
    printf("threads num:%d\n", num);
    omp_set_num_threads(num);
    #pragma omp parallel for
    for (int i = 0; i < all_src.size(); i++)
    {
		std::ofstream ofresult(output_txt_path.c_str(), std::ios::app);

        int case1 = all_src[i].first[0];
        int case2 = all_src[i].first[1];
        int case3 = all_src[i].first[2];
        float data = all_src[i].second;

		std::vector<float> min_targets;
		min_targets.clear();
		float MIN = 1000000;
		std::vector<std::pair<std::vector<int>, float> > all_dst;
		all_dst.clear();
		for (int a = 0; a < max_range; a++)
		{
			for (int b = 0; b < max_range; b++)
			{
				for (int c = 0; c < max_range; c++)
				{
					int sum = a + b + c;

					float ma = FMax(FMax(case1, case2), case3);
					float mi = FMin(FMin(case1, case2), case3);

					int down = std::floor(data / ma);
					int up = std::ceil(data / mi);

					if (!(sum >= down && sum <= up))
						continue;

					float target = float(case1 * a + case2 * b + case3 * c) - data;

					if (target < 0)
						continue;

					std::vector<int> abc;
					abc.clear();
					std::pair<std::vector<int>, float> abct;
					abc.push_back(a);
					abc.push_back(b);
					abc.push_back(c);
					abct.first = abc;
					abct.second = target;
					all_dst.push_back(abct);
					min_targets.push_back(target);
				}
			}
		}

		std::sort(min_targets.begin(), min_targets.end());
		std::unique(min_targets.begin(), min_targets.end());

		for (int j = 0; (j < min_targets.size() && j < min_target_num); j++)
		{
			for (int k = 0; k < all_dst.size(); k++)
			{
				if (all_dst[k].second == min_targets[j])
				{
					int a = all_dst[k].first[0];
					int b = all_dst[k].first[1];
					int c = all_dst[k].first[2];
					ofresult << data << " " << case1 << " " << case2 << " " << case3 << " " << a << " " << b << " " << c << " " << all_dst[k].second << "\n";
					printf("data:%f, case1:%d, case2:%d, case3:%d, a:%d, b:%d, c:%d, target:%f\n", i, data, case1, case2, case3, a, b, c, all_dst[k].second);
				}
			}
		}

		ofresult.close();
	}

	return 0;
}
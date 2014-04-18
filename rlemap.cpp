#include "CImg.h"
#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>

using namespace cimg_library;

int main() {
	const CImg<unsigned char> image("./img/chinamap.png");
	CImg<unsigned char> res = image.get_norm().normalize(0, 255).resize(92, 30);
	int inOut = 0;
	int sj = 1, oj = 1;
	std::vector<int> rle;
	for (int i = 3; i < res.height() - 1; i++) {
		//oj = 1;
		std::vector<int> tv;
		for (int j = 1; j < res.width() - 1; j++, sj++) {			
			if (res(j, i, 0) > 200 && inOut == 0) {
				inOut = 1;
				//std::cout << sj - oj << (inOut == 1 ? "C" : "J");
				int l = sj - oj;
				rle.push_back(l);
				oj = sj;
			} else if (res(j, i, 0) <= 200 && inOut == 1) {
				inOut = 0;
				//std::cout << sj - oj << (inOut == 1 ? "C" : "J");
				int l = sj - oj;
				rle.push_back(l);
				oj = sj;
			}
			
			///*
			if (inOut == 1)
				std::cout << "*";
			else
				std::cout << " ";
			//*/
		}
		std::cout << std::endl;
	}
	///*
	for (int i = 0; i < rle.size(); i++) {
		std::cout << rle[i] << ",";
	}
	std::cout << std::endl;
	int offset = 32;
	for (int i = 0; i < rle.size(); i++) {
		if (rle[i] + offset >= ' ' && rle[i] + offset <= '~')
			std::cout << (char)(rle[i] + offset);
		else {
			std::cout << (char)offset << (char)(90 + offset) << (char)(rle[i] + offset - 90);
		}
	}
	std::cout << std::endl;
	//*/
	(image, res).display();
	return 0;
}
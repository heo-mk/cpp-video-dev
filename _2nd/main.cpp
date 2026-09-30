#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "image.h"
#include "filter.h"

// ==========================================
// 메인 실행 함수
// ==========================================
int main() {
    const std::string input_file = "input.pgm";

    // 1. 원본 샘플 이미지가 없으면 자동 생성
    std::ifstream check(input_file);
    if (!check.good()) {
        std::cout << "[안내] 테스트용 원본 이미지(input.pgm)를 생성합니다." << std::endl;
        createSampleImage(input_file);
    }

    // 2. 필터 리스트 구성 (다형성 포인터 활용)
    struct FilterTask {
        std::string name;
        std::string output_file;
        std::unique_ptr<Filter> filter;
    };

    std::vector<FilterTask> tasks;
    tasks.push_back({"반전 필터", "output_invert.pgm", std::make_unique<InvertFilter>()});
    tasks.push_back({"임계값 필터 (기준: 128)", "output_threshold.pgm", std::make_unique<ThresholdFilter>(128)});
    tasks.push_back({"3x3 블러 필터", "output_blur.pgm", std::make_unique<BlurFilter>()});

    // 3. 각 필터 적용 및 저장
    for (auto& task : tasks) {
        Image img;
        if (loadPGM(input_file, img)) {
            std::cout << ">>> " << task.name << " 적용 중..." << std::endl;
            // 베이스 클래스 포인터를 통한 다형성 호출!
            task.filter->apply(img);
            savePGM(task.output_file, img);
            std::cout << "----------------------------------------" << std::endl;
        }
    }

    std::cout << "모든 작업이 완료되었습니다!" << std::endl;
    return 0;
}

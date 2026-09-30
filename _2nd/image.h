#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

// ==========================================
// 1. PGM 이미지 구조체 (P5 바이너리 포맷)
// ==========================================
struct Image {
    int width = 0;
    int height = 0;
    int max_val = 255;
    std::vector<unsigned char> data; // 픽셀 배열 (0 ~ 255)
};

// ==========================================
// PGM 파일 읽기 / 쓰기 / 샘플 생성 헬퍼 함수
// ==========================================
inline bool loadPGM(const std::string& filename, Image& img) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[오류] 파일 열기 실패: " << filename << std::endl;
        return false;
    }

    std::string magic;
    file >> magic;
    if (magic != "P5") {
        std::cerr << "[오류] 지원하지 않는 PGM 포맷입니다 (P5 바이너리만 지원): " << magic << std::endl;
        return false;
    }

    // 헤더 주석(#) 및 공백 건너뛰기
    auto skipComments = [&file]() {
        while (file >> std::ws && file.peek() == '#') {
            std::string comment;
            std::getline(file, comment);
        }
    };

    skipComments();
    file >> img.width;
    skipComments();
    file >> img.height;
    skipComments();
    file >> img.max_val;

    // 헤더 뒤에 오는 개행/공백 문자 1개 소비
    file.get();

    img.data.resize(img.width * img.height);
    file.read(reinterpret_cast<char*>(img.data.data()), img.data.size());

    if (!file) {
        std::cerr << "[오류] 픽셀 데이터 읽기 실패" << std::endl;
        return false;
    }

    std::cout << "[성공] 이미지 로드: " << filename << " (" << img.width << "x" << img.height << ")" << std::endl;
    return true;
}

inline bool savePGM(const std::string& filename, const Image& img) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[오류] 파일 저장 실패: " << filename << std::endl;
        return false;
    }

    // P5 헤더 작성 (텍스트)
    file << "P5\n" << img.width << " " << img.height << "\n" << img.max_val << "\n";
    // 픽셀 데이터 작성 (이진 바이트)
    file.write(reinterpret_cast<const char*>(img.data.data()), img.data.size());

    std::cout << "[성공] 이미지 저장 완료: " << filename << std::endl;
    return true;
}

// 테스트용 샘플 PGM 이미지 생성 (그라디언트 + 원형 패턴)
inline void createSampleImage(const std::string& filename, int width = 256, int height = 256) {
    Image img;
    img.width = width;
    img.height = height;
    img.max_val = 255;
    img.data.resize(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cx = width / 2;
            int cy = height / 2;
            double dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
            
            // 대각선 그라디언트 + 중앙 원형 하이라이트
            int val = (x + y) / 2;
            if (dist < 60) {
                val = 230;
            } else if (dist < 80) {
                val = 50;
            }
            img.data[y * width + x] = static_cast<unsigned char>(std::min(255, std::max(0, val)));
        }
    }
    savePGM(filename, img);
}

#pragma once

#include "image.h"
#include <cstddef>
#include <vector>
#include <utility>

// ==========================================
// 1. 필터 베이스 클래스 및 다형성 구조
// ==========================================
class Filter {
public:
    virtual ~Filter() = default;

    // TODO 3: 다형성(Polymorphism)을 위해 여기에 virtual 키워드를 올바르게 붙여보세요!
    // (힌트: virtual 키워드를 붙여야 파생 클래스의 apply가 실행됩니다.)
    /* TODO 3 */ void apply(Image& img) {}
};

// ==========================================
// 2. 반전 필터 (InvertFilter)
// ==========================================
class InvertFilter : public Filter {
public:
    void apply(Image& img) override {
        unsigned char* ptr = img.data.data();
        size_t total_pixels = img.data.size();

        for (size_t i = 0; i < total_pixels; ++i) {
            // TODO 1: 포인터(ptr)를 이용해 각 픽셀 값을 반전(255 - 현재값)시키세요.
            ptr[i] = 255 - ptr[i];
        }
    }
};

// ==========================================
// 3. 임계값 필터 (ThresholdFilter)
// ==========================================
class ThresholdFilter : public Filter {
private:
    unsigned char threshold_value;

public:
    explicit ThresholdFilter(unsigned char th = 128) : threshold_value(th) {}

    void apply(Image& img) override {
        unsigned char* ptr = img.data.data();
        size_t total_pixels = img.data.size();

        for (size_t i = 0; i < total_pixels; ++i) {
            // TODO 2: 기준값(threshold_value)과 비교하여 
            // 밝으면 255(흰색), 어두우면 0(검은색)으로 설정하는 조건문을 작성하세요.
            // if (...) { ... } else { ... }
        }
    }
};

// ==========================================
// 4. 블러 필터 (BlurFilter) - 3x3 박스 블러
// ==========================================
class BlurFilter : public Filter {
public:
    void apply(Image& img) override {
        int w = img.width;
        int h = img.height;
        std::vector<unsigned char> result = img.data;

        for (int y = 1; y < h - 1; ++y) {
            for (int x = 1; x < w - 1; ++x) {
                int sum = 0;
                // 3x3 주변 이웃 픽셀들의 평균 계산
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        sum += img.data[(y + dy) * w + (x + dx)];
                    }
                }
                result[y * w + x] = static_cast<unsigned char>(sum / 9);
            }
        }
        img.data = std::move(result);
    }
};

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <cmath>

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
bool loadPGM(const std::string& filename, Image& img) {
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
        char ch;
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

bool savePGM(const std::string& filename, const Image& img) {
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
void createSampleImage(const std::string& filename, int width = 256, int height = 256) {
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

// ==========================================
// 2. 필터 베이스 클래스 및 다형성 구조
// ==========================================
class Filter {
public:
    // TODO 3: 다형성(Polymorphism)을 위해 여기에 virtual 키워드를 올바르게 붙여보세요!
    // (가상 소멸자와 순수 가상 함수 apply 선언)
    /* TODO 3 */ virtual ~Filter() = default;
    /* TODO 3 */ void apply(Image& img); // <-- 힌트: 파생 클래스에서 오버라이딩되도록 virtual 키워드와 순수 가상(= 0) 선언이 필요합니다.
};

// ==========================================
// 3. 반전 필터 (InvertFilter)
// ==========================================
class InvertFilter : public Filter {
public:
    void apply(Image& img) override {
        unsigned char* ptr = img.data.data();
        size_t total_pixels = img.data.size();

        for (size_t i = 0; i < total_pixels; ++i) {
            // TODO 1: 포인터(ptr)를 이용해 각 픽셀 값을 반전(255 - 현재값)시키세요.
            // ptr[i] = ...
        }
    }
};

// ==========================================
// 4. 임계값 필터 (ThresholdFilter)
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
// 5. 블러 필터 (BlurFilter) - 3x3 박스 블러
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

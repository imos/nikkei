#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long int64;

// 画像の大きさ（正方形）
const int kImageSize = 100;
// 1年あたりの営業日日数（概数）
const int kBusinessYear = 262;
// 1年での上昇量のしきい値（期間の平方根で調整を行います）
const double kBaseRatio = log(1.5) / sqrt(kBusinessYear);

int64 GetEnvironmentInteger(const char* name, int64 default_value) {
  const char* value = getenv(name);
  int64 result;
  if (value != nullptr && sscanf(value, "%lld", &result) > 0) {
    return result;
  }
  return default_value;
}

double GetEnvironmentDouble(const char* name, double default_value) {
  const char* value = getenv(name);
  double result;
  if (value != nullptr && sscanf(value, "%lf", &result) > 0) {
    return result;
  }
  return default_value;
}

class AverageMap {
 public:
  AverageMap() {}

  void Init() {
    int date;
    double price;
    int previous_year = 0;
    while (scanf("%d%lf", &date, &price) > 0) {
      int year = 1970 + date / 365.25;
      if (previous_year != year) {
        year_indexes_.push_back(prices_.size());
        previous_year = year;
      }
      prices_.push_back(log(price));
    }
    year_indexes_.push_back(prices_.size());
    double accumulated_price = 0.0;
    accumulated_prices_.push_back(0.0);
    for (double price : prices_) {
      accumulated_price += price;
      accumulated_prices_.push_back(accumulated_price);
    }
  }

  void ColorMap() {
    printf("P3\n%d %d\n255\n", 400, 1);
    for (int i = 0; i <= 400; i++) {
      double score = i / 400.0 * 2 - 1.0;
      double score_ratio = GetEnvironmentDouble("RATIO", 1.0);
      score = 2.0 / (1.0 + exp(-score * 5.0 * score_ratio)) - 1.0;
      printf("%d %d %d\n",
             static_cast<int>(max(min(
                1.5 - fabs(score - 0.75) * 2, 1.0), 0.0) * 255),
             static_cast<int>(max(min(
                2.0 - fabs(score) * 2, 1.0), 0.0) * 255),
             static_cast<int>(max(min(
                1.5 - fabs(score + 0.75) * 2, 1.0), 0.0) * 255));
    }
  }

  void GenerateMap() {
    printf("P3\n%d %d\n255\n", kImageSize, kImageSize);
    double max_score = -1.0;
    int max_past;
    double max_ratio;
    for (int y = 0; y < kImageSize; y++) {
      for (int x = 0; x < kImageSize; x++) {
        int past = y + 1;
        double ratio = (static_cast<double>(x) / kImageSize * 2 - 1.0) *
                       sqrt(past) * kBaseRatio;
        double sum = 0.0;
        int range = 0;
        int year_start = GetEnvironmentInteger("YEAR_START", 2000) - 1970;
        int year_end = GetEnvironmentInteger("YEAR_END", 2000) - 1970;
        int index_begin = year_indexes_[year_start];
        int index_end = year_indexes_[year_end + 1];
        for (int index = max(index_begin, past);
             index + 1 < prices_.size() && index < index_end; index++) {
          range++;
          double old_price = (accumulated_prices_[index] -
                              accumulated_prices_[index - past]) / past;
          if (prices_[index] > old_price + ratio) {
            sum += prices_[index + 1] - prices_[index];
          }
        }
        double score = exp(sum / range * kBusinessYear) - 1.0;
        if (max_score < score) {
          max_past = past;
          max_ratio = ratio;
          max_score = score;
        }
        double score_ratio = GetEnvironmentDouble("RATIO", 1.0);
        score = 2.0 / (1.0 + exp(-score * 5.0 * score_ratio)) - 1.0;
        printf("%d %d %d\n",
               static_cast<int>(max(min(
                  1.5 - fabs(score - 0.75) * 2, 1.0), 0.0) * 255),
               static_cast<int>(max(min(
                  2.0 - fabs(score) * 2, 1.0), 0.0) * 255),
               static_cast<int>(max(min(
                  1.5 - fabs(score + 0.75) * 2, 1.0), 0.0) * 255));
      }
    }
    fprintf(stderr, "+%.4f\n", max_score);
    fprintf(stderr, "Max Past: %d\n", max_past);
    fprintf(stderr, "Max Ratio: %.4f\n", max_ratio);
  }

 private:
  vector<double> prices_;
  vector<double> accumulated_prices_;
  vector<int> year_indexes_;
};

int main() {
  AverageMap average_map;
  average_map.Init();
  if (GetEnvironmentInteger("COLOR", 0)) {
    average_map.ColorMap();
  } else {
    average_map.GenerateMap();
  }
  return 0;
}

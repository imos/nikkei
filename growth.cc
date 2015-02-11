#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <utility>
#include <vector>
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

class Growth {
 public:
  Growth() {}

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
      dates_.push_back(date);
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

  void Generate() {
    map<pair<int, double>, vector<double>> table;
    for (const auto& parameter :
         vector<pair<int, double>>({{0, log(1.0)},
                                    {1, log(1.00)},
                                    {5, log(1.00)},
                                    {20, log(1.00)},
                                    {50, log(1.00)},
                                    {100, log(1.00)}})) {
      const int past = parameter.first;
      const double ratio = parameter.second;
      vector<double> result;
      double sum = 0.0;
      for (int index = 0; index + 1 < prices_.size(); index++) {
        if (past == 0) {
          sum += prices_[index + 1] - prices_[index];
        } else {
          if (0 <= index - past) {
            double old_price = (accumulated_prices_[index] -
                                accumulated_prices_[index - past]) / past;
            if (prices_[index] > old_price + ratio) {
              sum += prices_[index + 1] - prices_[index];
            }
          }
        }
        result.push_back(sum);
      }
      table[parameter] = result;
    }

    vector<pair<int, double>> parameters;
    for (const auto& parameter_and_result : table) {
      parameters.push_back(parameter_and_result.first);
    }
    

    // ヘッダーの出力
    for (const auto& parameter : parameters) {
      printf("\t%d-%.f", parameter.first, parameter.second);
    }
    printf("\n");
    // 表の出力
    for (int index = 0; index + 1 < prices_.size(); index++) {
      printf("%d", dates_[index]);
      for (const auto& parameter : parameters) {
        printf("\t%.3f", table[parameter][index]);
      }
      printf("\n");
    }
  }

 private:
  vector<double> prices_;
  vector<int> dates_;
  vector<double> accumulated_prices_;
  vector<int> year_indexes_;
};

int main() {
  Growth growth;
  growth.Init();
  growth.Generate();
  return 0;
}

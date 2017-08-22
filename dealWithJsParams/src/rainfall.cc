#include "rainfall.h"
#include <algorithm>
#include <cmath>

double location::avg_rainfall() {
    double total = 0;
    for (const auto &sample : this->samples) {
      total += sample.rainfall;
    }
    return total / this->samples.size();
}

sample::sample() {
  date = ""; rainfall = 0;
}

sample::sample (string d, double r)  {
  date = d;
  rainfall = r;
}
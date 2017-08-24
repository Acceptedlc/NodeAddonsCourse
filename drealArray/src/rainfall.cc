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

rain_result location::calc_rain_stats() {
	rain_result result;
	double ss = 0;
	double total = 0;
	
	result.n = this->samples.size();

	for (const auto &sample : this->samples) {
   	 total += sample.rainfall;
  	}
  	result.mean = total / this->samples.size();
  	
  	for (const auto &sample : this->samples) {
   	 ss += pow(sample.rainfall - result.mean, 2);
  	}
  	result.standard_deviation = sqrt(ss/(result.n-1));

  	std::sort(this->samples.begin(), this->samples.end());
	if (result.n %2 == 0) {
		result.median = (this->samples[result.n / 2 - 1].rainfall + this->samples[result.n / 2].rainfall) / 2;
	}
	else {
		result.median = this->samples[result.n / 2].rainfall;
	}
	return result;
}

sample::sample() {
  date = ""; rainfall = 0;
}

sample::sample (string d, double r)  {
  date = d;
  rainfall = r;
}

bool operator<(const sample &s1, const sample &s2) {
	return s1.rainfall < s2.rainfall;
}
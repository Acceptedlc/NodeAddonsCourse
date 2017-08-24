#include <vector>
#include <string>
using namespace std;

class sample {
public:
  sample ();
  sample (string d, double r);

  string date;
  double rainfall;
};

class rain_result {
public:
    float median;
    float mean;
    float standard_deviation;
    int n;
};

class location {
public:
  double longitude;
  double latitude;
  vector<sample> samples;
  double avg_rainfall();
  rain_result calc_rain_stats();
};



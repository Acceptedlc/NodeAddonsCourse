#include <vector>
#include <string>
using namespace std;

class sample {
public:
    sample (); // in rainfall.cc
    sample (string d, double r) ; // in rainfall.cc
    string date;
    double rainfall;
};

class location {
public:
  double longitude;
  double latitude;
  vector<sample> samples;
};

double avg_rainfall(location & loc);
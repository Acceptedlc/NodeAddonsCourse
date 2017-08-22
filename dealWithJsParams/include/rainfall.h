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

class location {
public:
  double longitude;
  double latitude;
  vector<sample> samples;
  double avg_rainfall();
};


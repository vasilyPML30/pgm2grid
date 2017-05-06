#include "convert.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  string yaml, msg;
  if (argc == 1) {
    cout << "yaml file name:   ";
    cin >> yaml;
    cout << "output file name: ";
    cin >> msg;
  }
  else if (argc != 5) {
    cout << "usage: convert -y yaml_file -m msg_file" << endl;
    return 1;
  }
  else {
    for (int i = 1; i < 5; i += 2)
    if (argv[i][1] == 'y')
      yaml = string(argv[i + 1]);
    else if (argv[i][1] == 'm')
      msg = string(argv[i + 1]);
    else {
      cout << "usage: convert -y yaml_file -m msg_file" << endl;
      return 1;
    }
  }
  ifstream in(yaml);
  ofstream out(msg);
  if (!in || !out) {
    cout << "unable to open file" << endl;
    return 2;
  }
  convert(in, out);
  return 0;
}

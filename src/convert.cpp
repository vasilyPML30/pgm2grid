#include "convert.h"
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

void print(RosHeader &header, MapMetaData meta) {
  cout << header.seq << endl;
  cout << header.time_sec << endl;
  cout << header.time_nsec << endl;
  cout << header.frame_id[0] << endl;
  cout << meta.map_load_time_sec << endl;
  cout << meta.map_load_time_nsec << endl;
  cout << meta.resolution << endl;
  cout << meta.width << endl;
  cout << meta.height << endl;
  cout << meta.pos_x << endl;
  cout << meta.pos_y << endl;
  cout << meta.pos_z << endl;
  cout << meta.orient_x << endl;
  cout << meta.orient_y << endl;
  cout << meta.orient_z << endl;
  cout << meta.orient_w << endl;
}

void convert(ifstream &in, ofstream &out) {
  string image;
  string param;
  int negate = false;
  float32 occ, free;
  RosHeader header = {0, (uint32)time(0), 0, "0"};
  MapMetaData meta = {(uint32)time(0), 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  while (in >> param) {
    if (param == "image:")
      in >> image;
    else if (param == "resolution:")
      in >> meta.resolution;
    else if (param == "origin:") {
      char tmp;
      in >> tmp >> meta.pos_x;
      in >> tmp >> meta.pos_y;
      in >> tmp >> meta.pos_z >> tmp;
    }
    else if (param == "negate:")
      in >> negate;
    else if (param == "occupied_thresh:")
      in >> occ;
    else if (param == "free_thresh:")
      in >> free;
  }
  ifstream im_in(image);
  if (!im_in) {
    cout << "unable to open file" << endl;
    exit(0);
  }
  string line;
  do {
    getline(im_in, line);
  } while(line.empty() || line[0] < '0' || line[0] > '9');
  size_t pos;
  meta.width = stoi(line, &pos);
  meta.height = stoi(line.substr(pos));
  do {
    getline(im_in, line);
  } while(line.empty() || line[0] < '0' || line[0] > '9');

  out.write((char *) &header, sizeof(header));
  out.write((char *) &meta, sizeof(meta));

  for (uint32 i = 0; i < meta.width * meta.height; ++i) {
    uint8 pixel = im_in.get();
    float32 thresh = pixel / 255.0;
    int8 value;
    if (thresh >= occ)
      value = 100;
    else if (thresh <= free)
      value = 0;
    else
      value = -1;
    out.put(value);
  }
}
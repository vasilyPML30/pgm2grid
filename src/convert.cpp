#include "convert.h"
#include <string>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <functional>

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
  char format;
  string image;
  string param;
  int negate = false;
  float32 occ, free;
  RosHeader header = {0, 0, 0, "0"};
  MapMetaData meta = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
  } while(line.empty() || line[0] != 'P');
  format = line[1];
  do {
    getline(im_in, line);
  } while(line.empty() || line[0] < '0' || line[0] > '9');
  char *pos;
  meta.width = strtol(line.c_str(), &pos, 10);
  meta.height = strtol(pos, NULL, 10);
  do {
    getline(im_in, line);
  } while(line.empty() || line[0] < '0' || line[0] > '9');

  out.write((char *) &header, sizeof(header));
  out.write((char *) &meta, sizeof(meta));

  for (uint32 i = 0; i < meta.width * meta.height; ++i) {
    uint8 pixel;
    if (format == '5')
      pixel = im_in.get();
    else {
      int32 pix;
      im_in >> pix;
      pixel = pix;
    }
    float32 thresh = (255 - pixel) / 255.0;
    int8 value;
    if (thresh >= occ)
      value = (negate ? 0 : 100);
    else if (thresh <= free)
      value = (negate ? 100 : 0);
    else
      value = -1;
    out.put(value);
  }
}
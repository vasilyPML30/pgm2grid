#ifndef CONVERT_H_
#define CONVERT_H_

#include <fstream>
#include <stdint.h>

typedef double float64;
typedef float float32;
typedef uint32_t uint32;
typedef int32_t int32;
typedef char int8;
typedef unsigned char uint8;

#pragma pack(push, 1)

struct RosHeader {
  uint32 seq;
  uint32 time_sec;
  uint32 time_nsec;
  const char frame_id[2];
};

struct MapMetaData {
  uint32 map_load_time_sec;
  uint32 map_load_time_nsec;
  float32 resolution;
  uint32 width;
  uint32 height;
  float64 pos_x, pos_y, pos_z;
  float64 orient_x, orient_y, orient_z, orient_w;
};

#pragma pack(pop)

void convert(std::ifstream &in, std::ofstream &out);

#endif
#ifndef CYCLE_H
#define CYCLE_H

#include "Pose.h"

class Cycle{
public:
   Cycle();
   Cycle(Cycle *);
   Cycle operator*(const float& s);
   Cycle operator+(const Cycle& other);
   float * getPtr();
   Pose getCurrentPose(float v);
private:
   std::vector<Pose> poses;
};

#endif

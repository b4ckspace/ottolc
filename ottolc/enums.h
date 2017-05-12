#pragma once
#include <stdint.h>
enum Setting{
  trimLeftFoot,
  trimRightFoot,
  trimLeftLeg,
  trimRightLeg,
  trimChecksum,
};

// enum for servo->number in servos array
enum EServo{
	rightFoot=0,
	leftFoot,
	rightLeg,
	leftLeg,
};

struct __attribute__((packed)) AnimKeyframe{
	uint8_t leftFoot = 90;
	uint8_t rightFoot = 90;
	uint8_t leftLeg = 90;
	uint8_t rightLeg = 90;
	int16_t duration :11; //duration in ms
};
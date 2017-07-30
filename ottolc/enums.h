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
	uint8_t leftFoot;
	uint8_t rightFoot;
	uint8_t leftLeg;
	uint8_t rightLeg;
	int16_t duration; //duration in ms
};

struct __attribute__((packed)) AnimationCallback{
	typedef void (*AnimFun)(void* params);
	static constexpr size_t argsize = sizeof(AnimKeyframe)-sizeof(AnimFun);
	AnimFun fun;
	char args[argsize];
};

struct __attribute__((packed)) AnimStep_t{
	uint8_t options;
	enum optfields{
		IS_FRAME=0,
	}; 
	union data_t{
		AnimKeyframe keyframe;
		AnimationCallback cb;
	};
	data_t data;
};

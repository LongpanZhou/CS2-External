#pragma once
#include <vector>

// Copied from Asver
struct BoneJointData
{
	Vec3 Pos;
	float Scale;
	char pad[0x10];
};

enum BONEINDEX: int
{
	PELVIS = 0,
	NECK = 5,
	HEAD = 6,
	LEFT_SHOULDER = 8,
	LEFT_ELBOW = 9,
	LEFT_HAND = 11,
	RIGHT_SHOULDER = 13,
	RIGHT_ELBOW = 14,
	RIGHT_HAND = 16,
	LEFT_LEG = 22,
	LEFT_KNEE = 23,
	LEFT_FOOT = 24,
	RIGHT_LEG = 25,
	RIGHT_KNEE = 26,
	RIGHT_FOOT = 27
};

struct BoneConnection
{
	int bone_start;
	int bone_end;
	BoneConnection(int bone_start, int bone2) : bone_start(bone_start), bone_end(bone2) {}
};

BoneConnection Skeleton[] = {
	//BoneConnection(0,0),
	//BoneConnection(1,1),
	//BoneConnection(2,2),
	//BoneConnection(3,3),
	//BoneConnection(4,4),
	//BoneConnection(5,5),
	//BoneConnection(6,6),
	//BoneConnection(7,7),
	//BoneConnection(8,8),
	//BoneConnection(9,9),
	//BoneConnection(10,10),
	//BoneConnection(11,11),
	//BoneConnection(12,12),
	//BoneConnection(13,13),
	//BoneConnection(14,14),
	//BoneConnection(15,15),
	//BoneConnection(16,16),
	//BoneConnection(17,17),
	//BoneConnection(18,18),
	//BoneConnection(19,19),
	//BoneConnection(20,20),
	//BoneConnection(21,21),
	//BoneConnection(22,22),
	//BoneConnection(23,23),
	//BoneConnection(24,24),
	//BoneConnection(25,25),
	//BoneConnection(26,26),
	//BoneConnection(27,27),

	BoneConnection(HEAD, NECK),
	BoneConnection(NECK, PELVIS),
	BoneConnection(NECK, LEFT_SHOULDER),
	BoneConnection(NECK, RIGHT_SHOULDER),
	BoneConnection(PELVIS, LEFT_LEG),
	BoneConnection(PELVIS, RIGHT_LEG),
	BoneConnection(LEFT_SHOULDER, LEFT_ELBOW),
	BoneConnection(LEFT_ELBOW, LEFT_HAND),
	BoneConnection(RIGHT_SHOULDER, RIGHT_ELBOW),
	BoneConnection(RIGHT_ELBOW, RIGHT_HAND),
	BoneConnection(LEFT_LEG, LEFT_KNEE),
	BoneConnection(LEFT_KNEE, LEFT_FOOT),
	BoneConnection(RIGHT_LEG, RIGHT_KNEE),
	BoneConnection(RIGHT_KNEE, RIGHT_FOOT)
};
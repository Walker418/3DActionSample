#ifndef DAMAGE_H_
#define DAMAGE_H_

#include "../Math/Vector3.h"

// _[W\¢Ì
// »ìÒF½ âQi"Jacky" Ho Siu Kij

struct Damage
{
	Vector3 position{ Vector3::Zero };	// U»èÌÊu
	int power{ 0 };						// UÍ
	int impact{ 0 };					// ¯ÝliGÉÌÝg¤j
};

#endif // !DAMAGE_H_
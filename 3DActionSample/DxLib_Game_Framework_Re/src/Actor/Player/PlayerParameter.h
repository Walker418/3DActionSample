#ifndef PLAYER_PARAMETER_H_
#define PLAYER_PARAMETER_H_

// vC[Ìp[^
// »ìÒF½ âQi"Jacky" Ho Siu Kij
namespace PlayerParameter
{
	const int	HP{ 100 };					// ÅåÌÍ
	const float	WalkSpeed{ 0.3f };			// às¬x
	const float	DashSpeed{ 0.8f };			// _bV¬x
	const float	RotateSpeed{ 0.3f };		// ñ]¬x
	const float	Gravity{ 0.03f };			// º¬x

	const int	Power_Atk1{ 3 };			// U1iÚÌÐÍ
	const int	Power_Atk2{ 2 };			// U2iÚÌÐÍ
	const int	Power_Atk3{ 5 };			// U3iÚÌÐÍ
	const int	Power_JumpAtk1{ 5 };		// WvU1iÚÌÐÍ
	const int	Power_JumpAtk2{ 2 };		// WvU2iÚÌÐÍ
	const int	Power_GuardAtk{ 3 };		// K[hUÌÐÍ

	const int	Wince_Atk1{ 1 };			// U1iÚÌ¯Ýl
	const int	Wince_Atk2{ 1 };			// U2iÚÌ¯Ýl
	const int	Wince_Atk3{ 3 };			// U3iÚÌ¯Ýl
	const int	Wince_JumpAtk1{ 3 };		// WvU1iÚÌ¯Ýl
	const int	Wince_JumpAtk2{ 1 };		// WvU1iÚÌ¯Ýl
	const int	Wince_GuardAtk{ 1 };		// K[hUÌ¯Ýl

	// qbgXgbv
	const float HitStop_Short{ 4.0f };		// Z¢
	const float HitStop_Long{ 8.0f };		// ·¢

	const float EvasionTime{ 30.0f };		// ñðóÔÛt[
	const float InvincibleTime{ 12.0f };	// ³GóÔÛt[
	const float EvasionSpeed{ 1.2f };		// ñðÌÚ®¬x

	// ©®ñ
	const int RecoveryAmount{ 5 };			// ñÊ
	const int RecoveryInterval{ 8 };		// ñÔuibj
};

#endif // !PLAYER_PARAMETER_H_
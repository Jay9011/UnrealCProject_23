#pragma once

enum class DEBUG_NUMS
{
	DEFAULT_INFO,
	STATE,
	MOVEMENT,
	JUST_EVADE,
	GUARD,
	WEAPON_COMPONENT,
	FEET,
};

#define DEBUG_DEFAULT_INFO		(WITH_EDITOR && true)
#define DEBUG_STATE				(WITH_EDITOR && true)
#define DEBUG_MOVEMENT			(WITH_EDITOR && false)
#define DEBUG_JUST_EVADE		(WITH_EDITOR && true)
#define DEBUG_WEAPON_COMPONENT	(WITH_EDITOR && false)
#define DEBUG_ATTACHMENT		(WITH_EDITOR && false)
#define DEBUG_SKILL				(WITH_EDITOR && false)
#define DEBUG_FEET				(WITH_EDITOR && false)
#define DEBUG_HEAD				(WITH_EDITOR && false)
#define DEBUG_GUARD				(WITH_EDITOR && true)
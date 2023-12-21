#pragma once

enum class DEBUG_NUMS
{
	DEFAULT_INFO,
	STATE,
	JUST_EVADE,
	WEAPON_COMPONENT,
	FEET,
};

#define DEBUG_DEFAULT_INFO		(WITH_EDITOR && true)
#define DEBUG_STATE				(WITH_EDITOR && true)
#define DEBUG_JUST_EVADE		(WITH_EDITOR && true)
#define DEBUG_WEAPON_COMPONENT	(WITH_EDITOR && false)
#define DEBUG_ATTACHMENT		(WITH_EDITOR && true)
#define DEBUG_SKILL				(WITH_EDITOR && false)
#define DEBUG_FEET				(WITH_EDITOR && true)
#define DEBUG_HEAD				(WITH_EDITOR && true)
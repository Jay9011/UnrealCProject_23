#pragma once

enum class DEBUG_NUMS
{
	DEFAULT_INFO,
	STATE,
	AI_STATE,
	MOVEMENT,
	JUST_EVADE,
	GUARD,
	WEAPON_COMPONENT,
	FEET,
	AI_Patrol,
	AI_Label,
};

#define DEBUG_DEFAULT_INFO		(WITH_EDITOR && true)
#define DEBUG_STATE				(WITH_EDITOR && true)
#define DEBUG_AI_STATE			(WITH_EDITOR && true)
#define DEBUG_MOVEMENT			(WITH_EDITOR && false)
#define DEBUG_JUST_EVADE		(WITH_EDITOR && false)
#define DEBUG_WEAPON_COMPONENT	(WITH_EDITOR && true)
#define DEBUG_ATTACHMENT		(WITH_EDITOR && false)
#define DEBUG_SKILL				(WITH_EDITOR && false)
#define DEBUG_FEET				(WITH_EDITOR && false)
#define DEBUG_HEAD				(WITH_EDITOR && false)
#define DEBUG_GUARD				(WITH_EDITOR && false)
#define DEBUG_AI_Label			(WITH_EDITOR && false)
#define DEBUG_AI_Patrol			(WITH_EDITOR && false)
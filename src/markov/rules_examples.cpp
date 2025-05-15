#include "rules_examples.hpp"


Rule left_disapear = Rule(
	{	vec3(0, 0, 0),
		vec3(-1, 0, 0) },
	{	BlockType_Default, 
		BlockType_Empty, 
	},
	BlockType_Empty,
	0.05f
);

Rule build_base_from_xm = Rule(
	{ vec3(0, 0, 0),
		vec3(-1, 0, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);

Rule build_base_from_xp = Rule(
	{ vec3(0, 0, 0),
		vec3(1, 0, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);


Rule build_base_from_ym = Rule(
	{ vec3(0, 0, 0),
		vec3(0, -1, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);

Rule build_base_from_yp = Rule(
	{ vec3(0, 0, 0),
		vec3(0, 1, 0) },
	{ BlockType_Empty,
		BlockType_Default },
	BlockType_Default,
	1.0f
);


Rule build_from_under = Rule(
	{ vec3(0, 0, 0),
		vec3(0, 0, -1) },
	{ BlockType_Empty,
	BlockType_Default },
	BlockType_Default,
	1.0f
);


Rule appear_rule = Rule(
	{ vec3(0, 0, 0) },
	{ BlockType_Empty },
	BlockType_Default,
	1.0f
);

MarkovRule keep_only_full = MarkovRule({ left_disapear });
MarkovRule build_base = MarkovRule({ build_base_from_xm, build_base_from_xp, build_base_from_ym, build_base_from_yp });
MarkovRule elevate = MarkovRule({ build_from_under });
MarkovRule appear = MarkovRule({ appear_rule });
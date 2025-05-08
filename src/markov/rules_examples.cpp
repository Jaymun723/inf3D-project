#include "rules_examples.hpp"


Rule left_disapear = Rule(
	{	vec3(0, 0, 0),
		vec3(-1, 0, 0) },
	{	BlockType_Default, 
		BlockType_Empty, 
	},
	BlockType_Default
);


MarkovRule keep_only_full = MarkovRule({ left_disapear });

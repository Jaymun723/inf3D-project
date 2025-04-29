#include "rule.hpp"


Rule::Rule(std::vector<vec3> relative_positions, BlockType result) {
	relative_positions = relative_positions;
	result = result;
	return;
}


bool Rule::applies_to(const vec3& pos) const {
	return true;
}


void Rule::apply(const vec3& pos) const {
	return;
}
#pragma once

/** Comparable so ivec3 can be used in maps*/
struct IVec3Cmp
{
	bool operator()(const glm::ivec3& lhs, const glm::ivec3& rhs) const
	{
		if ((lhs.z < rhs.z)) { return true; }
		if ((lhs.z == rhs.z) && (lhs.y < rhs.y)) { return true; }
		if ((lhs.z == rhs.z) && (lhs.y == rhs.y) && (lhs.x < rhs.x)) { return true; }
	}
};

struct IVec3Equality
{
	bool operator()(const glm::ivec3& lhs, const glm::ivec3& rhs) const
	{
		return lhs == rhs;
	}
};

struct IVec3Hash
{
	size_t operator()(const glm::ivec3 & vec) const
	{
		return (vec.x * 12582917 + vec.z) * 6291469 + vec.y;
	}
};

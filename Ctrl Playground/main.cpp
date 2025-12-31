#include <iostream>
#include "Core/Monument/Monument.h"
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "Core/GreatBath/Logger.h"
#include "Tools/RuhrValley/Profiler.h"
#include "Core/OracleBone/obn.h"

#define TEST_STRING_ONE "this is a test hello"

// vector const

template <typename T>
int64_t profileVectorFind(const std::vector<T>& vec, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	valExists = std::find(vec.begin(), vec.end(), val) != vec.end();
	return 0;
}

template <typename T>
int64_t profileVectorFindManual(const std::vector<T>& vec, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	valExists = false;
	for(size_t i = 0; i < vec.size(); i++)
	{
		if(vec[i] == val)
		{
			valExists = true;
			break;
		}
	}
	return 0;
}

template <typename T>
int64_t profileVectorFindManualCachedSize(const std::vector<T>& vec, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	valExists = false;
	size_t size = vec.size();
	for(size_t i = 0; i < size; i++)
	{
		if(vec[i] == val)
		{
			valExists = true;
			break;
		}
	}
	return 0;
}

template <typename T>
int64_t profileVectorFindIterate(const std::vector<T>& vec, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	for(auto it = vec.begin(); it != vec.end(); it++)
	{
		if(*it == val)
		{
			valExists = true;
			break;
		}
	}
	return 0;
}

template <typename T>
int64_t profileVectorFindIterateCachedEnd(const std::vector<T>& vec, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	auto endIt = vec.end();
	for(auto it = vec.begin(); it != endIt; it++)
	{
		if(*it == val)
		{
			valExists = true;
			break;
		}
	}
	return 0;
}

// sets

template <typename T>
int64_t profileUnorderedSetFind(const std::unordered_set<T>& set, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	valExists = set.count(val) != 0;
	return 0;
}

template <typename T>
int64_t profileSetFind(const std::set<T>& set, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	valExists = set.count(val) != 0;
	return 0;
}

// map

template <typename T, typename C>
int64_t profileUnorderedMapFind(const std::unordered_map<T, C>& map, const T& val, bool& valExists)
{
	PROFILE_SCOPED();
	valExists = map.find(val) != map.end();
	return 0;
}

// vector string non-const

int64_t profileVectorStringPush(std::vector<std::string>& vec, int numVals)
{
	vec.clear();
	vec.shrink_to_fit();
	PROFILE_SCOPED();
	for(int i = 0; i < numVals; i++)
	{
		vec.push_back(std::to_string(i));
	}
	return 0;
}

int64_t profileVectorStringInsertSlow(std::vector<std::string>& vec, int numVals)
{
	vec.clear();
	vec.shrink_to_fit();
	PROFILE_SCOPED();
	for(int i = numVals; i >= 0; i--)
	{
		vec.insert(vec.begin(), std::to_string(i));
	}
	return 0;
}

int64_t profileVectorStringReservePush(std::vector<std::string>& vec, int numVals)
{
	vec.clear();
	vec.shrink_to_fit();
	PROFILE_SCOPED();
	vec.reserve(numVals);
	for(int i = 0; i < numVals; i++)
	{
		vec.push_back(std::to_string(i));
	}
	return 0;
}

int64_t profileVectorStringResizeAssign(std::vector<std::string>& vec, int numVals)
{
	vec.clear();
	vec.shrink_to_fit();
	PROFILE_SCOPED();
	vec.resize(numVals);
	for(int i = 0; i < numVals; i++)
	{
		vec[i] = std::to_string(numVals);
	}
	
	return 0;
}

int64_t profileVectorStringResizeMove(std::vector<std::string>& vec, int numVals)
{
	vec.clear();
	vec.shrink_to_fit();
	PROFILE_SCOPED();
	vec.resize(numVals);
	for(int i = 0; i < numVals; i++)
	{
		vec[i] = std::move(std::to_string(numVals));
	}
	
	return 0;
}

int64_t profileVectorStringInsertOneSlow(std::vector<std::string>& vec)
{
	PROFILE_SCOPED();
	vec.insert(vec.begin(), TEST_STRING_ONE);
	
	return 0;
}

int64_t profileVectorStringEraseOneSlow(std::vector<std::string>& vec)
{
	PROFILE_SCOPED();
	vec.erase(vec.begin(), vec.begin() + 1);
	
	return 0;
}

// unordered_set string non-const

int64_t profileUnorderedSetStringFromInsert(std::unordered_set<std::string>& set, int numVals)
{
	set.clear();
	PROFILE_SCOPED();
	for(int i = 0; i < numVals; i++)
	{
		set.insert(std::to_string(i));
	}
	
	return 0;
}

template <typename T>
int64_t profileUnorderedSetFromVec(std::unordered_set<T>& set, const std::vector<T>& vec)
{
	set.clear();
	PROFILE_SCOPED();
	set = std::unordered_set<T>(vec.begin(), vec.end());
	
	return 0;
}

// set string non-const

int64_t profileSetStringFromInsert(std::set<std::string>& set, int numVals)
{
	set.clear();
	PROFILE_SCOPED();
	for(int i = 0; i < numVals; i++)
	{
		set.insert(std::to_string(i));
	}
	
	return 0;
}

template <typename T>
int64_t profileSetFromVec(std::set<T>& set, const std::vector<T>& vec)
{
	set.clear();
	PROFILE_SCOPED();
	set = std::set<T>(vec.begin(), vec.end());
	
	return 0;
}

// map string non-const

int64_t profileUnorderedMapStringFromInsert(std::unordered_map<std::string, int>& map, int numVals)
{
	map.clear();
	PROFILE_SCOPED();
	for(int i = 0; i < numVals; i++)
	{
		map[std::to_string(i)] = i;
	}
	
	return 0;
}

// string find(std and my own impl)

int64_t profileStdStringFind(const std::string& str, const std::vector<std::string>& substrs)
{
	PROFILE_SCOPED_PRECISION(rvl::SunDialPrecision::SUNDIALPRECISION_MICROSECONDS);
	for(const std::string& substr : substrs)
	{
		volatile size_t index = str.find(substr);
		volatile size_t otherindex = str.rfind(substr);
		//LOG_MSG_QUEUE("std string find {}, {}", (int)index, (int)otherindex);
	}

	return 0;
}

int64_t profileStringUtilFind(const std::string& str, const std::vector<std::string>& substrs)
{
	PROFILE_SCOPED_PRECISION(rvl::SunDialPrecision::SUNDIALPRECISION_MICROSECONDS);
	for(const std::string& substr : substrs)
	{
		volatile size_t index = obn::string_nfind(str.c_str(), str.length(), substr.c_str(), substr.length());
		volatile size_t otherindex = obn::string_nrfind(str.c_str(), str.length(), substr.c_str(), substr.length());
		//LOG_MSG_QUEUE("string util find {}, {}", (int)index, (int)otherindex);
	}

	return 0;
}

int64_t profileStdStringEq(const std::vector<std::string>& strs)
{
	PROFILE_SCOPED_PRECISION(rvl::SunDialPrecision::SUNDIALPRECISION_MICROSECONDS);
	for(const std::string& str : strs)
	{
		for(const std::string& other : strs)
		{
			volatile bool equals = str == other;
		}
	}

	return 0;
}

int64_t profileStringUtilCmp(const std::vector<std::string>& strs)
{
	PROFILE_SCOPED_PRECISION(rvl::SunDialPrecision::SUNDIALPRECISION_MICROSECONDS);
	for(const std::string& str : strs)
	{
		for(const std::string& other : strs)
		{
			volatile bool equals = obn::string_ncmp(str.c_str(), other.c_str(), str.length());
		}
	}

	return 0;
}

int64_t profileStringUtilEq(const std::vector<std::string>& strs)
{
	PROFILE_SCOPED_PRECISION(rvl::SunDialPrecision::SUNDIALPRECISION_MICROSECONDS);
	for(const std::string& str : strs)
	{
		for(const std::string& other : strs)
		{
			volatile bool equals = obn::string_neq(str.c_str(), other.c_str(), str.length());
		}
	}

	return 0;
}

int64_t profileStdStringFindOf(const std::string& str, const std::vector<std::string>& charsets)
{
	PROFILE_SCOPED_PRECISION(rvl::SunDialPrecision::SUNDIALPRECISION_MICROSECONDS);
	for(const std::string& charset : charsets)
	{
		volatile size_t firstOf = str.find_first_of(charset);
		volatile size_t lastOf = str.find_last_of(charset);
		volatile size_t firstNotOf = str.find_first_not_of(charset);
		volatile size_t lastNotOf = str.find_last_not_of(charset);
		//LOG_MSG_QUEUE("std string find of {}, {}, {}, {}", (int)firstOf, (int)lastOf, (int)firstNotOf, (int)lastNotOf);
	}

	return 0;
}

int64_t profileStringUtilFindOf(const std::string& str, const std::vector<std::string>& charsets)
{
	PROFILE_SCOPED_PRECISION(rvl::SunDialPrecision::SUNDIALPRECISION_MICROSECONDS);
	for(const std::string& charset : charsets)
	{
		volatile size_t firstOf = obn::string_nfind_first_of(str.c_str(), str.length(), charset.c_str(), charset.length());
		volatile size_t lastOf = obn::string_nfind_last_of(str.c_str(), str.length(), charset.c_str(), charset.length());
		volatile size_t firstNotOf = obn::string_nfind_first_not_of(str.c_str(), str.length(), charset.c_str(), charset.length());
		volatile size_t lastNotOf = obn::string_nfind_last_not_of(str.c_str(), str.length(), charset.c_str(), charset.length());
		//LOG_MSG_QUEUE("string util find of {}, {}, {}, {}", (int)firstOf, (int)lastOf, (int)firstNotOf, (int)lastNotOf);
	}

	return 0;
}

void profileAll()
{
	int numElements = 10000;
	// ---------------- make vec

	std::vector<std::string> vec;
	int64_t vecpushus = profileVectorStringPush(vec, numElements);

	//very slow dun do this pls
	//std::vector<std::string> vec2;
	//int64_t vecinsertus = profileVectorStringInsertSlow(vec2, numElements);
	//std::cout << "Finished insert to vector for " << numElements << " elements in " << vecinsertus << "us" << std::endl;

	std::vector<std::string> vec3;
	int64_t vecreservepushus = profileVectorStringReservePush(vec3, numElements);

	std::vector<std::string> vec4;
	int64_t vecresizeassignus = profileVectorStringResizeAssign(vec4, numElements);

	std::vector<std::string> vec5;
	int64_t vecresizemoveus = profileVectorStringResizeMove(vec5, numElements);

	// ----------------- make uset

	std::unordered_set<std::string> uset;
	int64_t usetfrominsertus = profileUnorderedSetStringFromInsert(uset, numElements);

	std::unordered_set<std::string> uset2;
	int64_t usetfromvecus = profileUnorderedSetFromVec(uset2, vec);

	// ------------------ make set

	std::set<std::string> set;
	int64_t setfrominsertus = profileSetStringFromInsert(set, numElements);

	std::set<std::string> set2;
	int64_t setfromvecus = profileSetFromVec(set2, vec);

	// ------------------ make unordered map

	std::unordered_map<std::string, int> umap;
	int64_t umapfrominsertus = profileUnorderedMapStringFromInsert(umap, numElements);

	// -------------- misc vec stuff

	int64_t vecinsertoneus = profileVectorStringInsertOneSlow(vec);

	int64_t veceraseoneus = profileVectorStringEraseOneSlow(vec);

	// --------------  vec stuff

	bool found;
	std::string stringElement;
	stringElement = std::to_string(numElements - 1);
	int64_t vecfindslowus = profileVectorFind(vec, stringElement, found);

	int64_t vecfindslowmanualus = profileVectorFindManual(vec, stringElement, found);

	int64_t vecfindslowmanualcachedus = profileVectorFindManualCachedSize(vec, stringElement, found);

	int64_t vecfindslowiterus = profileVectorFindIterate(vec, stringElement, found);

	int64_t vecfindslowitercachedus = profileVectorFindIterateCachedEnd(vec, stringElement, found);

	// ------------- unordered set stuff

	int64_t usetfindus = profileUnorderedSetFind(uset, stringElement, found);
	// ------------- set stuff

	int64_t setfindus = profileSetFind(set, stringElement, found);
	// ------------- map stuff

	int64_t umapfindus = profileUnorderedMapFind(umap, stringElement, found);

	// ------------- string util stuff
	
	std::string haystack = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";
	std::vector<std::string> needles = { "sit", "laborum", "sunt in culpa qui officia deserunt mollit anim id est laborum", "reprehenderit", "", haystack, "Ut enim ad minim veniam" };
	int64_t stdstrfindus = profileStdStringFind(haystack, needles);
	int64_t strutilfindus = profileStringUtilFind(haystack, needles);
	int64_t stdstrequs = profileStdStringEq(needles);
	int64_t strutilncmpus = profileStringUtilCmp(needles);
	int64_t strutilnequs = profileStringUtilEq(needles);

	std::string example = "Hey don't think too hard this is just an example + * / \\ yup please this will be a ok. Just look the other way.";
	std::vector<std::string> charsets = { "", "\\+", "*/", " " };
	profileStdStringFindOf(example, charsets);
	profileStringUtilFindOf(example, charsets);
}

template <typename stringtype>
void testString(const c_string title, const stringtype& string)
{
	LOG_MSG_PUSH("{} max length {}, capacity {}, struct size {}, length {}, data \"{}\"", title, string.max_length(), string.capacity(), sizeof(string), string.length(), string.data());
}

enum class flags : int
{
	F1 = 1 << 0,
	F2 = 1 << 1,
	F3 = 1 << 2,
};
//*
ENUM_CLASS_FULL_OP_GEN(flags);
//*/

int main(int argc, char *argv[])
{
    REGISTER_LOGGING_STREAM(gbt::LOGLEVEL_PROFILE, std::cout);
    REGISTER_FILE_FOR_LOGGING("logs/playground.log");

	PROFILE_SECTION_START(allTests);
	profileAll();
	PROFILE_SECTION_END(allTests);

	LOG_TRACE_PUSH("{:}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));
	LOG_MSG_PUSH("Hello");
	LOG_WARNING_PUSH("Uh Oh");
	LOG_ERROR_PUSH("REEEEE");
	LOG_FATAL_PUSH("Ded");
	LOG_FLUSH();

    return 0;
}
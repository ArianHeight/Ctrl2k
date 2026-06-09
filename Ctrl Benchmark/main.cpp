#include <iostream>
#include <random>
#include "Core/Monument/Monument.h"
#include "Core/GreatBath/Logger.h"
#include "Tools/RuhrValley/Profiler.h"

constexpr rvl::TimePrecision BENCHMARKING_PRECISION = rvl::TimePrecision::MICROSECONDS;

void generateRandomInts(size_t numInts, int lower, int upper, std::vector<int>& out)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> distr(lower, upper);

    out.clear();
    out.reserve(numInts);
    for(size_t i = 0; i < numInts; i++)
    {
        out.push_back(distr(gen));
    }
}

void generateSortedInts(size_t numInts, int lower, int upper, std::vector<int>& out)
{
    generateRandomInts(numInts, lower, upper, out);
    std::sort(out.begin(), out.end());
}

void generateReverseSortedInts(size_t numInts, int lower, int upper, std::vector<int>& out)
{
    generateRandomInts(numInts, lower, upper, out);
    std::sort(out.rbegin(), out.rend());
}

void generateAlmostSortedInts(size_t numInts, size_t num_outliers, int lower, int upper, std::vector<int>& out)
{
    generateSortedInts(numInts, lower, upper, out);

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> indexDistr(0, (int)numInts - 1);
    std::uniform_int_distribution<> distr(lower, upper);

    for(size_t i = 0; i < num_outliers; i++)
    {
        out[indexDistr(gen)] = distr(gen);
    }
}

#define CHECK_VECTOR_SORTED(arr) \
for(int _CHECK_INDEX = 1; _CHECK_INDEX < arr.size(); _CHECK_INDEX++)\
{\
	if(arr[_CHECK_INDEX - 1] > arr[_CHECK_INDEX])\
	{\
		LOG_ERROR_QUEUE("Sorting failed, out of order elements detected...");\
	}\
}

#define CHECK_VECTOR_REVERSE_SORTED(arr) \
for(int _CHECK_INDEX = 1; _CHECK_INDEX < arr.size(); _CHECK_INDEX++)\
{\
	if(arr[_CHECK_INDEX - 1] < arr[_CHECK_INDEX])\
	{\
		LOG_ERROR_QUEUE("Sorting failed, out of order elements detected...");\
	}\
}

struct ReverseCompare
{
	ReverseCompare() = default;

	inline bool operator()(int a, int b)
	{
		return b < a;
	}
};

void benchMarkSelectionSort(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		selection_sort(arr.data(), arr.size());
	}

	CHECK_VECTOR_SORTED(arr);
}

void benchMarkInsertionSort(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		insertion_sort(arr.data(), arr.size());
	}

	CHECK_VECTOR_SORTED(arr);
}

void benchMarkInsertionSortReverse(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		insertion_sort(arr.data(), arr.size(), ReverseCompare());
	}

	CHECK_VECTOR_REVERSE_SORTED(arr);
}

void benchMarkStdSort(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		std::sort(arr.begin(), arr.end());
	}

	CHECK_VECTOR_SORTED(arr);
}

void benchMarkQuickSort(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		quick_sort(arr.data(), arr.size());
	}

	CHECK_VECTOR_SORTED(arr);
}

void benchMarkMergeSort(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		merge_sort(arr.data(), arr.size());
	}

	CHECK_VECTOR_SORTED(arr);
}

void benchMarkCtrlSort(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		sort(arr.data(), arr.size());
	}

	CHECK_VECTOR_SORTED(arr);
}

void benchMarkCtrlSortReverse(std::vector<int> arr)
{
	{
		BENCHMARK_SCOPED_PRECISION(BENCHMARKING_PRECISION);
		sort(arr.data(), arr.size(), ReverseCompare());
	}

	CHECK_VECTOR_REVERSE_SORTED(arr);
}

void benchMarkRound(const std::vector<int>& sortingData)
{
	//benchMarkSelectionSort(sortingData);
	//benchMarkInsertionSort(sortingData);
	benchMarkStdSort(sortingData);
	benchMarkQuickSort(sortingData);
	benchMarkMergeSort(sortingData);
	benchMarkCtrlSort(sortingData);
	benchMarkCtrlSortReverse(sortingData);
}

void benchMarkSmallInputs()
{
	LOG_MSG_QUEUE("Small Inputs--------------------------------------------");

    std::vector<int> sortingData;
    for(int i = 0; i < rvl::BENCHMARKER_MAX_STORAGE; i++)
    {
        generateRandomInts(128, 0, 256, sortingData);
        benchMarkInsertionSort(sortingData);
        benchMarkInsertionSortReverse(sortingData);
        benchMarkSelectionSort(sortingData);
    }

    BENCHMARK_LOG_RESULTS();
}

void benchMarkRandomInputs()
{
	LOG_MSG_QUEUE("Random Inputs--------------------------------------------");

	std::vector<int> sortingData;
	for(int i = 0; i < rvl::BENCHMARKER_MAX_STORAGE; i++)
	{
		generateRandomInts(5751, 0, 4096 << 1, sortingData);
		benchMarkRound(sortingData);
	}

	BENCHMARK_LOG_RESULTS();
}

void benchMarkLowCardinality()
{
	LOG_MSG_QUEUE("Low Cardinality--------------------------------------------");

	std::vector<int> sortingData;
	for(int i = 0; i < rvl::BENCHMARKER_MAX_STORAGE; i++)
	{
		generateRandomInts(5456, 0, 128, sortingData);
		benchMarkRound(sortingData);
	}

	BENCHMARK_LOG_RESULTS();
}

void benchMarkSortedInputs()
{
	LOG_MSG_QUEUE("Sorted Inputs--------------------------------------------");

	std::vector<int> sortingData;
	for(int i = 0; i < rvl::BENCHMARKER_MAX_STORAGE; i++)
	{
		generateSortedInts(5579, 0, 4096 << 1, sortingData);
		benchMarkRound(sortingData);
	}

	BENCHMARK_LOG_RESULTS();
}

void benchMarkReverseSortedInputs()
{
	LOG_MSG_QUEUE("Reverse Sorted Inputs--------------------------------------------");

	std::vector<int> sortingData;
	for(int i = 0; i < rvl::BENCHMARKER_MAX_STORAGE; i++)
	{
		generateReverseSortedInts(5724, 0, 4096 << 1, sortingData);
		benchMarkRound(sortingData);
	}

	BENCHMARK_LOG_RESULTS();
}

void benchMarkAlmostSortedInputs()
{
	LOG_MSG_QUEUE("Almost Sorted Inputs--------------------------------------------");

	std::vector<int> sortingData;
	for(int i = 0; i < rvl::BENCHMARKER_MAX_STORAGE; i++)
	{
		generateAlmostSortedInts(5291, 200, 0, 4096 << 1, sortingData);
		benchMarkRound(sortingData);
	}

	BENCHMARK_LOG_RESULTS();
}

void benchMarkSorting()
{
	LOG_MSG_QUEUE("Benchmarking sorting functions");

	benchMarkSmallInputs();
	benchMarkRandomInputs();
	benchMarkLowCardinality();
	benchMarkSortedInputs();
	benchMarkReverseSortedInputs();
	benchMarkAlmostSortedInputs();
}

int main(int argc, char* argv[])
{
    REGISTER_LOGGING_STREAM_VERBOSITY(gbt::LOGLEVEL_PROFILE, gbt::LOGVERBOSITY_LOW, std::cout);
    REGISTER_FILE_FOR_LOGGING("logs/benchmark_text.log");
    gbt::LoggingStreamSettings benchmarkSettings;
    benchmarkSettings.levelFlags = (gbt::LogLevelFlag)(gbt::LogLevelFlag::LOGLEVELFLAG_PROFILE | gbt::LogLevelFlag::LOGLEVELFLAG_MSG);
    benchmarkSettings.useLogTime = gbt::LogTime::LOGTIME_NONE;
    benchmarkSettings.usePrefix = gbt::LogPrefix::LOGPREFIX_NONE;
    benchmarkSettings.logFullPath = false;
    benchmarkSettings.showFile = false;
    benchmarkSettings.showLineNumber = false;
    benchmarkSettings.showTextColour = false;
    benchmarkSettings.showThreadId = false;
	REGISTER_FILE_FOR_LOGGING_CUSTOM(benchmarkSettings, "logs/benchmark_data.log");

    benchMarkSorting();

    LOG_FLUSH();

    return 0;
}

// test_stats.cpp
#include <sstream>   // For std::stringstream to capture cout
#include <iostream>  // For std::streambuf, std::cout
#include <string>    // For std::string
#include <memory>    // For std::unique_ptr
#include "gtest/gtest.h"
#include "Stats.hpp"

// Helper class to redirect std::cout to a std::stringstream
class CoutRedirector {
public:
    CoutRedirector(std::streambuf* new_buffer)
        : old_buffer_(std::cout.rdbuf(new_buffer)) {}

    ~CoutRedirector() {
        std::cout.rdbuf(old_buffer_); // Restore original buffer
    }

private:
    std::streambuf* old_buffer_;

    // Prevent copying and assignment
    CoutRedirector(const CoutRedirector&) = delete;
    CoutRedirector& operator=(const CoutRedirector&) = delete;
};

// Test fixture for the stats class
class StatsTest : public ::testing::Test {
protected:
    life::Stats s; // Instance of the class under test
    std::stringstream captured_cout_stream; // Stream to capture cout
    std::unique_ptr<CoutRedirector> cout_redirector_ptr; // RAII redirector

    // Per-test-case set-up
    void SetUp() override {
        // Redirect std::cout to our stringstream before each test
        cout_redirector_ptr = std::make_unique<CoutRedirector>(captured_cout_stream.rdbuf());
    }

    // Per-test-case tear-down
    void TearDown() override {
        // cout_redirector_ptr will be destroyed, restoring std::cout
        cout_redirector_ptr.reset();
        captured_cout_stream.str(""); // Clear the stream's content
        captured_cout_stream.clear(); // Clear any error flags
    }

    // Helper to get the captured output as a string
    std::string getCapturedOutput() {
        return captured_cout_stream.str();
    }
};

// Test case: Printing when no statistics have been recorded
TEST_F(StatsTest, Print_NoStats) {
    s.print();
    const std::string output = getCapturedOutput();
    // Based on the assumed print(), if counters is empty, it prints "Statistics:\n" and returns.
    EXPECT_EQ(output, "Statistics:\n");
}

// Test case: Basic start and stop for a single counter
TEST_F(StatsTest, StartStop_SingleCounter) {
    s.start("TestCounter", 100);
    s.stop("TestCounter", 150);
    s.print();
    std::string output = getCapturedOutput();
    std::string expected_output = "Statistics:\nTestCounter: 50 ms\n";
    EXPECT_EQ(output, expected_output);
}

// Test case: Multiple counters are recorded and printed correctly
// std::map sorts keys, so output order is predictable.
TEST_F(StatsTest, StartStop_MultipleCounters) {
    s.start("CounterB", 50); // Intentionally add out of alphabetical order
    s.stop("CounterB", 150);  // Duration 100
    s.start("CounterA", 10);
    s.stop("CounterA", 30);   // Duration 20

    s.print();
    std::string output = getCapturedOutput();
    // Expected output is sorted by counter name (CounterA, then CounterB)
    std::string expected_output = "Statistics:\nCounterA: 20 ms\nCounterB: 100 ms\n";
    EXPECT_EQ(output, expected_output);
}

// Test case: Calling start() multiple times for the same counter (last call wins)
TEST_F(StatsTest, Start_Overwrite) {
    s.start("OverwriteTest", 10);
    s.start("OverwriteTest", 20); // This start time should be used
    s.stop("OverwriteTest", 50);
    s.print();
    std::string output = getCapturedOutput();
    std::string expected_output = "Statistics:\nOverwriteTest: 30 ms\n"; // 50 - 20
    EXPECT_EQ(output, expected_output);
}

// Test case: Calling stop() multiple times for the same counter (last call wins)
TEST_F(StatsTest, Stop_Overwrite) {
    s.start("OverwriteTest", 10);
    s.stop("OverwriteTest", 50);
    s.stop("OverwriteTest", 60); // This stop time should be used
    s.print();
    std::string output = getCapturedOutput();
    std::string expected_output = "Statistics:\nOverwriteTest: 50 ms\n"; // 60 - 10
    EXPECT_EQ(output, expected_output);
}

// Test case: stop() is called before start()
// Assumes stat members are default-initialized to 0 (as per revised stats.h)
TEST_F(StatsTest, StopBeforeStart) {
    s.stop("BeforeStart", 50);  // counter.start = 0, counter.stop = 50
    s.start("BeforeStart", 10); // counter.start = 10, counter.stop remains 50
    s.print();
    std::string output = getCapturedOutput();
    std::string expected_output = "Statistics:\nBeforeStart: 40 ms\n"; // 50 - 10
    EXPECT_EQ(output, expected_output);
}

// Test case: start() is called, but stop() is not
// Assumes stat members are default-initialized to 0.
// stop - start will be 0 - start_time, resulting in Uint64 underflow.
TEST_F(StatsTest, StartWithoutStop) {
    s.start("NotStopped", 10); // counter.start = 10, counter.stop = 0
    s.print();
    std::string output = getCapturedOutput();

    // Calculate expected underflow result for Uint64 (uint64_t)
    Uint64 expected_diff = static_cast<Uint64>(0) - static_cast<Uint64>(10);

    std::stringstream ss_expected;
    ss_expected << "Statistics:\nNotStopped: " << expected_diff << " ms\n";
    EXPECT_EQ(output, ss_expected.str());
    // This test highlights that the simple print() can produce confusing output for this case.
    // A production-ready print() might show "(not stopped)" or similar.
}

// Test case: stop() is called, but start() is not (or start was effectively 0)
// Assumes stat members are default-initialized to 0.
TEST_F(StatsTest, StopWithoutStart_Explicit) {
    s.stop("NotStarted", 50); // counter.start = 0, counter.stop = 50
    s.print();
    std::string output = getCapturedOutput();
    std::string expected_output = "Statistics:\nNotStarted: 50 ms\n"; // 50 - 0
    EXPECT_EQ(output, expected_output);
}

// Test case: Using the predefined string_view constants
TEST_F(StatsTest, PredefinedNames) {
    // Convert string_view to string for map keys
    s.start(std::string(life::ITERATE), 1000);
    s.stop(std::string(life::ITERATE), 1050);   // Duration 50

    s.start(std::string(life::PRESENT), 2000);
    s.stop(std::string(life::PRESENT), 2080);   // Duration 80

    s.start(std::string(life::CALL), 3000);     // Not stopped
    // life::CALL: stop = 0, start = 3000. Diff = 0 - 3000 (underflow)

    s.print();
    std::string output = getCapturedOutput();

    std::stringstream ss_expected;
    ss_expected << "Statistics:\n";
    // std::map sorts keys: "Call", "Iterate", "Present"
    Uint64 call_diff = static_cast<Uint64>(0) - static_cast<Uint64>(3000);
    ss_expected << std::string(life::CALL) << ": " << call_diff << " ms\n";
    ss_expected << std::string(life::ITERATE) << ": 50 ms\n";
    ss_expected << std::string(life::PRESENT) << ": 80 ms\n";

    EXPECT_EQ(output, ss_expected.str());
}

// You would typically have a main function in a separate file or at the end
// of your test_*.cpp file to run the tests.
// Example:
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
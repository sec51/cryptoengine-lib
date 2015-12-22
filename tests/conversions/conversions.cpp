//
// Created by silenteh on 02/11/15.
//


#include <limits>

#include "gtest/gtest.h"
#include "cryptoengine_conversions.h"

namespace {

    class ConversionsTest : public ::testing::Test {

// The fixture for testing class Foo.
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        ConversionsTest() {
            // You can do set-up work for each test here.
        }

        virtual ~ConversionsTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for HKDF.
    };


    // Test int32_t BigEndian conversions
    TEST(Conversions, Int32BigEndian) {

        std::cout << "=========================================" << std::endl;
        // Zero
        int32_t zero(0);
        std::array<unsigned char, INT_SIZE> data = Conversions::BigEndianToInt(zero);
        int32_t back_zero = Conversions::BigEndianFromInt(data);
        std::cout << "Converted " << zero << " to BigEndian and back: " << back_zero << std::endl;
        ASSERT_EQ(back_zero, zero);

        // Zero
        int32_t small(79);
        data = Conversions::BigEndianToInt(small);
        int32_t back_small = Conversions::BigEndianFromInt(data);
        std::cout << "Converted " << small << " to BigEndian and back: " << back_small << std::endl;
        ASSERT_EQ(back_small, small);


        // MAX
        int32_t max(std::numeric_limits<int32_t >::max());
        data = Conversions::BigEndianToInt(max);
        int32_t back_max = Conversions::BigEndianFromInt(data);
        std::cout << "Converted " << max << " to BigEndian and back: " << back_max << std::endl;
        ASSERT_EQ(back_max, max);

        // MIN
        int32_t min(std::numeric_limits<int32_t >::min());
        data = Conversions::BigEndianToInt(min);
        int32_t back_min = Conversions::BigEndianFromInt(data);
        std::cout << "Converted " << min << " to BigEndian and back: " << back_min << std::endl;
        ASSERT_EQ(back_min, min);

        // Testing overflow
        int32_t over(std::numeric_limits<uint32_t >::max());
        data = Conversions::BigEndianToInt(over);
        int32_t back_over = Conversions::BigEndianFromInt(data);
        std::cout << "Converted " << over << " to BigEndian and back: " << back_over << std::endl;
        ASSERT_EQ(back_over, over);

        std::cout << "=========================================" << std::endl;

    }

    // Test int32_t BigEndian conversions
    TEST(Conversions, Uint64BigEndian) {

        std::cout << "=========================================" << std::endl;
        // Zero
        uint64_t zero(0);
        std::array<unsigned char, UINT64_SIZE> data = Conversions::BigEndianToUint64(zero);
        uint64_t back_zero = Conversions::BigEndianFromUint(data);
        std::cout << "Converted " << zero << " to BigEndian and back: " << back_zero << std::endl;
        ASSERT_EQ(back_zero, zero);

        // Small
        uint64_t small(79);
        data = Conversions::BigEndianToUint64(small);
        uint64_t back_small = Conversions::BigEndianFromUint(data);
        std::cout << "Converted " << small << " to BigEndian and back: " << back_small << std::endl;
        ASSERT_EQ(back_small, small);


        // MAX
        uint64_t max(std::numeric_limits<uint64_t >::max());
        data = Conversions::BigEndianToUint64(max);
        uint64_t back_max = Conversions::BigEndianFromUint(data);
        std::cout << "Converted " << max << " to BigEndian and back: " << back_max << std::endl;
        ASSERT_EQ(back_max, max);

        // MIN
        uint64_t min(std::numeric_limits<uint64_t >::min());
        data = Conversions::BigEndianToUint64(min);
        uint64_t back_min = Conversions::BigEndianFromUint(data);
        std::cout << "Converted " << min << " to BigEndian and back: " << back_min << std::endl;
        ASSERT_EQ(back_min, min);

        // Testing overflow
        uint64_t over(std::numeric_limits<uint64_t >::max());
        data = Conversions::BigEndianToUint64(over);
        uint64_t back_over = Conversions::BigEndianFromUint(data);
        std::cout << "Converted " << over << " to BigEndian and back: " << back_over << std::endl;
        ASSERT_EQ(back_over, over);

        std::cout << "=========================================" << std::endl;

    }

    // ==================================================================

    // Test int32_t SmallEndian conversions
    TEST(Conversions, Int32SmallEndian) {

        std::cout << "=========================================" << std::endl;
        // Zero
        int32_t zero(0);
        std::array<unsigned char, INT_SIZE> data = Conversions::SmallEndianToInt(zero);
        int32_t back_zero = Conversions::SmallEndianFromInt(data);
        std::cout << "Converted " << zero << " to SmallEndian and back: " << back_zero << std::endl;
        ASSERT_EQ(back_zero, zero);


        // SMALL
        int32_t small(79);
        data = Conversions::SmallEndianToInt(small);
        int32_t back_small = Conversions::SmallEndianFromInt(data);
        std::cout << "Converted " << small << " to SmallEndian and back: " << back_small << std::endl;
        ASSERT_EQ(back_small, small);


        // MAX
        int32_t max(std::numeric_limits<int32_t >::max());
        data = Conversions::SmallEndianToInt(max);
        int32_t back_max = Conversions::SmallEndianFromInt(data);
        std::cout << "Converted " << max << " to SmallEndian and back: " << back_max << std::endl;
        ASSERT_EQ(back_max, max);

        // MIN
        int32_t min(std::numeric_limits<int32_t >::min());
        data = Conversions::SmallEndianToInt(min);
        int32_t back_min = Conversions::SmallEndianFromInt(data);
        std::cout << "Converted " << min << " to SmallEndian and back: " << back_min << std::endl;
        ASSERT_EQ(back_min, min);

        // Testing overflow
        int32_t over(std::numeric_limits<uint32_t >::max());
        data = Conversions::SmallEndianToInt(over);
        int32_t back_over = Conversions::SmallEndianFromInt(data);
        std::cout << "Converted " << over << " to SmallEndian and back: " << back_over << std::endl;
        ASSERT_EQ(back_over, over);

        std::cout << "=========================================" << std::endl;

    }

    // Test int32_t SmallEndian conversions
    TEST(Conversions, Uint64SmallEndian) {

        std::cout << "=========================================" << std::endl;
        // Zero
        uint64_t zero(0);
        std::array<unsigned char, UINT64_SIZE> data = Conversions::SmallEndianToUint64(zero);
        uint64_t back_zero = Conversions::SmallEndianFromUint(data);
        std::cout << "Converted " << zero << " to SmallEndian and back: " << back_zero << std::endl;
        ASSERT_EQ(back_zero, zero);

        // SMALL
        uint64_t small(79);
        data = Conversions::SmallEndianToUint64(small);
        uint64_t back_small = Conversions::SmallEndianFromUint(data);
        std::cout << "Converted " << small << " to SmallEndian and back: " << back_small << std::endl;
        ASSERT_EQ(back_small, small);

        // MAX
        uint64_t max(std::numeric_limits<uint64_t >::max());
        data = Conversions::SmallEndianToUint64(max);
        uint64_t back_max = Conversions::SmallEndianFromUint(data);
        std::cout << "Converted " << max << " to BigEndian and back: " << back_max << std::endl;
        ASSERT_EQ(back_max, max);

        // MIN
        uint64_t min(std::numeric_limits<uint64_t >::min());
        data = Conversions::SmallEndianToUint64(min);
        uint64_t back_min = Conversions::SmallEndianFromUint(data);
        std::cout << "Converted " << min << " to SmallEndian and back: " << back_min << std::endl;
        ASSERT_EQ(back_min, min);

        // Testing overflow
        uint64_t over(std::numeric_limits<uint64_t >::max());
        data = Conversions::SmallEndianToUint64(over);
        uint64_t back_over = Conversions::SmallEndianFromUint(data);
        std::cout << "Converted " << over << " to SmallEndian and back: " << back_over << std::endl;
        ASSERT_EQ(back_over, over);

        std::cout << "=========================================" << std::endl;

    }

}
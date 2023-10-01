#include <util/delay.h>
#include <unity.h>

#include <fifo.h>
#include <fifoCircular.h>

void test_normal_flow(void)
{
    // 1 Setup
    Fifo f;
    // FifoCircular f;
    f.put(1);
    f.put(2);
    f.get();
    f.put(3);

    // 2-3 Execute and validate
    TEST_ASSERT_EQUAL(2, f.get());
    TEST_ASSERT_EQUAL(3, f.get());

    // 4 Cleanup
}

void test_underflow(void)
{
    // 1 Setup
    Fifo f;
    // FifoCircular f;
    f.put(1);
    f.put(2);
    f.get();
    f.get();

    // 2-3 Execute and validate
    TEST_ASSERT_TRUE(f.is_empty());

    // 4 Cleanup
}

void test_reset(void)
{
    // FifoCircular f;
    Fifo f;
    f.put(1);
    f.put(2);
    f.reset();
    TEST_ASSERT_TRUE(f.is_empty());
}

void test_overflow(void)
{
    // FifoCircular f;
    Fifo f;
    f.put(1);
    f.put(2);
    f.put(3);
    f.put(4);
    f.put(5);
    TEST_ASSERT_TRUE(f.is_full());
}

void test_overwritten(void)
{
    // FifoCircular f;
    Fifo f;
    f.put(1);
    f.put(2);
    f.put(3);
    f.put(4);
    f.put(5);
    f.put(6);

    f.get();
    f.get();
    f.get();
    f.get();
    f.get();

    TEST_ASSERT_TRUE(f.is_empty());
}

int main()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    _delay_ms(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!

    RUN_TEST(test_normal_flow); //normal flow
    RUN_TEST(test_underflow);   //underflow
    RUN_TEST(test_reset);  //reset case
    RUN_TEST(test_overflow);
    RUN_TEST(test_overwritten);

    UNITY_END(); // stop unit testing
}
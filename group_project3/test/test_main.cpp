#include <util/delay.h>
#include <unity.h>
#include "speed_controller_integrated.h"

void test_normal_flow(void)
{
    double kp = 0.001;
    double target = 5.0;
    PIController controller(kp, target, 0.021, 0.02);
    // PController controller(kp, target);
    
    double actual = 6;
    double control_signal = controller.updatePwm(actual);

    // The control signal should be within [0, 1]
    TEST_ASSERT_TRUE((control_signal > 0.0));
    TEST_ASSERT_TRUE((control_signal < 1.0));
}

void test_anti_windup(void)
{
    double kp = 0.001;
    double target = 5.0;
    PIController controller(kp, target, 0.021, 0.02);
    // PController controller(kp, target);

    double actual = 3.0;
    double control_signal;
    
    // Apply a large error to trigger integral wind-up
    for (int i = 0; i < 100; ++i){
        control_signal = controller.updatePwm(actual);
        TEST_ASSERT_TRUE((control_signal <= 1.0));
    } 
}

void test_no_saturation(void)
{
    double kp = 0.001;
    double target = 1200;
    PIController controller(kp, target, 0.021, 0.02);
    // PController controller(kp, target);
    
    double actual = 0;
    double control_signal = controller.updatePwm(actual);

    // The control signal should be within [0, 1]
    TEST_ASSERT_TRUE((control_signal > 0.0));
    TEST_ASSERT_TRUE((control_signal < 1.0));
}


int main(int argc, char **argv)
{
    _delay_ms(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!

    //Part 3: tests integral speed controller
    RUN_TEST(test_normal_flow); //normal flow
    RUN_TEST(test_anti_windup); //test anti-windup
    RUN_TEST(test_no_saturation); //test no saturation

    UNITY_END();
}
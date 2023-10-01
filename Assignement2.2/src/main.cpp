/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* 1Hz, period = 0.2s => off time = 0.1s */
#define SLEEP_TIME_MS 100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

// void main(void)
// {
// 	const struct device *dev;
// 	bool led_is_on = true;
// 	int ret;

// 	dev = device_get_binding(LED0);
// 	if (dev == NULL) {
// 		return;
// 	}

// 	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
// 	if (ret < 0) {
// 		return;
// 	}

// 	while (1) {
// 		gpio_pin_set(dev, PIN, (int)led_is_on);
// 		led_is_on = !led_is_on;
// 		k_msleep(SLEEP_TIME_MS);
// 	}
// }


#include <kernel.h>
#include <sys/printk.h>

#define STACKSIZE 1024

#define TASK0_PRIORITY 7

#define ONTASK_PRIORITY 7
#define OFFTASK_PRIORITY 7

K_MUTEX_DEFINE(my_mutex);	//Our mutex for part 3

const struct device *dev;
bool led_is_on;

int end_time;
int counter;

void task0(void)
{
    while (1) {
    	printk("Hello, I am task0\n");
    	k_busy_wait(1000000);
		k_yield();
    }
}

void on_task(void)
{
	while(k_uptime_get() < end_time)
	{
		k_mutex_lock(&my_mutex, K_FOREVER);	//lock mutex when starting the tasks
		led_is_on = true;
		gpio_pin_set(dev, PIN, (int)led_is_on);
		k_usleep(1);
		counter++;
		printk("Counter = %d\n", counter);
		k_mutex_unlock(&my_mutex);	//unlocking it slept for a certain time
	}
}

void off_task(void)
{
	while(k_uptime_get() < end_time)
	{
		k_mutex_lock(&my_mutex, K_FOREVER);
		led_is_on = false;
		gpio_pin_set(dev, PIN, (int)led_is_on);
		k_usleep(1);
		k_mutex_unlock(&my_mutex);
	}
}

void main()
{
	dev = device_get_binding(LED0);
	int ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	led_is_on = true;

	counter = 0;
	end_time = k_uptime_get() + 5000;	//eslapsed time + 1s
}

K_THREAD_DEFINE(on_task_id, STACKSIZE, on_task, NULL, NULL, NULL, ONTASK_PRIORITY, 0, 0);
K_THREAD_DEFINE(off_task_id, STACKSIZE, off_task, NULL, NULL, NULL, OFFTASK_PRIORITY, 0, 0);
// K_THREAD_DEFINE(task0_id, STACKSIZE, task0, NULL, NULL, NULL, TASK0_PRIORITY, 0, 0);

#!/bin/bash

IN_PIN=17
OUT_PIN=22

# Export if ressources are busy (if not comment this section)
echo $IN_PIN > /sys/class/gpio/unexport
echo $OUT_PIN > /sys/class/gpio/unexport

# Export the GPIO pins
echo $OUT_PIN > /sys/class/gpio/export
echo $IN_PIN > /sys/class/gpio/export
sleep 0.5

# Set pin directions
echo out > /sys/class/gpio/gpio$OUT_PIN/direction
echo in > /sys/class/gpio/gpio$IN_PIN/direction

HIGH_INPUT_DETECTED=0
START_TIME=0

COUNTER=0

while [ $COUNTER -lt 10 ]; do
    STATE_IN=$(cat /sys/class/gpio/gpio$IN_PIN/value)
    echo $STATE_IN

    if [ $STATE_IN -eq 1 ]; then 
        START_TIME=$(date +%s.%N)   # Record the start time
        echo 1 > /sys/class/gpio/gpio$OUT_PIN/value
        RESPONSE_TIME=$(echo "$ELAPSED_TIME - $START_TIME" | bc -l)
        echo "Input pin is high"
        ELAPSED_TIME=$(date +%s.%N)
        echo $ELAPSED_TIME
        COUNTER=10
    fi
done

# Unexport the GPIO pins when done
echo $OUT_PIN > /sys/class/gpio/unexport
echo $IN_PIN > /sys/class/gpio/unexport

echo $IN_PIN > /sys/class/gpio/unexport
echo $OUT_PIN > /sys/class/gpio/unexport
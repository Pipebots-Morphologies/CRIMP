#!/usr/bin/bash

if [[ "$1" == "" ]]; then
    echo "Enter a joint number (1, 2 or 3)"
    exit 1
fi

joint_names=("elbow_1" "elbow_2" "elbow_3")
joint="${joint_names[$(($1-1))]}"
angle="$2"

if [[ angle == "" ]]; then
    echo "Must input an angle"
    exit 1
fi

echo "Sending $joint to $angle degrees"

msg="{name: [\"$joint\"], position: [$angle], velocity: [0.0]}"

ros2 topic pub --once /joint_targets sensor_msgs/msg/JointState "$msg"
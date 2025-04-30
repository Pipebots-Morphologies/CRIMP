#!/usr/bin/bash

pos="$1"
if [[ $pos == "" ]]; then
	read -p "Enter position (1=open, 2=closed): " pos
fi

if [[ "$pos" == "1" ]]; then
  echo "Moving to open position"
  e1=270
  e2=180
  e3=90
elif [[ "$pos" == "2" ]]; then
  echo "Moving to closed position"
  e1=210
  e2=60
  e3=150
else
  echo "Invalid input"
  exit 1
fi

# Construct and publish the message
msg="{name: [\"elbow_1\", \"elbow_2\", \"elbow_3\"], position: [$e1, $e2, $e3], velocity: [600.0, 0.0, 600.0]}"

ros2 topic pub --once /joint_targets sensor_msgs/msg/JointState "$msg"

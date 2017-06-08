#!/bin/bash

# code to switch off extra cores
for x in /sys/devices/system/cpu/cpu*/online; do echo 0 > "$x"; done

echo "Number of online cores:"
cat /sys/devices/system/cpu/online

echo "Number of offline cores:"
cat /sys/devices/system/cpu/offline 

# DVFS variations handling
echo -n "performance" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo "DVFS setup:"
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor


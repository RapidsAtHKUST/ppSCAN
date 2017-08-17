#!/usr/bin/env bash
build/pSCANStatistics0 $1 0.3 5 output

# convert profile.log to callgrind compatible format
pprof --callgrind build/pSCANStatistics0 pscanProfile.log > profile.callgrind

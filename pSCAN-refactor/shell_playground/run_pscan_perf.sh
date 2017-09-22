#!/usr/bin/env bash
perf record --call-graph dwarf ../build/pSCANStatistics0 ../../dataset 0.3 5 output


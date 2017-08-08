#!/usr/bin/env bash
perf record --call-graph dwarf build/pSCAN ../dataset 0.3 5 output


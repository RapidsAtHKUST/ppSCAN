#!/usr/bin/env bash
build/pSCAN ../dataset 0.3 5 output

# convert profile.log to callgrind compatible format
pprof --callgrind build/pSCAN pscanProfile.log > profile.callgrind

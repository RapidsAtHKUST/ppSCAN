#!/usr/bin/env bash
valgrind --tool=callgrind build/pSCAN ../dataset 0.3 5 output


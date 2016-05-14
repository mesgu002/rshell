#!/bin/bash

./bin/rshell <<< 'ls -a; echo hello && mkdir test1 || echo world; git status'

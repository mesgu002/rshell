#!/bin/bash

./a.out <<< 'ls -a; echo hello && mkdir test1 || echo world; git status'

#!/bin/bash

./bin/rshell <<< 'ls ; mkdir test || echo hello'
#test failed

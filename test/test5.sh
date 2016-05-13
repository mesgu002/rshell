#!/bin/bash

./a.out <<< 'ls ; mkdir test || echo hello'
#test failed

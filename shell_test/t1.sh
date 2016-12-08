#!/bin/bash

#check with timeout
test1() {
local i=0

while [ x`cat ./input` = x1 ]  && [ $((++i)) -lt 100 ] ; do
    sleep 1
done

}

test

#!/bin/bash

if [ ! -e "bin" ]; then
    mkdir "bin"
fi

cd "bin"
cmake ".."

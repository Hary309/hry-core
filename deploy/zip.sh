#!/bin/bash
rm -rf hry-core-sdk.zip
7z a hry-core-sdk.zip CMakeLists.txt
cd zip
7z a ../hry-core-sdk.zip *
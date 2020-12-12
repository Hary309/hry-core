#!/bin/bash

rm -rf zip/include

mkdir -p zip
mkdir -p zip/include
mkdir -p zip/lib/debug
mkdir -p zip/lib/release

echo Coping Hry
cp -r ../include zip

echo Coping fmt
cp -r ../vendor/fmt/include zip

echo Coping Dear ImGui
cp ../vendor/imgui/imconfig.h zip/include
cp ../vendor/imgui/imgui.h zip/include

echo Coping json
cp -r ../vendor/json/include zip


#!/bin/bash

echo -e "Running in release mode.\n"
[ -f "../bin/Release-linux-x86_64/TravellersApp/TravellersApp" ] && valgrind ../bin/Release-linux-x86_64/TravellersApp/TravellersApp || valgrind ./bin/Release-linux-x86_64/TravellersApp/TravellersApp

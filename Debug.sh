#!/bin/bash

echo -e "Running in debug mode.\n"
[ -f "../bin/Debug-linux-x86_64/TravellersApp/TravellersApp" ] && valgrind ../bin/Debug-linux-x86_64/TravellersApp/TravellersApp || valgrind ./bin/Debug-linux-x86_64/TravellersApp/TravellersApp

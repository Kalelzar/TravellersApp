#!/bin/bash

echo -e "Running in debug mode.\n"
[ -f "../bin/Debug-linux-x86_64/TravellersApp/TravellersApp" ] && valgrind --track-origins=yes --leak-check=full -s ../bin/Debug-linux-x86_64/TravellersApp/TravellersApp || valgrind --track-origins=yes --leak-check=full -s ./bin/Debug-linux-x86_64/TravellersApp/TravellersApp

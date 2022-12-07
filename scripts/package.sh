#!/bin/bash
# Usage (assumes build/ directory in root directory of the repo):
# cd build
# ../scripts/prepare.sh

mkdir -p release
xz -T0 -9e -k -z -v ./src/libduckdb_static.a
mv ./src/libduckdb_static.a.xz release
XZ_OPT="-9e -k -T0 -z -v" tar cJf release/include.tar.xz -C ../src include

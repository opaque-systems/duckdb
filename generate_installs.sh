#!/bin/bash

set -e

# Get script directory
script_dir="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Build and install plain static library
mkdir -p "$script_dir/build"
cd "$script_dir/build"
mkdir -p "install"
cmake -DCMAKE_INSTALL_PREFIX="install" ..

make -j$(nproc)
make install

# Go back to DuckDB main directory
cd ..

# Build and install enclave static library
mkdir -p "$script_dir/build_enclave"
cd "$script_dir/build_enclave"
mkdir -p "install"
cmake -DCMAKE_INSTALL_PREFIX="install" -DLINK_OE=1 ..

make -j$(nproc)
make install

# Go back to DuckDB main directory
cd ..

# Tarball both install directories
cd build
tar -zcf duckdb_install.tar.gz install
cd ../build_enclave
tar -zcf duckdb_enclave_install.tar.gz install
cd ..


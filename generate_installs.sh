#!/bin/bash

set -e

# Get script directory
script_dir="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Build and install plain static library
mkdir -p "$script_dir/build"
cd "$script_dir/build"
mkdir -p "install"
cmake -GNinja -DCMAKE_INSTALL_PREFIX="install" ..

ninja -j$(nproc)
ninja install

# Go back to DuckDB main directory
cd ..

# Build and install enclave static library
mkdir -p "$script_dir/build_enclave"
cd "$script_dir/build_enclave"
mkdir -p "install"
cmake -GNinja -DCMAKE_INSTALL_PREFIX="install" -DLINK_OE=1 ..

ninja -j$(nproc)
ninja install

# Go back to DuckDB main directory
cd ..

# Build and install enclave static library in debug mode
mkdir -p "$script_dir/build_enclave_debug"
cd "$script_dir/build_enclave_debug"
mkdir -p "install"
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="install" -DLINK_OE=1 ..

ninja -j$(nproc)
ninja install

# Go back to DuckDB main directory
cd ..

# Tarball both install directories
cd build
tar -zcf duckdb_install.tar.gz install
cd ../build_enclave
tar -zcf duckdb_enclave_install.tar.gz install
cd ../build_enclave_debug
tar -zcf duckdb_enclave_debug_install.tar.gz install
cd ..


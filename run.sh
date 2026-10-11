
#!/usr/bin/env bash
set -Eeuo pipefail

cd "$(dirname "$0")"

echo "=== Cleaning build ==="
rm -rf build

sleep 1

echo "=== Configuring CMake ==="
cmake -B build

echo "=== Building VoxelEngine ==="
cmake --build build

echo "=== Launching VoxelEngine ==="
exec ./build/VoxelEngine

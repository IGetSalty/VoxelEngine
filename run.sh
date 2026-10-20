
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

echo "=== Launching VoxelEngine on NVIDIA ==="
export __NV_PRIME_RENDER_OFFLOAD=1
export __GLX_VENDOR_LIBRARY_NAME=nvidia

exec ./build/VoxelEngine

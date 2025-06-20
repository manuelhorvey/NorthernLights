#!/usr/bin/env bash

set -e

PROJECT_ROOT=$(dirname "$(realpath "$0")")/..
cd "$PROJECT_ROOT"

echo "[✓] Bootstrapping TheNorthernLights on Fedora..."

# ──────────────────────────────────────────────
# 1. Check for required commands
for cmd in cmake g++ git python3 virtualenv; do
    command -v $cmd >/dev/null 2>&1 || {
        echo "❌ Required command '$cmd' is not installed."; exit 1;
    }
done

# ──────────────────────────────────────────────
# 2. Install system dependencies
echo "[+] Installing system packages via DNF..."
sudo dnf install -y cmake gcc-c++ git python3 python3-virtualenv fmt-devel spdlog-devel catch2-devel

# ──────────────────────────────────────────────
# 3. Python virtual environment
echo "[+] Setting up Python virtual environment..."
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip

if [[ -f "requirements.txt" ]]; then
    pip install -r requirements.txt
else
    echo "[!] Skipping Python deps: no requirements.txt found."
fi

# ──────────────────────────────────────────────
# 4. Configure CMake
echo "[+] Configuring C++ project with CMake..."
mkdir -p build
cd build
cmake ..
cd ..

# ──────────────────────────────────────────────
# 5. Create required runtime folders
mkdir -p logs data/raw data/processed data/test

echo "[✓] Bootstrap completed successfully."
echo
echo "👉 Activate Python venv:   source .venv/bin/activate"
echo "👉 Build C++ code:         cd build && make"
echo "👉 Run test modules:       ./build/run_tests"

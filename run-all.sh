#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

INPUT_DIR="input"
OUTPUT_DIR="output"
BIN="./drop_calc"

if [ ! -x "$BIN" ] || [ -n "$(find main.cpp include -newer "$BIN" 2>/dev/null)" ]; then
  g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o "$BIN"
fi
mkdir -p "$OUTPUT_DIR"

shopt -s nullglob
files=("$INPUT_DIR"/*.txt)
if [ ${#files[@]} -eq 0 ]; then
  echo "No scenarios found in $INPUT_DIR/" >&2
  exit 1
fi

failed=0
for file in "${files[@]}"; do
  base=$(basename "$file" .txt)
  out="$OUTPUT_DIR/${base}-result.txt"
  if "$BIN" --input-file "$file" --output-file "$out"; then
    echo "OK   $file -> $out"
  else
    echo "FAIL $file" >&2
    failed=$((failed + 1))
  fi
done

echo "Processed ${#files[@]} scenario(s), failed: $failed"
[ "$failed" -eq 0 ]

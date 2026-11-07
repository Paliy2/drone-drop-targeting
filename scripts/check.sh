#!/usr/bin/env bash
# Runs every scenario in both trajectory models against expected/ and checks the
# empty-scenario contract. Usage: scripts/check.sh [path/to/drop_calc]
set -uo pipefail

cd "$(dirname "$0")/.."
BIN="${1:-build-release/drop_calc}"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

failed=0
fail() { echo "FAIL $*" >&2; failed=$((failed + 1)); }

for file in input/*.txt input/edge/edge-single.txt; do
  base=$(basename "$file" .txt)
  for trajectory in hover forward; do
    out="$TMP/$base-$trajectory.txt"
    if ! "$BIN" --input-file "$file" --output-file "$out" --trajectory "$trajectory" >/dev/null; then
      fail "$file [$trajectory]: non-zero exit"
    elif ! cmp -s "$out" "expected/$base-$trajectory.txt"; then
      fail "$file [$trajectory]: differs from expected/$base-$trajectory.txt"
    else
      echo "OK   $file [$trajectory]"
    fi
  done
done

for trajectory in hover forward; do
  out="$TMP/edge-empty-$trajectory.txt"
  err=$("$BIN" --input-file input/edge/edge-empty.txt --output-file "$out" --trajectory "$trajectory" 2>&1 >/dev/null)
  code=$?
  if [ "$code" -ne 1 ] || [ "$err" != "empty scenario" ] || [ -e "$out" ]; then
    fail "edge-empty [$trajectory]: code=$code stderr='$err' output_exists=$([ -e "$out" ] && echo yes || echo no)"
  else
    echo "OK   input/edge/edge-empty.txt [$trajectory]"
  fi
done

echo "binary: $BIN, failed: $failed"
[ "$failed" -eq 0 ]

#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

trap 'make -C "$ROOT" clean' EXIT

make -C "$ROOT" test

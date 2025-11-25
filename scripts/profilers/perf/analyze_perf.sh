#!/usr/bin/env sh
# Analyze perf stat output for cache efficiency.
# Counts L1-dcache and cache-miss rates per domain (cpu_core, cpu_atom, ...),
# including only domains where BOTH relevant counters are present and supported.
#
# Usage:
#   analyze_perf_stat.sh <perf_output.txt> [L1_THRESHOLD%] [CACHE_THRESHOLD%]
# Defaults:
#   L1_THRESHOLD = 2.0
#   CACHE_THRESHOLD = 60.0
#
# Exit codes:
#   0 - OK (within thresholds)
#   2 - Any metric exceeds threshold
#   1 - Error or bad input

set -eu

if [ $# -lt 1 ]; then
  echo "Usage: $0 <perf_output.txt> [L1_THRESHOLD%] [CACHE_THRESHOLD%]" >&2
  exit 1
fi

FILE="$1"
L1_THRESHOLD="${2:-2.0}"
CACHE_THRESHOLD="${3:-60.0}"

awk -v L1_THRESHOLD="$L1_THRESHOLD" -v CACHE_THRESHOLD="$CACHE_THRESHOLD" '
# --- Helper functions ---------------------------------------------------------
function strip_commas(s) { gsub(/,/, "", s); return s }

# Find first token that looks like cpu_domain/event/
function find_event_token(i) {
  for (i = 1; i <= NF; i++)
    if ($i ~ /^cpu_[^/]+\/[^/]+\/$/) return $i
  return ""
}

# Find first numeric value in a line
function find_number(i) {
  for (i = 1; i <= NF; i++)
    if ($i ~ /^[0-9][0-9,]*$/) return $i
  return ""
}

# --- Main parsing loop --------------------------------------------------------
{
  token = find_event_token()
  if (token == "") next

  split(token, parts, "/")
  domain = parts[1]   # e.g. cpu_core
  event  = parts[2]   # e.g. cache-misses
  key    = domain SUBSEP event

  seen_domains[domain] = 1
  seen_events[key]     = 1

  if ($0 ~ /<not supported>/) {
    unsupported[key] = 1
    next
  }

  value = find_number()
  if (value != "") {
    value = strip_commas(value) + 0
    event_values[key] += value
  }
}

# --- Aggregation --------------------------------------------------------------
END {
  for (domain in seen_domains) {
    # L1D loads and misses
    key_loads = domain SUBSEP "L1-dcache-loads"
    key_misses = domain SUBSEP "L1-dcache-load-misses"

    if ((key_loads in seen_events) && (key_misses in seen_events) &&
        !(key_loads in unsupported) && !(key_misses in unsupported)) {
      total_l1_loads += event_values[key_loads]
      total_l1_misses += event_values[key_misses]
      l1_supported = 1
    }

    # Cache references and misses
    key_refs = domain SUBSEP "cache-references"
    key_cache_misses = domain SUBSEP "cache-misses"

    if ((key_refs in seen_events) && (key_cache_misses in seen_events) &&
        !(key_refs in unsupported) && !(key_cache_misses in unsupported)) {
      total_cache_refs += event_values[key_refs]
      total_cache_misses += event_values[key_cache_misses]
      cache_supported = 1
    }
  }

  # --- Compute rates ----------------------------------------------------------
  l1_miss_rate = (l1_supported && total_l1_loads > 0) ? 100.0 * total_l1_misses / total_l1_loads : -1
  cache_miss_rate = (cache_supported && total_cache_refs > 0) ? 100.0 * total_cache_misses / total_cache_refs : -1

  # --- Report -----------------------------------------------------------------
  printf "==== Perf Stat Analysis ====\n"

  if (l1_miss_rate >= 0)
    printf "L1D-load-miss-rate: %.2f%% (threshold %.2f%%) %s\n",
           l1_miss_rate, L1_THRESHOLD, (l1_miss_rate > L1_THRESHOLD ? "❌ ALERT" : "✅ OK")
  else
    print  "L1D-load-miss-rate: N/A (no supported domains)"

  if (cache_miss_rate >= 0)
    printf "Cache-miss-rate:    %.2f%% (threshold %.2f%%) %s\n",
           cache_miss_rate, CACHE_THRESHOLD, (cache_miss_rate > CACHE_THRESHOLD ? "❌ ALERT" : "✅ OK")
  else
    print  "Cache-miss-rate:    N/A (no supported domains)"

  # --- Exit code --------------------------------------------------------------
  exceeded = ((l1_miss_rate >= 0 && l1_miss_rate > L1_THRESHOLD) ||
              (cache_miss_rate >= 0 && cache_miss_rate > CACHE_THRESHOLD))
  exit(exceeded ? 2 : 0)
}
' "$FILE"

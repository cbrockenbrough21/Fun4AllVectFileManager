#!/bin/bash

# File paths
LOG_FILE="struct_rewrite_test_results.log"
CSV_FILE="struct_rewrite_test_results.csv"

# Clear previous log and CSV files
: > "$LOG_FILE"
: > "$CSV_FILE"

# Write CSV header
echo "Algorithm,Compression Level,Basket Size,AutoFlush,File Size (MB),Write Time (s)" >> "$CSV_FILE"

# Parameters
BASKET_SIZES=(32000)
AUTOFLUSH_VALUES=(0)
INPUT_FILE="Struct-In.root"
OUTPUT_FILE="Struct-Out.root"

# Function to run tests
run_tests() {
  local algo=$1
  shift
  local levels=("$@")  # Remaining arguments are compression levels

  for level in "${levels[@]}"; do
    for basket_size in "${BASKET_SIZES[@]}"; do
      for autoflush in "${AUTOFLUSH_VALUES[@]}"; do

        # Log current test configuration
        echo "-----------------------------------------" | tee -a "$LOG_FILE"
        echo "Testing: Algo=$algo, Level=$level, BasketSize=$basket_size, AutoFlush=$autoflush" | tee -a "$LOG_FILE"

        # Run the StructRewrite command
        result=$(./StructRewrite "$INPUT_FILE" "$OUTPUT_FILE" "$algo" "$level" "$basket_size" "$autoflush")
        echo "$result" | tee -a "$LOG_FILE"

        # Extract results
        file_size=$(echo "$result" | grep -oP 'FILE_SIZE=\K[0-9.]+')
        write_time=$(echo "$result" | grep -oP 'WRITE_TIME=\K[0-9.]+')

        # Handle missing results
        [[ -z "$file_size" ]] && file_size="ERROR"
        [[ -z "$write_time" ]] && write_time="ERROR"

        # Append results to CSV
        echo "$algo,$level,$basket_size,$autoflush,$file_size,$write_time" >> "$CSV_FILE"
      done
    done
  done
}

# Run tests for Algo 1 with levels 5, 6, 7
run_tests 1 5 6 7

# Run tests for Algo 2 with levels 3, 4, 5
run_tests 2 3 4 5

# Final log summary
echo "-----------------------------------------" | tee -a "$LOG_FILE"
echo "All tests completed. Results are in:" | tee -a "$LOG_FILE"
echo " - $LOG_FILE" | tee -a "$LOG_FILE"
echo " - $CSV_FILE" | tee -a "$LOG_FILE"

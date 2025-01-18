#!/bin/bash

LOG_FILE="struct_rewrite_test_results.log"
CSV_FILE="struct_rewrite_test_results.csv"

: > "$LOG_FILE"
: > "$CSV_FILE"

echo "Algorithm,Compression Level,Basket Size,AutoFlush,File Size (MB),Write Time (s)" >> "$CSV_FILE"

# Common parameters
BASKET_SIZES=(32000)
AUTOFLUSH_VALUES=(0)
INPUT_FILE="Struct-In.root"
OUTPUT_FILE="Struct-Out.root"

########################################
# 1) Algo = 1 with levels 5, 6, 7
########################################
algo=1
for level in 5 6 7; do
  for basket_size in "${BASKET_SIZES[@]}"; do
    for autoflush in "${AUTOFLUSH_VALUES[@]}"; do

      echo "-----------------------------------------" | tee -a "$LOG_FILE"
      echo "Testing: Algo=$algo, Level=$level, BasketSize=$basket_size, AutoFlush=$autoflush" | tee -a "$LOG_FILE"

      result=$(./StructRewrite "$INPUT_FILE" "$OUTPUT_FILE" "$algo" "$level" "$basket_size" "$autoflush")

      echo "$result" | tee -a "$LOG_FILE"

      file_size=$(echo "$result"  | grep -oP 'FILE_SIZE=\K[0-9.]+')
      write_time=$(echo "$result" | grep -oP 'WRITE_TIME=\K[0-9.]+')

      [[ -z "$file_size" ]] && file_size="ERROR"
      [[ -z "$write_time" ]] && write_time="ERROR"

      echo "$algo,$level,$basket_size,$autoflush,$file_size,$write_time" >> "$CSV_FILE"
    done
  done
done

########################################
# 2) Algo = 2 with levels 3, 4, 5
########################################
algo=2
for level in 3 4 5; do
  for basket_size in "${BASKET_SIZES[@]}"; do
    for autoflush in "${AUTOFLUSH_VALUES[@]}"; do

      echo "-----------------------------------------" | tee -a "$LOG_FILE"
      echo "Testing: Algo=$algo, Level=$level, BasketSize=$basket_size, AutoFlush=$autoflush" | tee -a "$LOG_FILE"

      result=$(./StructRewrite "$INPUT_FILE" "$OUTPUT_FILE" "$algo" "$level" "$basket_size" "$autoflush")

      echo "$result" | tee -a "$LOG_FILE"

      file_size=$(echo "$result"  | grep -oP 'FILE_SIZE=\K[0-9.]+')
      write_time=$(echo "$result" | grep -oP 'WRITE_TIME=\K[0-9.]+')

      [[ -z "$file_size" ]] && file_size="ERROR"
      [[ -z "$write_time" ]] && write_time="ERROR"

      echo "$algo,$level,$basket_size,$autoflush,$file_size,$write_time" >> "$CSV_FILE"
    done
  done
done

echo "-----------------------------------------" | tee -a "$LOG_FILE"
echo "All tests completed. Results are in:" | tee -a "$LOG_FILE"
echo " - $LOG_FILE" | tee -a "$LOG_FILE"
echo " - $CSV_FILE" | tee -a "$LOG_FILE"

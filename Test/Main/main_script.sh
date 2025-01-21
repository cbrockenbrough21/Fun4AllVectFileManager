#!/bin/bash

# Set the base directory for the current script
BASE_DIR="/project/ptgroup/Catherine/Fun4AllVectFileManager/Test/Main"

# Set the root directory for the Fun4AllVectFileManager project
ROOT_DIR="/project/ptgroup/Catherine/Fun4AllVectFileManager"

# Ensure logs and results directories exist
mkdir -p "$BASE_DIR/logs"
mkdir -p "$BASE_DIR/results"
mkdir -p "$BASE_DIR/Struct"

# Configuration
VECTOR_FILES=("/project/ptgroup/Catherine/Fun4AllVectFileManager/Test/Main/Vector/run_002281_spill_000000000_spin_vector.root" "/project/ptgroup/Catherine/Fun4AllVectFileManager/Test/Main/Vector/run_002282_spill_000000000_spin_vector.root" "/project/ptgroup/Catherine/Fun4AllVectFileManager/Test/Main/Vector/run_002283_spill_000000000_spin_vector.root")
REWRITE_TYPES=("Struct" "Vector")  # Types of rewrite (StructRewrite, VectorRewrite)

# Combined results CSV file
COMBINED_CSV="$BASE_DIR/results/combined_results.csv"

# Check if --reset flag is provided
RESET=false
if [[ "$1" == "--reset" ]]; then
  RESET=true
fi

# Reset or append to the combined results CSV
if $RESET; then
  echo "Resetting combined results file..."
  echo "File Name,Rewrite Type,Algorithm,Compression Level,Basket Size,AutoFlush,File Size (MB),Write Time (s)" > "$COMBINED_CSV"
else
  echo "Appending to existing combined results file..."
fi

# Parameters for testing
BASKET_SIZES=(32000 64000)
AUTOFLUSH_VALUES=(0 2500 5000)

# Paths to executables
CONVERT_TO_STRUCT="$ROOT_DIR/Convert/ConvertVectToStruct/maker/ConvertVectToStruct"
VECTOR_REWRITE="$ROOT_DIR/Test/TestVector/maker/VectorRewrite"
STRUCT_REWRITE="$ROOT_DIR/Test/TestStruct/maker/StructRewrite"

# Function to run rewrite tests
run_rewrite_tests() {
  local rewrite_type=$1
  local input_file=$2
  local output_file=$3
  local log_file=$4
  local csv_file=$5
  local rewrite_executable
  
  # Extract the base file name (without the path)
  local base_name=$(basename "$input_file")
  base_name=${base_name%_vector}
  base_name=${base_name%_struct}  

  # Determine executable based on rewrite type
  if [[ "$rewrite_type" == "Vector" ]]; then
    rewrite_executable="$VECTOR_REWRITE"
  elif [[ "$rewrite_type" == "Struct" ]]; then
    rewrite_executable="$STRUCT_REWRITE"
  else
    echo "Invalid rewrite type: $rewrite_type"
    exit 1
  fi

  : > "$log_file"
  : > "$csv_file"

  echo "Algorithm,Compression Level,Basket Size,AutoFlush,File Size (MB),Write Time (s)" >> "$csv_file"

  Algo 1: Levels 5, 6, 7
  for algo in 1; do
    for level in 5 6 7; do
      for basket_size in "${BASKET_SIZES[@]}"; do
        for autoflush in "${AUTOFLUSH_VALUES[@]}"; do

          echo "Testing: $rewrite_type Algo=$algo Level=$level BasketSize=$basket_size AutoFlush=$autoflush" | tee -a "$log_file"

          result=$("$rewrite_executable" "$input_file" "$output_file" "$algo" "$level" "$basket_size" "$autoflush")
          echo "$result" | tee -a "$log_file"

          file_size=$(echo "$result" | grep -oP 'FILE_SIZE=\K[0-9.]+')
          write_time=$(echo "$result" | grep -oP 'WRITE_TIME=\K[0-9.]+')

          [[ -z "$file_size" ]] && file_size="ERROR"
          [[ -z "$write_time" ]] && write_time="ERROR"

          echo "$algo,$level,$basket_size,$autoflush,$file_size,$write_time" >> "$csv_file"

          # Append results to the combined CSV with the base name
          echo "$base_name,$rewrite_type,$algo,$level,$basket_size,$autoflush,$file_size,$write_time" >> "$COMBINED_CSV"

        done
      done
    done
  done

  # Algo 2: Levels 3, 4, 5
  for algo in 2; do
    for level in 3 4 5; do
      for basket_size in "${BASKET_SIZES[@]}"; do
        for autoflush in "${AUTOFLUSH_VALUES[@]}"; do

          echo "Testing: $rewrite_type Algo=$algo Level=$level BasketSize=$basket_size AutoFlush=$autoflush" | tee -a "$log_file"

          result=$("$rewrite_executable" "$input_file" "$output_file" "$algo" "$level" "$basket_size" "$autoflush")
          echo "$result" | tee -a "$log_file"

          file_size=$(echo "$result" | grep -oP 'FILE_SIZE=\K[0-9.]+')
          write_time=$(echo "$result" | grep -oP 'WRITE_TIME=\K[0-9.]+')

          [[ -z "$file_size" ]] && file_size="ERROR"
          [[ -z "$write_time" ]] && write_time="ERROR"

          echo "$algo,$level,$basket_size,$autoflush,$file_size,$write_time" >> "$csv_file"

          # Append results to the combined CSV with the base name
          echo "$base_name,$rewrite_type,$algo,$level,$basket_size,$autoflush,$file_size,$write_time" >> "$COMBINED_CSV"

        done
      done
    done
  done
}

# Main loop for each vector file
for vector_file in "${VECTOR_FILES[@]}"; do
    # Extract the base name and remove the '_vector' suffix
    base_name=$(basename "$vector_file" .root)
    base_name_without_vector="${base_name%_vector}"

    # Generate Struct file path
    struct_file="$BASE_DIR/Struct/${base_name_without_vector}_struct.root"

    # Run ConvertVectToStruct
    "$CONVERT_TO_STRUCT" "$vector_file" "$struct_file"

    # Run VectorRewrite tests
    run_rewrite_tests "Vector" "$vector_file" "$BASE_DIR/Vector/${base_name}_rewritten.root" \
    "$BASE_DIR/logs/${base_name}_vector_log.txt" "$BASE_DIR/results/${base_name}_results.csv"

    # Run StructRewrite tests
    run_rewrite_tests "Struct" "$struct_file" "$BASE_DIR/Struct/${base_name_without_vector}_struct_rewritten.root" \
    "$BASE_DIR/logs/${base_name_without_vector}_struct_log.txt" "$BASE_DIR/results/${base_name_without_vector}_struct_results.csv"
done

echo "All tests completed. Logs and results can be found in the logs/ and results/ directories."

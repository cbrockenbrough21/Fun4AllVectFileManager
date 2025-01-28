#!/bin/bash

# Check for required arguments
if [[ $# -lt 4 ]]; then
  echo "Usage: $0 <file_specific_csv> <combined_csv> <file_name> <rewrite_type>"
  exit 1
fi

# Input arguments
FILE_SPECIFIC_CSV=$1
COMBINED_CSV=$2
FILE_NAME=$3
REWRITE_TYPE=$4

# Check if the file-specific CSV exists
if [[ ! -f $FILE_SPECIFIC_CSV ]]; then
  echo "Error: File-specific CSV '$FILE_SPECIFIC_CSV' not found!"
  exit 1
fi

# Check if the combined CSV exists
if [[ ! -f $COMBINED_CSV ]]; then
  echo "Combined CSV does not exist. Creating it now..."
  # Create the combined CSV with headers
  echo "File Name,Rewrite Type,Algorithm,Compression Level,Basket Size,AutoFlush,File Size (MB),Write Time (s)" > "$COMBINED_CSV"
fi

# Append data with added columns for File Name and Rewrite Type
tail -n +2 "$FILE_SPECIFIC_CSV" | while IFS= read -r line; do
  echo "$FILE_NAME,$REWRITE_TYPE,$line"
done >> "$COMBINED_CSV"

echo "Appended data from '$FILE_SPECIFIC_CSV' to '$COMBINED_CSV' with File Name='$FILE_NAME' and Rewrite Type='$REWRITE_TYPE'."

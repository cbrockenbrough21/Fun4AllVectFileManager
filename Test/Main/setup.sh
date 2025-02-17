#!/bin/bash

# Source the spinquest environment
#source /project/ptgroup/spinquest/this-e1039.sh

module purge
module load miniforge
module load gcc/11.4.0
module load openmpi/4.1.4
module load root/6.32.06
module load intel-compilers/2023.1.0
module load impi/2021.9.0
module load python/3.11.4

# Set the base directory for the current script
BASE_DIR="/project/ptgroup/Catherine/Fun4AllVectFileManager/Test/Main"

# Set the root directory for the Fun4AllVectFileManager project
ROOT_DIR="/project/ptgroup/Catherine/Fun4AllVectFileManager"

# Add the library paths to LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$ROOT_DIR/StructDef/inst/lib:$ROOT_DIR/Test/TestVector/lib:$ROOT_DIR/Test/TestStruct/lib:$LD_LIBRARY_PATH

# Add the path to executables to PATH (optional, if you want to call them directly without specifying full paths)
export PATH=$ROOT_DIR/Convert/ConvertVectToStruct/maker:$ROOT_DIR/Test/TestStruct/maker:$ROOT_DIR/Test/TestVector/maker:$PATH

export LD_LIBRARY_PATH=/project/ptgroup/Catherine/Fun4AllVectFileManager/Test/TestVector/inst/lib:$LD_LIBRARY_PATH


# Debugging output for verification
echo "==================== Setup Configuration ===================="
echo "BASE_DIR: $BASE_DIR"
echo "ROOT_DIR: $ROOT_DIR"
echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo "PATH: $PATH"
echo "============================================================="

# Paths to executables (optional, if referenced directly in scripts)
export CONVERT_TO_VECT="$ROOT_DIR/Convert/ConvertToVect/maker/ConvertToVect"
export CONVERT_TO_STRUCT="$ROOT_DIR/Convert/ConvertVectToStruct/maker/ConvertVectToStruct"
export VECTOR_REWRITE="$ROOT_DIR/Test/TestVector/maker/VectorRewrite"
export STRUCT_REWRITE="$ROOT_DIR/Test/TestStruct/maker/StructRewrite"

# Debugging output for executable paths
echo "CONVERT_TO_VECT: $CONVERT_TO_VECT"
echo "CONVERT_TO_STRUCT: $CONVERT_TO_STRUCT"
echo "VECTOR_REWRITE: $VECTOR_REWRITE"
echo "STRUCT_REWRITE: $STRUCT_REWRITE"

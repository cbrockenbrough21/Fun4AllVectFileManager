import ROOT

# Input file paths
file_5_path = "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_002482_spill_000000000_spin_vector.root"
# file_7_path = "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_004062_spill_000000000_spin_vector.root"

# Output file path
output_path = "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/large_combined_vect_file.root"

# Open input files
file_5 = ROOT.TFile.Open(file_5_path)
# file_7 = ROOT.TFile.Open(file_7_path)

# Get the trees
tree_5 = file_5.Get("tree")
# tree_7 = file_7.Get("tree")

# Create a new output file and tree
output_file = ROOT.TFile(output_path, "RECREATE")
output_tree = tree_5.CloneTree(0)  # Clone the structure of Tree 5

for i in range(8):
    for entry in tree_5:
        output_tree.Fill()
    
    # print("Filling tree_7 entry")
    # for entry in tree_7:
    #     output_tree.Fill()

# Write the combined tree to the output file
output_tree.Write()
output_file.Close()

print(f"Combined file created: {output_path}")

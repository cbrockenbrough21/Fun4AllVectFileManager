import ROOT

# List of your vector ROOT file paths
vector_files = [
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_002281_spill_000000000_spin_vector.root",
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_002282_spill_000000000_spin_vector.root",
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_002283_spill_000000000_spin_vector.root",
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_002291_spill_000000000_spin_vector.root",
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_002482_spill_000000000_spin_vector.root",
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_003644_spill_000000000_spin_vector.root",
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_004062_spill_000000000_spin_vector.root",
    # "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/run_005459_spill_000000000_spin_vector.root"
    "/project/ptgroup/Catherine/Fun4AllVectFileManager/Convert/ConvertToVect/large_combined_file.root"
]

# Function to get the number of events in a ROOT file
def get_number_of_events(file_path):
    try:
        file = ROOT.TFile(file_path, "READ")
        if not file.IsOpen():
            print(f"Error: Could not open file {file_path}")
            return None
        
        # Assuming the TTree is named "tree" (update if your tree name is different)
        tree = file.Get("tree")  
        if not isinstance(tree, ROOT.TTree):
            print(f"Error: No TTree named 'tree' found in {file_path}")
            return None
        
        num_events = tree.GetEntries()  # Get the number of events (entries)
        file.Close()
        return num_events
    except Exception as e:
        print(f"Error processing file {file_path}: {e}")
        return None

# Iterate over each vector file and get the number of events
for file_path in vector_files:
    num_events = get_number_of_events(file_path)
    if num_events is not None:
        print(f"File: {file_path}, Number of Events: {num_events}")
    print("-" * 50)

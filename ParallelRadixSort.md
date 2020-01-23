# Step 1:
1. Divide records evenly into blocks of size M / T (M = num of records, T = num of threads)
2. Each thread calculates a histogram H_M(bin) by iterating through the data records in each block. 

# Step 2:
1. Each thread calculates the offset Bin_offset_m which is the write offset of the bin in all records.
2. Bin_offset_m = H_M(bin) (where bin < this_bin) + H_M(bin) (where bin = this_bin and M's position < m block's position)
This step requires synchronization between all bins, which means each thread need to write histogram results in local thread cache and a sweep of all such histograms should be done across all threads. Barrier is needed to implement this function.

# Step 3:  
1. Read record and update H_M(bin)++, where bin is the radix bucket of the record in the current cycle. Record is written in _local_ cache, and the position is d * B + H_M(bin) % B (B = max number of records that could be stored locally in each bin, d = number of bins before this bin in this block). 
2. If H_M(bin) % B == B - 1, we can confirm that the buffer is full for this bin in local cache, we flush records in this bin in the local memory to the global memory at H_M(bin) - (B - 1).


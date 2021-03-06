# This is an example for the initialization of a linear kernel on string data. The 
# strings are all of the same length and consist of the characters 'ACGT' corresponding 
# to the DNA-alphabet. Each column of the matrices of type char corresponds to 
# one training/test example.

library("sg")

size_cache <- 10

fm_train_dna <- as.matrix(read.table('../data/fm_train_dna.dat'))
fm_test_dna <- as.matrix(read.table('../data/fm_test_dna.dat'))

# Linear String
print('LinearString')

dump <- sg('set_kernel', 'LINEAR', 'CHAR', size_cache)

dump <- sg('set_features', 'TRAIN', fm_train_dna, 'DNA')
km <- sg('get_kernel_matrix', 'TRAIN')

dump <- sg('set_features', 'TEST', fm_test_dna, 'DNA')
km <- sg('get_kernel_matrix', 'TEST')

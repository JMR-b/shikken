# With the distance kernel one can use any of the following distance metrics: 
# MINKOWSKI MANHATTAN HAMMING CANBERRA CHEBYSHEW GEODESIC JENSEN CHISQUARE TANIMOTO COSINE BRAYCURTIS EUCLIDIAN

library("sg")

size_cache <- 10

fm_train_real <- t(as.matrix(read.table('../data/fm_train_real.dat')))
fm_test_real <- t(as.matrix(read.table('../data/fm_test_real.dat')))

# Distance
print('Distance')

width=1.7
dump <- sg('set_distance', 'EUCLIDIAN', 'REAL')
dump <- sg('set_kernel', 'DISTANCE', size_cache, width)

dump <- sg('set_features', 'TRAIN', fm_train_real)
km=sg('get_kernel_matrix', 'TRAIN')

dump <- sg('set_features', 'TEST', fm_test_real)
km=sg('get_kernel_matrix', 'TEST')

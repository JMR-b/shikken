# This is an example for the initialization of the diag-kernel. 
# The diag kernel has all kernel matrix entries but those on 
# the main diagonal set to zero. 

library("sg")

size_cache <- 10

fm_train_real <- as.matrix(read.table('../data/fm_train_real.dat'))
fm_test_real <- as.matrix(read.table('../data/fm_test_real.dat'))

# Diag
print('Diag')

diag=23.
dump <- sg('set_kernel', 'DIAG', 'REAL', size_cache, diag)

dump <- sg('set_features', 'TRAIN', fm_train_real)
km <- sg('get_kernel_matrix', 'TRAIN')

dump <- sg('set_features', 'TEST', fm_test_real)
km <- sg('get_kernel_matrix', 'TEST')

setGeneric("%in%", function(x, table) standardGeneric("%in%"))
setMethod("%in%", c(x="ANY", "ANY"),
function(x, table) {
  fmatch(x, table, nomatch = 0L) > 0L
})
#include <shikken.h>

using namespace shogun;

void r_print_message(FILE* target, const char* str)
{
    if (target == stdout) {
        Rprintf((char*) "%s", str);
    } else {
        fprintf(target, "%s", str);
    }
}

void r_print_warning(FILE* target, const char* str)
{
    if (target == stdout) {
        Rprintf((char*) "%s", str);
    } else {
        fprintf(target, "%s", str);
    }
}

void r_print_error(FILE* target, const char* str)
{
    if (target != stdout) {
        fprintf(target, "%s", str);
    }
}

void r_cancel_computations(bool &delayed, bool &immediately)
{
    //R_Suicide((char*) "sg stopped by SIGINT\n");
    // see http://tolstoy.newcastle.edu.au/R/e13/devel/11/04/1045.html
    R_CheckUserInterrupt();
}

/* ------------------------- Shogun Object Handling ------------------------ */

/**
 * Decrements reference count to a shogun object and "clears" the R pointer
 * if count hits zero.
 * 
 * This method is called from within the C stack, and typically as the
 * "finalizer" parameter, eg:
 * 
 *   R_RegisterCFinalizer(some_shogun_obj_ptr, _shogun_ref_count_down); 
 */
void _shogun_ref_count_down(SEXP ptr) {
    Rprintf("  [C] _shogun_ref_count_down triggered\n");
    // Rcpp::XPtr<CSGObject> sptr(ptr); // doesn't work??
    CSGObject* sptr = reinterpret_cast<CSGObject*>(R_ExternalPtrAddr(ptr));
    int32_t ref_count = -1;
    if (sptr) {
        Rprintf("  [C] ... decrementing ref count\n");
        ref_count = sptr->unref();
        if (ref_count == 0) {
            sptr = NULL;
            Rprintf("  [C] ... calling R_ClearExternalPtr, too\n");
            R_ClearExternalPtr(ptr);
        }
    }
}

/**
 * Entry point from R to decrement Shogun object reference count, eg.
 * 
 *   dipose <- function(x) .Call("shogun_ref_count_down", externalptr)
 *   ...//Rprintf
 *   reg.finalizer(some.shogun.ptr, disposeShogunPointer)
 * 
 * This shouldn't be used, as we are currently favoring registering 
 * object finalization from the C-side.
 */ 
RcppExport SEXP shogun_ref_count_down(SEXP ptr) {
    // get refcount?
    Rcpp::XPtr<CSGObject> sptr(ptr);
    int ref_count = sptr->ref_count() - 1;
    _shogun_ref_count_down(ptr);
    return Rcpp::wrap(ref_count);
}

RcppExport SEXP shogun_ref_count_up(SEXP ptr) {
    Rcpp::XPtr<CSGObject> sptr(ptr);
    if (sptr) {
        sptr->ref();
    }
    return Rcpp::wrap(sptr->ref_count());
}

RcppExport SEXP shogun_ref_count(SEXP ptr) {
    Rcpp::XPtr<CSGObject> sptr(ptr);
    return Rcpp::wrap(sptr->ref_count());
}

/* -------------------------------- Threads ---------------------------------*/
RcppExport SEXP shogun_threads(SEXP n) {
BEGIN_RCPP
    Parallel *par = get_global_parallel();
    int nthreads = -1;
    
    if (!par) {
        Rprintf("ERROR: sg_parallel is not initialized.");
        return R_NilValue;
    }
    
    if (n == R_NilValue) {
        nthreads = par->get_num_threads();
    } else {
        nthreads = Rcpp::as<int>(n);
        par->set_num_threads(nthreads);
    }
    
    SG_UNREF(par);
    return Rcpp::wrap(nthreads);
END_RCPP
}

/* ------------------------------- Plumbing ---------------------------------*/
RcppExport SEXP shogun_version() {
BEGIN_RCPP
    return Rcpp::wrap(SHOGUN_VERSION);
END_RCPP
}

RcppExport SEXP init_shikken() {
BEGIN_RCPP
    init_shogun(&r_print_message, &r_print_warning, &r_print_error,
                &r_cancel_computations);
    
    return R_NilValue;
END_RCPP
}

RcppExport SEXP exit_shikken() {
    shogun::exit_shogun();
    return R_NilValue;
}

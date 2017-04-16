// import header files
#include <omp.h>
// use armadillo
#define ARMA_USE_MKL_ALLOC
#include <armadillo>
// include matlab mex header files
#include "mex.h"
#include "matrix.h"

using namespace arma;

void armaSetPr(mxArray *matlabMatrix, const Mat<double>& armaMatrix)
{
    double *dst_pointer = mxGetPr(matlabMatrix);
    const double *src_pointer = armaMatrix.memptr();
    std::memcpy(dst_pointer, src_pointer, sizeof(double)*armaMatrix.n_elem);
}

int compare_vec(const rowvec& mat_row, const rowvec& pivot_row)
{
  int v = 0;
  for (uword i = 0; i < mat_row.n_elem; i++)
  {
    if (mat_row(i) < pivot_row(i))
      v = 1;
    else if (mat_row(i) > pivot_row(i))
      v = -1;
    if (v != 0)
      break;
  }
  return v;
}

void sortrows_f(mat& M, const int left, const int right)
{
  if (left < right)
  {
    int i = left, j = right;
    uword mid_loc = (uword) (left+right)/2, pivot_loc = mid_loc;
    if (right - left > 5)
    {
      uvec sortIndex = stable_sort_index(M.col(0).subvec(mid_loc-2, mid_loc+2));
      pivot_loc = as_scalar(find(sortIndex == 2)) + mid_loc - 1;
    }
    rowvec pivot_row = M.row(pivot_loc);
    while (i <= j)
    {
      while (compare_vec(M.row( (uword) i), pivot_row) == 1)
        i++;
      while (compare_vec(M.row( (uword) j), pivot_row) == -1)
        j--;
      if (i <= j)
      {
        M.swap_rows((uword) i, (uword) j);
        i++;
        j--;
      }
    }
    if (j > 0)
      sortrows_f(M, left, j);
    if (i < (int) M.n_rows - 1)
      sortrows_f(M, i, right);
  }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arma_rng::set_seed_random();
    int max_threads_mkl = mkl_get_max_threads();
    mkl_set_num_threads(max_threads_mkl);

    mat x = Mat<double>(mxGetPr(prhs[0]), (uword) mxGetM(prhs[0]), (uword) mxGetN(prhs[0]), false, true);
    sortrows_f(x, 0, x.n_rows - 1);
    
    uvec unique_v = join_cols(ones<uvec>(1), any(x.rows(0, x.n_rows-2) != x.rows(1, x.n_rows-1), 1));
    mat output = x.rows(find(unique_v));
    plhs[0] = mxCreateDoubleMatrix(output.n_rows, output.n_cols, mxREAL);
    armaSetPr(plhs[0], output);
}


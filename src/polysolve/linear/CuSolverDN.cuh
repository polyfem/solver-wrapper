#pragma once

#ifdef POLYSOLVE_WITH_CUSOLVER

////////////////////////////////////////////////////////////////////////////////
#include "Solver.hpp"

#include <cuda_runtime.h>
#include <cusolverDn.h>

#include <vector>

////////////////////////////////////////////////////////////////////////////////
//
// https://docs.nvidia.com/cuda/cusolver/index.html#cuSolverDN-function-reference
//

namespace polysolve::linear
{
    template <typename T>
    class CuSolverDN : public Solver
    {

    public:
        CuSolverDN();
        ~CuSolverDN();

    private:
        POLYSOLVE_DELETE_MOVE_COPY(CuSolverDN)

    public:
        //////////////////////
        // Public interface //
        //////////////////////

        // Retrieve memory information from cuSolverDN
        virtual void get_info(json &params) const override;

        // Factorize system matrix (sparse)
        virtual void factorize(const StiffnessMatrix &A) override;

        // Factorize system matrix (dense, preferred)
        virtual void factorize_dense(const Eigen::MatrixXd &A) override;

        bool is_dense() const override { return true; }

        // Solve the linear system Ax = b
        virtual void solve(const Ref<const VectorXd> b, Ref<VectorXd> x) override;

        // Name of the solver type (for debugging purposes)
        virtual std::string name() const override { return "cuSolverDN"; }

    protected:
        void init();

    private:
        cusolverDnHandle_t cuHandle;
        cusolverDnParams_t cuParams;
        cudaStream_t stream;

        // device copies
        bool d_A_alloc = false;
        T *d_A;
        bool d_b_alloc = false;
        T *d_b;
        int64_t *d_Ipiv;

        // device work buffers
        size_t d_lwork = 0;     // size of workspace
        void *d_work = nullptr; // device workspace for getrf
        size_t h_lwork = 0;     // size of workspace
        void *h_work = nullptr; // host workspace for getrf
        int *d_info = nullptr;  // error info

        int numrows;
    };

} // namespace polysolve::linear

#endif

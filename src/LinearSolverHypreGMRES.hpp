#pragma once

#ifdef POLYSOLVE_WITH_HYPRE

////////////////////////////////////////////////////////////////////////////////
#include <polysolve/LinearSolver.hpp>
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <vector>

#include <HYPRE_utilities.h>
#include <HYPRE.h>
#include <HYPRE_parcsr_ls.h>
#include <HYPRE_parcsr_mv.h>
#include <IJ_matrix.h>

////////////////////////////////////////////////////////////////////////////////
//
// https://computation.llnl.gov/sites/default/files/public/hypre-2.11.2_usr_manual.pdf
// https://github.com/LLNL/hypre/blob/v2.14.0/docs/HYPRE_usr_manual.pdf
//

namespace polysolve
{

    class LinearSolverHypreGMRES : public LinearSolver
    {

    public:
        LinearSolverHypreGMRES();
        ~LinearSolverHypreGMRES();

    private:
        POLYSOLVE_DELETE_MOVE_COPY(LinearSolverHypreGMRES)

    public:
        //////////////////////
        // Public interface //
        //////////////////////

        // Set solver parameters
        virtual void setParameters(const json &params) override;

        // Retrieve memory information from Pardiso
        virtual void getInfo(json &params) const override;

        // Analyze sparsity pattern
        virtual void analyzePattern(const StiffnessMatrix &A, const int precond_num) override;

        // Factorize system matrix
        virtual void factorize(const StiffnessMatrix &) override {}

        // Solve the linear system Ax = b
        virtual void solve(const Ref<const VectorXd> b, Ref<VectorXd> x) override;

        // Name of the solver type (for debugging purposes)
        virtual std::string name() const override { return "HypreGMRES"; }

    protected:
        int dimension_ = 1; // 1 = scalar (Laplace), 2 or 3 = vector (Elasticity)
        int max_iter_ = 5000;
        int pre_max_iter_ = 1;
        double conv_tol_ = 1e-4;

        HYPRE_Int num_iterations;
        HYPRE_Complex final_res_norm;

    private:
        bool has_matrix_ = false;

        HYPRE_IJMatrix A;
        HYPRE_ParCSRMatrix parcsr_A;
    };

} // namespace polysolve

#endif

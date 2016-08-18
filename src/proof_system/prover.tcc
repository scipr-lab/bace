/** @file
 *****************************************************************************
 Implementation of interfaces for the prover.

 See prover.hpp .
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef PROVER_TCC_
#define PROVER_TCC_

namespace bace {

template<typename FieldT>
void prover(const arithmetic_circuit_t<FieldT> &circuit,
            const input_batch_t<FieldT> &input_batch,
            proof_t<FieldT> &proof)
{
    const size_t batch_size = input_batch.size();
    const size_t input_size = get_input_size(input_batch);
    const size_t column_size = get_column_size(batch_size);
    const size_t large_degree = get_large_degree(column_size, circuit.degree());
    const domain_t<FieldT> domain = get_evaluation_domain<FieldT>(large_degree);

    column_lde_t<FieldT> column_lde = compute_column_lde(input_batch, column_size);

    for (size_t i = 0; i < input_size; i++)
    {
        column_lde[i].resize(large_degree, FieldT::zero());
        domain->FFT(column_lde[i]);
    }

    proof.resize(large_degree);
    std::vector<FieldT> input_column(input_size);
    for (size_t i = 0; i < large_degree; i++)
    {
        for (size_t j = 0; j < input_size; j++)
        {
            input_column[j] = column_lde[j][i];
        }
        
        proof[i] = circuit.evaluate(input_column);
    }
    domain->iFFT(proof);
}

} // bace

#endif // PROVER_TCC_

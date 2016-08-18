/** @file
 *****************************************************************************
 Implementation of interfaces for the verifier.

 See verifier.hpp .
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef VERIFIER_TCC_
#define VERIFIER_TCC_

#include "polynomial_arithmetic/naive_evaluate.hpp"

namespace bace {

template<typename FieldT>
void verifier(const arithmetic_circuit_t<FieldT> &circuit,
              const input_batch_t<FieldT> &input_batch,
              output_batch_t<FieldT> &output_batch,
              const proof_t<FieldT> &proof)
{
    const size_t batch_size = input_batch.size();
    const size_t input_size = get_input_size(input_batch);
    const size_t column_size = get_column_size(batch_size);
    const size_t large_degree = get_large_degree(column_size, circuit.degree());
    const domain_t<FieldT> domain = get_evaluation_domain<FieldT>(large_degree);

    const column_lde_t<FieldT> column_lde = compute_column_lde(input_batch, column_size);

    const FieldT random_element = FieldT::random_element();
    std::vector<FieldT> random_input(input_size);
    for (size_t i = 0; i < input_size; i++)
    {
        random_input[i] = libfqfft::evaluate_polynomial(column_size, column_lde[i], random_element);
    }
    
    output_batch.clear();
    const FieldT output_mine = circuit.evaluate(random_input);
    const FieldT output_proof = libfqfft::evaluate_polynomial(large_degree, proof, random_element);
    if (output_mine == output_proof)
    {
        output_batch = proof;
        domain->FFT(output_batch);
        
        for (size_t i = 0; i < batch_size; i++)
        {
            output_batch[i] = output_batch[get_embedded_index(i, column_size, large_degree)];
        }
        output_batch.resize(batch_size);
    }
}

} // bace

#endif // VERIFIER_TCC_

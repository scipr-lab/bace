/** @file
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "algebra/curves/mnt/mnt4/mnt4_pp.hpp"

#include "src/arithmetic_circuit/arithmetic_circuit.hpp"
#include "src/proof_system/prover.hpp"
#include "src/proof_system/verifier.hpp"
#include "src/proof_system/naive_evaluation.hpp"

using namespace bace;

template<typename FieldT>
void test_verifier()
{
    /* Arithmetic circuit over F_q with input_size variables */
    const size_t input_size = 8;

    /* Evaluation for batch_size points */
    const size_t batch_size = 8;

    /* Input input_batch_1, ... , input_batch_{batch_size} in (F_q)^input_size */
    input_batch_t<FieldT> input_batch(batch_size);
    for (int i = 0; i < batch_size; i++)
    {
        input_batch[i] = std::vector<FieldT>(input_size);
        for (int j = 0; j < input_size; j++)
        {
            input_batch[i][j] = FieldT(rand() % FieldT::s);
        }
    }

    /* Arithmetic circuit construction */
    arithmetic_circuit_t<FieldT> circuit = arithmetic_circuit_t<FieldT>(input_size);
    circuit.add_quadratic_inner_product_gates();

    /* Proof */
    proof_t<FieldT> proof;
    prover(circuit, input_batch, proof);

    /* Output output_batch_1, ... , output_batch_k in F_q, s.t. output_batch_i = circuit(input_batch_i) */
    output_batch_t<FieldT> output_batch;
    output_batch_t<FieldT> output_batch_naive;
    verifier(circuit, input_batch, output_batch, proof);
    naive_evaluate(circuit, input_batch, output_batch_naive);

    printf("%ld == %ld\n", output_batch.size(), output_batch_naive.size());
    assert(output_batch.size() == output_batch_naive.size());
    for (size_t i = 0; i < output_batch.size(); i++)
    {
        printf("%ld == %ld\n", output_batch[i].as_ulong(), output_batch_naive[i].as_ulong());
        assert(output_batch[i] == output_batch_naive[i]);
    }
}

int main()
{
    libff::mnt4_pp::init_public_params();
    test_verifier<libff::Fr<libff::mnt4_pp> >();
    return 0;
}

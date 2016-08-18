/** @file
 *****************************************************************************
 Declaration of interfaces for the prover.
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef PROVER_HPP_
#define PROVER_HPP_

#include "src/arithmetic_circuit/arithmetic_circuit.hpp"
#include "src/proof_system/common.hpp"

namespace bace {

/*
 * Returns a proof for the verifier, given an arithmetic circuit and
 * a batch of inputs.
 *
 * The prover constructs a proof by using the input batch to compose a
 * column_lde_t and extend it to a large degree. The prover takes the FFT
 * of the each column_lde_t and evaluates the columns index-wise on the
 * provided circuit, then takes the inverse FFT of this vector construction.
 * This results in a large degree sized vector of circuit evaluations, which
 * serves as the basis for the proof that is provided to the verifier.
 *
 * proof = circuit.evaluate(column_lde[1][i], ... , column_lde[input_size][i])
 *
 * In the case that there is a size mismatch among the batch of inputs,
 * the prover will return a proof composed of a vector of zeros, or error if
 * the input size fails to match the circuit's defined input size.
 */
template<typename FieldT>
void prover(const arithmetic_circuit_t<FieldT> &circuit,
            const input_batch_t<FieldT> &input_batch,
            proof_t<FieldT> &proof);

} // bace

#include "prover.tcc"

#endif // PROVER_HPP_

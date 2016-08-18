/** @file
 *****************************************************************************
 Declaration of interfaces for the verifier.
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef VERIFIER_HPP_
#define VERIFIER_HPP_

#include "src/arithmetic_circuit/arithmetic_circuit.hpp"
#include "src/proof_system/common.hpp"

namespace bace {

/*
 * Returns a batch of outputs, given an arithmetic circuit, batch of inputs,
 * and proof from the prover.
 *
 * The verifier first performs a probabilistic check of the proof provided by
 * the prover. The verifier constructs the column_lde_t and selects a random
 * field element, evaluating the random element on the columns to compose an
 * input vector of input_size. Then, the verifier evaluates this input vector
 * on the given circuit and evaluates the random field element on the proof to
 * verify that the two outputs match.
 *
 * If the two evaluations match, there is a large probability
 * that the proof is correct, and the verifier proceeds to derive
 * the batch of outputs from the proof. After evaluating on a large degree
 * by taking the FFT of the proof, the verifier selects the outputs, seperated
 * approximately degree indices apart by properties of the FFT.
 *
 * random = FieldT::random_element()
 * random_input = [column_lde[1](random), ... , column_lde[input_size](random)]
 * output_mine = circuit.evaluate(random_input)
 * output_proof = proof(random)
 *
 * In the case that the outputs from the evaluation of the random input and
 * proof do not match, the verifier will return an empty vector as output. If
 * there is a size mismatch among the batch of inputs, the verifier will either
 * return an output batch composed of zeros, or error if the input size fails
 * to match the circuit's defined input size.
 */
template<typename FieldT>
void verifier(const arithmetic_circuit_t<FieldT> &circuit,
              const input_batch_t<FieldT> &input_batch,
              output_batch_t<FieldT> &output_batch,
              const proof_t<FieldT> &proof);

} // bace

#include "verifier.tcc"

#endif // VERIFIER_HPP_

/** @file
 *****************************************************************************
 Declaration of interfaces for naive evaluation.
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef NAIVE_EVALUATION_HPP_
#define NAIVE_EVALUATION_HPP_

#include "src/proof_system/common.hpp"

namespace bace {

/*
 * Performs the direct circuit evaluation of an input batch.
 *
 * Given a batch of inputs, all of equal length and matching the
 * circuit's input size, the circuit will evaluate the batch of inputs
 * and construct an ordered batch of outputs.
 */
template<typename FieldT>
void naive_evaluate(const arithmetic_circuit_t<FieldT> &circuit,
                    const input_batch_t<FieldT> &input_batch,
                    output_batch_t<FieldT> &output_batch);

} // bace

#include "naive_evaluation.tcc"

#endif // NAIVE_EVALUATION_HPP_

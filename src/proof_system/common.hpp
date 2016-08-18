/** @file
 *****************************************************************************
 Declaration of interfaces for common utilities.
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include "src/proof_system/domain.hpp"

namespace bace {

/***************************** INPUT / OUTPUT ********************************/

template<typename FieldT>
using input_t = std::vector<FieldT>;

template<typename FieldT>
using input_batch_t = std::vector<input_t<FieldT> >;

template<typename FieldT>
using output_batch_t = std::vector<FieldT>;

/**************************** PROVER / VERIFIER ******************************/

template<typename FieldT>
using column_lde_t = std::vector<std::vector<FieldT> >;

template<typename FieldT>
using proof_t = std::vector<FieldT>;

/*
 * Returns the input size of a given batch of inputs, or 0 if there is a
 * mismatch among input sizes in the batch.
 *
 * Given a batch of inputs, the function determines whether all inputs have
 * matching input size. As inputs should have, by definition, at least
 * one element, the function returns 0 in the case that there is a mismatch
 * of input sizes. One can use this return value as a sanity check for
 * input size correctness.
 */
template<typename FieldT>
size_t get_input_size(const input_batch_t<FieldT> &input_batch);

/*
 * Returns the column_lde_t of a given input batch in vector form, or an
 * empty vector if there is a mismatch among input sizes.
 *
 * Given a batch of inputs, consider these to be ordered as rows of inputs.
 * The function computes the column-wise low degree extension by taking
 * the inverse FFT of the columns on a column_size domain.
 *
 * column_lde[i] = IFFT([input_batch[1][i], ... , input_batch[batch_size][i]])
 */
template<typename FieldT>
column_lde_t<FieldT> compute_column_lde(const input_batch_t<FieldT> &input_batch,
                                        const size_t &column_size);

} // bace

#include "common.tcc"

#endif // COMMON_HPP_

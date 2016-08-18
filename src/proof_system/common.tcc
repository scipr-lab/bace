/** @file
 *****************************************************************************
 Implementation of interfaces for common utilities.
 
 See common.hpp .

 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef COMMON_TCC_
#define COMMON_TCC_

namespace bace {

template<typename FieldT>
size_t get_input_size(const input_batch_t<FieldT> &input_batch)
{
    const size_t input_size = input_batch[0].size();
    const bool match = std::all_of(begin(input_batch), end(input_batch),
                        [input_size](const input_t<FieldT>& input){ return input.size() == input_size; });
    return match ? input_size : 0;
}

template<typename FieldT>
column_lde_t<FieldT> compute_column_lde(const input_batch_t<FieldT> &input_batch,
                                        const size_t &column_size)
{
    const size_t batch_size = input_batch.size();
    const size_t input_size = get_input_size(input_batch);
    const domain_t<FieldT> domain = get_evaluation_domain<FieldT>(column_size);

    column_lde_t<FieldT> column_lde(input_size);
    for (size_t i = 0; i < input_size; i++)
    {
        column_lde[i] = std::vector<FieldT>(column_size);
        for (size_t j = 0; j < batch_size; j++) // Only fill up to batch_size
        {
            column_lde[i][j] = input_batch[j][i];
        }
        domain->iFFT(column_lde[i]);
    }

    return column_lde;
}

} // bace

#endif // COMMON_TCC_

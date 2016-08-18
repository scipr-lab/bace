/** @file
 *****************************************************************************
 Implementation of interfaces for domain.

 See domain.hpp .
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef DOMAIN_TCC_
#define DOMAIN_TCC_

namespace bace {

template<typename FieldT>
domain_t<FieldT> get_evaluation_domain(const size_t &domain_size)
{
    return domain_t<FieldT>(new libfqfft::basic_radix2_domain<FieldT>(domain_size));
}

unsigned int get_previous_power_of_two(const unsigned int &n)
{
    uint32_t p = n;

    p |= (p >> 1);
    p |= (p >> 2);
    p |= (p >> 4);
    p |= (p >> 8);
    p |= (p >> 16);
    p = p - (p >> 1);

    return p;
}

unsigned int get_embedded_index(const size_t &index,
                                const size_t &small_domain_size,
                                const size_t &large_domain_size)
{
    const unsigned int embedding_factor = large_domain_size / small_domain_size;
    const unsigned int jump = get_previous_power_of_two(embedding_factor);
    return index * jump;
}

size_t get_column_size(const size_t &batch_size)
{
    return libff::get_power_of_two(batch_size);
}

size_t get_large_degree(const size_t &column_size, const size_t &degree)
{
    return libff::get_power_of_two(column_size * degree);
}

} // bace

#endif // DOMAIN_TCC_

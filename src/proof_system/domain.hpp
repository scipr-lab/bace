/** @file
 *****************************************************************************
 Declaration of interfaces for domain.
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef DOMAIN_HPP_
#define DOMAIN_HPP_

#include "evaluation_domain/evaluation_domain.hpp"

namespace bace {

template<typename FieldT>
using domain_t = std::shared_ptr<libfqfft::evaluation_domain<FieldT> >;

/*
 * Returns an evaluation domain given the domain_size.
 *
 * Our construction makes the assumption that the roots of unity always exist
 * by choosing to use the basic radix-2 domain. This ensures the small domain
 * used to construct the column_lde_t will always be embedded within the
 * large domain. This structure makes it convenient to select the embedded
 * indices of the output on the large domain, as the points will be a power
 * of two apart.
 */
template<typename FieldT>
domain_t<FieldT> get_evaluation_domain(const size_t &domain_size);

/*
 * Returns the closest previous power of two, or itself if it's a power of two.
 *
 * Note: The technique will only work up to 32-bit values. For the purposes of
 * this library, this issue should not arise, as it is used with respect to
 * the circuit's degree size.
 *
 * https://stackoverflow.com/questions/2679815/previous-power-of-2
 */
unsigned int get_previous_power_of_two(const unsigned int &n);

/*
 * Returns the corresponding, embedded index in the large_domain_size for a
 * given index in the small_domain_size.
 *
 * The function assumes the small domain is embedded in the large domain,
 * allowing it to find a corresponding relation in index between the the
 * two domains. This implementation uses the basic radix-2 domain, where
 * the large_domain_size is a factor of the small_domain_size. So the jump
 * between points of interest in the large domain is some distance apart,
 * namely the factor's previous power of two in a radix-2 domain.
 */
unsigned int get_embedded_index(const size_t &index,
                                const size_t &small_domain_size,
                                const size_t &large_domain_size);

/*
 * Returns the column size given a batch_size.
 *
 * This function extends the batch size to the nearest power of two. This is
 * necessary for constructing a column_lde_t on the radix-2 domain.
 */
size_t get_column_size(const size_t &batch_size);

/*
 * Returns the large_degree domain size given a column_size and circuit degree.
 *
 * This function determines the large_degree domain size by extending the
 * product of the column_size and degree to the nearest power of two. This
 * ensures there exists an embedding of the small domain in the large domain.
 */
size_t get_large_degree(const size_t &column_size, const size_t &degree);

} // bace

#include "domain.tcc"

#endif // DOMAIN_HPP_

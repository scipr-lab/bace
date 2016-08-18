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

using namespace bace;

template<typename FieldT>
void test_circuit_evaluate()
{
    /* Arithmetic circuit C over F_q with n variables 
     * C = ((x_1 + x_2) + x_3 + x_3) * (x_1 + x_2) * x_4 */
    const size_t n = 4;
    arithmetic_circuit_t<FieldT> C = arithmetic_circuit_t<FieldT>(n);

    /* First gate */
    const input_element_t<FieldT> e1 = { VARIABLE, 1 };
    const input_element_t<FieldT> e2 = { VARIABLE, 2 };
    const gate_t<FieldT> g1 = { SUM, std::vector<input_element_t<FieldT> > { e1, e2 } };
    const int g1_number = C.add_gate(g1);

    /* Second gate */
    const input_element_t<FieldT> e3 = { VARIABLE, g1_number };
    const input_element_t<FieldT> e4 = { VARIABLE, 3 };
    const gate_t<FieldT> g2 = { SUM, std::vector<input_element_t<FieldT> > { e3, e4, e4 } };
    const int g2_number = C.add_gate(g2);

    /* Second gate */
    const input_element_t<FieldT> e5 = { VARIABLE, g2_number };
    const input_element_t<FieldT> e6 = { VARIABLE, 4 };
    const gate_t<FieldT> g3 = { PRODUCT, std::vector<input_element_t<FieldT> > { e5, e6, e3 } };
    C.add_gate(g3);

    /* Output y_1, ... , y_k in F_q, s.t. y_i = C(x_i) */
    const FieldT res = C.evaluate(std::vector<FieldT> { 2, 7, 6, 2 });
    printf("%ld == 378\n", res.as_ulong());
    assert(res == 378);
}

template<typename FieldT>
void test_circuit_evaluate_inner_product()
{
    /* Arithmetic circuit C over F_q with input_size variables */
    const size_t input_size = 4;
    arithmetic_circuit_t<FieldT> circuit = arithmetic_circuit_t<FieldT>(input_size);
    circuit.add_inner_product_gates();

    /* Output y_1, ... , y_k in F_q, s.t. y_i = C(x_i) */
    const FieldT res = circuit.evaluate(std::vector<FieldT> { 2, 7, 6, 2 });
    printf("%ld == 26\n", res.as_ulong());
    assert(res == 26);
}

template<typename FieldT>
void test_circuit_evaluate_quadratic_inner_product()
{
    /* Arithmetic circuit C over F_q with input_size variables */
    const size_t input_size = 4;
    arithmetic_circuit_t<FieldT> circuit = arithmetic_circuit_t<FieldT>(input_size);
    circuit.add_quadratic_inner_product_gates();

    /* Output y_1, ... , y_k in F_q, s.t. y_i = C(x_i) */
    const FieldT res = circuit.evaluate(std::vector<FieldT> { 2, 7, 6, 2 });
    printf("%ld == 424\n", res.as_ulong());
    assert(res == 424);
}

int main()
{
    libff::mnt4_pp::init_public_params();
    test_circuit_evaluate<libff::Fr<libff::mnt4_pp> >();
    test_circuit_evaluate_inner_product<libff::Fr<libff::mnt4_pp> >();
    test_circuit_evaluate_quadratic_inner_product<libff::Fr<libff::mnt4_pp> >();
    return 0;
}

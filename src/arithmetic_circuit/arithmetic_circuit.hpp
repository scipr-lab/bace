/** @file
 *****************************************************************************
 Declaration of interfaces for arithmetic circuit.
 
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef ARITHMETIC_CIRCUIT_HPP_
#define ARITHMETIC_CIRCUIT_HPP_

#include "src/proof_system/common.hpp"

namespace bace {

/************************ ARITHMETIC CIRCUIT GATE ****************************/

/* 
 * A circuit gate is composed of a type (ex. SUM, PRODUCT) and a vector of
 * input gates. Each input gate is defined by its input type, which is
 * either an index reference to the variable (gate number), or a field element
 * treated as a constant value.
 *
 * For a detailed explanation and example, see the evaluate() function below.
 */

enum input_type_t { CONSTANT, VARIABLE };

template<typename FieldT>
union input_value_t
{
    int variable;
    FieldT constant;
};

template<typename FieldT>
struct input_element_t
{
    input_type_t type;
    input_value_t<FieldT> value;
};

enum gate_type_t { SUM, PRODUCT };

template<typename FieldT>
struct gate_t
{
    gate_type_t type;
    std::vector<input_element_t<FieldT> > input_gates;
};

/*************************** ARITHMETIC CIRCUIT ******************************/

template<typename FieldT>
class arithmetic_circuit_t {
public:
    arithmetic_circuit_t(const size_t &input_size) : _input_size(input_size) {};

    /*
     * Returns the evaluation of the circuit for the given input. Note that
     * the size of input_t must match the circuit's input size. If the circuit
     * contains no gates, the evaluation will return 0.
     */
    FieldT evaluate(const input_t<FieldT> &input) const;

    /* 
     * Adds the provided gate to the circuit. Each gate is composed of
     * two parts, a gate type (ex. SUM, PRODUCT) and a vector of input gates.
     *
     * The input gates are composed of input_element_t's. Each input_element_t
     * is composed of a type (ex. CONSTANT, VARIABLE) and a value. For an
     * input type of CONSTANT, provide the constant field element. For an
     * input type of VARIABLE, provide the integer gate number.
     *
     * The following is an example construction:
     *
     * input_element_t<FieldT> e1 = { VARIABLE, gate_number_1 };
     * input_element_t<FieldT> e2 = { VARIABLE, gate_number_2 };
     * gate_t<FieldT> gate = { PRODUCT, std::vector<input_element_t<FieldT> > { e1, e2 } };
     */
    int add_gate(const gate_t<FieldT> &g);

    /* Clears all sum and product gates from the circuit */
    void clear_gates();

    /* Returns the sum of input gates, sum gates, and product gates. */
    size_t size() const;

    /* Returns the largest degree computed by any particular gate. */
    size_t degree() const;

    /* Returns the number of inputs for the circuit */
    size_t num_inputs() const;

    /* Prints circuit size, circuit degree, and number of inputs */
    void print_info() const;

    /* 
     * Adds a quadratic circuit of degree 3, by dividing the input length
     * into two components, left and right, each of length middle = input / 2.
     * The circuit takes an inner product of the left component with the
     * right component.
    */
    void add_inner_product_gates();

    /* 
     * Adds a quadratic circuit of degree 3, by dividing the input length
     * into two components, left and right, each of length middle = input / 2.
     * The circuit takes an inner product of the left component with itself
     * length middle times. This constructs a new length middle component,
     * call it square. The circuit then takes an inner product of the 
     * square component with the right component.
    */
    void add_quadratic_inner_product_gates();

private:
    const size_t _input_size;
    std::vector<gate_t<FieldT> > _gates;
};

} // bace

#include "arithmetic_circuit.tcc"

#endif // ARITHMETIC_CIRCUIT_HPP_

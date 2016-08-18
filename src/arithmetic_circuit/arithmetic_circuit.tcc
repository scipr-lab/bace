/** @file
 *****************************************************************************
 Implementation of interfaces for arithmetic circuit.
 
 See arithmetic_circuit.hpp .

 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef ARITHMETIC_CIRCUIT_TCC_
#define ARITHMETIC_CIRCUIT_TCC_

#include <algorithm>
#include <cassert>
#include <stdlib.h>
#include <vector>

namespace bace {

template<typename FieldT>
FieldT arithmetic_circuit_t<FieldT>::evaluate(const input_t<FieldT> &input) const
{
    assert(input.size() == this->_input_size);
    assert(this->_gates.size() > 0);

    std::vector<FieldT> gate_output(input);
    gate_output.resize(this->size(), FieldT::zero());

    FieldT output;
    size_t i = this->_input_size;
    for (auto gate: this->_gates)
    {
        if (gate.type == SUM)
        {
            output = FieldT::zero();
            for (auto input_gate: gate.input_gates)
            {
                if (input_gate.type == CONSTANT) output += input_gate.value.constant;
                else output += gate_output[input_gate.value.variable - 1];
            }
        }
        else if (gate.type == PRODUCT)
        {
            if (gate.input_gates[0].type == CONSTANT) output = gate.input_gates[0].value.constant;
            else output = gate_output[gate.input_gates[0].value.variable - 1];

            for (int j = 1; j < gate.input_gates.size(); j++)
            {
                if (gate.input_gates[j].type == CONSTANT) output *= gate.input_gates[j].value.constant;
                else output *= gate_output[gate.input_gates[j].value.variable - 1];
            }
        }
        gate_output[i++] = output;
    }

    return output;
}

template<typename FieldT>
int arithmetic_circuit_t<FieldT>::add_gate(const gate_t<FieldT> &g)
{
    assert(g.input_gates.size() > 0);
    assert(g.type == SUM || g.type == PRODUCT);

    this->_gates.emplace_back(g);
    return this->size();
}

template<typename FieldT>
void arithmetic_circuit_t<FieldT>::clear_gates()
{
    this->_gates.clear();
}

template<typename FieldT>
size_t arithmetic_circuit_t<FieldT>::size() const
{
    return this->_input_size + this->_gates.size();
}

template<typename FieldT>
size_t arithmetic_circuit_t<FieldT>::degree() const
{
    std::vector<size_t> degree(this->_input_size, 1);
    degree.resize(this->size(), 0);

    size_t max_degree = 0;
    size_t i = this->_input_size;
    for (auto gate: this->_gates)
    {
        size_t gate_degree = 0;
        if (gate.type == SUM)
        {
            for (auto input_gate: gate.input_gates)
            {
                if (input_gate.type == VARIABLE)
                {
                    gate_degree = std::max(gate_degree, degree[input_gate.value.variable - 1]);
                }
            }
        }
        else if (gate.type == PRODUCT)
        {
            for (auto input_gate: gate.input_gates)
            {
                if (input_gate.type == VARIABLE)
                {
                    gate_degree += degree[input_gate.value.variable - 1];
                }
            }
        }

        degree[i++] = gate_degree;
        max_degree = std::max(max_degree, gate_degree);
    }

    return max_degree;
}

template<typename FieldT>
size_t arithmetic_circuit_t<FieldT>::num_inputs() const
{
    return this->_input_size;
}

template<typename FieldT>
void arithmetic_circuit_t<FieldT>::print_info() const
{
    printf("* Circuit size: %zu\n", this->size());
    printf("* Circuit degree: %zu\n", this->degree());
    printf("* Number of inputs: %zu\n", this->num_inputs());
}

template<typename FieldT>
void arithmetic_circuit_t<FieldT>::add_inner_product_gates()
{
    const bool odd = this->_input_size % 2 == 1;
    const int mid = odd ? (this->_input_size / 2) + 1 : this->_input_size / 2;

    std::vector<input_element_t<FieldT> > input_gates;
    for (int i = 1; i <= mid; i++)
    {
        if (i == mid - 1 && odd) continue;

        const input_element_t<FieldT> left = { VARIABLE, i };
        const input_element_t<FieldT> right = { VARIABLE, mid + i };
        const gate_t<FieldT> product_gate = { PRODUCT, std::vector<input_element_t<FieldT> > { left, right } };
        const int gate_number = this->add_gate(product_gate);

        const input_element_t<FieldT> element = { VARIABLE, gate_number };
        input_gates.emplace_back(element);
    }

    const gate_t<FieldT> sum_gate = { SUM, input_gates };
    this->add_gate(sum_gate);
}

template<typename FieldT>
void arithmetic_circuit_t<FieldT>::add_quadratic_inner_product_gates()
{
    const bool odd = this->_input_size % 2 == 1;
    const int mid = odd ? (this->_input_size / 2) + 1 : this->_input_size / 2;

    std::vector<input_element_t<FieldT> > product_input_gates;
    for (int i = 0; i < mid; i++)
    {
        std::vector<input_element_t<FieldT> > input_gates;
        for (int j = 1; j <= mid; j++)
        {
            if (j == mid - 1 && odd) continue;

            const input_element_t<FieldT> left = { VARIABLE, j };
            const input_element_t<FieldT> right = { VARIABLE, j };
            const gate_t<FieldT> product_gate = { PRODUCT, std::vector<input_element_t<FieldT> > { left, right } };
            const int gate_number = this->add_gate(product_gate);

            const input_element_t<FieldT> element = { VARIABLE, gate_number };
            input_gates.emplace_back(element);
        }

        const gate_t<FieldT> sum_gate = { SUM, input_gates };
        const int gate_number = this->add_gate(sum_gate);

        const input_element_t<FieldT> element = { VARIABLE, gate_number };
        product_input_gates.emplace_back(element);
    }
    
    std::vector<input_element_t<FieldT> > input_gates;
    for (int i = 1; i <= mid; i++)
    {
        if (i == mid - 1 && odd) continue;

        const input_element_t<FieldT> right = { VARIABLE, mid + i };
        const gate_t<FieldT> product_gate = { PRODUCT, std::vector<input_element_t<FieldT> > { product_input_gates[i-1], right } };
        const int gate_number = this->add_gate(product_gate);

        const input_element_t<FieldT> element = { VARIABLE, gate_number };
        input_gates.emplace_back(element);
    }

    const gate_t<FieldT> sum_gate = { SUM, input_gates };
    this->add_gate(sum_gate);
}

} // bace

#endif // ARITHMETIC_CIRCUIT_TCC_

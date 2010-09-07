/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Deals with the address space of the overlay
 */

#ifndef SIM_HELPER_H
#define SIM_HELPER_H
typedef unsigned int v_space_t;
v_space_t v_space_abs_dist(v_space_t v0, v_space_t v1);
int v_space_compare(v_space_t v0, v_space_t v1);
#endif

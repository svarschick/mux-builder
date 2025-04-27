#ifndef __MUX_H
#define __MUX_H

#include "binary_tree.h"
#include "func_table.h"
#include "element.h"
#include <iomanip>
#include <algorithm>
#include <limits.h>
#include <thread>
#include <chrono>
#include <atomic>
#include "cmath"

class mux : public element
{
public:
	std::string d0;
	std::string d1;
	std::string a0;

public:
	mux(const std::string& d0, const std::string& d1, const std::string& a0) : d0(d0), d1(d1), a0(a0) {}
	friend std::ostream& operator<<(std::ostream& os, const mux& mx) {
		std::size_t setw = 9 + std::to_string(mx.index()).size();
		os << ".mux2_1 (.index = " << mx.index() << ", .d0 = " << std::setw(setw) << mx.d0 << ", .d1 = " << std::setw(setw) << mx.d1 << ", .a0 = " << mx.a0 << ")";
		return os;
	}
};



void _comlexity_func(const func_table<>& table, std::shared_ptr<binary_tree<mux>::_node> node, func<> func, const std::size_t min_complexity, std::size_t curr_complexity) {
	if (!node) return;
	if (min_complexity < curr_complexity) 
		return;

	std::size_t func_size = func.size();

	func_table<>::_func lf = func.left_half();
	if (lf.equal0()) node->val.d0 = "0";
	else if (lf.equal1()) node->val.d0 = "1";
	else {
		for (std::size_t i = 0; i < lf.rang(); i++) {
			func_table<>::_func var_func = table.func_var(table.order_index_var(i)).sub_func(0, static_cast<std::size_t>(std::pow(2, lf.rang())));
			var_func.resize(std::pow(2, lf.rang()), false);
			if (lf == var_func)
				node->val.d0 = "x" + std::to_string(table.order_index_var(i));
			if (lf == var_func.flip())
				node->val.d0 = "!x" + std::to_string(table.order_index_var(i));
		}
	}

	func_table<>::_func rf = func.right_half();
	if (rf.equal0()) node->val.d1 = "0";
	else if (rf.equal1()) node->val.d1 = "1";
	else {
		for (std::size_t i = 0; i < rf.rang(); i++) {
			func_table<>::_func var_func = table.func_var(table.order_index_var(i)).sub_func(0, static_cast<std::size_t>(std::pow(2, rf.rang())));
			var_func.resize(std::pow(2, rf.rang()), false);
			if (rf == var_func)
				node->val.d1 = "x" + std::to_string(table.order_index_var(i));
			if (rf == var_func.flip())
				node->val.d1 = "!x" + std::to_string(table.order_index_var(i));
		}
	}

	if (node->val.d0 == "") {
		node->left = std::make_shared<binary_tree<mux>::_node>(mux("", "", "x" + std::to_string(table.order_index_var(func.rang() - 2))));
		_comlexity_func(table, node->left, lf, min_complexity, curr_complexity + 1);
	}
	if (node->val.d1 == "") {
		node->right = std::make_shared<binary_tree<mux>::_node>(mux("", "", "x" + std::to_string(table.order_index_var(func.rang() - 2)) ));
		_comlexity_func(table, node->right, rf, min_complexity, curr_complexity + 1);
	}
}
void _connect_mux(std::shared_ptr<binary_tree<mux>::_node> node) {
	if (!node) return;

	if (node->left) {
		node->val.d0 = ".mux2_1[" + std::to_string(node->left->val.index()) + "]";
		_connect_mux(node->left);
	}
	if (node->right) {
		node->val.d1 = ".mux2_1[" + std::to_string(node->right->val.index()) + "]";
		_connect_mux(node->right);
	}
}
std::size_t complexity_func(const func_table<>& table, const std::size_t min_complexity, bool logs = false, bool mux_connect = false) {
	if (table.equal0() || table.equal1())
		return 0;

	for (std::size_t i = 0; i < table.rang(); i++) {
		func_table<>::_func var_func = table.func_var(table.order_index_var(i));
		var_func.resize(std::pow(2, table.rang()), false);
		if (table.func() == var_func)
			return 0;
		if (table.func() == var_func.flip())
			return 0;
	}

	mux mx0("", "", "x" + std::to_string(table.order_index_var(table.rang() - 1)));
	binary_tree<mux> bt(std::make_shared<binary_tree<mux>::_node>(mx0));
	_comlexity_func(table, bt.head(), table, min_complexity, 1);
	if (mux_connect) {
		_connect_mux(bt.head());
	}
	if (logs) {
		bt.print();
	}

	return bt.size();
}
std::size_t fact(const std::size_t x) {
	std::size_t result = 1;
	for (std::size_t i = 1; i <= x; i++) result *= i;
	return result;
}
static std::size_t total_permutation = 0;
void show_animation(std::atomic<bool>& running) {
	std::cout << "" << std::endl;
	const std::string animation = "/-\\|";
	const std::size_t refresh_delay = 500;
	std::size_t index = 0;
	std::size_t prev_total_tree = 0;
	printf("\n\n\n\n");

	while (running) {
		float curr_permutation = total_tree / static_cast<float>(total_permutation) * 100;

		printf("\033[4A\033[2K\033[0G" "search best function on mux... %4.2f%% %c\n", curr_permutation, animation[index]);
		printf("\033[2K\033[0G"        "avg trees:   %d per second\n", static_cast<int>((total_tree - prev_total_tree) / (refresh_delay / 1000.0f)));
		printf("\033[2K\033[0G"        "total mux:   %d\n", constructor);
		printf("\033[2K\033[0G"        "total trees: %d\n", total_tree);
		
		index = (index + 1) % animation.size();
		prev_total_tree = total_tree;
		std::this_thread::sleep_for(std::chrono::milliseconds(refresh_delay));
	}
}
void find_mux(const func_table<>& table, bool logs = false) {
	auto xvars = table.xvarset();
	func_table<> wtable = table;
	func_table<> best_table = table;
	
	std::size_t best_complexity = ULLONG_MAX;
	std::size_t iteration = 0;

	std::atomic<bool> running(true);
	total_permutation = fact(table.rang());
	std::thread animation_thread(show_animation, std::ref(running));

	do {
		if (logs) {
			std::cout << "iteration: " << iteration++ << std::endl;
			wtable.print_table();
		}
		std::size_t complexity = complexity_func(wtable, best_complexity, logs);
		if (logs) {
			std::cout << "mux complexity: " << complexity << std::endl;
			std::cout << std::endl;
		}

		if (complexity < best_complexity) {
			best_complexity = complexity;
			best_table = wtable;
		}
		wtable.reorder_vars(xvars[0].index, xvars[1].index);
	} 
	while (std::next_permutation(xvars.begin(), xvars.end()));

	running = false;
	animation_thread.join();

	std::cout << "\n---> best function <---" << std::endl;
	best_table.print_table();
	complexity_func(best_table, best_complexity, true, true);
	std::cout << "mux complexity: " << best_complexity << std::endl;
}

#endif
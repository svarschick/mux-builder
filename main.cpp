#include <iostream>

#include "func.h"
#include "xvar.h"
#include "func_table.h"
#include "mux.h"
#include <memory>
#include <chrono>
#include <iostream>
#include <thread>
#include <stdio.h>

void call()
{
	binary_tree<std::string> bn_mux(std::make_shared<node<std::string>>("s1"));

	bn_mux.head()->left = std::make_shared<node<std::string>>("s2");
	bn_mux.head()->right = std::make_shared<node<std::string>>("s3");
	bn_mux.head()->left->left = std::make_shared<node<std::string>>("s4");
	bn_mux.head()->left->right = std::make_shared<node<std::string>>("s5");
	bn_mux.head()->right->left = std::make_shared<node<std::string>>("s6");
	bn_mux.head()->right->right = std::make_shared<node<std::string>>("s7");
	bn_mux.head()->left->left->left = std::make_shared<node<std::string>>("s8");
	bn_mux.head()->left->left->right = std::make_shared<node<std::string>>("s9");
	bn_mux.head()->left->right->left = std::make_shared<node<std::string>>("s10");
	bn_mux.head()->left->right->right = std::make_shared<node<std::string>>("s11");
	bn_mux.head()->right->left->left = std::make_shared<node<std::string>>("s12");
	bn_mux.head()->right->left->right = std::make_shared<node<std::string>>("s13");
	bn_mux.head()->right->right->left = std::make_shared<node<std::string>>("s14");
	bn_mux.head()->right->right->right = std::make_shared<node<std::string>>("s15");

	bn_mux.print();
	node<std::string>::_node_p find_node = bn_mux.find_node(3, 2);
	std::cout << bn_mux.size() << std::endl;

	std::cout << "constructor: " << constructor << "\ncopy_constructor: " << copy_constructor
		<< "\nmove_constructor: " << move_constructor << "\ncopy_operator: " << copy_operator
		<< "\nmove_operator: " << move_operator << "\ndelete_constructor: " << delete_constructor << std::endl;
}
#include <iostream>

int main()
{
	auto start = std::chrono::system_clock::now();
	{
		// ABF08B12ABF08B12ABF08B12ABF08B12ABF08B12ABF08B12ABF08B12ABF08B12
		std::vector<xvar<>> set{ { 3, 0 }, { 2, 1 }, { 1, 1 }, {0, 1} };
		std::reverse(set.begin(), set.end());
		func_table table("0x3A5AD5B8C542F7056A40FBDDDB2C67C7");
		std::cout << "info: " << std::endl;
		std::cout << "F = 0x" << table.to_hex_string() << std::endl;
		table.print_table();
		find_mux(table, false);
	}
	auto end = std::chrono::system_clock::now();
	auto delay = end - start;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(delay);
	std::cout << "work time: " << ms.count() / 1000.0 << "s" << std::endl;

	std::cout << "total trees: " << total_tree << "\nconstructor: " << constructor << "\ncopy_constructor: " << copy_constructor
		<< "\nmove_constructor: " << move_constructor << "\ncopy_operator: " << copy_operator
		<< "\nmove_operator: " << move_operator << "\ndelete_constructor: " << delete_constructor << std::endl;
	
	int a;
	std::cin >> a;


	return 0;
}

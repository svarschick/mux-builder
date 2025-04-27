#ifndef __BINARY_TREE
#define __BINARY_TREE

#include <type_traits>
#include <utility>
#include <string>
#include <memory>

#define __WARNING

// 
template<typename, typename>
struct has_cout : std::false_type {};

template<typename _Ty>
struct has_cout< _Ty, std::void_t< decltype(operator<<(std::declval<std::iostream>(), std::declval<_Ty>())) > > : std::true_type {};

template<typename _Ty>
constexpr bool is_cout = has_cout<_Ty, void>::value;
// ^^^ - 

static int constructor = 0;
static int copy_constructor = 0;
static int move_constructor = 0;
static int copy_operator = 0;
static int move_operator = 0;
static int delete_constructor = 0;
static int total_tree = 0;
static int total_node = 0;

template<typename _Ty>
class node
{
public:
	using _node_p = std::shared_ptr<node>;

	_Ty val;
	_node_p left;
	_node_p right;

	//node(_Ty val = {}, node* left = nullptr, node* right = nullptr) : val(val), left(left), right(right) {
	//	constructor++;
	//};
	node(_Ty val = {}, _node_p left = nullptr, _node_p right = nullptr) : val(val), left(left), right(right) {
		constructor++;
	};
	//
	node(const node& node) : val(node.val) {
		copy_constructor++;

		if (node.left) this->left = new node(node.left->val, node.left->left, node.left->right);
		else this->left = nullptr;

		if (node.right) this->right = new node(node.right->val, node.right->left, node.right->right);
		else this->right = nullptr;
	}
	node(node&& node) : val(std::move(node.val)) {
		move_constructor++;
		node.left = this->left;
		node.right = this->right;
		this->left = nullptr;
		this->right = nullptr;
	}
	//
	node& operator=(const node& node) {
		copy_operator++;
		this->val = node.val;
		if (node.left)
			this->left = new node(node.left->val, node.left->left, node.left->right);
		if (node.right)
			this->right = new node(node.right->val, node.right->left, node.right->right);
	}
	node& operator=(node&& node) {
		move_operator++;
		this->val = std::move(node.val);
		this->left = node.left;
		this->right = node.right;
		node.left = nullptr;
		node.right = nullptr;
	}
	~node() {
		total_node++;
		delete_constructor++;
		left = nullptr;
		right = nullptr;
	}

	template<typename _Ty>
	friend std::ostream& operator<<(std::ostream& os, const node<_Ty>& f);
};

//
template<typename _Ty>
std::ostream& operator<<(std::ostream& os, const node<_Ty>& tn) {
	if constexpr (is_cout<_Ty>)
		os << tn.val;
#ifdef __WARNING
	else std::cerr << "WARNING: the type \"" + std::string(typeid(_Ty).name()) + "\" does not support the output stream via std::cout";
#endif

	return os;
}

template<typename _Ty>
class binary_tree
{
public:
	using _node = node< _Ty>;
	using _node_p = typename _node::_node_p;

	binary_tree() { total_tree++; };
	binary_tree(_node_p node) : _head(node) { total_tree++; };
	~binary_tree() {
		delete_tree();
	}

	inline _node_p head() noexcept { return _head; }

	// 
	void print() const noexcept {
		_print(_head);
	}

	// Returns a nullptr if the node could not be found at the specified coordinates
	_node_p find_node(std::size_t deep, std::size_t weight) const noexcept {
		_node_p curr_node = _head;
		if (weight > deep * 2)
			return nullptr;

		for (std::size_t i = 0; i < deep; i++) {
			if (curr_node) {
				if (weight & (static_cast<std::size_t>(1) << deep - i - static_cast<std::size_t>(1)))
					curr_node = curr_node->right;
				else
					curr_node = curr_node->left;
			}
			else return nullptr;
		}

		return curr_node;
	}
	// 
	void delete_tree() noexcept {
		_delete_node(_head);
	}
	// 
	bool delete_node(std::size_t deep, std::size_t weight) noexcept {
		_node d_node = find_node(deep, weight);
		if (d_node) {
			_delete_node(d_node);
			return true;
		}
		return false;
	}
	// Counts the number of elements in the entire tree
	std::size_t size() const noexcept {
		return _size(_head, 1);
	}

private:
	void _print(const _node_p node, std::size_t deep = 0, std::size_t weight = 0) const noexcept {
		std::cout << "ords: " << deep << "." << weight << "\t" << *node << std::endl;
		if (node->left)  _print(node->left, deep + 1, weight * 2);
		if (node->right) _print(node->right, deep + 1, weight * 2 + 1);
		return;
	}
	std::size_t _size(const _node_p node, std::size_t count) const noexcept {
		if (node->left)  count += _size(node->left, 1);
		if (node->right) count += _size(node->right, 1);
		if (!node->left && !node->right) return 1;
		return count;
	}
	void _delete_node(_node_p node) noexcept {
		node = nullptr;
	}

private:
	_node_p _head;
};

#endif
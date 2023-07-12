#include "merkle_tree.h"

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")

merkle_leaf::merkle_leaf(uint32_t in) 
{
    uint8_t data[4];
    data[0] = in & 0xff;
    data[1] = (in >> 8) & 0xff;
    data[2] = (in >> 16) & 0xff;
    data[3] = (in >> 24) & 0xff;

    uint32_t out_size = 32;
    EVP_Digest(data, 4, hash_value.data(), &out_size, EVP_sha256(), NULL);
}
merkle_leaf::~merkle_leaf() = default;


Node::Node() : hash_value{}, leaf_nums{ 0 } {}

Node::Node(merkle_leaf& in) : hash_value{ in.get_hash_value() }, leaf_nums{ 1 } {}

Node::~Node() = default;

Node::Node(Node& in1, Node& in2) {
    hash_value = in1.hash_value;
    hash_value.insert(hash_value.end(), in2.hash_value.begin(), in2.hash_value.end());
    leaf_nums = in1.leaf_nums + in2.leaf_nums;
}
std::vector<HASH_TYPE> Node::get_hash_value() {
    return hash_value;
}
uint64_t Node::get_leaf_nums() {
    return leaf_nums;
}

void Node::print_hash() {
    for (auto x : hash_value) {
        for (auto i : x) {
            printf("%02x", i);
        }
        printf("\n");
    }
}


void merkle_tree::add(uint32_t value) {
    is_calculated = false;
    num_nodes++;
    merkle_leaf leaf(value);
    leaf_node.push_back(leaf);

    int max_nodes_in_level = 1 << (level - 1);
    if (num_nodes > max_nodes_in_level) {
        level++;
    }
}
void merkle_tree::update() {
    internal_node.clear();
    internal_node.resize((1 << level) - 1);

    leaf_start_idx = (1 << (level - 1)) - 1;
    for (uint64_t i = 0; i < leaf_node.size(); i++) {
        internal_node[leaf_start_idx + i] = Node(leaf_node[i]);
    }
    for (uint64_t j = leaf_start_idx - 1; j > 0; j--) {
        internal_node[j] = Node(internal_node[(j << 1) + 1], internal_node[(j << 1) + 2]);
    }
    internal_node[0] = Node(internal_node[1], internal_node[2]);
    is_calculated = true;
}

uint64_t merkle_tree::sibling_node(uint64_t index) {
    return index ^ 1;
}

std::vector<HASH_TYPE> merkle_tree::make_proof() {
    if (!is_calculated)
        update();

    return internal_node[0].get_hash_value();
}

merkle_tree::merkle_tree() : num_nodes(0), level(1), is_calculated(false) {
    internal_node.clear();
    leaf_node.clear();
}

merkle_tree::~merkle_tree() = default;


bool is_internal_node_equal(Node* in1, Node* in2) {
    auto value1 = in1->get_hash_value();
    auto value2 = in2->get_hash_value();

    return value1 == value2;
}
bool merkle_tree::inclusion_proof(uint64_t index, merkle_leaf* in) {
    if (!is_calculated) {
        update();
    }
    uint64_t tree_idx = leaf_start_idx + index;
    Node vrfy_node = Node(*in);

    while (tree_idx != root_index) {
        uint64_t pa_idx = ((tree_idx - 1) >> 1);
        if (tree_idx & 1) {
            vrfy_node = Node(vrfy_node, internal_node[(pa_idx << 1) + 2]);
        }
        else {
            vrfy_node = Node(internal_node[(pa_idx << 1) + 1], vrfy_node);
        }
        tree_idx = pa_idx;
    }

    bool is_valid = is_internal_node_equal(&vrfy_node, &internal_node[0]);

    std::cout << "Inclusion proof for index " << index << " is ";
    if (is_valid) {
        std::cout << "valid.\n";
    }
    else {
        std::cout << "invalid.\n";
    }

    return is_valid;
}

uint64_t merkle_tree::get_num_nodes() {
    return num_nodes;
}

bool merkle_tree::consistency_proof(merkle_tree* old_tree) {
    if (!is_calculated) {
        update();
    }
    old_tree->update();

    uint64_t old_size = old_tree->num_nodes;
    uint64_t new_size = num_nodes;

    if (old_size == new_size) {
        std::cout << "The trees are already consistent." << std::endl;
        return true;
    }

    std::cout << "Checking consistency proof between old tree (size " << old_size << ") and new tree (size " << new_size << ")..." << std::endl;

    uint64_t log_old_size = int(log2(old_size));
    Node con_vrfy = internal_node[leaf_start_idx];
    uint64_t node_idx = leaf_start_idx;

    for (uint64_t i = 0; i < log_old_size; i++) {
        node_idx = (node_idx - 1) >> 1;
        con_vrfy = internal_node[node_idx];
    }

    uint64_t leaf_num = con_vrfy.get_leaf_nums();

    while (leaf_num != old_size) {
        node_idx += 1;

        if ((old_size - leaf_num) == internal_node[node_idx].get_leaf_nums()) {
            con_vrfy = Node(con_vrfy, internal_node[node_idx]);
            break;
        }
        if ((old_size - leaf_num) > internal_node[node_idx].get_leaf_nums()) {
            con_vrfy = Node(con_vrfy, internal_node[node_idx]);
            node_idx += 1;
        }
        else {
            node_idx = (node_idx << 1);
        }
        leaf_num = con_vrfy.get_leaf_nums();
    }

    auto node = old_tree->internal_node[0];
    bool ans = is_internal_node_equal(&con_vrfy, &node);
    if (!ans) {
        std::cout << "Consistency proof error: the old tree differs from the new tree at node " << node_idx << "." << std::endl;
        return false;
    }

    auto audit_node = internal_node[node_idx];
    while (node_idx != root_index) {
        if (node_idx & 1) {
            audit_node = Node(audit_node, internal_node[node_idx + 1]);
        }
        else {
            audit_node = Node(internal_node[node_idx - 1], audit_node);
        }
        node_idx = (node_idx - 1) >> 1;
    }

    ans = is_internal_node_equal(&audit_node, &internal_node[root_index]);
    if (!ans) {
        std::cout << "Consistency proof error: the new tree root differs from the expected value." << std::endl;
        return false;
    }

    std::cout << "Consistency proof successful." << std::endl;
    return true;
}

int main() {
    merkle_tree tree;
    LARGE_INTEGER t1, t2, t3, t4, tc;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    // 构建10w个节点的 Merkle 树
    for (int i = 1; i <= 100000; i++) {
        tree.add(i);
    }
    std::cout << "Merkle tree with " << tree.get_num_nodes() << " nodes has been successfully built!" << std::endl;
    QueryPerformanceCounter(&t2);
    std::cout << "Build time：" << ((long double)(t2.QuadPart - t1.QuadPart) / (long double)tc.QuadPart) * 1000 << "ms\n";

    auto root_hash = tree.make_proof();

    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    // 进行包含证明测试
    std::cout << "Inclusion proofs:" << std::endl;
    for (int i = 0; i < 5; i++) {
        merkle_leaf leaf(i + 1);
        bool included = tree.inclusion_proof(i, &leaf);
    }
    QueryPerformanceCounter(&t2);
    std::cout << "Inclusion proofs time：" << ((long double)(t2.QuadPart - t1.QuadPart) / (long double)tc.QuadPart) * 1000 << "ms\n";
    // 进行一致性证明测试
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    std::cout << "Consistency proof:" << std::endl;
    merkle_tree old_tree;
    for (int i = 1; i <= 5; i++) {
        old_tree.add(i);
    }
    bool consistent = tree.consistency_proof(&old_tree);
    std::cout << "Consistency proof result: " << (consistent ? "Valid" : "Invalid") << std::endl;
    QueryPerformanceCounter(&t2);
    std::cout << "Consistency proof time：" << ((long double)(t2.QuadPart - t1.QuadPart) / (long double)tc.QuadPart) * 1000 << "ms\n";
    return 0;
}
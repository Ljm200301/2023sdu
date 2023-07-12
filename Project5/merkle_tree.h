#pragma once
#include <iostream>
#include<windows.h>
#include <iomanip>
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <array>
#include <cmath>


typedef std::array<uint8_t, 32> HASH_TYPE;
//openssl1.1й╣ож

class merkle_leaf
{
private:
    HASH_TYPE hash_value;
    uint32_t value;
public:
    merkle_leaf() = default;
    merkle_leaf(uint32_t in);
    HASH_TYPE get_hash_value() { return hash_value; };
    uint32_t get_value() { return value; };
    ~merkle_leaf();
};


class Node
{
private:
    std::vector<HASH_TYPE> hash_value;
    uint64_t leaf_nums;

public:
    Node();
    Node(merkle_leaf& in);
    Node(Node& in1, Node& in2);
    ~Node();
    std::vector<HASH_TYPE> get_hash_value();
    uint64_t get_leaf_nums();
    void print_hash();

};

bool is_internal_node_equal(Node* in1, Node* in2);

class merkle_tree
{
private:
    std::vector<Node> internal_node;
    std::vector<merkle_leaf> leaf_node;
    uint64_t num_nodes;

    uint64_t level;
    bool is_calculated;
    uint64_t leaf_start_idx;
public:

    merkle_tree();
    ~merkle_tree();
    const uint64_t root_index = 0;
    uint64_t sibling_node(uint64_t node_idx);
    uint64_t get_num_nodes();
    void add(uint32_t value);
    void update();
    std::vector<HASH_TYPE> make_proof();
    bool inclusion_proof(uint64_t idx, merkle_leaf* in);
    bool consistency_proof(merkle_tree* old_tree);

};


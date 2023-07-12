# 实现要求
• Construct a Merkle tree with 10w leaf nodes

• Build inclusion proof for specified element

• Build exclusion proof for specified element

# 实现方式
利用openssl1.1实现，在merkle_tree.h头文件中定义了几个类：merkle_leaf类表示Merkle树中的叶节点，Node类表示Merkle树中的内部节点，merkle_tree类表示Merkle树数据结构。

以下是对几个重要函数的解释说明:

void merkle_tree::add(uint32_t value)：此函数将新的叶节点添加到Merkle树中，其整数值为value。它创建一个新的Merkle叶节点并将其附加到叶节点列表中。如果树中的节点数超过当前级别的最大节点数，则增加树的级别。

void merkle_tree::update()：此函数通过计算树的内部节点来更新Merkle树。它首先清除内部节点列表并将其调整为适当的大小。然后为每个叶节点创建Merkle节点，并将其存储在内部节点列表的适当位置。然后迭代地组合节点以计算树的内部节点。

uint64_t merkle_tree::sibling_node(uint64_t index)：此函数返回给定索引的节点的兄弟节点的索引。

std::vector<HASH_TYPE> merkle_tree::make_proof()：此函数返回Merkle树根哈希值，其可用作整个树完整性的证明。

bool is_internal_node_equal(Node* in1, Node* in2)：此函数比较两个Merkle树节点的哈希值，如果它们相等，则返回true。

bool merkle_tree::inclusion_proof(uint64_t index, merkle_leaf* in)：此函数检查给定索引处的叶节点的Merkle证明是否有效。如果需要，它首先更新树，然后从叶节点到根节点遍历树，计算沿途节点的哈希值。然后比较计算出的根哈希值与预期的根哈希值，如果相等，则返回true。

uint64_t merkle_tree::get_num_nodes()：此函数返回Merkle树中的节点总数。

bool merkle_tree::consistency_proof(merkle_tree* old_tree)：此函数检查新的Merkle树是否与旧的Merkle树一致。如果需要，它首先更新两个树，然后计算旧树中与新树相同的最后一个节点的索引。然后验证旧树和新树在该节点处的哈希值是否匹配，并验证两个树在根节点处的哈希值是否匹配。如果两个检查都通过，则该函数返回true。
# 实现效果
<div align="center">
  <img src="https://github.com/******/dbscan_clustering_algorithm/blob/master/data/DBSCAN.png">
</div>

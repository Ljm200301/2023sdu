# 实现方式
## 长度扩展攻击
长度扩展攻击利用哈希算法的长度扩展特性,即知道原始消息的哈希值、长度和填充后长度,就可以计算出任意长度扩展消息的哈希值。
## 代码分析
代码中具体做以下事情:
- 计算原始消息message1的SM3哈希值,存储在hash数组中
- 定义length_extension_attack函数,利用已知的原始哈希值hash和消息长度len, 计算出扩展消息message3的哈希值
  - 具体实现:
  - 填充原始消息,存储在padded_message数组中,计算其bit长度message_bit_length
  - 根据填充长度pad_len计算扩展消息extended_message的长度extended_len
  - 用hash作为初始状态,初始化SM3上下文ctx
  - 拼接填充后的原始消息和扩展消息message3,构造extended_message
  - 调用SM3_process处理extended_message,计算其哈希值

- 主函数中先计算message1的哈希值,然后调用length_extension_attack实现长度扩展攻击,计算message3的哈希值

# 实现方式
代码使用 Python 编写，功能是通过Blockstream.info的API获取Bitcoin（比特币）交易信息，并解析交易数据。  
首先，定义了一个比特币交易的哈希值 tx_hash，用于指定要获取的交易信息。接下来，使用 requests 库向 Blockstream.info 的API发送HTTP GET请求，获取指定哈希值对应的比特币交易信息。  
将获取到的数据写入到名为 bitcoin.txt 的文件中，然后再读取该文件的内容。这个步骤是为了避免反复发送请求，也可以避免可能的网络问题。  
然后将获取到的JSON格式的交易信息解析为Python字典 tx_data。  
最后，打印交易信息。

# 实现效果
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/send_a_tx.png">
</div>

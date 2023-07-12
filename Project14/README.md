# 实现方式 
PGP是一个加密软件套件，使用混合加密方法以保护数据的机密性、完整性和认证性。它首先使用非对称加密算法（SM2）生成一对公钥和私钥，然后使用对称加密算法（SM4）生成一个临时会话密钥。具体过程如下：

首先，使用非对称加密算法（如SM2）生成一对公钥和私钥。公钥可以公开发布，任何人都可以使用公钥来加密数据，但只有私钥的持有者才能解密数据。  
然后，使用SM2密钥交换算法生成一个临时会话密钥，该会话密钥将用于加密要传输的数据。  
接下来，使用公钥加密临时会话密钥，将加密后的会话密钥和使用对称加密算法加密后的数据一起发送给接收者。  
接收者使用私钥解密会话密钥，然后使用会话密钥解密数据。  

这种混合加密方法结合了非对称加密算法和对称加密算法的优势，可以实现更高效的加密和解密过程，同时保证数据的机密性和完整性。
<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/PGP.png">
</div>

在具体代码中，定义了一个generate_key函数，用于生成随机的16字节（128位）会话密钥。  
```python
def generate_key():
    key = secrets.token_bytes(16)
    return key.hex()
```
调用generate_key函数生成一个随机的会话密钥，并使用SM4算法对明文进行加密，得到密文。  
```python
key = generate_key().encode()
crysm4 = sm4.CryptSM4()
crysm4.set_key(key, sm4.SM4_ENCRYPT)
plaintext = 'Liujiaming'
ciphertext = crysm4.crypt_ecb(plaintext.encode())
```
使用SM2算法对会话密钥进行加密，得到加密后的会话密钥。
```python
sm2crypt = sm2.CryptSM2(public_key=public_key, private_key=private_key)
enckey = sm2crypt.encrypt(key)
```
使用SM2算法对加密后的会话密钥进行解密，得到原始的会话密钥。  
```python 
deckey = sm2crypt.decrypt(enckey)
```
使用原始的会话密钥对密文进行解密，得到明文。
```python 
crysm42 = sm4.CryptSM4()
crysm42.set_key(deckey, sm4.SM4_DECRYPT)
dectext = crysm42.crypt_ecb(ciphertext)
```

# 实现效果

<div align="center">
  <img src="https://github.com/Ljm200301/ljm/blob/main/pictures/SM2_PGP.jpg">
</div>

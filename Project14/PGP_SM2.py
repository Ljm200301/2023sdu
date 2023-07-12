from gmssl import sm2,sm3,sm4,func
import hashlib
import random
import secrets


private_key = '00B9AB0B828FF68872F21A837FC303668428DEA11DCD1B24429D0C99E24EED83D5'
public_key = 'B9C9A6E04E9C91F7BA880429273747D7EF5DDEB0BB2FF6317EB00BEF331A83081A6994B8993F3F5D6EADDDB81872266C87C018FB4162F5AF347B483E24620207'

def generate_key():
    key = secrets.token_bytes(16)
    return key.hex()

if __name__ == "__main__":
    key = generate_key().encode()
    print("Randomly generated session key:", key)

    crysm4 = sm4.CryptSM4()
    crysm4.set_key(key, sm4.SM4_ENCRYPT)

    plaintext = 'Liujiaming'
    ciphertext = crysm4.crypt_ecb(plaintext.encode())
    print("Plaintext:", plaintext)
    print("SM4 ciphertext:", ciphertext.hex())

    sm2crypt = sm2.CryptSM2(public_key=public_key, private_key=private_key)
    enckey = sm2crypt.encrypt(key)
    print("Session key encrypted with SM2 public key:", enckey.hex())

    deckey = sm2crypt.decrypt(enckey)
    print("Session key decrypted with SM2 private key:", deckey.hex())

    crysm42 = sm4.CryptSM4()
    crysm42.set_key(deckey, sm4.SM4_DECRYPT)

    dectext = crysm42.crypt_ecb(ciphertext)
    print("SM4 decrypted plaintext:", dectext.decode())

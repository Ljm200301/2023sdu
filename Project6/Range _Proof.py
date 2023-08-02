import hashlib
import os
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding

# Trusted Issuer
def trusted_issuer():
    seed = os.urandom(16) # 128-bit random seed
    s = hashlib.sha256(seed).digest() # compute s= H_0(seed)

    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
    )
    public_key = private_key.public_key()

    k = 2100 - 1978
    c = s
    for _ in range(k):
        c = hashlib.sha256(c).digest() # c = H_1^k(s)

    sig_c = private_key.sign(
        c,
        padding.PSS(
            mgf=padding.MGF1(hashes.SHA256()),
            salt_length=padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    ) # sign over c as sig_c

    return s, sig_c, public_key

# Alice
def alice(s):
    d_0 = 2000 - 1978 # compute d_0 = 2000 − 1978
    p = s
    for _ in range(d_0):
        p = hashlib.sha256(p).digest() # compute proof p=H_1^d_0(s)
    return p

# Bob
def bob(p, sig_c, public_key):
    d_1 = 2100 - 2000 # compute d_1 = 2100 − 2000
    c_prime = p
    for _ in range(d_1):
        c_prime = hashlib.sha256(c_prime).digest() # compute c'=H_1^d_1(p)

    try:
        public_key.verify(
            sig_c,
            c_prime,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
        print("Alice's proof is valid.")
    except:
        print("Alice's proof is invalid.")

# Test the code
s, sig_c, public_key = trusted_issuer()
p = alice(s)
bob(p, sig_c, public_key)
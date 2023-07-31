import time
import hashlib

BALANCE = 123
AMOUNT = int(input("Enter amount in BTC to spend: "))

seed = b"12345667"

start = time.time()

proof = hashlib.md5(seed)
encrypted_balance = hashlib.md5(seed)

for i in range(1, BALANCE - AMOUNT + 1):
    proof = hashlib.md5(proof.digest())

for i in range(1, BALANCE + 1):
    encrypted_balance = hashlib.md5(encrypted_balance.digest())

verified_balance = proof

for i in range(AMOUNT):
    verified_balance = hashlib.md5(verified_balance.digest())

print(f"Alice's Balance: {BALANCE}")
print(f"Amount to prove: {AMOUNT}")

print(f"Proof: {proof.hexdigest()}")
print(f"Encrypted Balance: {encrypted_balance.hexdigest()}")
print(f"Verified Balance: {verified_balance.hexdigest()}")

if encrypted_balance.hexdigest() == verified_balance.hexdigest():
    print("Transaction is valid")
else:
    print("Insufficient funds")

end = time.time()
print(f"Time: {end - start} seconds to run")
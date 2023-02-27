# Implementation of the SHA-256 algorithm

## SHA-256 Overview
SHA-256 is a part of the SHA-2 cryptographic algorithm, it is a hash function with digest length of 256 bits. It's a keyless hash function (Manipulation Detection Code)

### Basically, we can split the process into 3 steps:
#### • Padding:
We want to ensure that the message has length multpile to 512 bits, thus:
- first a bit 1 is appended.
- next, $k$ bits 0 are appended with $k$ being the solution of the equation $l+1+k = 448 \ [512]$ with $l$ the length in bits of the initial message.
- finally we represent the length $l$ with exactly 64 bits and we add these at the end of the message.

So by now, we have a message that is a multiple of 512 bits with the bit representation of the length of the initial message as the 64 last bits.

#### • Block decomposition
For each block $\{0,1\}^{512}$, we construct 64 words of 32 bits as follows:
- the first 16 bits are obtained by splitting $M$ in 32-bit blocks:
$$M=W_1||W_2||...||W_{15}||W_{16}$$ 
- the remaining 48 are obtained as follows:
$$\forall i \in [17,64] \quad W_i = \sigma_1(W_{i-2})+W_{i-7}+\sigma_0(W_{i-15})+W_{i-16}$$

#### • Hash computation
The hash is the following: $$H=H_1^{(N)}||H_2^{(N)}||H_3^{(N)}||H_4^{(N)}||H_5^{(N)}||H_6^{(N)}||H_7^{(N)}||H_8^{(N)}$$
With the $H_i^{(k)}$ recursively computed considering the initial values $H_1^{0},\ H_2^{0},\ H_3^{0},\ H_4^{0},\ H_5^{0},\ H_6^{0},\ H_7^{0},\ H_8^{0}$ that are set as the first 32 bits of the fractional part of the square roots of the first 8 prime numbers, we compute the hash value one block $M_i$ at a time:

$\forall i \in [1, n]:$

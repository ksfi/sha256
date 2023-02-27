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
For each block {$0,1$}$^{512}$, we construct 64 words of 32 bits as follows:
- the first 16 bits are obtained by splitting $M$ in 32-bit blocks:
$$M=W_1||W_2||...||W_{15}||W_{16}$$ 
- the remaining 48 are obtained as follows:
$$\forall i \in [17,64] \quad W_i = \sigma_1(W_{i-2})+W_{i-7}+\sigma_0(W_{i-15})+W_{i-16}$$
with $\sigma_0(x) = RotR(x, 7) \bigoplus RotR(x, 18) \bigoplus ShR(x, 3)$ and $\sigma_1(x) = RotR(x,17) \bigoplus RotR(x, 19) \bigoplus ShR(x, 10)$.

$RotR(x, n)$ being the circular right shift of $n$ bits of the binary word $x$ and $ShR(x, n)$ being the the right shift of $n$ bits of the binary word $A$.

#### • Hash computation
The hash is the following: $$H=H_1^{(N)}||H_2^{(N)}||H_3^{(N)}||H_4^{(N)}||H_5^{(N)}||H_6^{(N)}||H_7^{(N)}||H_8^{(N)}$$
With the $H_i^{(k)}$ recursively computed considering the initial values $H_1^{0},\ H_2^{0},\ H_3^{0},\ H_4^{0},\ H_5^{0},\ H_6^{0},\ H_7^{0},\ H_8^{0}$ that are set as the first 32 bits of the fractional part of the square roots of the first 8 prime numbers and

$\forall k \in [1, n]:$
- we set $$(a,\ b,\ c,\ d,\ e,\ f,\ g,\ h)=(H_1^{(k-1)},\ H_2^{(k-1)},\ H_3^{(k-1)},\ H_4^{(k-1)},\ H_5^{(k-1)},\ H_6^{(k-1)},\ H_7^{(k-1)},\ H_8^{(k-1)})$$
- 64 times the following operations:

$T_1 = h + \Sigma_1(e) + Ch(e,f,g) + K_i + W_i$

$T_2 = \Sigma_0(a) + Maj(a,b,c)$

$h = g$

$g = f$

$f=e$

$e=d + T_1$

$d = c$

$c = b$

$b = a$

$a = T_1 + T_2$

And then we have the new values 

$H_1^{(k)}=H_1^{(k-1)} + a,\ H_2^{(k)}=H_2^{(k-1)} + b,\ H_3^{(k)}=H_3^{(k-1)} + c,\ H_4^{(k)}=H_4^{(k-1)} + d$
$H_5^{(k)}=H_5^{(k-1)} + e,\ H_6^{(k)}=H_6^{(k-1)} + f,\ H_7^{(k)}=H_7^{(k-1)} + g,\ H_8^{(k)}=H_8^{(k-1)} + h$

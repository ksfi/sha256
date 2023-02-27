# Implementation of the SHA-256 algorithm

## SHA-256 Overview
SHA-256 is a part of the SHA-2 cryptographic algorithm, it is a hash function with digest length of 256 bits. It's a keyless hash function (Manipulation Detection Code)

### Basically, we can split the process into 3 steps:
#### • Padding:
We want to ensure that the message has length multpile to 512 bits, thus:
- first a bit 1 is appended.
- next, k bits 0 are appended with k being the solution of the following equation: $l+1+k = 448[512]$ with $l$ the length in bits of the initial message.
- finally we represent the length $l$ with exactly 64 bits and we add these at the end of the message.

So by now, we have a message that is a multiple of 512 bits with the bit representation of the length of the initial message as the 64 last bits.

#### • Block decomposition
For each block $\{0,1\}^{512}$

# Implementation of the SHA-256 algorithm

## SHA-256 Overview
SHA-256 is a part of the SHA-2 cryptographic algorithm, it is a hash function with digest lenth of 256 bits. It's a keyless hash function (Manipulation Detection Code)

### Basically, we can split the process into 3 steps:
#### • Padding:
We want to ensure that the message has length multpile to 512 bits, thus:
- first a bit 1 is appended

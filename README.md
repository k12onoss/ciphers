# Classical Ciphers in C

This repository contains implementation of some of the famous classical ciphers in C.

Currently implemented ciphers include:
-  Affine cipher
- Feistal network
- Playfair cipher

## Playfair Cipher
**General rules of the Playfair cipher are as follows:**
1. Repeating plaintext letters that are in the same pair are separated with a filler letter, such as x.
2. Two plaintext letters that fall in the same row of the matrix are each replaced by the letter to the right, with the first element of the row circularly following the last.
3. Two plaintext letters that fall in the same column are each replaced by the letter beneath, with the top element of the column circularly following the last.
4. Otherwise, each plaintext letter in a pair is replaced by the letter that lies in its own row and the column occupied by the other plaintext letter.

**Rules specific to the implementation:**
1. The letters `i` and `j` count as one letter.
2. If there is a pair `xx` in the plaintext, then the filler letter used to seperate them is `z`.
3. If required, filler letter `z` is used to make the length of the plaintext even. Although, if the plaintext ends with `z` itself, then letter `x` is used for the same.

**Limitations:**
Any character not belonging to small case `a` to `z`, is not supported. This includes upper case letters, puntuations, spaces etc.
# SHA-256

This is a very basic implementation of a SHA-256 hash according to the [FIPS
180-4 standard](http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf)
in C. I did it for educational purposes, the code is not optimized at all, but
(almost) corresponds line by line to the standard.

The algorithm to process a small block of data is quite simple and very well
described in the standard. I found that correctly implementing the padding is
much harder.

It does not have any dependencies (except for the C standard library of course)
and can be compiled with `make`. When `sha256sum` is installed, a short test can
be run with `make test`.

Usage:

```
./main <input file>
```

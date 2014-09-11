# libreset

libReSet = lib Reentrant Set

## What it is

libreset is a library in pure C for dealing with sets.

## What it is capable of

You can, besides required functionality such as inserting, deleting, getting the
cardinality and checking equality of sets, do common set operations, including
creating unions, intersections, excluding one set from another, xor'ing two sets
etc.

## How to build

The build is [cmake](http://cmake.org) based. If you want to build the library,
run the following commands from the repository's root directory:

```sh
cmake .
make
```

## How it works

If you want to know how it works, please have a look at the paper (in
`./paper`) which is provided by the "paper" target.

## Contributing

Please have a look at the CONTIBUTING.md file

## License

LGPLv2.1 Licensed.

Please have a look at the LICENSE file

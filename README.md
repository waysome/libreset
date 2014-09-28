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

Alternatively, if you are helping with development, you can create a
debug build.

```sh
cmake -DCMAKE_BUILD_TYPE=Debug .
make
```

There is also a Hard Mode option, which sets the Werror flag and thus
warnings are treated as errors. (Except a few, like unused functions.)

```sh
cmake -DHARD_MODE=On .
make
```

If you only want to have a look at the paper, you can build it like this:

```sh
cmake .
make paper
```

The same thing is true for the documentation, which you can build using the
following commands:

```sh
cmake .
make doc
```

If this does not generate output, you are missing doxygen from your machine.
Make sure to re-run `cmake .` again after installation.

## How it works

If you want to know how it works, please have a look at the paper (in
`./paper`) which is provided by the "paper" target.

## Contributing

Please have a look at the CONTIBUTING.md file

## License

LGPLv2.1 Licensed.

Please have a look at the LICENSE file

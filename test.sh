#!/bin/bash

should_be_empty() {
  if [[ -n $1 ]]; then
    echo "Test failed:"
    echo "Expected empty string, Received: $1"
  else
    echo "Test succeeded"
  fi
}

should_not_be_empty() {
  if [[ -n $1 ]]; then
    echo "Test succeeded"
  else
    echo "Test failed: expected some message, received empty"
  fi
}

make main

touch ~/path
out=$(./a.out ~/path)
should_be_empty "$out"

touch path ~/path2
out=$(./a.out path ~/path2)
should_be_empty "$out"

touch path path2
out=$(./a.out path -f --force path2)
should_be_empty "$out"

touch path ~/path2 path3 -- -foo
out=$(./a.out path -f --force ~/path2 -- -foo path3)
should_be_empty "$out"

touch path
out=$(./a.out path -i -I --interactive=never -r -R --recursive --one-file-system --no-preserve-root --preserve-root -d --dir -v --verbose)
should_be_empty "$out"

touch path
out=$(./a.out path --fake-option -foo)
should_not_be_empty "$out"

out=$(./a.out --version)
should_not_be_empty "$out"

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

test() {
  declare -n arr=$1
  declare -n arr2=$3

  for FILE in "${arr[@]}"; do
    touch "$FILE"
  done

  "$2" "$(./a.out "${arr2[@]}")"

  for FILE in "${arr[@]}"; do
    if [ -f "$FILE" ]; then
        echo "エラー：$FILE は削除されませんでした。"
    fi
  done
}

make main

a=("$HOME/path" "path2")
test a should_be_empty a

a=("path" "$HOME/path2")
test a should_be_empty a

a=("pa  th")
test a should_be_empty a

a=("path" "path2")
a2=("path" "-f" "--force" "path2")
test a should_be_empty a2

a=("path" "$HOME/path2" "./-foo" "path3")
a2=("path" "-f" "--force" "$HOME/path2" "--" "-foo" "path3")
test a should_be_empty a2

a=("path")
a2=("path" "-i" "-I" "--interactive=never" "-r" "-R" "--recursive" "--one-file-system" "--no-preserve-root" "--preserve-root" "-d" "--dir" "-v" "--verbose")
test a should_be_empty a2
a2=("path" "--fake-option" "2>&1")
test a should_not_be_empty a2

out=$(./a.out --version)
should_not_be_empty "$out"

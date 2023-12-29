#!/bin/bash

TURING=../bin/turing
TEST_DIR=../tests/args

testargs() {
    local test_name=$1
    local command=$2
    local code=$3
    $command 1>/dev/null 2>$TEST_DIR/$test_name/actual.txt
    local ret=$?
    if [ $ret -ne $code ]; then
        echo "Test $test_name failed: command $command should return $code, but got $ret"
        exit 1
    fi
    diff $TEST_DIR/$test_name/expected.txt $TEST_DIR/$test_name/actual.txt > $TEST_DIR/$test_name/diff.txt
    if [ $? -ne 0 ]; then
        echo "Test $test_name failed: command $command failed with wrong output"
        echo "Diff:"
        echo "----------------------"
        echo "$(cat $TEST_DIR/$test_name/diff.txt)"
        exit 1
    fi

}

testargs help "$TURING -h" 0
testargs no_tm_path "$TURING" 1
testargs no_input "$TURING 1" 1
testargs too_many_args "$TURING 1 2 3" 1
testargs invalid_option "$TURING -x" 1

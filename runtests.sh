#!/usr/bin/env bash

# Runs all the tests added in test/CMakelists.txt if invoked with one single argument starting with the character c.
# Otherwise runs each test specified in the array _TESTS on line 18.
# EXIT values:
# with the value 2 IF
#       | $_BUILDDIR does not exist (run ./configure.sh)
#       | was called with bad arguments
#       | the test specified in _TESTS has not been built (binary not found in _BINDIR)
#       | Google test framework exits with code 2 (not sure if it ever will)
# with the value returned from ctest OR 
# the exit value for the first individual erroneus value returned from the individual _TESTS
#   NOTE: This will take priority from the exit code 2 in case any $test specified in _TESTS was not found

# Configuration
_BUILDDIR="build/debug"
_BINDIR="bin"
_TESTS=(
    "ColorTest"
    "GeometryTest"
    "LoggerTest"
    "PhysicsTest"
    "TimetoolsTest"
)

if [ ! -d "${_BUILDDIR}" ]; then
    echo "Build directory './${_BUILDDIR}' missing. Aborting."
    exit 2
fi

if [ "$#" -eq 1 ] && [[ "$1" == c* ]]; then
    if ! command -v ctest &> /dev/null; then
        echo "'ctest' not found. Running the individual tests instead."
    else
        cd "${_BUILDDIR}" && ctest
        exit # Exit with the last exit code (ctest exit code if the _BUILDDIR exists)
    fi
elif [ "$#" -gt 0 ]; then
    echo "Bad argument(s). Usage: ${0} [c[test]]"
    echo "That is, run this script without any arguments to run the individual test binaries or with one single argument starting with the char 'c' to run the tests with ctest"
    exit 2
fi

exit_status=0

for test in "${_TESTS[@]}"; do
    if [ ! -x "${_BINDIR}/${test}" ]; then
        echo "Test '${test}' not found. Skipping.."
        exit_status=2
        continue
    fi

    echo "Running test './${_BINDIR}/${test}':"
    ./"${_BINDIR}/${test}"
    test_exit_status="$?"
    # Set exit_status to error IFF it was in {0, 2} and test exited with an error
    test "${test_exit_status}" -ne 0 && test "${exit_status}" -eq 0 || test "${exit_status}" -eq 2 && exit_status="${test_exit_status}"
done

exit "$exit_status"

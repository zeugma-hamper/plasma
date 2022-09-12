#!/bin/sh
# Script run at beginning and end of ctest if COVERAGE enabled
# to output coverage test report.

# Assumes current directory is top of binary output directory.

# Get absolute top source directory for yovo
SRCTOP="$(cd "$(dirname "$0")"; cd ../..; pwd)"

# FIXME: If we need .xml output to integrate with some tool, consider
# switching from lcov to gcovr

case "$1" in
start)
  echo "Zeroing coverage counters"
  lcov --quiet --base-directory "$SRCTOP" --directory . --zerocounters
  ;;
end)
  echo "Generating coverage report"
  # Check for strange bug where .gcda files are owned by root?!
  if find . -name '*.gcda' -uid 0 | grep .
  then
    echo "warning: somehow .gcda files are owned by root"
  fi
  lcov --quiet --base-directory "$SRCTOP" --directory . --no-external --capture -o coverage-raw.lcov
  # We don't want to include tests in the coverage output normally,
  # but it is sometimes interesting to see which tests are being
  # skipped.  We could output a separate report for that.
  #
  # Use absolute output path to work around bug fixed by
  # https://github.com/linux-test-project/lcov/commit/632c25a0d1f5e4d2f4fd5b28ce7c8b86d388c91f
  # FIXME: figure out where negative counts come from
  lcov --quiet --remove coverage-raw.lcov -o "$(pwd)/coverage.lcov" "*/tests/*" "*/t/*" "*/tests-mmap-only/*" "gtest*"

  genhtml --quiet --output-directory coverage-report coverage.lcov
  echo "To view coverage report, open $(pwd)/coverage-report/index.html in a browser"
  ;;
*)
  echo "usage: $0 [start|end]"
  exit 1
  ;;
esac

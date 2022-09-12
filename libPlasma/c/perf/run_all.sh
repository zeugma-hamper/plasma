#!/bin/bash

# Run all the perf tests, outputting to files and creating graphs with
# gnuplot.
#
# XXX add gnuplot to yobuild?
#
# All output files go into the out/ directory.  They include:
#
# <test_name>.out - The raw output of the tests (reads/s, etc.)
# <test_name>.gnuplot_in - The data for gnuplot to plot
#                          (currently identical to the .out)
# <test_name>.gnuplot_cmd - The commands to graph the data in gnuplot
#
# The raw output looks like a couple of shell-style comments
# describing the test and the columns in the data, and then one line
# of output for each test point.
#
# By default, the graphs are simple ASCII printed to stdout (this is
# the "dumb" terminal type).  If you want something nicer, set the
# GNUPLOT_FORMAT and GNUPLOT_SUFFIX variables according to the
# format you want.  The output format is defined by the "set terminal"
# command in gnuplot; type "set terminal" in gnuplot for a list of
# available formats.  For example, to get Postscript color graphs in
# landscape format named <test_name>.ps:
#
# GNUPLOT_FORMAT="postscript landscape color"
# GNUPLOT_SUFFIX="ps"
#
# If GNUPLOT_SUFFIX is not set, the output will go to stdout, which
# can be exciting for non-ASCII formats.
#
# Note that you can generate gnuplot graphs without running the
# benchmarks; just run "gnuplot <test_name>.gnuplot_cmd".

# if [ "$#" != "1" ]; then
#     echo "Usage: $0 <pool name prefix>"
#     exit 1;
# fi

thisdir=$(dirname $0)

export POOL_NAME=$TEST_POOL
# Define the cases for # of pools/threads we'll be looking at
export RANGE_MAX=128
export RANGE="1 2 4 6 8 12 16 32 64 ${RANGE_MAX}"
# Use small range for speedy testing
#export RANGE_MAX=64
#export RANGE="1 16 ${RANGE_MAX}"

# How long to run each iteration of the test (each iteration generates
# one data point).  Note that SECONDS is a builtin variable returning
# time since the shell was executed. :)
export TEST_SECONDS=3

# Format of gnuplot output.  Prints ASCII graphs to stdout by default.
# Unset to skip printing graphs.
#GNUPLOT_FORMAT="${GNUPLOT_FORMAT:=dumb}"
# Set below to get the graphs output to <test_name>.<suffix>
#GNUPLOT_SUFFIX=
# Uncomment below for pretty color graphs
GNUPLOT_FORMAT="postscript landscape color"
GNUPLOT_SUFFIX=ps

# For maximum reproducibility and representiveness, we fill all the
# pools we're going to use, recreate them, and fill them with
# proteins.  An empty or partially full pool is not a common case and
# may perform very differently than a full pool.

echo "Filling pools ..."
for (( i=0; i<${RANGE_MAX}; i++ )); do
    echo -n "$i "
    # Below we stop (delete) the pool to make sure it is created with
    # the expected size, contents, and other characteristics.
    ../p-stop -q "${POOL_NAME}$i"
    ../fill_pool "${POOL_NAME}$i"
done
echo ""

# Bring the output diretory to an existent but empty state.
if [ ! -d out ]; then
    mkdir out
else
    # Not using recursive for safety
    rm -f out/*
fi

subtests=(\
    $thisdir/one_pool_many_depositors.sh \
    $thisdir/many_pools_one_reader_one_depositor.sh \
    $thisdir/many_pools_one_depositor.sh \
    $thisdir/one_depositor_many_readers.sh \
    $thisdir/many_pools_one_reader.sh \
    $thisdir/one_pool_many_readers.sh \
    )

# Uncomment below for a quicker test
# subtests=(\
#     $thisdir/one_pool_many_depositors.sh \
#     )

for i in ${subtests[@]}
do
    # Export variables that the test scripts want to use
    export BASENAME=`basename $i .sh`
    export OUTPUT="out/${BASENAME}.out"
    export GNUPLOT_CMD="out/${BASENAME}.gnuplot_cmd"
    export GNUPLOT_INPUT="out/${BASENAME}.gnuplot_in"
    # Default gnuplot commands
    echo "set logscale x" >> "${GNUPLOT_CMD}"
#    echo "set logscale y" >> "${GNUPLOT_CMD}"
#    echo "set yrange [1000:]" >> "${GNUPLOT_CMD}"
    echo "set terminal ${GNUPLOT_FORMAT}" >> "${GNUPLOT_CMD}"
    echo "unset key" >> "${GNUPLOT_CMD}"
    if [ "${GNUPLOT_SUFFIX}" != "" ]; then
        GNUPLOT_OUTPUT="out/${BASENAME}.${GNUPLOT_SUFFIX}"
        echo "set output \"${GNUPLOT_OUTPUT}\"" >> "${GNUPLOT_CMD}"
    fi

    # Run the test!
    ${i}

    # And graph it if requested
    if [ "${GNUPLOT_FORMAT}" != "" ]; then
        # The raw output doesn't need any massaging for gnuplot to
        # read at present, but if it does in the future, this is the
        # place to do it.
        cp "${OUTPUT}" "${GNUPLOT_INPUT}"
        gnuplot "${GNUPLOT_CMD}"
    fi
done

echo "Stopping pools ..."
for (( i=0; i<${RANGE_MAX}; i++ )); do
    echo -n "$i "
    ../p-stop "${POOL_NAME}$i"
done
echo ""

# Create a summary graph

SUMMARY_CMD="out/summary.gnuplot_cmd"
if [ "${GNUPLOT_SUFFIX}" != "" ]; then
    SUMMARY_OUTPUT="out/summary.${GNUPLOT_SUFFIX}"
    echo "set output \"${SUMMARY_OUTPUT}\"" >> "${SUMMARY_CMD}"
fi
echo "set logscale x" >> "${SUMMARY_CMD}"
echo "set logscale y" >> "${SUMMARY_CMD}"
echo "set yrange [1000:]" >> "${SUMMARY_CMD}"
echo "set terminal ${GNUPLOT_FORMAT}" >> "${SUMMARY_CMD}"
echo "set title \"Summary\"" >> "${SUMMARY_CMD}"
echo "set xlabel \"threads/pools\"" >> "${SUMMARY_CMD}"
echo "set ylabel \"proteins/s\"" >> "${SUMMARY_CMD}"

# Note below we skip line color (lc) 6 because it is yellow and yellow
# does not show up well against a white background.

cat >> "${SUMMARY_CMD}" <<EOF

plot \\
   "out/one_pool_many_depositors.gnuplot_in" title "N dep., 1 pool" with linespoints lc 1 pt 1, \\
   "out/one_pool_many_readers.gnuplot_in" title "N readers., 1 pool" with linespoints lc 2 pt 2, \\
   "out/many_pools_one_depositor.gnuplot_in" title "1 dep., N pools" with linespoints lc 3 pt 1, \\
   "out/many_pools_one_reader.gnuplot_in" title "1 reader, N pools" with linespoints lc 4 pt 2, \\
   "out/one_depositor_many_readers.gnuplot_in" using 1:3 title "1 dep., N readers, dep./s" with linespoints lc 5 pt 1, \\
   "out/one_depositor_many_readers.gnuplot_in" using 1:2 title "1 dep., N readers, reads/s" with linespoints lc 5 pt 2, \\
   "out/many_pools_one_reader_one_depositor.gnuplot_in" using 1:3 title "1 dep., 1 reader, N pools, dep./s" with linespoints lc 7 pt 1, \\
   "out/many_pools_one_reader_one_depositor.gnuplot_in" using 1:2 title "1 dep., 1 reader, N pools, reads/s" with linespoints lc 7 pt 2
EOF

gnuplot "${SUMMARY_CMD}"

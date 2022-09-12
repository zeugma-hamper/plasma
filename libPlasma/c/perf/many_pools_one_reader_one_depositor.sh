#!/bin/bash

#
# Find protein throughput vs. number of pool/reader/writer pairs
#

# Custom gnuplot commands
echo "set title \"Many pools, one reader, one depositor\"" >> "${GNUPLOT_CMD}"
echo "set xlabel \"threads\"" >> "${GNUPLOT_CMD}"
echo "set ylabel \"proteins/s\"" >> "${GNUPLOT_CMD}"
#echo "set yrange [1:]" >> "${GNUPLOT_CMD}"
echo "set logscale y" >> "${GNUPLOT_CMD}"

# Override no key default since we have two lines
echo "set key" >> "${GNUPLOT_CMD}"
# Plot both reads and deposits
echo "plot \""${GNUPLOT_INPUT}"\" using 1:2 title \"reads/s\" with linespoints, \""${GNUPLOT_INPUT}"\" using 1:3 title \"deposits/s\" with linespoints" >> "${GNUPLOT_CMD}"

# Header info on raw output
echo "# Results of many pools, one reader, one depositor performance test" | tee -a "${OUTPUT}"
echo "# <threads/pools> <reads/s> <deposits/s>" | tee -a "${OUTPUT}"

thisdir=$(dirname $0)

for i in ${RANGE}; do
    echo -n "$i " | tee -a "${OUTPUT}"
    ../matrix_test -d $i -r $i -p $i -m 1 -S "${TEST_SECONDS}" \
        "${POOL_NAME}" | $thisdir/add_results.sh  | tee -a "${OUTPUT}"
done

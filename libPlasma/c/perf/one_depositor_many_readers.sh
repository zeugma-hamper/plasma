#!/bin/bash

#
# Find max read rate for one depositor pool
#

# Custom gnuplot commands
echo "set title \"One pool, one depositor, many readers\"" >> "${GNUPLOT_CMD}"
echo "set xlabel \"readers\"" >> "${GNUPLOT_CMD}"
echo "set ylabel \"proteins/s\"" >> "${GNUPLOT_CMD}"
# Use log for y because deposits are much smaller than reads
#echo "set yrange [10000:]" >> "${GNUPLOT_CMD}"
echo "set logscale y" >> "${GNUPLOT_CMD}"
# Override no key default since we have two lines
echo "set key" >> "${GNUPLOT_CMD}"
# Plot both reads and deposits
echo "plot \""${GNUPLOT_INPUT}"\" using 1:2 title \"reads/s\" with linespoints, \""${GNUPLOT_INPUT}"\" using 1:3 title \"deposits/s\" with linespoints" >> "${GNUPLOT_CMD}"

# Header info on raw output
echo "# Results of one pool, one depositor, many readers performance test" | tee -a "${OUTPUT}"
echo "# <threads/pools> <reads/s> <deposits/s>" | tee -a "${OUTPUT}"

thisdir=$(dirname $0)

for i in ${RANGE}; do
    echo -n "$i " | tee -a "${OUTPUT}"
    ../matrix_test -d 1 -r $i -S "${TEST_SECONDS}" "${POOL_NAME}" | \
        $thisdir/add_results.sh  | tee -a "${OUTPUT}"
done


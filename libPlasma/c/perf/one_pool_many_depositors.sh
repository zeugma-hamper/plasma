#!/bin/bash

#
# Find protein throughput vs. number of readers for one pool
#

# Custom gnuplot commands
echo "set title \"One pool, many depositors\"" >> "${GNUPLOT_CMD}"
echo "set xlabel \"threads\"" >> "${GNUPLOT_CMD}"
echo "set ylabel \"deposits/s\"" >> "${GNUPLOT_CMD}"
echo "plot \""${GNUPLOT_INPUT}"\" with linespoints" >> "${GNUPLOT_CMD}"

# Header info on raw output
echo "# Results of one pool with many depositors performance test" | tee -a "${OUTPUT}"
echo "# <threads/pools> <deposits/s>" | tee -a "${OUTPUT}"

thisdir=$(dirname $0)

for i in ${RANGE}; do
    echo -n "$i " | tee -a "${OUTPUT}"
    ../matrix_test -d $i -r 0 -S "${TEST_SECONDS}" "${POOL_NAME}" | \
        $thisdir/add_results.sh | awk '{print $2}' | tee -a "${OUTPUT}"
done

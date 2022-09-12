#!/bin/bash

#
# Find protein throughput vs. number of pool/reader pairs
#

# Custom gnuplot commands
echo "set title \"Many pools, one reader per pool\"" >> "${GNUPLOT_CMD}"
echo "set xlabel \"pools (one reader per pool)\"" >> "${GNUPLOT_CMD}"
echo "set ylabel \"reads/s\"" >> "${GNUPLOT_CMD}"
echo "plot \""${GNUPLOT_INPUT}"\" with linespoints" >> "${GNUPLOT_CMD}"

# Header info on raw output
echo "# Results of many pools with one reader performance test" | tee -a "${OUTPUT}"
echo "# <threads/pools> <reads/s>" | tee -a "${OUTPUT}"

thisdir=$(dirname $0)

for i in ${RANGE}; do
    echo -n "$i " | tee -a "${OUTPUT}"
    ../matrix_test -d 0 -r $i -p $i -m 1 -S "${TEST_SECONDS}" \
        "${POOL_NAME}" | $thisdir/add_results.sh | awk '{print $1}' | \
        tee -a "${OUTPUT}"
done

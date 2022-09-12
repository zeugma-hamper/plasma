# Example of using creduce to track down a system problem in yovo

delta.sh is a script that tests matrix_test.i to see if it
is a) sane, and b) exhibits the hang of interest.

It uses four helper files:

- makefile, knows how to build the test, and run it repeatedly with check.sh
- oracle.sh, uses makefile to run the test, and verifies result
- check.sh, a hacked version of obj-*/yotest that just runs the one test of interest.
- btrace, script to do a backtrace on a hung process

Here's how to use it (assuming the hang is in matrix_test):

1) On a machine that can reproduce the problem (in this case, any i9-9900K),
clone yovo into ~/src/yovo

2) Build yovo with meson, and verify tests pass, as follows:

    cd ~/src/yovo
    ./mesongen.sh c --unity on --debug -Db_sanitize=address
    ./mesongen.sh b
    ./mesongen.sh start
    ./mesongen.sh t     # if the hang of interest occurs, just retry

(Plasma v1 tests will fail, as that's disabled in unity builds.)

3) enable ptrace by doing

    echo 0 | tee /proc/sys/kernel/yama/ptrace_scope

4) Build the all-in-one-source-file version of the test,
and verify it compiles:

    cd bld/delta
    make unitize
    make matrix_test

Note: because creduce tends to turn matrix_test into a fork
bomb, I've hand-copied main() and reap_children() into main.c,
so 'make matrix_test' will fail until you remove those from matrix_test.i
(or remove my hacked main.c).

5) verify that delta.sh detects the problem on the first run:
    ./delta.sh
    echo $?          # show exit status

This runs it until it hangs, verifies that it was a good run,
and exits with status zero.  It takes about a minute or two.
If it exits with nonzero exit status, sommat's wrong.

6) run creduce to find a minimal reproducer as follows:

    sudo apt install creduce
    creduce --n 2 delta.sh matrix_test.i

Pressing 's' will skip a pass, which may be handy if you think it's stuck.

When it's in a good mood, it'll replace matrix_test.i with a new,
smaller version about every 20 to 3000 seconds.

delta.sh will save what it thinks are good runs in /tmp/delta.$$.oracle.log
and /tmp/delta.$$.matrix_test.i.

You should check every eight hours or so to make sure it hasn't
gone down a false path; just ^C out of creduce,
run 'mesongen.sh stop; mesongen.sh start' to clear out the old pool server logs,
and verify that 'sh delta.sh' still behaves as expected
(i.e. finds a hang after a while, and the log from the hang looks
like it has the right number of readers and depositors connecting).
Then start creduce again.

If it goes south somehow, you can roll back to an earlier version of
the concatenated, preprocessed source file by just stopping creduce,
copying a known good matrix_test.i (say, one of /tmp/delta.*.matrix_test.i)
into the current directory before restarting creduce.

Of course, if you're looking for some other problem in some other test,
you'll need to adjust the scripts to build and run that test.

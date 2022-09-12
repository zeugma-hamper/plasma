# /* (c)  oblong industries */

project(PlasmaBench CXX)

set(
  PlasmaBench_SOURCES

  LoneWorker.cpp
  PingPonger.cpp
  Reader.cpp
  SerialWorker.cpp
  TimeKeeper.cpp
  utils.cpp
  Worker.cpp
  Writer.cpp
)
add_library(PlasmaBench STATIC ${PlasmaBench_SOURCES})

# Expand @builddir@ in *.bench to make *.sh
# Kludge: doing it at configure time is easy but somewhat wrong
set(
  PlasmaBench_noinst_SCRIPTS

  benchmarks
  parallel
  samples
  sequential
)
set(builddir "${PlasmaBench_BINARY_DIR}")
foreach(f ${PlasmaBench_noinst_SCRIPTS})
    configure_file(${f}.bench ${f}.sh @ONLY)
endforeach()
unset(builddir)
# kludge: because we fake-built the scripts, add their dependencies indirectly
add_dependencies(PlasmaBench p-create p-stop pool_tcp_server)

# Test programs needed by the scripts in the above list
set(
  PlasmaBench_noinst_PROGRAMS

  braid
  bread
  butter
  sandwich
)

foreach (prog ${PlasmaBench_noinst_PROGRAMS})
  add_executable(${prog} ${prog}.cpp)
  target_link_libraries(${prog} PlasmaBench Plasma++ Loam++ Plasma Loam)
endforeach(prog)

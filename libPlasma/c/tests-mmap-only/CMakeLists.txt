# /* (c)  oblong industries */

project(PlasmaTestsMmapOnly)

set(PlasmaTestsMmapOnly_LINK_LIBS Plasma ${Plasma_LINK_LIBS})

set(
  PlasmaTestsMmapOnly_PROGRAMS

  compare-pool-and-file
  fifo_exists
)
if (NOT WIN32)
  list(APPEND PlasmaTestsMmapOnly_PROGRAMS
    semaphore-hostility
  )
endif()
foreach (prog ${PlasmaTestsMmapOnly_PROGRAMS})
  add_executable(${prog} ${prog}.c)
  target_link_libraries(${prog} ${PlasmaTestsMmapOnly_LINK_LIBS})
endforeach(prog)

set(
  PlasmaTestsMmapOnly_TESTS

  create_noipc_pool.sh
  mac-acl.sh
)
if (WIN32)
  message("Skipping tests not passing on win32 yet")
else()
  list(APPEND PlasmaTestsMmapOnly_TESTS
    fifo_exists.sh
    bad-permission.sh
    copy_pool.sh
    old-pool.sh
    pool-permissions.rb
  )
endif()

generate_tests("${PlasmaTestsMmapOnly_TESTS}" "${PlasmaTestsMmapOnly_LINK_LIBS}")

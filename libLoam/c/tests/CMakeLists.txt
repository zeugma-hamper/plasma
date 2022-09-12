# /* (c)  oblong industries */

project(LoamTests)

set(
  LoamTests_TESTS

  LoamGTest
  TestAtomic
  TestFile
  test-endian
  test-env
  test-hash
  test-logging
  test-obversion
  test-paths
  test-prepost
  test-rand
  test-time
  test-userid
  test-uuid
  test-vasprintf
  test-version
)

set(LoamTests_LINK_LIBS ${Loam_LINK_LIBS} Loam)
generate_tests("${LoamTests_TESTS}" "${LoamTests_LINK_LIBS}")

set(
  LoamTests_MANUAL_C_TESTS

  ob-cat
)

set(SUFFIX "c")
foreach( test ${LoamTests_MANUAL_C_TESTS} )
   add_executable(${test} ${test}.${SUFFIX})
   target_link_libraries(${test} Loam)
endforeach()


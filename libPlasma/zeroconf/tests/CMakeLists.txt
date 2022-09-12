# /* (c)  oblong industries */

project(ZeroconfTests)

set(
  ZeroconfTests_TESTS

  PoolServerTest
  ZeroconfTest
)

set(ZeroconfTests_LINK_LIBS PlasmaZeroconf Plasma ${Plasma_LINK_LIBS})
generate_tests("${ZeroconfTests_TESTS}" "${ZeroconfTests_LINK_LIBS}")

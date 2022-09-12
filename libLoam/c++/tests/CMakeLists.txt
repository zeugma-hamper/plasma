# /* (c)  oblong industries */

project(Loam++Tests)

set(
  Loam++Tests_TESTS

  AxisAlignedBoxTests
  BasicStrTest
  Cxx11Test
  EmptyStrTest
  FatherTimeTest
  FrustumTests
  GeomClassesTest
  IterationStrTest
  GeomSlabTest
  MinimalLoamTest
  MoveTest
  NullStrEqTest
  ObAcaciaTest
  ObConsTest
  ObCrawlTest
  ObInfoTest
  ObHormelTest
  ObMapTest
  ObRefTest
  ObRetortTest
  ObTroveTest
  RectangleTests
  RegexpStrTest
  RetortPodTest
  SphereTests
  VectTests
  Vect4Tests
  TheForceCanHaveAStrongInfluenceOnTheWeakMinded
  ankle-object-interface-specialization-test
  aptest1
  aptest3
  test_math
  test-prepost-cxx
  test_rand
)

set(Loam++Tests_LINK_LIBS ${Loam++_LINK_LIBS} Loam++ Loam)
generate_tests("${Loam++Tests_TESTS}" "${Loam++Tests_LINK_LIBS}")

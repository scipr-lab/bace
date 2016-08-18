include_directories(.)

# Profiling
add_executable(
  profile

  profiling/profile.cpp
)
target_link_libraries(
  profile

  ${LIBFF_LIBRARIES}
  ${GMP_LIBRARIES}
  ${GMPXX_LIBRARIES}
  ${PROCPS_LIBRARIES}
)
set_target_properties(
  profile

  PROPERTIES
  COMPILE_FLAGS "-fopenmp"
  LINK_FLAGS "-fopenmp"
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}
)
target_compile_definitions(
  profile

  PUBLIC
  -DMULTICORE
)

# Tests
add_executable(
  test_circuit
  EXCLUDE_FROM_ALL

  test/test_circuit.cpp
)
target_link_libraries(
  test_circuit

  ${LIBFF_LIBRARIES}
  ${GMP_LIBRARIES}
  ${GMPXX_LIBRARIES}
  ${PROCPS_LIBRARIES}
)

add_executable(
  test_verifier
  EXCLUDE_FROM_ALL

  test/test_verifier.cpp
)
target_link_libraries(
  test_verifier

  ${LIBFF_LIBRARIES}
  ${GMP_LIBRARIES}
  ${GMPXX_LIBRARIES}
  ${PROCPS_LIBRARIES}
)

include(CTest)
add_test(
  NAME test_circuit
  COMMAND test_circuit
)
add_test(
  NAME test_verifier
  COMMAND test_verifier
)

add_dependencies(check test_circuit)
add_dependencies(check test_verifier)
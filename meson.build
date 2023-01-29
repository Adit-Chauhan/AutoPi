project('hello','cpp')
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
log_dep = subproject('spdlog').get_variable('spdlog_dep')
executable('hello','src/hello.cpp',
  dependencies : [log_dep])


tests_src = [
  'test/test_main.cpp',
  'test/sample_test.cpp',
]

test_bin = executable(
  'gtest-all',
  tests_src,
  dependencies : [
    gtest_dep,
    gmock_dep,
    log_dep])

test('gtest',test_bin)

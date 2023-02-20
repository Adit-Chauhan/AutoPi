project('hello','cpp')
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
log_dep = subproject('spdlog').get_variable('spdlog_dep')

bin_src = ['src/utils/i2cBase.cpp']

executable('hello',['src/hello.cpp'] + bin_src,
  dependencies : [log_dep])


tests_src = [
  'test/test_main.cpp',
  'test/sample_test.cpp',
  'test/i2c_test.cpp',
] + bin_src

test_bin = executable(
  'gtest-all',
  tests_src,
  dependencies : [
    gtest_dep,
    gmock_dep,
    log_dep])

test('gtest',test_bin)

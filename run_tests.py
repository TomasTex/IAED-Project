import os


test_cmd = "./proj.bin < public_tests/test{:02d}.in > public_tests/test{:02d}.result"
diff_cmd = "colordiff public_tests/test{:02d}.result public_tests/test{:02d}.out"

for i in range(1, 16):
    print("running public test ", i)
    os.system(test_cmd.format(i, i))
    os.system(diff_cmd.format(i, i))


test_cmd = (
    "./proj.bin < community_tests/c_test{:02d}.in > community_tests/c_test{:02d}.result"
)
diff_cmd = (
    "colordiff community_tests/c_test{:02d}.result community_tests/c_test{:02d}.out"
)

for i in range(1, 11):
    print("running community test ", i)
    os.system(test_cmd.format(i, i))
    os.system(diff_cmd.format(i, i))


test_cmd = "./proj.bin < extensive_tests/tests/ext_test{:02d}.in > extensive_tests/tests/ext_test{:02d}.result"
diff_cmd = "colordiff extensive_tests/tests/ext_test{:02d}.result extensive_tests/tests/ext_test{:02d}.out"

for i in range(1, 2):
    print("running extensive test ", i)
    os.system(test_cmd.format(i, i))
    os.system(diff_cmd.format(i, i))

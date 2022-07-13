import glob
import os
from sys import platform
import subprocess


# Want to skip files in generated directories and tests
exclude_folders = ['build', 'out', 'tests']
exclude_files = ['TestAutomatic.hpp',
                 'TestManually.hpp',
                 'Faked.cpp',
                 'Faked.hpp',
                 'Helpers.hpp']

# Grabbing all source files
print("Globbing files...")
cpp_files = glob.glob("**/*.cpp", recursive=True)
hpp_files = glob.glob("**/*.hpp", recursive=True)

include_paths = set([os.path.dirname(path) for path in hpp_files])

# Ensure generated files aren't included
print("Excluding files...")
for exclude_folder in exclude_folders:
    cpp_files = [path for path in cpp_files if
                 not os.path.abspath(path).startswith(os.path.abspath(exclude_folder) + os.sep)]
    hpp_files = [path for path in hpp_files if
                 not os.path.abspath(path).startswith(os.path.abspath(exclude_folder) + os.sep)]

for exclude_file in exclude_files:
    cpp_files = [path for path in cpp_files if exclude_file not in path]
    hpp_files = [path for path in hpp_files if exclude_file not in path]

# Create the clang-tidy command
command = 'clang-tidy ' + ' '.join(cpp_files) + ' ' + ' '.join(hpp_files) + ' -- -std=c++17 ' + ' -I' + ' -I'.join(
    include_paths)

# We can run the command directly on Linux, otherwise paste into appropriate console manually
if platform == 'linux':
    print("Running linter...")
    try:
        output = subprocess.run(command, shell=True, check=True, capture_output=True, text=True)
        lint_output = output.stdout.splitlines()

        if len(lint_output) > 0:
            for lint_err in lint_output:
                print(lint_err)
            exit(-1)
        else:
            print("No lint errors found")
    except subprocess.CalledProcessError as e:
        raise RuntimeError("command '{}' return with error (code {}): {}".format(e.cmd, e.returncode, e.output))
else:
    print(command)

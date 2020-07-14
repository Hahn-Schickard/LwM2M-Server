import subprocess
import sys
import os
import shutil


def is_installed(name: str):
    try:
        subprocess.run(
            [name, "--version"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError:
        print("Program: ", name, " is not installed!")
        return False
    return True


def read_ignore_list(filename: str):
    if os.path.isfile(filename):
        file = open(filename, "r")
        ignore_list = file.readlines()
        ignore_list = [ignore_pattern.strip()
                       for ignore_pattern in ignore_list]
        file.close
        return ignore_list


def generate_coverage_report():
    if is_installed("lcov"):
        try:
            workspace_name = "code_coverage_report"
            if os.path.exists(workspace_name):
                shutil.rmtree(workspace_name, ignore_errors=True)
            os.mkdir(workspace_name)
            output_file = "code_coverage.info"
            if os.path.isfile(output_file):
                os.remove(output_file)
            subprocess.run(["lcov", "--directory", "build/", "--capture",
                            "--output-file", output_file, "-rc", "lcov_branch_coverage=1"])
            current_file_loc = os.path.dirname(os.path.realpath(__file__))
            ignore_list = read_ignore_list(
                current_file_loc + "/Coverage_Ignores.txt")
            for ignore_pattern in ignore_list:
                print("Ignoring filenaems that fit: ",
                      ignore_pattern, "pattern!")
                subprocess.run(["lcov", "--remove", output_file,
                                ignore_pattern, "-o", output_file])
            subprocess.run(["lcov", "--list", output_file])
            subprocess.run(
                ["genhtml", output_file, "--branch-coverage", "--output-directory", workspace_name + "/"])
        except subprocess.CalledProcessError:
            sys.exit(1)
    else:
        sys.exit(1)


generate_coverage_report()

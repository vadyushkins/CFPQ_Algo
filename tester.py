from matplotlib import pyplot as plt
import numpy as np
import pandas as pd

import subprocess
import os
import re

dirs = {}
dirs["input"] = "./input/"
dirs["graph"] = "./input/graph/"
dirs["grammar"] = "./input/grammar/"
dirs["queries"] = "./input/queries/"
dirs["results"] = "./results/"
dirs["Deconstruct-queries"] = "./input/queries/Deconstruct/"
dirs["Deconstruct-results"] = "./results/Deconstruct/"
dirs["Construct-queries"] = "./input/queries/Construct/"
dirs["Construct-results"] = "./results/Construct/"
dirs["Delete-cutpoints-queries"] = "./input/queries/Cutpoint/"
dirs["Delete-cutpoints-results"] = "./results/Cutpoint/"
dirs["Benchmark-queries"] = "./input/queries/Benchmark/"
dirs["Benchmark-results"] = "./results/Benchmark/"


default_graph = dirs["graph"] + "Empty.txt"
default_grammar = dirs["grammar"] + "Brackets.txt"
default_queries = dirs["queries"] + "Empty.txt"

MAX_N = 8

def init():
    print(dirs.values())
    for dir in dirs.values():
        os.mkdir(dir) if not os.path.exists(dir) else None
    open(default_graph, "w").close() if not os.path.exists(default_graph) else None
    open(default_queries, "w").close() if not os.path.exists(default_queries) else None
    if (os.path.exists("./main") is False):
        subprocess.run("make", shell=True)
    if (os.path.exists(default_grammar) is False):
        with open(default_grammar, "w") as file:
            file.write("s a b\n")
            file.write("s a s1\n")
            file.write("s1 s b\n")
            file.write("a A\n")
            file.write("b B\n")

def test_deconstruct_graph():
    for n in [2 ** p for p in range(1, MAX_N)]:
        input_graph = dirs["graph"] + f"Worstcase_{n}.txt"
        #print(input_graph, os.path.exists(input_graph))
        if (os.path.exists(input_graph) is False):
            with open(input_graph, "w") as file:
                [file.write(f"{i} A {(i + 1) % n}\n") for i in range(n)]
                [file.write(f"{i} B {(i + 1) % (2 * n)}\n") for i in range(n, 2 * n)]
                [file.write(f"{0} B {n}\n")]

        input_grammar = default_grammar
        #print(input_grammar)

        input_queries_brute = dirs["Deconstruct-queries"] + f"Worstcase_{n}_brute.txt"
        #print(input_queries_brute, os.path.exists(input_queries_brute))
        if (os.path.exists(input_queries_brute) is False):
            with open(input_queries_brute, "w") as file:
                [file.write(f"brute-edge-delete {i} {(i + 1) % n} A\n") for i in range(n)]
                [file.write(f"brute-edge-delete {i} {(i + 1) % (2 * n)} B\n") for i in range(n, 2 * n)]
                [file.write(f"brute-edge-delete {0} {n} B\n")]

        input_queries_smart = dirs["Deconstruct-queries"] + f"Worstcase_{n}_smart.txt"
        if (os.path.exists(input_queries_smart) is False):
            with open(input_queries_smart, "w") as file:
                [file.write(f"smart-edge-delete {i} {(i + 1) % n} A\n") for i in range(n)]
                [file.write(f"smart-edge-delete {i} {(i + 1) % (2 * n)} B\n") for i in range(n, 2 * n)]
                [file.write(f"smart-edge-delete {0} {n} B\n")]

        results_brute = dirs["Deconstruct-results"] + f"Worstcase_{n}_brute_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_brute} --with-time > {results_brute}", shell=True)

        results_smart = dirs["Deconstruct-results"] + f"Worstcase_{n}_smart_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_smart} --with-time > {results_smart}", shell=True)    

def test_construct_graph():
    for n in [2 ** p for p in range(1, MAX_N)]:
        input_graph = default_graph

        input_grammar = default_grammar
        #print(input_grammar)

        input_queries_brute = dirs["Construct-queries"] + f"Worstcase_{n}_brute.txt"
        #print(input_queries_brute, os.path.exists(input_queries_brute))
        if (os.path.exists(input_queries_brute) is False):
            with open(input_queries_brute, "w") as file:
                [file.write(f"brute-edge-add {i} {(i + 1) % n} A\n") for i in range(n)]
                [file.write(f"brute-edge-add {i} {(i + 1) % (2 * n)} B\n") for i in range(n, 2 * n)]
                [file.write(f"brute-edge-add {0} {n} B\n")]

        input_queries_smart = dirs["Construct-queries"] + f"Worstcase_{n}_smart.txt"
        if (os.path.exists(input_queries_smart) is False):
            with open(input_queries_smart, "w") as file:
                [file.write(f"smart-edge-add {i} {(i + 1) % n} A\n") for i in range(n)]
                [file.write(f"smart-edge-add {i} {(i + 1) % (2 * n)} B\n") for i in range(n, 2 * n)]
                [file.write(f"smart-edge-add {0} {n} B\n")]

        results_brute = dirs["Construct-results"] + f"Worstcase_{n}_brute_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_brute} --with-time > {results_brute}", shell=True)

        results_smart = dirs["Construct-results"] + f"Worstcase_{n}_smart_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_smart} --with-time > {results_smart}", shell=True)

def delete_cutpoints():
    for n in [2 ** p for p in range(1, MAX_N)]:
        input_graph = dirs["graph"] + f"Worstcase_{n}.txt"
        #print(input_graph, os.path.exists(input_graph))
        if (os.path.exists(input_graph) is False):
            with open(input_graph, "w") as file:
                [file.write(f"{i} A {(i + 1) % n}\n") for i in range(n)]
                [file.write(f"{i} B {(i + 1) % (2 * n)}\n") for i in range(n, 2 * n)]
                [file.write(f"{0} B {n}\n")]

        input_grammar = default_grammar
        #print(input_grammar)

        input_queries_brute = dirs["Delete-cutpoints-queries"] + f"Worstcase_{n}_brute.txt"
        #print(input_queries_brute, os.path.exists(input_queries_brute))
        if (os.path.exists(input_queries_brute) is False):
            with open(input_queries_brute, "w") as file:
                file.write("brute-vertex-delete 0")

        input_queries_smart = dirs["Delete-cutpoints-queries"] + f"Worstcase_{n}_smart.txt"
        if (os.path.exists(input_queries_smart) is False):
            with open(input_queries_smart, "w") as file:
                file.write("smart-vertex-delete 0")

        results_brute = dirs["Delete-cutpoints-results"] + f"Worstcase_{n}_brute_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_brute} --with-time > {results_brute}", shell=True)

        results_smart = dirs["Delete-cutpoints-results"] + f"Worstcase_{n}_smart_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_smart} --with-time > {results_smart}", shell=True)


def benchmark_worstcases():
    for n in [2 ** p for p in range(1, MAX_N)]:
        input_graph = default_graph

        input_grammar = default_grammar
        #print(input_grammar)

        input_queries_brute = dirs["Benchmark-queries"] + f"Worstcase_{n}_brute.txt"
        #print(input_queries_brute, os.path.exists(input_queries_brute))
        if (os.path.exists(input_queries_brute) is False):
            with open(input_queries_brute, "w") as file:
                [file.write(f"brute-edge-add {i} {(i + 1) % n} A\n") for i in range(n)]
                [file.write(f"brute-edge-add {i} {(i + 1) % (2 * n)} B\n") for i in range(n, 2 * n)]
                [file.write(f"brute-edge-add {0} {n} B\n")]
                for i in range(2 * n - 1):
                    for j in range(2 * n - 1):
                        file.write(f"find-path {i} {j}\n")

        input_queries_smart = dirs["Benchmark-queries"] + f"Worstcase_{n}_smart.txt"
        if (os.path.exists(input_queries_smart) is False):
            with open(input_queries_smart, "w") as file:
                [file.write(f"smart-edge-add {i} {(i + 1) % n} A\n") for i in range(n)]
                [file.write(f"smart-edge-add {i} {(i + 1) % (2 * n)} B\n") for i in range(n, 2 * n)]
                [file.write(f"smart-edge-add {0} {n} B\n")]
                for i in range(2 * n - 1):
                    for j in range(2 * n - 1):
                        file.write(f"find-path {i} {j}\n")

        results_brute = dirs["Benchmark-results"] + f"Worstcase_{n}_brute_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_brute} --no-time > {results_brute}", shell=True)

        results_smart = dirs["Benchmark-results"] + f"Worstcase_{n}_smart_results.txt"
        subprocess.run(f"./main {input_graph} {input_grammar} {input_queries_smart} --no-time > {results_smart}", shell=True)

        results_diff = dirs["Benchmark-results"] + f"Worstcase_{n}_diff.txt"
        subprocess.run(f"diff {results_brute} {results_smart} > {results_diff}", shell=True)


if __name__ == "__main__":
    init()
    benchmark_worstcases()
    subprocess.run("make clean", shell=True)
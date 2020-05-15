from matplotlib import pyplot as plt
from pathlib import Path
import numpy as np
import pandas as pd

import subprocess
import os
import re

CFPQ_DATA_DIR = "./deps/CFPQ_Data/"
DATA_DIR = "data/"

MATRICES_DIR = "Matrices/"
GRAMMARS_DIR = "Grammars/"
QUERIES_DIR  = "Queries/"
RESULTS_DIR = "Results/" 

TOOLS_DIR = "tools/"
CONVERTER = CFPQ_DATA_DIR + TOOLS_DIR + "RDF_to_triple/converter.py"
GRAMMAR_TO_CNF = CFPQ_DATA_DIR + TOOLS_DIR + "grammar_to_cnf/grammar_to_cnf.py"

def construct_graph(graph, grammar, queries_dir, results_dir, title):
    brute_queries_dir = queries_dir + "Brute/Construct/"
    if (os.path.exists(brute_queries_dir) is False):
        Path(brute_queries_dir).mkdir(parents=True, exist_ok=True)
    
    brute_results_dir = results_dir + "Brute/Construct/"
    if (os.path.exists(brute_results_dir) is False):
        Path(brute_results_dir).mkdir(parents=True, exist_ok=True)

    smart_queries_dir = queries_dir + "Smart/Construct/"
    if (os.path.exists(smart_queries_dir) is False):
        Path(smart_queries_dir).mkdir(parents=True, exist_ok=True)

    smart_results_dir = results_dir + "Smart/Construct/"
    if (os.path.exists(smart_results_dir) is False):
        Path(smart_results_dir).mkdir(parents=True, exist_ok=True)

    bq = open(f"{brute_queries_dir}{title}", "w")
    sq = open(f"{smart_queries_dir}{title}", "w")

    with open(graph, "r") as f:
        for line in f:
            v, edge, to = line.split()
            bq.write(f"brute-edge-add {v} {to} {edge}\n")
            sq.write(f"smart-edge-add {v} {to} {edge}\n")

    bq.close()
    sq.close()

    bq_run = f"./main {graph} {grammar} {brute_queries_dir}{title} --with-time > {brute_results_dir}{title}"
    sq_run = f"./main {graph} {grammar} {smart_queries_dir}{title} --with-time > {smart_results_dir}{title}"

    subprocess.run(bq_run, shell=True)
    subprocess.run(sq_run, shell=True)

    subprocess.run(f"tail -n 1 {brute_results_dir}{title} > {results_dir}construct_graph_diff_{title}", shell=True)
    subprocess.run(f"tail -n 1 {smart_results_dir}{title} >> {results_dir}construct_graph_diff_{title}", shell=True)


def deconstruct_graph_by_edge_deleting(graph, grammar, queries_dir, results_dir, title):
    brute_queries_dir = queries_dir + "Brute/Edges_Deconstruct/"
    if (os.path.exists(brute_queries_dir) is False):
        Path(brute_queries_dir).mkdir(parents=True, exist_ok=True)
    
    brute_results_dir = results_dir + "Brute/Edges_Deconstruct/"
    if (os.path.exists(brute_results_dir) is False):
        Path(brute_results_dir).mkdir(parents=True, exist_ok=True)

    smart_queries_dir = queries_dir + "Smart/Edges_Deconstruct/"
    if (os.path.exists(smart_queries_dir) is False):
        Path(smart_queries_dir).mkdir(parents=True, exist_ok=True)

    smart_results_dir = results_dir + "Smart/Edges_Deconstruct/"
    if (os.path.exists(smart_results_dir) is False):
        Path(smart_results_dir).mkdir(parents=True, exist_ok=True)

    bq = open(f"{brute_queries_dir}{title}", "w")
    sq = open(f"{smart_queries_dir}{title}", "w")

    with open(graph, "r") as f:
        for line in f:
            v, edge, to = line.split()
            bq.write(f"brute-edge-delete {v} {to} {edge}\n")
            sq.write(f"smart-edge-delete {v} {to} {edge}\n")

    bq.close()
    sq.close()

    bq_run = f"./main {graph} {grammar} {brute_queries_dir}{title} --with-time > {brute_results_dir}{title}"
    sq_run = f"./main {graph} {grammar} {smart_queries_dir}{title} --with-time > {smart_results_dir}{title}"

    subprocess.run(bq_run, shell=True)
    subprocess.run(sq_run, shell=True)

    subprocess.run(f"tail -n 1 {brute_results_dir}{title} > {results_dir}deconstruct_graph_by_edge_deleting_diff_{title}", shell=True)
    subprocess.run(f"tail -n 1 {smart_results_dir}{title} >> {results_dir}deconstruct_graph_by_edge_deleting_diff_{title}", shell=True)


def deconstruct_graph_by_vertex_deleting(graph, grammar, queries_dir, results_dir, title):
    brute_queries_dir = queries_dir + "Brute/Vertecies_Deconstruct/"
    if (os.path.exists(brute_queries_dir) is False):
        Path(brute_queries_dir).mkdir(parents=True, exist_ok=True)
    
    brute_results_dir = results_dir + "Brute/Vertecies_Deconstruct/"
    if (os.path.exists(brute_results_dir) is False):
        Path(brute_results_dir).mkdir(parents=True, exist_ok=True)

    smart_queries_dir = queries_dir + "Smart/Vertecies_Deconstruct/"
    if (os.path.exists(smart_queries_dir) is False):
        Path(smart_queries_dir).mkdir(parents=True, exist_ok=True)

    smart_results_dir = results_dir + "Smart/Vertecies_Deconstruct/"
    if (os.path.exists(smart_results_dir) is False):
        Path(smart_results_dir).mkdir(parents=True, exist_ok=True)

    bq = open(f"{brute_queries_dir}{title}", "w")
    sq = open(f"{smart_queries_dir}{title}", "w")

    vertecies = []

    with open(graph, "r") as f:
        for line in f:
            v, edge, to = line.split()
            if v not in vertecies:
                vertecies.append(v)
                bq.write(f"brute-vertex-delete {v}\n")
                sq.write(f"smart-vertex-delete {v}\n")
            if to not in vertecies:
                vertecies.append(to)
                bq.write(f"brute-vertex-delete {to}\n")
                sq.write(f"smart-vertex-delete {to}\n")

    bq.close()
    sq.close()

    bq_run = f"./main {graph} {grammar} {brute_queries_dir}{title} --with-time > {brute_results_dir}{title}"
    sq_run = f"./main {graph} {grammar} {smart_queries_dir}{title} --with-time > {smart_results_dir}{title}"

    subprocess.run(bq_run, shell=True)
    subprocess.run(sq_run, shell=True)

    subprocess.run(f"tail -n 1 {brute_results_dir}{title} > {results_dir}deconstruct_graph_by_vertex_deleting_diff_{title}", shell=True)
    subprocess.run(f"tail -n 1 {smart_results_dir}{title} >> {results_dir}deconstruct_graph_by_vertex_deleting_diff_{title}", shell=True)


def test_fullgraph():
    graph_dir = CFPQ_DATA_DIR + DATA_DIR + "FullGraph/"

    matrices_dir = graph_dir + MATRICES_DIR
    grammars_dir = graph_dir + GRAMMARS_DIR
    queries_dir = graph_dir + QUERIES_DIR
    results_dir = graph_dir + RESULTS_DIR
    
    if (os.path.exists(graph_dir) is False):
        subprocess.run(f"pip3 install -r {CFPQ_DATA_DIR}" + "requirements.txt", shell=True)
        subprocess.run(f"python {CFPQ_DATA_DIR}" + "init.py", shell=True)

    if (os.path.exists(queries_dir) is False):
        subprocess.run(f"mkdir {queries_dir}", shell=True)

    if (os.path.exists(results_dir) is False):
        subprocess.run(f"mkdir {results_dir}", shell=True)
    
    graph_xmls = list(filter(
        lambda x: re.fullmatch(".*\.xml", x) is not None, 
        os.listdir(matrices_dir)))

    grammars = list(filter(
        lambda x: re.fullmatch(".*_cnf\.txt", x) is None, 
        os.listdir(grammars_dir)))

    graph_txts = []
    for g in graph_xmls:
        g_txt = re.sub("(.*)(\.xml)", "\g<1>.txt", g)
        graph_txts.append(g_txt)
        if (os.path.exists(f"{matrices_dir}{g_txt}") is True):
            continue
        subprocess.run(f"python {CONVERTER} {matrices_dir}{g} {matrices_dir}convconfig", shell=True)
    
    grammar_cnfs = []
    for gr in grammars:
        gr_cnf = re.sub("(.*)(\.txt)", "\g<1>_cnf.txt", gr)
        grammar_cnfs.append(gr_cnf)
        if (os.path.exists(f"{grammars_dir}{gr_cnf}") is True):
            continue
        subprocess.run(f"python {GRAMMAR_TO_CNF} {grammars_dir}{gr} -o {grammars_dir}{gr_cnf}", shell=True)

    for g in graph_txts:
        g_name = re.sub("(.*)(\.txt)", "\g<1>", g)
        for gr in grammar_cnfs:
            gr_name = re.sub("(.*)(_cnf\.txt)", "\g<1>", gr)

            graph = matrices_dir + g
            grammar = grammars_dir + gr

            construct_graph(graph, grammar, queries_dir, results_dir, f"{g_name}_{gr_name}")
            deconstruct_graph_by_edge_deleting(graph, grammar, queries_dir, results_dir, f"{g_name}_{gr_name}")
            deconstruct_graph_by_vertex_deleting(graph, grammar, queries_dir, results_dir, f"{g_name}_{gr_name}")


def test_worstcase():
    graph_dir = CFPQ_DATA_DIR + DATA_DIR + "WorstCase/"

    matrices_dir = graph_dir + MATRICES_DIR
    grammars_dir = graph_dir + GRAMMARS_DIR
    queries_dir = graph_dir + QUERIES_DIR
    results_dir = graph_dir + RESULTS_DIR
    
    if (os.path.exists(graph_dir) is False):
        subprocess.run(f"pip3 install -r {CFPQ_DATA_DIR}" + "requirements.txt", shell=True)
        subprocess.run(f"python {CFPQ_DATA_DIR}" + "init.py", shell=True)

    if (os.path.exists(queries_dir) is False):
        subprocess.run(f"mkdir {queries_dir}", shell=True)

    if (os.path.exists(results_dir) is False):
        subprocess.run(f"mkdir {results_dir}", shell=True)
    
    graph_xmls = list(filter(
        lambda x: re.fullmatch(".*\.xml", x) is not None, 
        os.listdir(matrices_dir)))

    grammars = list(filter(
        lambda x: re.fullmatch(".*_cnf\.txt", x) is None, 
        os.listdir(grammars_dir)))

    graph_txts = []
    for g in graph_xmls:
        g_txt = re.sub("(.*)(\.xml)", "\g<1>.txt", g)
        graph_txts.append(g_txt)
        if (os.path.exists(f"{matrices_dir}{g_txt}") is True):
            continue
        subprocess.run(f"python {CONVERTER} {matrices_dir}{g} {matrices_dir}convconfig", shell=True)
    
    grammar_cnfs = []
    for gr in grammars:
        gr_cnf = re.sub("(.*)(\.txt)", "\g<1>_cnf.txt", gr)
        grammar_cnfs.append(gr_cnf)
        if (os.path.exists(f"{grammars_dir}{gr_cnf}") is True):
            continue
        subprocess.run(f"python {GRAMMAR_TO_CNF} {grammars_dir}{gr} -o {grammars_dir}{gr_cnf}", shell=True)

    for g in graph_txts:
        g_name = re.sub("(.*)(\.txt)", "\g<1>", g)
        for gr in grammar_cnfs:
            gr_name = re.sub("(.*)(_cnf\.txt)", "\g<1>", gr)

            graph = matrices_dir + g
            grammar = grammars_dir + gr

            construct_graph(graph, grammar, queries_dir, results_dir, f"{g_name}_{gr_name}")
            deconstruct_graph_by_edge_deleting(graph, grammar, queries_dir, results_dir, f"{g_name}_{gr_name}")
            deconstruct_graph_by_vertex_deleting(graph, grammar, queries_dir, results_dir, f"{g_name}_{gr_name}")


if __name__ == "__main__":
    subprocess.run("make", shell=True)
    test_fullgraph()
    test_worstcase()
    subprocess.run("make clean", shell=True)
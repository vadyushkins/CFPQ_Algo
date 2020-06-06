from termcolor import colored
from tqdm import tqdm
from glob import glob
import subprocess as sp
import random
import sys
import os
import re

TEST_GRAPHS = [
    'FullGraph',
    'WorstCase',
    'SparseGraph',
    'ScaleFree',
    'RDF',
]

TEST_TYPES = [
    'Construct',
    'Correctness',
]

def log(s):
    print(colored(s, 'green'))


def filename(path):
    return os.path.splitext(os.path.basename(path))[0]


def filesize(path):
    return int(sp.run(f'wc -l {path}', capture_output=True, shell=True).stdout.split()[0].decode('utf-8'))


def init(tests):
    log('Start building executables...')
    sp.run('make JOBS=32', shell=True)
    log('Finish building executables...')

    if os.path.exists('input') is False:
        log('Created input directory')
        os.mkdir('input')

    if os.path.exists('results') is False:
        log('Created results directory')
        os.mkdir('results')

    if os.path.exists('Empty.txt') is False:
        sp.run('touch Empty.txt', shell=True)
    
    for test in tests:
        if os.path.exists(f'input/{test}') is False:
            os.mkdir(f'input/{test}')
            log(f'Created input/{test} directory')

            os.mkdir(f'input/{test}/Graphs')
            log(f'Created input/{test}/Graphs directory')

            os.mkdir(f'input/{test}/Grammars')
            log(f'Created input/{test}/Grammars directory')

            os.mkdir(f'input/{test}/Queries')
            log(f'Created input/{test}/Queries directory')
            
    pwd = os.path.abspath('.')
    
    if os.path.exists('deps/CFPQ_Data/data/FullGraph/Matrices') is False:
        log('Start initialize CFPQ_Data...')
        sp.run('git clone https://github.com/viabzalov/CFPQ_Data.git deps/CFPQ_Data', shell=True)
        sp.run(f'pip3 install -r requirements.txt', cwd=f'{pwd}/deps/CFPQ_Data',shell=True)
        sp.run(f'python3 init.py', cwd=f'{pwd}/deps/CFPQ_Data', shell=True)
        log('Finish initialize CFPQ_Data...')

    for test in tests:
        graphs = os.listdir(f'deps/CFPQ_Data/data/{test}/Matrices')
        for g in tqdm(graphs):
            if filesize(f'deps/CFPQ_Data/data/{test}/Matrices/{g}') <= int(75000):
                g_txt = re.sub('(.*)(\.(xml|owl|rdf))', '\g<1>.txt', g)
                if os.path.exists(f'input/{test}/Graphs/{g_txt}') is False:
                    log(f'Start adding graph {g} to input...')
                    sp.run(f'python3 deps/CFPQ_Data/tools/RDF_to_triple/converter.py deps/CFPQ_Data/data/{test}/Matrices/{g} deps/CFPQ_Data/data/{test}/convconfig', shell=True)
                    sp.run(f'mv deps/CFPQ_Data/data/{test}/Matrices/{g_txt} input/{test}/Graphs/{g_txt}', shell=True)
                    log(f'Finish adding graph {g} to input...')

                construct_graph_queries(test, g_txt)
                correctness_graph_queries(test, g_txt)
        
        grammars = os.listdir(f'deps/CFPQ_Data/data/{test}/Grammars')
        for gr in tqdm(grammars):
            gr_cnf = re.sub('(.*)(\.txt)', '\g<1>_cnf.txt', gr)
            if os.path.exists(f'input/{test}/Grammars/{gr_cnf}') is False:
                log(f'Start adding grammar {gr_cnf} to input...')
                sp.run(f'python3 deps/CFPQ_Data/tools/grammar_to_cnf/grammar_to_cnf.py deps/CFPQ_Data/data/{test}/Grammars/{gr} -o input/{test}/Grammars/{gr_cnf}', shell=True)
                log(f'Finish adding grammar {gr_cnf} to input...')


def construct_graph_queries(test, graph):
    q_dir = f'input/{test}/Queries/{filename(graph)}/Construct/'
    if os.path.exists(q_dir) is False:
        os.makedirs(q_dir, exist_ok=True)    
    for type in ['brute', 'smart']:
        with open(f'input/{test}/Graphs/{graph}', 'r') as fin:
            q_path = q_dir + f'{type}.txt'
            with open(q_path, 'w') as fout:
                log(f'Start adding queries to {q_path}...')
                for line in tqdm(fin):
                    v, edge, to = line.split()
                    fout.write(f'{type}-edge-add {v} {to} {edge}\n')
                log(f'Finish adding queries to {q_path}...')


def correctness_graph_queries(test, graph):
    q_dir = f'input/{test}/Queries/{filename(graph)}/Correctness/'
    if os.path.exists(q_dir) is False:
        os.makedirs(q_dir, exist_ok=True)

    min_v = int(10**18)
    max_v = -int(10**18)
    with open(f'input/{test}/Graphs/{graph}', 'r') as fin:
        for line in fin:
            v, edge, to = line.split()
            min_v = min(list(map(int, [v, to, min_v])))
            max_v = max(list(map(int, [v, to, max_v])))
    
    for type in ['brute', 'smart']:
        with open(f'input/{test}/Graphs/{graph}', 'r') as fin:
            q_path = q_dir + f'{type}.txt'
            with open(q_path, 'w') as fout:
                log(f'Start adding queries to {q_path}...')
                for line in tqdm(fin):
                    v, edge, to = line.split()
                    fout.write(f'{type}-edge-add {v} {to} {edge}\n')
                for i in range(min_v, max_v + 1):
                    for j in range(min_v, max_v + 1):
                        if i != j:
                            fout.write(f'find-path {i} {j}\n')
                log(f'Finish adding queries to {q_path}...')


def test_one_graph(test, graph, grammar, queries, save_log=False):
    g_name = filename(graph)
    gr_name = filename(grammar)
    q_name = filename(queries)
    
    if os.path.exists(test) is False:
        os.makedirs(test, exist_ok=True)

    results_path = f'{test}/{test}_{g_name}_{gr_name}_{q_name}_log.txt'

    log(f'Start testign Graph: {g_name} with Grammar: {gr_name} and Queries: {q_name}...')

    time = 0

    for i in range(100):
        sp.run(f'./main {graph} {grammar} {queries} > {results_path}', shell=True)
        time += get_time(results_path)

    log(f'Total time: {time} s')

    if save_log is False:
        os.remove(results_path)

    log(f'Finish testign Graph: {g_name} with Grammar: {gr_name} and Queries: {q_name}...')

    return round(time / 100.0, 6)


def get_time(results_path):
    time = None
    with open(results_path, 'r') as fin:
        for line in fin:
            if re.fullmatch('(Total time:) (.*) s\n', line) is not None:
                time = re.sub('(Total time:) (.*) s\n', '\g<2>', line)
    return round(float(time), 6)


def test_all(tests):
    for test_graph in tests:
        with open(f'results/{test_graph}.md', 'w') as fout:
            graphs = glob(f'input/{test_graph}/Graphs/*')
            grammars = glob(f'input/{test_graph}/Grammars/*')

            fout.write(f'# {test_graph}\n\n')
            
            for gr in sorted(grammars):
                if 'Construct' in TEST_TYPES:
                    gr_name = filename(gr)
                    fout.write(f'## Grammar: {gr_name}\n')
                    fout.write(f'## Test type: Construct\n\n')
                    fout.write(f'| Graph | Brute | Smart |\n')
                    fout.write(f'|:-----:|:-----:|:-----:|\n')
                    for g in sorted(graphs):
                        g_name = filename(g)
                        results = {}
                        for type in ['brute', 'smart']:
                            qrs = f'input/{test_graph}/Queries/{g_name}/Construct/{type}.txt'
                            time = None
                            time = test_one_graph(test_graph, 'Empty.txt', gr, qrs)
                            results[type] = time
                        result_brute = results['brute']
                        result_smart = results['smart']
                        fout.write(f'| {g_name} | {result_brute} | {result_smart} |\n')
                        fout.flush()
                    fout.write('\n')

                if 'Correctness' in TEST_TYPES:
                    gr_name = filename(gr)
                    fout.write(f'## Grammar: {gr_name}\n')
                    fout.write(f'## Test type: Correctness\n\n')
                    fout.write(f'| Graph | equal(Brute, Smart) |\n')
                    fout.write(f'|:-----:|:-------------------:|\n')
                    for g in sorted(graphs):
                        g_name = filename(g)
                        for type in ['brute', 'smart']:
                            qrs = f'input/{test_graph}/Queries/{g_name}/Correctness/{type}.txt'
                            time = None
                            time = test_one_graph(test_graph, 'Empty.txt', gr, qrs, save_log=True)
                            results[type] = time
                        brute_log = f'{test_graph}/{test_graph}_Empty_{gr_name}_brute_log.txt'
                        smart_log = f'{test_graph}/{test_graph}_Empty_{gr_name}_smart_log.txt'
                        sp.run(f'diff {brute_log} {smart_log} | grep path > diff_log', shell=True)
                        res = (os.stat('diff_log').st_size == 0)
                        fout.write(f'| {g_name} | {res} |\n')
                        fout.flush()
                    fout.write('\n')
                        

if __name__ == '__main__':
    test_graphs = list(map(str, sys.argv[1:]))
    init(test_graphs)
    test_all(test_graphs)
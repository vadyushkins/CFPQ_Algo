from termcolor import colored
from tqdm import tqdm
from glob import glob
import subprocess as sp
import random
import os
import re

TESTS = [
    'FullGraph',
    'WorstCase',
    'SparseGraph',
    'ScaleFree',
    'RDF',
]

def log(s):
    print(colored(s, 'green'))


def filename(path):
    return os.path.splitext(os.path.basename(path))[0]

def init():
    log('Start building executables...')

    sp.run('make', shell=True)

    log('Finish building executables...')

    if os.path.exists('input') is False:
        log('Created input directory')
        os.mkdir('input')
    
    for test in TESTS:
        if os.path.exists(f'input/{test}') is False:
            os.mkdir(f'input/{test}')
            log(f'Created input/{test} directory')

            os.mkdir(f'input/{test}/Graphs')
            log(f'Created input/{test}/Graphs directory')

            os.mkdir(f'input/{test}/Grammars')
            log(f'Created input/{test}/Grammars directory')

            os.mkdir(f'input/{test}/Queries')
            log(f'Created input/{test}/Queries directory')

    if os.path.exists('results') is False:
        log('Created results directory')
        os.mkdir('results')
            
    pwd = os.path.abspath('.')
    
    if os.path.exists('deps/CFPQ_Data/data/FullGraph/Matrices') is False:
        log('Start initialize CFPQ_Data...')
        sp.run('git clone https://github.com/viabzalov/CFPQ_Data.git deps/CFPQ_Data', shell=True)
        sp.run(f'pip3 install -r requirements.txt', cwd='{pwd}/deps/CFPQ_Data',shell=True)
        sp.run(f'python3 init.py', cwd='{pwd}/deps/CFPQ_Data', shell=True)
        log('Finish initialize CFPQ_Data...')

    for test in TESTS:
        graphs = os.listdir(f'deps/CFPQ_Data/data/{test}/Matrices')
        for g in tqdm(graphs):
            g_txt = re.sub('(.*)(\.xml)', '\g<1>.txt', g)
            if os.path.exists(f'input/{test}/Graphs/{g_txt}') is False:
                log(f'Start adding graph {g} to input...')
                sp.run(f'python3 deps/CFPQ_Data/tools/RDF_to_triple/converter.py deps/CFPQ_Data/data/{test}/Matrices/{g} deps/CFPQ_Data/data/{test}/convconfig', shell=True)
                sp.run(f'mv deps/CFPQ_Data/data/{test}/Matrices/{g_txt} input/{test}/Graphs/{g_txt}', shell=True)
                log(f'Finish adding graph {g} to input...')

            construct_graph_queries(test, g_txt)
            deconstruct_graph_queries(test, g_txt)
            for p in [10, 25, 50, 75, 90]:
                random_graph_queries(test, g_txt, p)
        
        grammars = os.listdir(f'deps/CFPQ_Data/data/{test}/Grammars')
        for gr in tqdm(grammars):
            gr_cnf = re.sub('(.*)(\.txt)', '\g<1>_cnf.txt', gr)
            if os.path.exists(f'input/{test}/Grammars/{gr_cnf}') is False:
                log(f'Start adding grammar {gr_cnf} to input...')
                sp.run(f'python3 deps/CFPQ_Data/tools/grammar_to_cnf/grammar_to_cnf.py deps/CFPQ_Data/data/{test}/Grammars/{gr} -o input/{test}/Grammars/{gr_cnf}', shell=True)
                log(f'Finish adding grammar {gr_cnf} to input...')


def construct_graph_queries(test, graph):
    if os.path.exists(f'input/{test}/Queries/Construct') is False:
        os.mkdir(f'input/{test}/Queries/Construct')
        for type in ['brute', 'smart']:
            with open(f'input/{test}/Graphs/{graph}', 'r') as fin:
                with open(f'input/{test}/Queries/Construct/{type}.txt', 'w') as fout:
                    log(f'Start adding queries to input/{test}/Queries/Construct/{type}.txt...')
                    for line in tqdm(fin):
                        v, edge, to = line.split()
                        fout.write(f'{type}-edge-add {v} {to} {edge}\n')
                    log(f'Finish adding queries to input/{test}/Queries/Construct/{type}.txt...')


def deconstruct_graph_queries(test, graph):
    if os.path.exists(f'input/{test}/Queries/Deconstruct') is False:
        os.mkdir(f'input/{test}/Queries/Deconstruct')
        for type in ['brute', 'smart']:
            with open(f'input/{test}/Graphs/{graph}', 'r') as fin:
                with open(f'input/{test}/Queries/Deconstruct/{type}.txt', 'w') as fout:
                    log(f'Start adding queries to input/{test}/Queries/Deconstruct/{type}.txt...')
                    for line in tqdm(fin):
                        v, edge, to = line.split()
                        fout.write(f'{type}-edge-delete {v} {to} {edge}\n')
                    log(f'Finish adding queries to input/{test}/Queries/Deconstruct/{type}.txt...')


def random_graph_queries(test, graph, percentage):
    if os.path.exists(f'input/{test}/Queries/Random') is False:
        os.mkdir(f'input/{test}/Queries/Random')
        batch = []
        for type in ['brute', 'smart']:
            with open(f'input/{test}/Graphs/{graph}', 'r') as fin:
                with open(f'input/{test}/Queries/Random/{type}_{percentage}.txt', 'w') as fout:
                    log(f'Start adding queries to input/{test}/Queries/Random/{type}_{percentage}.txt...')
                    for line in tqdm(fin):
                        v, edge, to = line.split()
                        batch.append((v, edge, to))
                        if len(batch) == 100:
                            for (v, edge, to) in random.sample(batch, percentage):
                                fout.write(f'{type}-edge-delete {v} {to} {edge}\n')
                            batch.clear()
                    log(f'Finish adding queries to input/{test}/Queries/Random/{type}_{percentage}.txt...')


def test(graph, grammar, queries):
    results = re.sub('(.*)/(.*)/(.*)/(.*)\.txt', 'results/\g<2>/\g<4>.txt', graph)
    log(f'Start testign Graph: {filename(graph)} with Grammar: {filename(grammar)} and Queries: {filename(queries)}...')
    sp.run(f'./main {graph} {grammar} {queries} --total-time > {results}', shell=True)
    log(f'Finish testign Graph: {filename(graph)} with Grammar: {filename(grammar)} and Queries: {filename(queries)}...')
    return results


def get_time(results):
    with open(results, 'r') as fin:
        for line in fin:
            if re.fullmatch('(Total time:) (.*) s', line) is not None:
                return re.sub('(Total time:) (.*) s', '\g<2>', line)

def test_all():
    for test in TESTS:
        with open(f'results/{test}.md', 'w') as fout:
            graphs = glob(f'input/{test}/Graphs/*')
            grammars = glob(f'input/{test}/Grammars/*')
            queries = glob(f'input/{test}/Queries/*/*')

            fout.write(f'| Graph | Grammar | Queries | Time (s) |\n')
            fout.write(f'|:-----:|:-------:|:-------:|:--------:|\n')
            
            for g in graphs:
                g_name = filename(g)
                for gr in grammars:
                    gr_name = filename(gr)
                    for q in queries:
                        q_name = filename(q)
                        fout.write(f'| {g_name} | {gr_name} | {q_name} | {get_time(test(g, gr, q))} |\n')
                        

if __name__ == '__main__':
    init()
    test_all()
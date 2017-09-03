# Experiments

documents: [observation.md](documents/observation.md), [challenge.md](documents/challenge.md).

category | detail
--- | ---
algorithm introduction and visualization | see [algorithm_vis](algorithm_vis), with a toy case [demo_input_graph.txt](algorithm_vis/demo_input_graph.txt)
dataset description | see [dataset_description](dataset_description/ReadMe.md)
scalability experiment | see [case-study0](case_studies/figures-case-study0), parameter setting: `eps:0.3`, `min_pts:5`.
scalability robust scheduling experiment | see [case-study2-robust](case_studies/figures-case-study2-robust)， parameter setting: `eps:0.3`, `min_pts:5`
scalability robust different eps | see [case-study3-scalability-differ-eps](case_studies/figures-case-study3-scalability-differ-eps), , parameter setting: `min_pts:5`.
workload/serial-runtime experiment | see [case-study1](case_studies/figures-case-study1), parameter setting: `min_pts:5`

## Todo

* try to shape another branch to add the power of reducing possible eval computations
    * eps small, easy to be a core
    * eps large, hard to be a core
* study other possible ways to reduce computations, e.g, optimization for skewed comparison
* robustness of parallel scheduling
* profile to summarize performance related issues
* visual aids of pscan+ comparison to pscan
* deeper analysis of anySCAN, pSCAN, SCAN++ 
* compare to anySCAN and SCAN++, too 

## Scripts

category | scripts
--- | ---
scalability | [run_scalability_experiment.py](run_scalability_experiment.py), [scalability_figure.py](scalability_figure.py), [generate_markdown0.py](playground/generate_markdown0.py)
workload | [run_workload_experiment.py](run_workload_experiment.py), [workload_figure.py](workload_figure.py), [generate_markdown1.py](playground/generate_markdown1.py), [generate_markdown2.py](playground/generate_markdown2.py)
algorithm demo | [pscan_algo.py](algorithm_vis/pscan_algo.py), [pscan_algo_vis.py](algorithm_vis/pscan_algo_vis.py)
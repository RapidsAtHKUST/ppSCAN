# Experiments

## Introduction 

category | detail
--- | ---
algorithm: introduction and visualization | see [algorithm_vis](algorithm_vis), with a toy case [demo_input_graph.txt](algorithm_vis/demo_input_graph.txt)
dataset: description | see [dataset_description](dataset_description/ReadMe.md)
[documents](documents) | [observation.md](documents/observation.md), [challenge.md](documents/challenge.md).

## Todo

* robustness of parallel scheduling
* profile to summarize performance related issues
* deeper analysis of anySCAN, pSCAN, SCAN++ 
* compare to anySCAN and SCAN++, too 

## Case Studies

### Scalability

category | detail
--- | ---
scalability: speedup/comp-io portion experiment | see [case-study0](case_studies/figures-case-study0), parameter setting: `eps:0.3`, `min_pts:5`.
scalability: robust scheduling speedup/comp-io portion experiment | see [case-study2-robust](case_studies/figures-case-study2-robust)， parameter setting: `eps:0.3`, `min_pts:5`
scalability: robust scheduling speedup/comp-io portion, different eps | see [case-study3-scalability-differ-eps](case_studies/figures-case-study3-scalability-differ-eps), , parameter setting: `min_pts:5`.
scalability: new work-efficient but not balanced | see [case-study6-scalability-new0](case_studies/figures-case-study6-scalability-new0)

### Workload

category | detail
--- | ---
workload: prune/eval, serial-runtime experiment | see [case-study1](case_studies/figures-case-study1), parameter setting: `min_pts:5`
workload: avg cmp/eval portion experiment | see [case-study4](case_studies/case-study4-workload), varying `min_pts` and varying `eps`
work-efficient: prune/eval, serial-runtime experiment | see [case-study5](case_studies/figures-case-study5-workload-efficient), parameter setting: `min_pts:5`

### Synthetic Graphs

category | detail
--- | ---
lfr-benchmark graph | [case-study-synthetic0](case_studies/case-study-synthetic0)

## Scripts

### Demo

algorithm demo : [pscan_algo.py](algorithm_vis/pscan_algo.py), [pscan_algo_vis.py](algorithm_vis/pscan_algo_vis.py)

### Scalability 

category | scripts
--- | ---
run i/o statistics experiment | [run_io_statistics_experiment.py](run_io_statistics_experiment.py)
run scalability experiment | [run_scalability_experiment.py](run_scalability_experiment.py)
draw scalability figure, `[1,2,4,8,16,24,32,40]`| [scalability_figure.py](scalability_figure.py)
draw speedup/comp-io portion figure | [scalability_figure_overview.py](scalability_figure_overview.py)
differ eps: draw scalability figure, `[1,2,4,8,16,24,32,40]` | [scalability_differ_eps.py](scalability_differ_eps.py)
differ eps: draw speedup/comp-io portion figure | [scalability_differ_eps_overview.py](scalability_differ_eps_overview.py)
generate scalability overview tables markdown |  [generate_markdown0.py](playground/generate_markdown0.py)

### Workload

category | scripts
--- | ---
run workload experiment | [run_workload_experiment.py](run_workload_experiment.py)
run single statistics program workload experiment | [run_eval_workload_experiment.py](run_eval_workload_experiment.py)
draw workload and runtime figure | [workload_figure.py](workload_figure.py)
workload analysis and generate markdown | [workload_analysis.py](workload_analysis.py)
generate workload time markdown | [generate_markdown1_workload_time.py](playground/generate_markdown1_workload_time.py)
generate workload amount markdown | [generate_markdown2_workload_amount.py](playground/generate_markdown2_workload_amount.py)

### Others

exploration: [run_new_version_explore_experiment.py](run_new_version_explore_experiment.py)
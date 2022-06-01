import cma
# help(cma)

es = cma.CMAEvolutionStrategy(4 * [0], 0.5)
# es.optimize(cma.ff.rosen)
es.result_pretty()
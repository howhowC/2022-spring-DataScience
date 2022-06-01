from multiprocessing.sharedctypes import Value
import numpy as np

# you must use python 3.6, 3.7, 3.8, 3.9 for sourcedefender
import sourcedefender
from HomeworkFramework import Function

import cma

class RS_optimizer(Function): # need to inherit this class "Function"
    def __init__(self, target_func):
        super().__init__(target_func) # must have this init to work normally

        self.lower = self.f.lower(target_func)
        self.upper = self.f.upper(target_func)
        self.dim = self.f.dimension(target_func)

        self.target_func = target_func

        self.eval_times = 0
        self.optimal_value = float("inf")
        self.optimal_solution = np.empty(self.dim)

        

    def get_optimal(self):
        return self.optimal_solution, self.optimal_value

    def run(self, FES): # main part for your implementation
        
        while self.eval_times < FES:
            # print('=====================FE=====================')
            # print(self.eval_times)
            # print(self.dim)
            # solution = np.random.uniform(np.full(self.dim, self.lower), np.full(self.dim, self.upper), self.dim)
            # print(solution)
            es = cma.CMAEvolutionStrategy(self.dim * [0], 0.5, {'bounds': [self.lower, self.upper]})
            # es.result_pretty()
            while not es.stop():
                solution = es.ask()
                ls = []
                for x in solution:
                    # if ()
                    # print(x)
                    # print(np.random.uniform(np.full(self.dim, self.lower), np.full(self.dim, self.upper), self.dim))
                    print('=====================FE=====================')
                    print(self.eval_times)
                    value = np.array(x)
                    # print(value)
                    ls.append(self.f.evaluate(func_num, value))

                    if ls[-1] == "ReachFunctionLimit":
                        print("ReachFunctionLimit")
                        break
                    # print(ls[-1], )            
                    if float(ls[-1]) < self.optimal_value:
                        self.optimal_solution[:] = x
                        self.optimal_value = float(ls[-1])

                    print("optimal: {}\n".format(self.get_optimal()[1]))
                    self.eval_times += 1
                    if self.eval_times == FES:
                        return

                # print(solution, len(solution), ls , len(ls))
                es.tell(solution, ls)
                # print("qwer")

                es.logger.add()  # write data to disc to be plotted
                es.disp()

            # value = self.f.evaluate(func_num, solution)
            # self.eval_times += 1

            # if value == "ReachFunctionLimit":
            #     print("ReachFunctionLimit")
            #     break            
            # if float(value) < self.optimal_value:
            #     self.optimal_solution[:] = solution
            #     self.optimal_value = float(value)

            # print("optimal: {}\n".format(self.get_optimal()[1]))
            

if __name__ == '__main__':
    func_num = 1
    fes = 0
    #function1: 1000, function2: 1500, function3: 2000, function4: 2500
    aa = []
    while func_num < 5:
        if func_num == 1:
            fes = 1000
        elif func_num == 2:
            fes = 1500
        elif func_num == 3:
            fes = 2000 
        else:
            fes = 2500

        # you should implement your optimizer
        op = RS_optimizer(func_num)
        op.run(fes)
        
        best_input, best_value = op.get_optimal()
        print(best_input, best_value)
        aa.append(best_value)

        # change the name of this file to your student_ID and it will output properlly
        with open("{}_function{}.txt".format(__file__.split('_')[0], func_num), 'w+') as f:
            for i in range(op.dim):
                f.write("{}\n".format(best_input[i]))
            f.write("{}\n".format(best_value))
        func_num += 1 
    print("result: ", aa)

from random import shuffle
import time
import json

def simple_pcim (iterations, tile_size, lgn, nProbes, lgnProbes, num_pc_wu):
    subsetSize = tile_size - len(lgnProbes)
    allProbes = range(nProbes)
    otherProbes = list(set(allProbes) - set(lgnProbes))

    work_unit = 1
    counter = dict() # create a dictionary to store frequencies
    lst_lst = []
    final_lst = []

    for i in range(iterations):

        shuffle(otherProbes)
        probeBag = list(otherProbes) # probes not in the LGN to be extracted

        while probeBag != []:
            if (len(lst_lst) % num_pc_wu == 0) and lst_lst:
                lst_lst = []
                work_unit += 1

            subset, probeBag = probeBag[:subsetSize], probeBag[subsetSize:] # takes a subset of non-LGN indices
            toAdd = subsetSize - len(subset)

            if toAdd > 0:
                extra = list(set(otherProbes).difference(set(subset)))
                shuffle(extra)
                exxtra = extra[:toAdd] # get exactly extra probes to add
                subset += exxtra

                for key in exxtra:
                    if key in counter:
                        counter[key] += 1
                    else:
                        counter[key] = 1

            subset = subset + lgnProbes # adds the LGN to the subset
            shuffle(subset)
#            lst_lst.append(subsetToString(subset))

        counter = dict()

def main():
    iter = 3
    lgn = [22]
    lgn_size = len(lgn)

    data = {"benchmark" : []}

    v_size = 100
    while (v_size < 150000):
        tile_size = int(float(v_size)/100.0*5.0)

        start_time = time.time_ns()
        simple_pcim (iter, tile_size, lgn_size, v_size, lgn, 1)
        end_time = time.time_ns()

        delta_time = end_time - start_time
        data["benchmark"].append({"size" : v_size, "time" : delta_time})
        # print("size:", v_size, "time:", delta_time)
        v_size += 10000

    print(data)
    with open('data.json', 'w') as f:
        json.dump(data, f)

if __name__ == "__main__":
    main()











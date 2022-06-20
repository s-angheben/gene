from random import shuffle

def simple_pcim (iterations, tile_size, lgn, nProbes, lgnProbes, num_pc_wu):
    subsetSize = tile_size - len(lgnProbes)
    allProbes = range(nProbes)
    otherProbes = list(set(allProbes) - set(lgnProbes))

    work_unit = 1
    counter = dict() # create a dictionary to store frequencies
    lst_lst = []
    final_lst = []

    for i in range(iterations):
        print()
        print("ITERATION ", i)
        print()

        shuffle(otherProbes)
        probeBag = list(otherProbes) # probes not in the LGN to be extracted

        while probeBag != []:
            if (len(lst_lst) % num_pc_wu == 0) and lst_lst:
                final_lst.append("a");
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
            print(subset)

        print("FREQUENCIES:")
        print(counter)
        counter = dict()

def main():
    iter = 2
    lgn = [22, 33]
    lgn_size = len(lgn)
    tile_size = 30
    v_size = 40000
              # (iterations, tile_size, lgn, nProbes, lgnProbes, num_pc_wu)
#    simple_pcim (2, 5, 3, 30, [1,2,3], 1)
    simple_pcim (iter, tile_size, lgn_size, v_size, lgn, 1)

if __name__ == "__main__":
    main()











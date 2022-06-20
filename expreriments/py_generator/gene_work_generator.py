#!/usr/bin/env python

__author__ = 'Francesco Asnicar, Luca Masera, Paolo Morettin, Nadir Sella, Thomas Tolio'
__version__ = '0.15'
__date__ = '14 March 2017'

import logging
from sys import argv, exit
import os
from random import shuffle
from MySQLdb import connect
from time import sleep, time
from subprocess import call, Popen, PIPE
import smtplib
from email.mime.text import MIMEText
import xml.etree.ElementTree as ET
import multiprocessing as mp
import math
import shutil
from bz2 import BZ2File


# database "gene" configuration
GENE_HOSTNAME = 'localhost'
GENE_DATABASE = 'gene'
GENE_USERNAME = 'ugene'
GENE_PASSWORD = 'db@G3ne'
TBL_PCIM = 'pcim'
TBL_PCIM_TO_EXECUTE = 'pcim_to_execute'
TBL_PCIM_ERRORS = 'pcim_errors'
TBL_EXPERIMENTS = 'experiments'
TBL_PCIM_EXPERIMENTS = 'pcim_experiments'
# TBL_USER = 'user'
# TBL_USER_PCIM = 'user_pcim'
TBL_WG_PARAMS = 'wg_params'
TBL_BENCHMARK = 'benchmark'

# default output file name for the pcim
DEFAULT_TILE_FILENAME = 'tile.txt.bz2'
OUTPUT = "out_file"


def initt(terminating_):
    # This places terminating in the global namespace of the worker subprocesses.
    # This allows the worker function to access `terminating` even though it is
    # not passed as an argument to the function.
    global terminating
    terminating = terminating_


def getLgnProbes(logging, lgn_path, obs_path):
    '''
    return the indexes of the lgn probes.
    lgn_path must be a file with a header, representing edges.
    obs_path must be a file with a header, in the first column must be present the probes

    lgn.txt
    -------
    from,to
    node1,node2
    node1,node3
    node2,node4

    obs.txt
    -------
    probe,exp1,exp2,exp3
    node1,12.4,13.9,19.6
    node2,10.5,17.2,13.1
    node3,16.9,12.7,14.8
    '''
    lgn = []

    with open(lgn_path) as f:
        for row in f.readlines()[1:]:
            lgn += [a.strip() for a in row.strip().split(',')]

    lgnNames = set(lgn) # filters repetitions on probe names
    lgnIndices = []

    with open(obs_path) as f:
        rowNumber = 0

        for row in f.readlines()[1:]:
            probeId = row.split(',')[0]

            if probeId in lgnNames: # the row correspond to a LGN node
                lgnIndices.append(rowNumber)

            rowNumber += 1

    if len(lgnIndices) == 0:
        logging.warning("Creating an empty for lgn: {} and experiment: {}".format(lgn_path, obs_path))

    return (lgnIndices, rowNumber)


def subsetToString(subList):
    if len(subList) <= 0:
        logging.warning('Empty "subList"')

    return ' '.join([str(a) for a in subList])


def fix_lgn_string(lgn_name):
    # find and remove the extension and replace '_' wit '-'
    return lgn_name[:lgn_name.rfind('.')].replace("_", "-")


def createWU((create_wu, tilePath, in_sticky_path, lst_lst, exps_list_str, pcim_name, app_name, exps_list, alpha, deadline, time_estimation, replication_factor, col_number, out_template, cut_results)):
    """ Create a single work unit with the passed parameters, calling the create_one_wu program """
    # write the tile input file

    if not terminating.is_set():
        retval = -1
        in_nosticky_path = tilePath[:-8] + "_in_nosticky.txt"

        with BZ2File(tilePath, 'w') as f:
            f.write('\n'.join([exps_list_str] + lst_lst) + '\n')

        with open(in_nosticky_path, 'w') as expsFile:
            expsFile.write(tilePath)

        try:
            # call the work-generator with the passed parameters
            retval = call([create_wu, pcim_name, app_name, out_template, repr(time_estimation), in_sticky_path, repr(len(exps_list)), in_nosticky_path, repr(1), repr(replication_factor), repr(deadline), repr(alpha), OUTPUT, repr(col_number), repr(cut_results)])
            call(['rm', '-f', in_nosticky_path])
        except Exception as e:
            raise Exception('make_one_wu failed with exception: ' + str(e))

        if retval != 0:
            raise Exception('make_one_wu return an error state: ' + str(retval))

        return retval

    terminating.set()


# parte computazionale
def pcim(logging, wu_name, pcim_name, app_name, exps_list, lgn_path, alpha, iterations, tile_size, num_pc_wu, deadline, num_cols, replication_factor, out_template, create_wu, executions_path, cut_results, time_one_pc):
    """ Execute an entire pc-im run creating the tiles for the work-units and executing the createWU function """
    obs_path, _ = exps_list[0]

    # gets the LGN indices from the LGN network file, also returns the total number of probes from the 'complete' file
    lgnProbes, nProbes = getLgnProbes(logging, lgn_path, obs_path)

    # get the number of genes to add to the LGN
    subsetSize = tile_size - len(lgnProbes)

    # check size of subsets
    if(subsetSize <= 0):
        logging.critical("Tile dimension minor or equal to lgn size")
        exit(1)

    allProbes = range(nProbes)

    # indices of non-LGN nodes, shuffled in order to take different subset on each iteration
    otherProbes = list(set(allProbes) - set(lgnProbes))

    # creates a working directory for the pc-im run with all the files
    workingDirectory = executions_path+'/'+str(wu_name)+'/'

    if os.path.isdir(workingDirectory):
        logging.critical('Directory "'+workingDirectory+'" already exists!')
        exit(1)

    os.makedirs(workingDirectory+'results/', 0775)

    # compose the string for experiments list
    exps_list_str = ' '.join([a for _, a in exps_list])

    # print parameters file
    params_path = workingDirectory+"parameters.txt"

    with open(params_path, 'w') as f:
        f.write('\n'.join(["PC-IM: "+pcim_name, "LGN: "+lgn_path, "alpha: "+str(alpha), "iterations: "+str(iterations), "tile size: "+str(tile_size), "PC x WU: "+str(num_pc_wu), "deadline: "+str(deadline), "num cols: "+str(num_cols), "WU FLOP: "+str(time_one_pc)]) + "\n")

    # print input file downloads
    in_sticky_path = workingDirectory+"in_sticky.txt"

    with open(in_sticky_path, 'w') as expsFile:
        expsFile.write('\n'.join([a for a, _ in exps_list]))

    lgn_name = lgn_path[(lgn_path.rfind('/')+1):]
    lgn_name = fix_lgn_string(lgn_name)

    work_unit = 1
    counter = dict() # create a dictionary to store frequencies
    lst_lst = []
    final_lst = []

    t0=time()

    for i in range(iterations):
        shuffle(otherProbes)
        probeBag = list(otherProbes) # probes not in the LGN to be extracted

        while probeBag != []:
            # check if a tile is completed
            if (len(lst_lst) % num_pc_wu == 0) and lst_lst:
                tilePath = workingDirectory+str(wu_name)+'_'+str(int(round(time()*1000)))+"_wu-"+str(work_unit)+"_"+DEFAULT_TILE_FILENAME
                final_lst.append([create_wu, tilePath, in_sticky_path, lst_lst, exps_list_str, (str(wu_name)+"_"+str(lgn_name)+"_wu-"+str(work_unit)+"_"+str(int(round(time()*1000)))), app_name, exps_list, alpha, deadline, (time_one_pc*num_pc_wu), replication_factor, num_cols, out_template, cut_results])
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
            lst_lst.append(subsetToString(subset))

    # write the unwritten
    if lst_lst:
        tilePath = workingDirectory+str(wu_name)+'_'+str(int(round(time()*1000)))+"_wu-"+str(work_unit)+"_"+DEFAULT_TILE_FILENAME
        final_lst.append([create_wu, tilePath, in_sticky_path, lst_lst, exps_list_str, str(wu_name)+"_"+str(lgn_name)+"_wu-"+str(work_unit)+"_"+str(int(round(time()*1000))), app_name, exps_list, alpha, deadline, (time_one_pc*len(lst_lst)), replication_factor, num_cols, out_template, cut_results])

    print('critical_loop\t{}'.format(int(time()-t0)))
    t0 = time()

    # parallel work-unit creation
    terminating = mp.Event()
    pool = mp.Pool(initializer=initt, initargs=(terminating, ), processes=3)
    chunk = int(math.ceil(len(final_lst)/30.0))

    try:
        pool.imap_unordered(createWU, final_lst, chunksize=chunk if chunk > 1 else 1)
        pool.close()
        pool.join()
    except Exception as e:
        pool.terminate()
        pool.join()
        logging.critical('createWU returned error "{}"'.format(str(e)))
        exit(1)

    print('create_wu\t{}'.format(int(time()-t0)))

    # print frequency
    with open(workingDirectory+"frequency.txt", 'w') as freqFile:
        for k, c in counter.iteritems():
            freqFile.write(str(c) + "\t" + str(k) + "\n")

    return work_unit


def check_wg_params(cushion, replication_factor, max_sleep_time, num_pc_wu, deadline, out_template, executions_path):
    """ Check the working generator parameters """

    # check if they are string
    try:
        executions_path = str(executions_path)
    except:
        logging.error('Executions path: "'+executions_path+'" is not a string')
        return False

    # check if they are int
    try:
        cushion = int(cushion)
    except:
        logging.error('Cushion: "'+cushion+'" is not an int')
        return False

    try:
        replication_factor = int(replication_factor)
    except:
        logging.error('Replication factor: "'+replication_factor+'" is not an int')
        return False

    try:
        max_sleep_time = int(max_sleep_time)
    except:
        logging.error('Max sleep time: "'+max_sleep_time+'" is not an int')
        return False

    try:
        num_pc_wu = int(num_pc_wu)
    except:
        logging.error('Number of PCs per WU: "'+num_pc_wu+'" is not an int')
        return False

    try:
        deadline = int(deadline)
    except:
        logging.error('Deadline: "'+deadline+'" is not an int')
        return False

    # check their values
    if cushion <= int(0):
        logging.error('Cushion: "'+cushion+'" is out of the valid range')
        return False

    if max_sleep_time <= int(0):
        logging.error('Max sleep time: "'+max_sleep_time+'" is out of the valid range')
        return False

    if num_pc_wu <= int(0):
        logging.error('Number of PCs per WU: "'+num_pc_wu+'" is out of the valid range')
        return False

    try:
        out_template = str(out_template)
    except:
        logging.error('Out template: "'+out_template+'" is not a string')
        return False

    # it should be all good
    return True


def check_pcim_params(organism, pcim_name, lgn_path, alpha, iterations, tile_size, npc, cutoff, priority):
    """ Check the pc-im parameters """

    # check if it is a string
    try:
        organism = str(organism)
    except:
        msg_error = 'Organism: "'+pcim_name+'" is not a string'
        logging.error(msg_error)
        return (False, msg_error)

    try:
        pcim_name = str(pcim_name)
    except:
        msg_error = 'PC-IM name: "'+pcim_name+'" is not a string'
        logging.error(msg_error)
        return (False, msg_error)

    # check if the files exists
    if not os.path.isfile(lgn_path):
        msg_error = 'LGN: "'+lgn_path+'" is not a valid file'
        logging.error(msg_error)
        return (False, msg_error)

    # check if it is a float
    try:
        alpha = float(alpha)
    except:
        msg_error = 'Alpha: "'+alpha+'" is not a float'
        logging.error(msg_error)
        return (False, msg_error)

    # check alpha value
    if (alpha <= float(0)) or (alpha > float(1)):
        msg_error = 'Alpha: "'+alpha+'" is out of the valid range'
        logging.error(msg_error)
        return (False, msg_error)

    # check if they are int
    try:
        iterations = int(iterations)
    except:
        msg_error = 'Iterations: "'+iterations+'" is not an int'
        logging.error(msg_error)
        return (False, msg_error)

    try:
        tile_size = int(tile_size)
    except:
        msg_error = 'Tile size: "'+tile_size+'" is not an int'
        logging.error(msg_error)
        return (False, msg_error)

    try:
        npc = int(npc)
    except:
        msg_error = 'Number of PCs: "'+npc+'" is not an int'
        logging.error(msg_error)
        return (False, msg_error)

    try:
        cutoff = int(cutoff)
    except:
        msg_error = 'Results cutoff: "'+cutoff+'" is not an int'
        logging.error(msg_error)
        return (False, msg_error)

    try:
        priority = int(priority)
    except:
        msg_error = 'Priority: "'+priority+'" is not an int'
        logging.error(msg_error)
        return (False, msg_error)

    # it should be all good
    return (True, '')


def check_exps_list_size(logging, exps_list):
    """ check that all the experiments have the same number of rows, and return the sum of all columns (ibridizations) """

    exp, _ = exps_list[0]
    row_number = 0
    col_number = 0

    with open(exp) as f:
        lines = f.readlines()
        row_number = len(lines)
        col_number += len(lines[0].split(','))-1

    for exp, _ in exps_list[1:]:
        count = 0

        with open(exp) as f:
            lines = f.readlines()
            count = len(lines)

            if row_number != count:
                logging.error("Experiments files have different lengths")
                return (False, 0)

            col_number += len(lines[0].split(','))-1

    return (True, col_number)


def send_email(subject, email_body, db_handle, db_cursor, pcim_id):
    """ Send an e-mail to the user that submitted the pc-im job [need to be completed] """

    COMMASPACE = ', '
    _from = 'boincadm@gene.disi.unitn.it' # maybe should be put in the db (wg_params?)
    _to = ['f.asnicar@unitn.it'] # must be removed and substitute with the following commented lines

    # retrieve the e-mail of the user that submitted the pc-im execution
    # db_handle.execute("SELECT e_mail FROM {} WHERE user_id IN (SELECT user_id FROM {} WHERE pcim_id = {})".format(TBL_USER, TBL_USER_PCIM, pcim_id))
    # db_cursor.commit()

    # if db_cursor.rowcount > 0:
    msg = MIMEText(email_body, 'plain')
    msg['From'] = _from
    msg['To'] = COMMASPACE.join(_to)
    msg['Subject'] = subject

    try:
        smtpObj = smtplib.SMTP('localhost')
        smtpObj.sendmail(_from, _to, msg.as_string())
        smtpObj.quit()

        logging.info('E-mail sent to: "'+COMMASPACE.join(_to)+'" with subject: "'+subject+'"')
    except:
        logging.error('Unable to send e-mail "'+subject+'" to: "'+COMMASPACE.join(_to)+'"')
    # else:
    #   logging.error("Unable to find the e-mail address for the user that submitted the pcim: \"" + pcim_id + "\".")


def mark_pcim_error(db, cur, TBL_PCIM, pcim_id, pcim_name, msg):
    """ set the pc-im workunit as an error in the database """

    cur.execute("UPDATE {} SET error = 1, email_sent = 1 WHERE pcim_id = {}".format(TBL_PCIM, pcim_id))
    db.commit()
    send_email('PC-IM \"' + pcim_name + '\" error', 'Your PC-IM \"' + pcim_name + '\" returned the following error: \"' + msg + '\". We saved it into the \"' + TBL_PCIM_ERRORS + '\" table.', db, cur, pcim_id)
    logging.error("\"" + pcim_name + "\" returned the following error: \"" + msg + "\". Saved into \"" + TBL_PCIM_ERRORS + "\"")


def main(argv):
    # set-up a format for the logging prints in BOINC style
    ch = logging.StreamHandler()
    formatter = logging.Formatter("%(asctime)s [%(levelname)s] %(message)s")
    ch.setFormatter(formatter)
    logging.getLogger().addHandler(ch)

    # check input params
    if len(argv) < 3:
        logging.critical("Wrong input parameters: get_queue make_one_wu app_name [debug_level]")
        exit(1)
    else:
        # save the c programs that check the unsent wus
        check_queue = str(argv[0])
        create_wu = str(argv[1])
        app_name = str(argv[2])

    # check if the c program exists
    if not os.path.isfile(check_queue):
        logging.critical("The program \"" + check_queue + "\" is not a valid file")
        exit(1)

    # check if the c program exists
    if not os.path.isfile(create_wu):
        logging.critical("The program \"" + create_wu + "\" is not a valid file")
        exit(1)

    # check the debug level
    #   0: print only critical
    #   1: print error and critical
    #   2: print warning, error and critical
    #   3: print info, warning, error and critical
    #   4: print debug, info, warning, error and critical
    if len(argv) > 3:
        try:
            debug_level = int(argv[3])
        except:
            logging.error("Invalid debug level \"" + argv[2] + "\", set default value: 2 (warning)")
            debug_level = 2
    else:
        logging.warning("No debug level found, set default value: 2 (warning)")
        debug_level = 2

    if (debug_level < 0) or (debug_level > 4):
        logging.warning("Wrong debug level \"" + str(debug_level) + "\", set default value: 2 (warning)")
        debug_level = 2

    # set the logging level accordingly
    if debug_level == 0:
        logging.getLogger().setLevel(logging.CRTICAL) # set the logging level to CRTICAL

    if debug_level == 1:
        logging.getLogger().setLevel(logging.ERROR) # set the logging level to ERROR

    if debug_level == 2:
        logging.getLogger().setLevel(logging.WARNING) # set the logging level to WARNING

    if debug_level == 3:
        logging.getLogger().setLevel(logging.INFO) # set the logging level to INFO

    if debug_level == 4:
        logging.getLogger().setLevel(logging.DEBUG) # set the logging level to DEBUG

    # sleep back-off
    i = 1

    # start daemon
    while True:
        # check the "config.xml" file and verify the disable bit
        config_xml = ET.parse(open("../config.xml", "r"))
        root = config_xml.getroot()
        daemons = root.findall("daemons/daemon")

        for daemon in daemons:
            daemon_name = str(daemon.find('cmd').text)

            if daemon_name.startswith('gene_work_generator'):
                if bool(int(daemon.find('disabled').text)):
                    logging.warning("Work generator disabled in \"config.xml\" file. Quitting...")
                    exit(1)

        # connect to the MySQL database
        db = connect(host=GENE_HOSTNAME, db=GENE_DATABASE, user=GENE_USERNAME, passwd=GENE_PASSWORD)

        with db:
            cur = db.cursor()

            # check the unsent wus
            try:
                p = Popen([check_queue], stdout=PIPE)
            except:
                logging.critical("The program \"" + check_queue + "\" is not an executable file")
                exit(1)

            # save the number of unsent wus present in the queue
            try:
                unsent_wus = int(p.communicate()[0])
            except:
                logging.critical("The daemon should not be running")
                exit(1)

            # count the number of pcim to execute present
            cur.execute("SELECT * FROM {}".format(TBL_PCIM_TO_EXECUTE))
            db.commit()

            logging.info("unsent wus: {}, pcim to execute: {}".format(unsent_wus, cur.rowcount))

            # query to read the work-generator parameters:
            #     cushion: the threshold under which the work generator starts creating work-units
            #     replication_factor: the number of replication for each work-unit
            #     max_sleep_time: maximum sleep time, above it the exponential back-off will be reset
            #     num_pc_wu: the number of PC executions for each work unit
            cur.execute("SELECT * FROM {} LIMIT 1".format(TBL_WG_PARAMS))
            db.commit()

            if cur.rowcount > 0:
                cushion, replication_factor, max_sleep_time, num_pc_wu, deadline, out_template, executions_path, _ = cur.fetchone()
            else:
                logging.critical("Could not read the work-generator parameters")
                exit(1)

            if check_wg_params(cushion, replication_factor, max_sleep_time, num_pc_wu, deadline, out_template, executions_path):
                cushion = int(cushion)
                replication_factor = int(replication_factor)
                max_sleep_time = int(max_sleep_time)
                num_pc_wu = int(num_pc_wu)
                deadline = int(deadline)

                if not out_template.endswith('/'):
                    out_template += '/'
                out_template = str(out_template)+app_name+'_result.xml'

                if not executions_path.endswith('/'):
                    executions_path += '/'
                executions_path = str(executions_path)+app_name+'_results'

                logging.debug("cushion: {}, replication_factor: {}, max_sleep_time: {}, num_pc_wu: {}, deadline: {}, out_template: {}, executions_path: {}".format(cushion, replication_factor, max_sleep_time, num_pc_wu, deadline, out_template, executions_path))
            else:
                logging.critical("Work-generator parameters not valid")
                exit(1)

            # query to read a tuple, i.e., a PC-IM execution
            #     pcim_id: primary key of the tuple in order to be able to remove it
            #     pcim_name: the string that identifies the PC-IM execution
            #     lgn_path: the path to the lgn file
            #     alpha: the value of alpha
            #     iterations: the number of iterations through the genome
            #     tile_size: the number of nodes for each sub-graph
            #     priority: the task priority
            cur.execute("SELECT * FROM {} LIMIT 1".format(TBL_PCIM_TO_EXECUTE))
            db.commit()

            # verify the unsent_wus with the cushion and the if there are PC-IM to execute
            if (unsent_wus < cushion) and (cur.rowcount > 0):
                pcim_id, organism, pcim_name, lgn_path, alpha, iterations, tile_size, npc, cutoff, priority = cur.fetchone()

                # if all the params in the db are ok
                chk_pcim_params, msg_error = check_pcim_params(organism, pcim_name, lgn_path, alpha, iterations, tile_size, npc, cutoff, priority)
                if chk_pcim_params:
                    organism = str(organism)
                    pcim_name = str(pcim_name)
                    lgn_path = str(lgn_path)
                    alpha = float(alpha)
                    iterations = int(iterations)
                    tile_size = int(tile_size)
                    npc = int(npc)
                    cutoff = int(cutoff)
                    priority = int(priority)

                    # if not zero, use the specific number of PC per work-unit provided
                    if npc:
                        num_pc_wu = npc

                    logging.info('"{}" to be executed'.format(pcim_name))
                    logging.debug("organism: {}, pcim_name: {}, lgn_path: {}, alpha: {}, iterations: {}, tile_size: {}, npc: {}, priority: {}".format(organism, pcim_name, lgn_path, alpha, iterations, tile_size, npc, priority))

                    # retrieve exp_id
                    cur.execute("SELECT exp_id FROM {} WHERE pcim_id = {}".format(TBL_PCIM_EXPERIMENTS, pcim_id))
                    db.commit()

                    if cur.rowcount > 0:
                        (exp_id, ) = cur.fetchone()
                    else:
                        logging.critical("Experiments ID not found in "+TBL_PCIM_EXPERIMENTS+" for PC-IM: "+pcim_id)
                        exit(1)

                    # read time-estimate
                    cur.execute("SELECT pc_time, host_flops FROM {} WHERE exp_id = {} AND pc_tsize = {} AND pc_alpha = {} AND app_name LIKE '{}' AND app_version LIKE '{}'".format(TBL_BENCHMARK, exp_id, tile_size, alpha, app_name, "1.00"))
                    db.commit()

                    if cur.rowcount > 0:
                        pc_time, host_flops = cur.fetchone()
                    else:
                        logging.critical('Unable to find host estimates, query: "SELECT pc_time, host_flops FROM {} WHERE exp_id = {} AND pc_tsize = {} AND pc_alpha = {} AND app_name LIKE \'{}\' AND app_version LIKE \'{}\'"'.format(TBL_BENCHMARK, exp_id, tile_size, alpha, app_name, "1.00"))
                        exit(1)

                    try:
                        pc_time = float(pc_time)
                    except:
                        logging.error("Could not convert pc_time ({}) to float".format(pc_time))
                        exit(1)

                    try:
                        host_flops = float(host_flops)
                    except:
                        logging.error("Could not convert host_flops ({}) to float".format(host_flops))
                        exit(1)

                    exps_list = []
                    error = False

                    # retrieve the experiments files
                    cur.execute("SELECT exp_path, exp_name FROM {} WHERE exp_id = {}".format(TBL_EXPERIMENTS, exp_id))
                    db.commit()

                    if cur.rowcount > 0:
                        exps = cur.fetchall()

                        for e in exps:
                            exps_list.append((str(e[0]), str(e[1])))

                        # check if experiments exists
                        for e, _ in exps_list:
                            if not os.path.isfile(e):
                                msg_error = "The experiment \"{}\" is not a valid file".format(e)
                                logging.warning(msg_error)
                                error = True

                        # check if experiments are all of the same row size
                        ret, num_cols = check_exps_list_size(logging, exps_list)

                        if not ret:
                            msg_error = "Incompatible experiments, have different lenghts"
                            logging.warning(msg_error)
                            error = True
                    else:
                        msg_error = "No experiments found in database"
                        logging.warning(msg_error)
                        error = True

                    if not error:
                        try: # call pc-im
                            number_wu = pcim(logging, str(pcim_id)+'_'+organism, pcim_name, app_name, exps_list, lgn_path, alpha, iterations, tile_size, num_pc_wu, deadline, num_cols, replication_factor, out_template, create_wu, executions_path, cutoff, pc_time*host_flops*1.6)

                            cur.execute("UPDATE {} SET in_execution = 1, number_wus = {}, time_creation = NOW() WHERE pcim_id = {}".format(TBL_PCIM, number_wu, pcim_id)) # mark the record as 'in execution'
                            db.commit()

                            logging.info('"'+pcim_name+'" in execution')
                        except Exception as e:
                            logging.error(str(e))
                            mark_pcim_error(db, cur, TBL_PCIM, pcim_id, pcim_name, str(e)) # mark the record as 'error'
                    else:
                        mark_pcim_error(db, cur, TBL_PCIM, pcim_id, pcim_name, msg_error) # mark the record as 'error'
                else:
                    mark_pcim_error(db, cur, TBL_PCIM, pcim_id, pcim_name, msg_error) # mark the record as 'error'

                i = 1 # reset the back-off
            else: # wait
                sleep(i)
                logging.info("Slept {} seconds".format(i))
                i = i * 2

                if i > max_sleep_time: # avoid sleep too much
                    i = 1
                    logging.debug("Exceeded the max_sleep_time value, restart from 1")


main(argv[1:])


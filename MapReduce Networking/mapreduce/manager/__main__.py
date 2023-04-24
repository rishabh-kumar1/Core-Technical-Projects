"""MapReduce framework Manager node."""
import os
import tempfile
import logging
import json
import time
import click
import mapreduce.utils
import threading
import socket
import pathlib
import queue
import shutil
import time 


# Configure logging
LOGGER = logging.getLogger(__name__)


class Manager:
    """Represent a MapReduce framework Manager node.""" 

    host = ""
    port = ""
    job_queue = queue.Queue()
    worker_list = []
    mapping_tasks = {}
    reducing_tasks = {}
    worker_timestamps = {}
    curr_jobid = 0
    curr_job = None
    shutdown = False

    def __init__(self, host, port):
        """Construct a Manager instance and start listening for messages."""
        self.host = host
        self.port = port

        LOGGER.info(
            "Starting manager:%s", self.port,
        )

        thread_tcp = threading.Thread(target=self.manager_server, args = ())
        thread_tcp.start()

        job_tcp = threading.Thread(target=self.run_jobs, args = ())
        job_tcp.start()

        check_workers = threading.Thread(target=self.check_workers)
        check_workers.start()

        thread_udp = threading.Thread(target=self.manager_heartbeat)
        thread_udp.start()
        time.sleep(10)
        
        # thread_udp.join()
        thread_tcp.join()
        job_tcp.join()
        check_workers.join()
        thread_udp.join()

    def manager_server(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            # Bind the socket to the server
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.bind((self.host, self.port))
            sock.listen()
            # Socket accept() will block for a maximum of 1 second.  If you
            # omit this, it blocks indefinitely, waiting for a connection.
            sock.settimeout(1)

            while not self.shutdown:
                LOGGER.info("starting tcp")



                # Wait for a connection for 1s.  The socket library avoids
                # consuming CPU while waiting for a connection.
                try:
                    clientsocket, address = sock.accept()
                except socket.timeout:
                    continue

                clientsocket.settimeout(1)
               
                with clientsocket:
                    message_chunks = []
                    while True:
                        try:
                            data = clientsocket.recv(4096)
                        except socket.timeout:
                            continue
                        if not data:
                            break
                        message_chunks.append(data)
                # Decode list-of-byte-strings to UTF8 and parse JSON data
                message_bytes = b''.join(message_chunks)
                message_str = message_bytes.decode("utf-8")
                try:
                    message_dict = json.loads(message_str)
                except json.JSONDecodeError:
                    continue

                LOGGER.debug(
                    "recieved\n", message_dict
                )

                if message_dict["message_type"] == "register":
                    #do register worker
                    # 0 = dead, 1 = ready, 2 = busy
                    temp_worker = {"worker_host": message_dict["worker_host"],
                                    "worker_port": message_dict["worker_port"], 
                                    "worker_status": 1}
                    self.worker_list.append(temp_worker)

                    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock2:
                        sock2.connect((temp_worker["worker_host"], temp_worker["worker_port"]))
                        reg_message = json.dumps(
                            {
                                "message_type": "register_ack",
                                "worker_host": temp_worker["worker_host"],
                                "worker_port": temp_worker["worker_port"],
                            }
                        )
                        sock2.sendall(reg_message.encode('utf-8'))
                    
                    LOGGER.info(
                        "registered worker %s:%d", temp_worker["worker_host"], temp_worker["worker_port"],
                    )
           
                elif message_dict["message_type"] == "shutdown":
                    self.shutdown = True
                    LOGGER.info("Set shutdown to true")

                    for i in self.worker_list:
                        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock2:
                            sock2.connect((i["worker_host"], i["worker_port"]))
                            reg_message = json.dumps(
                                {
                                    "message_type": "shutdown"
                                }
                            )
                            sock2.sendall(reg_message.encode('utf-8'))
                    LOGGER.info(
                        "shutting down"
                    )
                    LOGGER.info(self.shutdown)
                
                elif message_dict["message_type"] == "new_manager_job":
                    
                    temp_job = {
                        "input_directory": message_dict["input_directory"],
                        "output_directory": message_dict["output_directory"],
                        "mapper_executable": message_dict["mapper_executable"],
                        "reducer_executable": message_dict["reducer_executable"],
                        "num_mappers": message_dict["num_mappers"],
                        "num_reducers": message_dict["num_reducers"],
                        "job_id": self.curr_jobid
                    }

                    self.curr_jobid+=1
                    self.job_queue.put(temp_job)

                    #make output dir
                    output = pathlib.Path(temp_job["output_directory"])
                    if output.exists():
                        shutil.rmtree(output)
                    output.mkdir(parents=True, exist_ok=True)
                elif message_dict["message_type"] == "finished":
                    for temp_worker in self.worker_list:
                        if temp_worker["worker_port"] == message_dict["worker_port"]:
                            temp_worker["worker_status"] = 1
                    LOGGER.info(
                        "task done"
                    )

            #have to add another elif for worker mapping done 


    def run_jobs(self):
        while not self.shutdown:
            time.sleep(0.1)
            LOGGER.info(self.shutdown)
            if not self.job_queue.empty() and self.curr_job == None: 
                self.curr_job = self.job_queue.get()


                #create temp directory
                tmp_id = self.curr_job["job_id"]
                prefix = f"mapreduce-shared-job{tmp_id:05d}-"
                with tempfile.TemporaryDirectory(prefix=prefix) as tmpdir:
                    LOGGER.info("Created tmpdir %s", tmpdir)

                    job_done = False


                    while not job_done and not self.shutdown: 

                        #mapping
                        tmp_path = pathlib.Path(self.curr_job["input_directory"]).glob('**/*')
                        files = [x for x in tmp_path if x.is_file()]
                        files.sort()

                        task_id = 0

                        tmp_mapping = queue.Queue()

                        for i in range(self.curr_job["num_mappers"]):
                            self.mapping_tasks[i] = []
                            tmp_mapping.put(i)

                        for x in files:
                            self.mapping_tasks[task_id].append(str(x))
                            task_id+=1
                            if task_id == self.curr_job["num_mappers"]:
                                task_id = 0


                        # for loop should be based on number of tasks and not on self.worker_list
                        while not tmp_mapping.empty() and len(self.worker_list) != 0:
                            for temp_worker in self.worker_list:
                                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                                    if (temp_worker["worker_status"] == 1):
                                        map_task = tmp_mapping.get()
                                        sock.connect((temp_worker["worker_host"], temp_worker["worker_port"]))
                                        reg_message = json.dumps(
                                            {
                                                "message_type": "new_map_task",
                                                "task_id": map_task,
                                                "input_paths": self.mapping_tasks[map_task],
                                                "executable": self.curr_job["mapper_executable"],
                                                "output_directory": tmpdir,
                                                "num_partitions": self.curr_job["num_reducers"],
                                                "worker_host": temp_worker["worker_host"],
                                                "worker_port": temp_worker["worker_port"]
                                            }
                                        )
                                        sock.sendall(reg_message.encode('utf-8'))
                                        temp_worker["worker_status"] = 2
                                        LOGGER.info(
                                            "map task created"
                                        )
                                time.sleep(0.5)


                        #reducing
                        tmp_path = pathlib.Path(tmpdir).glob('**/*')
                        files = [str(x) for x in tmp_path if x.is_file()]
                        
                        tmp_reducing = queue.Queue()
                        for i in range(self.curr_job["num_reducers"]):
                            self.reducing_tasks[i] = []
                            tmp_reducing.put(i)

                        for x in files:
                            tmp_partition = x[-5:]
                            self.reducing_tasks[int(tmp_partition)].append(x)
                        
                        while not tmp_reducing.empty() and len(self.worker_list) != 0:
                            for temp_worker in self.worker_list:
                                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                                    if (temp_worker["worker_status"] == 1):
                                        reduce_task = tmp_reducing.get()
                                        sock.connect((temp_worker["worker_host"], temp_worker["worker_port"]))
                                        reg_message = json.dumps(
                                            {
                                            "message_type": "new_reduce_task",
                                                "task_id": reduce_task,
                                                "executable": self.curr_job["reducer_executable"],
                                                "input_paths": self.reducing_tasks[reduce_task],
                                                "output_directory": self.curr_job["output_directory"],
                                                "worker_host": temp_worker["worker_host"],
                                                "worker_port": temp_worker["worker_port"]
                                            }
                                        )
                                        sock.sendall(reg_message.encode('utf-8'))
                                        temp_worker["worker_status"] = 2
                                        LOGGER.info(
                                            "worker task created")
                                time.sleep(0.5)
                        
                        job_done = True
                        self.curr_job = None
                        self.mapping_tasks={}
                        self.reducing_tasks = {}
                        time.sleep(1)
                        
                LOGGER.info("Cleaned up tmpdir %s", tmpdir)

                #if all workers busy then how do we make manager wait at THIS step here
                #while there is a job but no worker: time.sleep a little and then continneu
                #do time.sleep and continue the while loop for while there is a task availbale to give
                LOGGER.info("exit with")


    def manager_heartbeat(self):
        #recieve heartbeats
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        # Bind the UDP socket to the server
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.bind((self.host, self.port))
            sock.settimeout(1)

            while True:
                try:
                    message_bytes = sock.recv(4096)
                except socket.timeout:
                    continue
                message_str = message_bytes.decode("utf-8")
                message_dict = json.loads(message_str)
                if message_dict["message_type"] == "hearbeat": 
                    self.worker_timestamps["worker_port"] = time.time()                  

    
    def check_workers(self):
        #iterate through workers timestamps
        while not self.shutdown: 
            for key in self.workers_timestamps: 
                if (time.time() - self.worker_timestamps[key] > 10): 
                    #this worker is now dead, mark as dead in the list 
                    #and reassign the work it was doing
                    for i in self.worker_list: 
                        if i["worker_port"] == key: 
                            i["worker_status"] = 0


@click.command()
@click.option("--host", "host", default="localhost")
@click.option("--port", "port", default=6000)
@click.option("--logfile", "logfile", default=None)
@click.option("--loglevel", "loglevel", default="info")
@click.option("--shared_dir", "shared_dir", default=None)
def main(host, port, logfile, loglevel, shared_dir):
    """Run Manager."""
    tempfile.tempdir = shared_dir
    if logfile:
        handler = logging.FileHandler(logfile)
    else:
        handler = logging.StreamHandler()
    formatter = logging.Formatter(
        f"Manager:{port} [%(levelname)s] %(message)s"
    )
    handler.setFormatter(formatter)
    root_logger = logging.getLogger()
    root_logger.addHandler(handler)
    root_logger.setLevel(loglevel.upper())
    Manager(host, port)


if __name__ == "__main__":
    main()
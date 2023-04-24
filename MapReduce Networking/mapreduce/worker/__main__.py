"""MapReduce framework Worker node."""
from asyncio import protocols
from contextlib import ExitStack
from mmap import PROT_WRITE
import os
import logging
import json
import tempfile
import time
import click
import pathlib
import threading
import shutil
import hashlib
import socket
import subprocess
import heapq
import mapreduce.utils


# Configure logging
LOGGER = logging.getLogger(__name__)


class Worker:
    """A class representing a Worker node in a MapReduce cluster."""
    #class variables
    host = ""
    port = ""
    manager_host = ""
    manager_port = ""
    shutdown = False

    def __init__(self, host, port, manager_host, manager_port):
        #class variables
        self.host = host
        self.port = port
        self.manager_host = manager_host
        self.manager_port = manager_port
        self.threads = []
        #make sure to put status as an int in a worker struct for manager class
        
        """Construct a Worker instance and start listening for messages."""
        LOGGER.info(
            "Starting worker host=%s port=%s pwd=%s",
            host, port, os.getcwd(),
        )
        LOGGER.info(
            "manager_host=%s manager_port=%s",
            manager_host, manager_port,
        )
 
        tcp_thread = threading.Thread(target=self.worker_tcp_server)
        self.threads.append(tcp_thread)
        tcp_thread.start()
        
        time.sleep(10)
        tcp_thread.join()  # Wait for server thread to shut down


        #print("main() shutting down")
        

    def worker_tcp_server(self):
        """Wait on a message from a socket OR a shutdown signal."""
        #print("server() starting")
        # Create an INET, STREAMing socket, this is TCP
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            # Bind the socket to the server
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.bind((self.host, self.port))
            sock.listen()

            sock.settimeout(1)

            #send registration message to manager
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock2:
                sock2.connect((self.manager_host, self.manager_port))
                reg_message = json.dumps(
                    {
                        "message_type": "register",
                        "worker_host": self.host,
                        "worker_port": self.port,
                    }
                )
                sock2.sendall(reg_message.encode('utf-8'))


            while not self.shutdown:
                print("waiting ...")
                time.sleep(0.1)

                try:
                    clientsocket, address = sock.accept()
                except socket.timeout:
                    continue
                print("Connection from", address[0])

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
                    "received %s", str(message_dict["message_type"])
                )

                if message_dict["message_type"] == "register_ack":
                    #start UDP thread
                    udp_thread = threading.Thread(target=self.worker_udp_server)
                    self.threads.append(udp_thread)
                    udp_thread.start()                  
                elif message_dict["message_type"] == "shutdown":
                    self.shutdown = True
                    LOGGER.info(
                        "shutting down"
                    )
                elif message_dict["message_type"] == "new_map_task":
                    self.do_mapping(message_dict)
                elif message_dict["message_type"] == "new_reduce_task":
                    self.do_reducing(message_dict)

    def worker_udp_server(self):
        while not self.shutdown: 
            with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock2:
                sock2.connect((self.manager_host, self.manager_port))
                reg_message = json.dumps(
                    {
                        "message_type": "heartbeat",
                        "worker_host": self.host,
                        "worker_port": self.port,
                    }
                )
                sock2.sendall(reg_message.encode('utf-8'))
            time.sleep(2)

    def do_mapping(self, message):
        exec = message["executable"]
        task_id = message["task_id"]
        prefix = f"mapreduce-local-task{task_id:05d}-"
        with tempfile.TemporaryDirectory(prefix=prefix) as tmpdir:
            LOGGER.info("Created tmpdir %s", tmpdir)
            with ExitStack() as stack:
                files = [stack.enter_context(open(fname)) for fname in message["input_paths"]]
                #make files here
                output_files = []
                sorted_words = []
                for parts in range(message["num_partitions"]):
                    temp_path = f"maptask{task_id:05d}-part{parts:05d}"
                    output_files.append(temp_path)
                    temp_sort = []
                    sorted_words.append(temp_sort)
                outputs = [stack.enter_context(open(fname, "a+")) for fname in output_files] 
                for i in files:
                    with subprocess.Popen(
                        [exec],
                        stdin=i,
                        stdout=subprocess.PIPE,
                        text=True,
                    ) as map_process:
                        for line in map_process.stdout:
                            separate = line.split()
                            key = separate[0]
                            if(len(separate) == 1):
                                key = ''
                            hexdigest = hashlib.md5(key.encode("utf-8")).hexdigest()
                            keyhash = int(hexdigest, base=16)
                            partition = keyhash % message["num_partitions"]
                            sorted_words[partition].append(line)


                LOGGER.info("Executed %s", exec)

                #sort
                for temp in range(len(outputs)):
                    data = sorted_words[temp]
                    LOGGER.info(len(data))
                    data.sort()
                    for line in data:
                        outputs[temp].write(line)
                    LOGGER.info("Sorted %s", tmpdir + "/" + outputs[temp].name)

                #move
                for temp in range(len(outputs)):
                    temp_file = pathlib.Path(outputs[temp].name)
                    shutil.move(temp_file, message["output_directory"])
                    LOGGER.info("Moved %s to %s", tmpdir + "/" + outputs[temp].name, message["output_directory"])

                LOGGER.info("Removed tmpdir %s", tmpdir)

        #send finished message to manager
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock2:
            sock2.connect((self.manager_host, self.manager_port))
            reg_message = json.dumps(
                {
                    "message_type": "finished",
                    "task_id": task_id,
                    "worker_host": self.host,
                    "worker_port": self.port,
                }
            )
            sock2.sendall(reg_message.encode('utf-8'))


    def do_reducing(self, message):
        exec = message["executable"]
        task_id = message["task_id"]
        output_dir = message["output_directory"]
        prefix = f"mapreduce-local-task{task_id:05d}-"
        with ExitStack() as stack:
            files = [stack.enter_context(open(fname)) for fname in message["input_paths"]]
            #heapq sort
            combined = heapq.merge(*files)
            heap_write = "sorted.txt"
            sorted_file = open("sorted.txt", "w")
            for line in combined:
                sorted_file.write(line)
            with tempfile.TemporaryDirectory(prefix=prefix) as tmpdir:
                LOGGER.info("Created tmpdir %s", tmpdir)
                temp_path = f"part-{task_id:05d}"
                with open(heap_write) as infile, open(temp_path) as outfile:
                    with subprocess.Popen(
                        [exec],
                        text=True,
                        stdin=subprocess.PIPE,
                        stdout=outfile,                            
                    ) as reduce_process:
                        for line in infile:
                            reduce_process.stdin.write(line)

                LOGGER.info("Executed %s", exec)

                #move
                shutil.move(temp_path, output_dir)
                LOGGER.info("Moved %s to %s", tmpdir + "/" + temp_path, output_dir)

                LOGGER.info("Removed tmpdir %s", tmpdir)

        #send finished message to manager
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock2:
            sock2.connect((self.manager_host, self.manager_port))
            reg_message = json.dumps(
                {
                    "message_type": "finished",
                    "task_id": message["task_id"],
                    "worker_host": self.host,
                    "worker_port": self.port,
                }
            )
            sock2.sendall(reg_message.encode('utf-8'))


@click.command()
@click.option("--host", "host", default="localhost")
@click.option("--port", "port", default=6001)
@click.option("--manager-host", "manager_host", default="localhost")
@click.option("--manager-port", "manager_port", default=6000)
@click.option("--logfile", "logfile", default=None)
@click.option("--loglevel", "loglevel", default="debug")
def main(host, port, manager_host, manager_port, logfile, loglevel):
    """Run Worker."""
    if logfile:
        handler = logging.FileHandler(logfile)
    else:
        handler = logging.StreamHandler()
    formatter = logging.Formatter(f"Worker:{port} [%(levelname)s] %(message)s")
    handler.setFormatter(formatter)
    root_logger = logging.getLogger()
    root_logger.addHandler(handler)
    root_logger.setLevel(loglevel.upper())
    Worker(host, port, manager_host, manager_port)


if __name__ == "__main__":
    main()

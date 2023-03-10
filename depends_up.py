import subprocess
from multiprocessing import Process

path = "depends/kafka/"

def call_zookeper():
    p = subprocess.Popen([path + "bin/zookeeper-server-start.sh", path + "config/zookeeper.properties"])#, stdout=myoutput, stderr=subprocess.PIPE, universal_newlines=True)
    a, b = p.communicate()

def call_kafka():
    p = subprocess.Popen([path + "bin/kafka-server-start.sh", path + "config/server.properties"])#, stdout=myoutput, stderr=subprocess.PIPE, universal_newlines=True)
    a, b = p.communicate()

def call_topics():
    p = subprocess.Popen([path + "bin/kafka-topics.sh", "--create", "--topic", "OrderEvents", "--bootstrap-server", "localhost:9092", "--partitions", "2"])#, stdout=myoutput, stderr=subprocess.PIPE, universal_newlines=True)
    a, b = p.communicate()
    p = subprocess.Popen([path + "bin/kafka-topics.sh", "--create", "--topic", "UserEvents", "--bootstrap-server", "localhost:9092"])#, stdout=myoutput, stderr=subprocess.PIPE, universal_newlines=True)
    a, b = p.communicate()

p1 = Process(target=call_zookeper)
p2 = Process(target=call_kafka)
p3 = Process(target=call_topics)
p1.start()
p2.start()
p3.start()
p3.join()
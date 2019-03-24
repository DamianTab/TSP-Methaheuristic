import subprocess
command = "TSP_Metaheuristic.exe"
instances=["bier127.txt","tsp250.txt","tsp500.txt","tsp1000.txt"]
time="300"
while(True):
    for which in instances:
        process = subprocess.call([command,which,time], shell=True)

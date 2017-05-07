import subprocess
subprocess.call('osl true 1000 1000 ..', shell = True)
print("Hej")
subprocess.call('osl false 1000 1000 ..', shell = True)
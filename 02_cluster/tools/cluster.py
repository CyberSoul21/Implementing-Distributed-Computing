from subprocess import PIPE, Popen
#Install sshpass
#password = 'wilsonalmario'
password = '20Naei\$\$Rnt2021#1'

ips = []


def cmdline(command):
		process = Popen(
        args=command,
        stdout=PIPE,
        shell=True
    )
		return process.communicate()[0]


def cmdssh(ip):

    strssh = 'sshpass -p "' + password + '" ssh -o StrictHostKeyChecking=no ' + 'gfnun@' + ip + ' hostname >> hostname1.txt'
    strssh2 = 'sshpass -p "' + password + '" ssh -o StrictHostKeyChecking=no ' + 'gfnun@' + ip + ' hostname -I >> hostname2.txt'
    strssh3 = 'sshpass -p "' + password + '" ssh -o StrictHostKeyChecking=no ' + 'gfnun@' + ip + ' nproc >> process.txt'
    return strssh, strssh2, strssh3

def cmd_clear():
		clear = 'rm hostname1.txt hostname2.txt ips.txt cluster_structure_new.txt'
		cmdline(clear)
		return clear

def lst_ips():
		list_ips = []
		fileref = open("ips.txt", "r")
		text=fileref.readlines()
		for x in text:
				aux = x.split('\n')
				list_ips.append(aux[0])
		return list_ips

def get_hostname(lst_ips):
		ips_ok = []
		for x in lst_ips:
				cmd1,cmd2, cmd3= cmdssh(x)
				cmdline(cmd1)
				cmdline(cmd2)
				cmdline(cmd3)
				#e=cmdline(cmdssh(x))
		print(ips_ok)

def concatenate():
	list_ips = []
	list_names = []
	file2 = open("hostname2.txt", "r")
	file1 = open("hostname1.txt", "r")
	ips = file2.readlines()
	names = file1.readlines()
	c=0
	for x in names:
		aux = x.split('\n')
		#print(aux)
		list_names.append(aux[0])
		c += 1
	c=0
	#print(ips[0].split('\n'))
	for y in ips:
		aux = y.split('\n')
		list_ips.append(aux[0][0:13])

		c += 1
	print('Concatenate ok')
	return list_ips, list_names

def make_structure(lst_ips,lst_names):
	file1 = open("cluster_structure.txt", "r")
	struc = file1.readlines()
	lst_pc = []
	outfile = open("cluster_structure_new.txt","w")
	for data in struc:
		aux = data.strip().split(",")
		for data2 in lst_names:
			if data2 in aux:
				x=lst_names.index(data2)
				aux.append(lst_ips[x])
		if(len(aux)<4):
			aux.append('000.000.0.00')
		lst_pc.append(aux)
		#print(len(aux))
	for x in lst_pc:
		for y in x:
			outfile.write(str(y) + " ")
			#print(str(y))
		outfile.write("\n")
	outfile.close()
	return lst_pc
#print(cmdline("cat /etc/services"))
#print(cmdline('hostname -I >> myip.txt'))
#print(cmdline('nslookup google.com'))i

cmd_test = 'sshpass -p "20Naei\$\$Rnt2021#1" ssh -o StrictHostKeyChecking=no gfnun@192.168.9.1 pwd >> out.txt'
cmd_get_IPs = 'nmap -n -sn 192.168.9.0/24 -oG - | awk \'/Up$/{print $2}\' >>  ips.txt'
cmd_delete_files = 'rm hostname1.txt hostname2.txt ips.txt cluster_structure_new.txt'

#***************************************************************************************
#***************************************************************************************
cmd_clear()
print('Deleted')
print(cmdline(cmd_get_IPs))
ips = lst_ips()
print('IPs ok')
get_hostname(ips)
print('hostname ok')
clus_ip, clus_names = concatenate()
#print(clus_ip)
#print(clus_names)
lst_pcs = []#[['slave80','nigromanta','00:E0:81:E5:01:86',''],['slave80','nigromanta','00:E0:81:E5:01:86','192.168.9.230'],['slave80','nigromanta','00:E0:81:E5:01:86','192.168.9.230']]
lst_pcs = make_structure(clus_ip,clus_names)
#print(lst_pcs)
#cmd_clear()

sp1 = '                  '
sp2 = '            '
sp3 = '               '

print("================================================================================")
print("| Name in cluster  |   Name PC  |         MAC       |       ip      | Available |")

for v in range(0,len(lst_pcs)):
	if(lst_pcs[v][3]=='000.000.0.00'):
		lst_pcs[v].append('NO ')
	else:
		lst_pcs[v].append('YES')

p=0
for p in range(0,len(lst_pcs)):
	#print(lst_pcs[p][0] + ":::" + lst_pcs[p][1])
  print("|     "+lst_pcs[p][0]+sp1[0:18-(len(lst_pcs[p][0])+5)]+"| "+lst_pcs[p][1]+sp2[0:12-(len(lst_pcs[p][1])+1)]+"| "+lst_pcs[p][2]+" | "+lst_pcs[p][3]+sp3[0:15-(len(lst_pcs[p][3])+1)]+"|     "+lst_pcs[p][4]+"   |")
print("================================================================================")

#|     master      | MrHerbert  | 30:5A:3A:4A:7B:43 | 192.168.9.10  |     YES   |
#|     slave1      | mauricio   | 34:17:EB:C7:E4:90 | 192.168.9.17  |     YES   |
#|     slave6      | nigromanta | 00:25:64:F8:A2:9A | 192.168.9.9   |     YES   |
#18
#12


#f = len(clus_names)
#print("Longitud " + str(f))
#for c in clus_names:
#		print(c)

#**************************************************************************************
#**************************************************************************************

#print(cmdline(cmd_test))

#print(cmdssh('192.0.0.0'))

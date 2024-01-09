# Simple dehasher by Thomas Aranda Verstraete

## Prerequisites:

	- Verify the presence of the necessary packages; otherwise, the installation commands for Debian are as follows:

sudo apt-get update
sudo apt install build-essential
sudo apt install libssl-dev

	- Two virtual machines on an internal network (make sure to install the necessary files beforehand).
Recommended IP addresses: (both machines should be able to ping each other).

	- Server: 192.168.1.10
	- Client: 192.168.1.11

## Instructions:

	1. Place each file (client and server) on their respective machines.

	2. Compilation:

	 - Server: `gcc -o main main.c`
	 - Client: `gcc -o main main.c -lssl -lcrypto`

	3. Startup (begin with the server):

	- Server: `./main 192.168.1.11 8080`
 	- Client: `./main 192.168.1.10 8080`

	4. Choose one of the files containing hashes included in the /server directory:

 	- `hashesdemo.txt`: Demonstration file with 10 easy-to-find combinations.
 	- `hashesfull.txt`: File with 1000 combinations of uppercase, lowercase, and numeric characters possible with 5 characters.

## Disclaimer:

	This project is reserved for educational purposes only. The creator is not responsible for any misuse of this code.

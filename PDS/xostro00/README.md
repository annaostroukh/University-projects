
PDS (Data Communications, Computer Networks and Protocols)
-------------
**Task - TCPstats**

The task of this project is to create an application which analyzes TCP stream, computes particular statistics and presents them in an appropriate format on a web page. 
There is no limitation on implementation language in this project though the application has to be able to run on a reference test OS (Ubuntu 14.04). A part of the application which generates statistics has to be separated from a part which generates a web page (the web page has just render the computed statistics).

The application has to be able to show the following information:

 - How the sender/receiver window was changing during the connection?
 - What speed was during the connection? 
 - What round trip time was?
 -  Analysis of sequence numbers: slow - start

Input format:
    

> tcpstats <*file*>

For simplicity you have to consider that on input will be just one TCP stream, the format of this stream is pcapng/pcap file. 
The output of the project is a web page xlogin00.html/xhtml/xml. This page has to be a project documentation as well - list there which technologies you used in your project (implementation language, etc.), information about TCP connection - overall statistics (amount of packets, etc.) and output statistics.

Application usage:

 1. Run tcpstats application with a TCP stream file on input which will generate log file
 2. Opening the web page which shows general information and builds graphs from the log file


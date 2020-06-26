執行
======
    gcc httpserver.c  
    ./a.out  
 程式會以儲存目錄位置作為根目錄  
 
 執行結果
 ======
    [root@vm1 html]# ls  
    a.out  http.c  image.png  index2.html  index.html  pic.jpg  server.c
    
    [root@vm1 html]# ifconfig enp0s8
    enp0s8: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
            inet 192.168.56.115  netmask 255.255.255.0  broadcast 192.168.56.255
            inet6 fe80::dd1e:9c91:c428:1983  prefixlen 64  scopeid 0x20<link>
            ether 08:00:27:4c:ec:61  txqueuelen 1000  (Ethernet)
            RX packets 7562  bytes 565898 (552.6 KiB)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 11204  bytes 19265159 (18.3 MiB)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
            
    [root@vm1 html]# curl 192.168.56.115
    hello world!
    [root@vm1 html]# curl 192.168.56.115/index.html
    hello world!
    [root@vm1 html]# curl 192.168.56.115/index2.html
    <p>hello world!</p>
    <p><img src="pic.jpg" /></p>
    <p><img src="image.png" /></p>
    <p>It's a good day.<p>

chrome瀏覽器執行  
![Alt text](https://i.imgur.com/zwJ5tll.png)


 
 
 
參考資料 
======
部分原始碼來自底下程式或範例  
Beej's Guide to Network Programming 正體中文版  
<http://beej-zhtw.netdpi.net/09-man-manual/9-2-bind>  
親手打造 HTTP 網路服務：超小 Web Server 的撰寫  
<https://fred-zone.blogspot.com/2007/09/http-web-server.html>  
Program : nweb.c  
<https://people.cs.nctu.edu.tw/~cjtsai/courses/ics/homework/nweb.c>

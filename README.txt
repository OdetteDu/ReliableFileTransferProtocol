Reliable File Transfer Protocol

By Yanfei Wu, Yang Du, Kai Wu, Cheng Wan

Possible Steps:
Send 1 packet and wait until the ack arrives
1. 搞定1个文件完全木有错误的情况下，1对1的发送
2. 把文件切开，切N块然后发送
3. 切完以后，如果send的顺序不一样，重组了发送
4. 用CRC来检查packet是否出错，然后把出错的扔掉，然后重发
Window 阶段
5.开始implement window slide
6.先写个pingpong，然后算出delay
7.然后根据公式算出window大小 
8.最后搞定window，解决timeout的问题
9.然后就是congestion control要不要做是个问题

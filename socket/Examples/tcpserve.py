def sockServer():
    # 创建一个socket
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    # 监听端口
    s.bind(('127.0.0.1',9999))
    # 等待连接的最大数量
    s.listen(5)

    # 具体处理连接的方法
    def tcplink(sock,addr):
        print('Accept new connection from {0}:{1}'.format(addr[0],addr[1]))
        # 发送欢迎信息，对应下面客户端的接受欢迎信息
        sock.send(b'Welcome...')
        while True:
            # 接收从客户端发过来的信息
            data = sock.recv(1024)
            time.sleep(1)
            # 如果没有信息或者信息为exit，则推出循环
            if not data or data.decode('utf-8') == 'exit':
                break
            # 发送Hello信息
            sock.send(('Hello, {0}'.format(data.decode('utf-8'))).encode('utf-8'))
        # 关闭连接
        sock.close()
        # 打印连接地址和端口
        print('Connection from {0}:{1}'.format(addr[0],addr[1]))

    print('Waiting for connection...')
    while True:
        # 接受连接并返回（sock,address）,其中sock是新的套接字对象，可以用来接收和发送数据。address是连接客户端的地址
        sock,addr = s.accept()
        # 线程的方式
        t = threading.Thread(target=tcplink,args=(sock,addr))
        t.start()

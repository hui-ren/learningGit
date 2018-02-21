 # client
def sockClient(url,port=9999):
    # 创建一个socket
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    # 连接到服务器
    s.connect((url,port))
    # 接受从服务器上发送的欢迎信息
    print(s.recv(1024).decode('utf-8'))
    for data in [b'Michael',b'Tracy',b'Sarah']:
        # 发送数据到服务器
        s.send(data)
        # 打印出从服务器返回的Hello信息
        print(s.recv(1024).decode('utf-8'))
    # 发送exit
    s.send(b'exit')
    # 关闭连接
    s.close()

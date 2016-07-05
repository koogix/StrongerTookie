Socket服务
===

* net

    tcp协议的socket服务，及其连接的客户端实现

* net/ws

	websocket服务实现
	
	
简要描述：
---
    使用时需要开启PHP的sockets扩展，一个简单的socket服务器类！
    对应其中的 ws 文件夹内包含了websocket协议服务的实现，
    当不想使用php的swoole、workman这些大块头来实现websocket的时候，
    可以考虑选择这个类库，其中，可以使用libevent扩展实现socket监听，
    只使用ws文件夹里的Protocol.php及DataFrame.php文件来实现websocket。

使用说明：
---
    * （1）包含 src/loader.php 文件，调用 KoogixAutoloader::reg()
           注册类的自动加载，或使用自己的autoload方法使用能加载src中的php文件
    * （2）使用 koogix\net\Service::factory()->on()...->ws()->run();
           其中 factory() 中指定服务器ip及端口，
           对应 on() 方法绑定回调事件，
           run() 开始服务（TCP socket服务），
           中间调用 ws() 指定使用 websocket 协议，这样启动的时websocket服务

实现例程：
--- 
    具体参看 examples 文件夹中的文件
    on() 方法可以指定三种事件的回调
    * （1）Session::CONNECTED     连接成功
    * （2）Session::RECIEVED      收到数据
    * （3）Session::DISCONNECTED  断开连接
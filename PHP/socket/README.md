Socket����
===

* net

    tcpЭ���socket���񣬼������ӵĿͻ���ʵ��

* net/ws

	websocket����ʵ��
	
	
��Ҫ������
---
    ʹ��ʱ��Ҫ����PHP��sockets��չ��һ���򵥵�socket�������࣡
    ��Ӧ���е� ws �ļ����ڰ�����websocketЭ������ʵ�֣�
    ������ʹ��php��swoole��workman��Щ���ͷ��ʵ��websocket��ʱ��
    ���Կ���ѡ�������⣬���У�����ʹ��libevent��չʵ��socket������
    ֻʹ��ws�ļ������Protocol.php��DataFrame.php�ļ���ʵ��websocket��

ʹ��˵����
---
    * ��1������ src/loader.php �ļ������� KoogixAutoloader::reg()
           ע������Զ����أ���ʹ���Լ���autoload����ʹ���ܼ���src�е�php�ļ�
    * ��2��ʹ�� koogix\net\Service::factory()->on()...->ws()->run();
           ���� factory() ��ָ��������ip���˿ڣ�
           ��Ӧ on() �����󶨻ص��¼���
           run() ��ʼ����TCP socket���񣩣�
           �м���� ws() ָ��ʹ�� websocket Э�飬����������ʱwebsocket����

ʵ�����̣�
--- 
    ����ο� examples �ļ����е��ļ�
    on() ��������ָ�������¼��Ļص�
    * ��1��Session::CONNECTED     ���ӳɹ�
    * ��2��Session::RECIEVED      �յ�����
    * ��3��Session::DISCONNECTED  �Ͽ�����
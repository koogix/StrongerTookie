<?php
	/**
	 * socket服务类
	 *
	 * 实现了基于TCP协议的基本SOCKET服务流程
	 * 可以通过使用 setProvider() 方法
	 * 设置不同协议的实现，如 websocket
	 * 
	 * 使用 Session 类保存客户端连接相关信息
	 * 使用 StreamBuf 类读取发送数据
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */
	
	namespace koogix\net;

	/**
	 * Socket服务类
	 *
	 * 使用 factory() 方法创建服务监听
	 * 使用 on() 方法设置回调函数
	 * 使用 run() 启动服务
	 * 使用 stop() 停止服务
	 *
	 */
	Class Service
	{
		private static $arrInstance;

		private $connections = array();
		private $options = array(
			'backlog' => 5,
			'timeout' => 5
		);
		private $host    = '0.0.0.0';
		private $port    = 50018;
		private $running = false;

		private $sessionProvider = null;

		private $onConnected    = null;
		private $onRecieved     = null;
		private $onDisconnected = null;

		/**
		 * 创建服务对象
		 *
		 * 同个ip与端口只能启动一个实例
		 *
		 * @static 
		 *
		 * @param  string $host    服务器ip地址
		 * @param  number $port    监听端口
		 * @param  array  $options 选项，目录只有 backlog 与 timeout 两个
		 * @return Service         服务实例对象
		 */
		public static function& factory($host = '0.0.0.0', $port = 50018, $options = array())
		{
			$cckey = md5(sprintf('%s:%s', $host, $port));
			if (!isset($arrInstance[$cckey]))
			{
				self::$arrInstance[$cckey] = new self($host, $port, $options);
			}
			return self::$arrInstance[$cckey];
		}

		/**
		 * 设置Session类实现
		 *
		 * 所提供的类继承于 Session 类
		 *
		 * @param  string $classname Session实现类名
		 * @return Service           服务实例对象
		 */
		public function setProvider($classname)
		{
			$this->sessionProvider = new \ReflectionClass($classname);
			return $this;
		}

		/**
		 * 设置websocket服务标志
		 *
		 * 使用websocket实现的Session类实现websocket服务
		 *
		 * @return Service  服务实例对象
		 */
		public function ws()
		{
			return $this->setProvider('koogix\net\ws\Session');
		}

		/**
		 * 设置回调事件
		 *
		 * 回调事件有连接成功、收到数据、断开连接三类事件
		 *
		 * @param  string  $etype    事件名称
		 * @param  Closure $callback 回调方法
		 * @return Service           服务实例对象
		 */
		public function on($etype, $callback)
		{
			if ($etype == Session::CONNECTED)
			{
				$this->onConnected = $callback;
			}
			else if ($etype == Session::RECIEVED)
			{
				$this->onRecieved = $callback;
			}
			else if ($etype == Session::DISCONNECTED)
			{
				$this->onDisconnected = $callback;
			}
			return $this;
		}

		/**
		 * 启动服务
		 *
		 * @return void
		 */
		public function run()
		{
			while($this->running)
			{
				$this->loop();
			}
		}

		/**
		 * 停止服务
		 *
		 * @return void
		 */
		public function stop()
		{
			$this->running = false;
		}

		/**
		 * 释构函数，断开连接
		 *
		 * @return void
		 */
		public function __destruct()
		{
			foreach ($this->connections as $key => $session)
			{
				unset($session);
			}
			if ($this->socket)
			{
				@socket_close($this->socket);
			}
		}

		/**
		 * 构造函数，初始化socket及启动监听
		 *
		 * 不能直接new创建服务，只能通过factory()方法创建 
		 *
		 * @param  string $host    服务器ip地址
		 * @param  number $port    监听端口
		 * @param  array  $options 选项，目录只有 backlog 与 timeout 两个
		 */
		private function __construct($host, $port, $options)
		{
			$this->options = array_merge($this->options, $options);
			$this->host = $host;
			$this->port = $port;

			$this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
			if ($this->socket === false)
			{
				throw new Exception('Socket create failure');
			}
			if (socket_set_option($this->socket, SOL_SOCKET, SO_REUSEADDR, 1) === false)
			{
				Exception::newThrow($this->socket);
			}
			if (socket_set_nonblock($this->socket) === false)
			{
				Exception::newThrow($this->socket);
			}
			if (socket_bind($this->socket, $this->host, $this->port) === false)
			{
				Exception::newThrow($this->socket);
			}
			if (socket_listen($this->socket, $this->options['backlog']) === false)
			{
				Exception::newThrow($this->socket);
			}
			$this->running = true;
		}

		/**
		 * 服务主体循环方法
		 *
		 * 接收请求及数据等相关处理
		 * 触发对应的回调事件
		 *
		 * @return void
		 */
		private function loop()
		{
			$arrRead = array(
				$this->socket
			);
			$arrWrite  = null;
			$arrExcept = null;
			foreach ($this->connections as $key => $session)
			{
				$arrRead[] = $session->socket;
			}
			$changes = socket_select($arrRead, $arrWrite, $arrExcept, $this->options['timeout']);
			if ($changes === false)
			{
				Exception::newThrow($this->socket);
			}
			if ($changes == 0)
			{
				return;
			}
			foreach ($arrRead as $socket)
			{
				if ($socket == $this->socket)
				{
					$this->doAccept();
					continue;
				}
				$this->doRead($socket);
			}
		}

		/**
		 * 接受连接请求
		 *
		 * @return void
		 */
		private function doAccept()
		{
			$client = @socket_accept($this->socket);
			if ($client === false)
			{
				return Exception::display($this->socket);
			}
			$sessid = $this->genid($client);
			if ($this->sessionProvider == null)
			{
				$this->setProvider('koogix\net\Session');
			}
			$this->connections[$sessid] = $this->sessionProvider->newInstance($client, $sessid);
			if ($this->onConnected == null)
			{
				return;
			}
			$closure = $this->onConnected;
			$closure($this->connections[$sessid], $this->connections);
		}

		/**
		 * 读取缓冲区数据
		 *
		 * @param  Resource $socket 网络连接句柄
		 * @return void
		 */
		private function doRead($socket)
		{
			$sessid = $this->genid($socket);
			$stream = $this->connections[$sessid]->read();
			if ($stream === false)
			{
				if ($this->connections[$sessid]->valid() === false)
				{
					$closure = $this->onDisconnected;
					$closure(
						$this->connections[$sessid],
						$this->connections
					);
					unset($this->connections[$sessid]);

				}
				return;
			}
			if ($this->onRecieved == null)
			{
				return;
			}
			$closure = $this->onRecieved;
			$closure($stream, $this->connections[$sessid], $this->connections);
		}

		/**
		 * 生成 session id 值
		 *
		 * @param  Resource $socket 网络连接句柄
		 * @return void
		 */
		private function genid($socket)
		{
			return md5(strval($socket));
		}
	}

?>
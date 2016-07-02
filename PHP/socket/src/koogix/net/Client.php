<?php
	/**
	 * socket客户端类
	 *
	 * 实现了基于TCP协议的基本SOCKET客户端流程
	 *
	 * 使用 Session 类保存客户端连接相关信息
	 * 使用 StreamBuf 类读取发送数据
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */

	namespace koogix\net;

	/**
	 * socket客户端类
	 *
	 * 使用 factory() 方法创建服务监听
	 * 使用 on() 方法设置回调函数
	 * 使用 run() 启动服务
	 * 使用 stop() 停止服务
	 *
	 */
	class Client
	{

		private static $arrInstance;

		private $session = null;

		private $host    = '127.0.0.1';
		private $port    = 50018;
		private $running = false;

		private $onConnected    = null;
		private $onRecieved     = null;
		private $onDisconnected = null;

		/**
		 * 创建客户端连接对象
		 *
		 * 指定服务端ip与端口连接
		 *
		 * @static 
		 *
		 * @param  string $host    服务器ip地址
		 * @param  number $port    服务器端口
		 * @return Service         服务实例对象
		 */
		public static function& factory($host = '127.0.0.1', $port = 50018)
		{
			$cckey = md5(sprintf('%s:%s', $host, $port));
			if (!isset($arrInstance[$cckey]))
			{
				self::$arrInstance[$cckey] = new self($host, $port);
			}
			return self::$arrInstance[$cckey];
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
		 * 发送数据
		 *
		 * 通过socket连接发送数据
		 *
		 * @param  StreamBuf  $content  对象或字符串
		 * @return bool
		 */
		public function send($content)
		{
			return $this->session->send($strmbuf);
		}

		/**
		 * 连接服务器并运行
		 *
		 * @return void
		 */
		public function run()
		{
			if (@socket_connect($this->socket, $this->host, $this->port) === false)
			{
				Exception::newThrow($this->socket);
			}
			$this->session = new Session($this->socket);
			if ($this->onConnected != null)
			{
				$closure = $this->onConnected;
				$closure($this->session);
			}
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
		 * 客户端主体循环方法
		 *
		 * 接收数据等相关处理
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
			$changes = socket_select($arrRead, $arrWrite, $arrExcept, 5);
			if ($changes === false)
			{
				Exception::newThrow($this->socket);
			}
			if ($changes == 0)
			{
				return;
			}
			$stream = $this->session->read();
			if ($stream === false)
			{
				if ($this->session->valid() === false)
				{
					$closure = $this->onDisconnected;
					$closure($this->session);
				}
				return;
			}
			if ($this->onRecieved == null)
			{
				return;
			}
			$closure = $this->onRecieved;
			$closure($stream, $this->session);
		}

		/**
		 * 释构函数，断开连接
		 *
		 * @return void
		 */
		public function __destruct()
		{
			if ($this->socket)
			{
				@socket_close($this->socket);
			}
		}

		/**
		 * 构造函数，初始化socket
		 *
		 * 不能直接new创建连接，只能通过factory()方法创建 
		 *
		 * @param  string $host    服务器ip地址
		 * @param  number $port    服务器端口
		 */
		private function __construct($host, $port)
		{
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
			$this->running = true;
		}
	}


?>
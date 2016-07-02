<?php
	/**
	 * Session类
	 * 
	 * 用于存放连接相关信息
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */
	
	namespace koogix\net;

	class Session
	{
		// ~ max buffer size
		const READMAXSIZE = 1024;

		// ~ event defines
		const CONNECTED    = 'Session::conn';	// ~ 连接成功
		const RECIEVED     = 'Session::recv';	// ~ 收到数据
		const DISCONNECTED = 'Session::close';	// ~ 断开连接

		/**
		 * 网络连接句柄
		 * 
		 * 声明为公有方便使用
		 * @public
		 */
		public $socket = null;
		/**
		 * 辅助数据对象
		 * 
		 * 声明为公有方便使用
		 * @public
		 *
		 * 如设置用户姓名可以直接使用
		 * $sess->data->username = '';
		 */
		public $data   = null;

		protected $buffer = '';
		protected $status = true;

		private $_id   = 0;
		private $_host = '';
		private $_port = '';

		/**
		 * 构造函数
		 *
		 * @param Resource $socket  网络连接句柄
		 * @param string   $sessid  Session id值
		 */
		public function __construct($socket, $sessid = 0)
		{
			if (@socket_getpeername($socket, $this->_host, $this->_port) === false)
			{
				Exception::display($this->socket);
			}
			if (empty($sessid))
			{
				$sessid = md5(strval($socket));
			}
			$this->_id    = $sessid;
			$this->socket = $socket;
			$this->status = true;
			$this->data   = new \stdClass;
		}

		/**
		 * 释构函数
		 *
		 * 断开连接
		 *
		 */
		public function __destruct()
		{
			if ($this->socket)
			{
				$this->close();
			}
		}

		/**
		 * session id 值
		 *
		 * @return string   Session id值
		 */
		public function id()
		{
			return $this->_id;
		}

		/**
		 * ip 地址
		 *
		 * @return string  IP地址
		 */
		public function host()
		{
			return $this->_host;
		}

		/**
		 * 端口
		 *
		 * @return number  连接端口
		 */
		public function port()
		{
			return $this->_port;
		}

		/**
		 * 是否有效
		 *
		 * Session是否还有效
		 * 若服务异常或连接断开
		 * 则session变为无效状态
		 *
		 * @return bool  是否有效
		 */
		public function valid()
		{
			return $this->status;
		}

		/**
		 * 读取数据
		 *
		 * 通过socket读取数据
		 *
		 * @return StreamBuf  数据对象，若失败返回false
		 */
		public function read()
		{
			$this->buffer = '';
			$rdbuf = '';
			while (true)
			{
				$bytes = @socket_recv($this->socket, $rdbuf, self::READMAXSIZE, 0);
				if ($bytes === false)
				{
					$errno = socket_last_error($this->socket);
					if ($errno === 0)
					{
						if (empty($this->buffer))
						{
							$this->status = false;
						}
						break;
					}
					Exception::display($this->socket);
					break;
				}
				$this->buffer .= $rdbuf;
				if ($bytes < self::READMAXSIZE) break;
			}
			if (empty($this->buffer))
			{
				return false;
			}
			return new StreamBuf($this->buffer);
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
			$buffer = is_string($content)
						? $content
						: $content->data();
			$length = strlen($buffer);
			$bytes = @socket_send($this->socket, $buffer, $length, 0);
			if ($bytes === false)
			{
				$errno = socket_last_error($this->socket);
				if ($errno === 0)
				{
					return true;
				}
				Exception::display($this->socket);
				return false;
			}
			return true;
		}

		/**
		 * 断开socket连接
		 *
		 * @return void
		 */
		public function close()
		{
			@socket_close($this->socket);
		}

	}


?>
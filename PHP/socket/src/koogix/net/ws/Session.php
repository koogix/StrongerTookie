<?php
	/**
	 * Websocket协议的Session类实现
	 *
	 * websocket连接除了tcp连接请求外
	 * 还需websocket自动的握手验证
	 * 
	 * 通过验证后
	 * 对应websocket的报文有特殊的格式要求
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */
	
	namespace koogix\net\ws;

	use koogix\net\Session as NetSession;
	use koogix\net\StreamBuf;

	/**
	 * Websocket的Session类实现
	 *
	 * websocket连接时，使用Protocol类验证握手信息
	 * 成功后，使用DataFrame类来解释或打包传输数据
	 * 
	 */
	class Session extends NetSession
	{
		/**
		 * 是否已通过websocket的握手验证
		 * 
		 * @var boolean
		 */
		private $isReady = false;
		private $options = array();

		/**
		 * 读取数据
		 *
		 * 通过websocket读取数据
		 * 注意，此时返回的数据非报文中的数据
		 * 而是经过DataFrame解包解释出来的数据
		 *
		 * @return StreamBuf  数据对象，若失败返回false
		 */
		public function read()
		{
			$strmbuf = parent::read();
			if ($strmbuf === false)
			{
				return false;
			}
			if ($this->isReady == false && $this->ready($strmbuf->data()) === false)
			{
				return false;
			}
			$this->options = array();
			$data = DataFrame::unpack($strmbuf->data(), $this->options);
			if ($data == 'PING')
			{
				$this->send('PONG');
				return false;
			}
			else if ($this->options['isPing'])
			{
				parent::send(
					new StreamBuf(
						$package,
						DataFrame::OPCODE_PONG
					)
				);
				return false;
			}
			else if ($this->options['isClose'])
			{
				$this->close();
				$this->status = false;
				return false;
			}
			return new StreamBuf($data);
		}

		/**
		 * 发送数据
		 *
		 * 通过websocket连接发送数据
		 * 对应数据内容会自动通过DataFrame打包后发送
		 *
		 * @param  StreamBuf  $content  对象或字符串
		 * @return bool
		 */
		public function send($content)
		{
			if (! is_string($content))
			{
				$content = $content->data();
			}
			$data = DataFrame::pack($content);
			$strm = new StreamBuf($data);
			return parent::send($strm);
		}

		/**
		 * 数据内容是否为二进制流
		 *
		 * 通过websocket协议中的opcode位判断
		 * 
		 * @return bool
		 */
		public function isBinary()
		{
			if (!isset($this->options['isBinary']))
			{
				return false;
			}
			return $this->options['isBinary'];
		}

		/**
		 * 是否成功握手验证
		 *
		 * 若否，则使用Protocol类
		 * 来验证Websocket的头信息
		 * 及生成对应的响应头信息
		 * 
		 * @return bool  
		 */
		private function ready($buffer)
		{
			if ($this->isReady)
			{
				return true;
			}
			$proto = new Protocol($buffer);
			if ($proto->parse() === false)
			{
				return false;
			}
			$output = $proto->response();
			$outbuf = new StreamBuf($output);
			if (parent::send($outbuf))
			{
				$this->isReady = true;
			}
			return false;
		}
	}


?>
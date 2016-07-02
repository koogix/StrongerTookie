<?php
	/**
	 * Websocket协议
	 *
	 * 实现websocket的握手验证阶段
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */
	namespace koogix\net\ws;


	/**
	 * Websocket协议
	 *
	 */
	class Protocol
	{
		const SALTCODE = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11';

		const SECKEY   = 'Sec-WebSocket-Key';
		const SECEXT   = 'Sec-WebSocket-Extensions';
		const PROTOCOL = 'Sec-WebSocket-Protocol';
		const VERSION  = 'Sec-WebSocket-Version';
		const ORIGIN   = 'Origin';

		const HEADEOL  = "\r\n";

		private $databuf = array();
		private $options = array();

		private $answers = array(
			'protocol'   => 'HTTP/1.1 101 Switching Protocols',
			'Server'     => 'WenQiang LEE Websocket Server',
			'Upgrade'    => 'Websocket',
			'Connection' => 'Upgrade',
			'Date'		 => '',
			'Access-Control-Allow-Credentials' => 'true',
			'Access-Control-Allow-Headers'     => 'content-type',
			'Sec-WebSocket-Accept'             => ''
		);

		public function __construct($content)
		{
			$this->databuf = explode(self::HEADEOL, $content);
		}

		public function parse()
		{
			foreach ($this->databuf as $line)
			{
				$line = trim($line);
				if (empty($line))
				{
					continue;
				}
				$offset = strpos($line, ':');
				if ($offset === false)
				{
					continue;
				}
				$key = trim(substr($line, 0, $offset));
				$val = trim(substr($line, $offset + 1));
				$this->options[$key] = $val;
			}
			if (   isset($this->options[self::SECEXT])
				&& isset($this->options[self::SECKEY])
				&& ! empty($this->options[self::SECEXT])
				&& ! empty($this->options[self::SECKEY])
			) {
				return true;
			}
			return false;
		}

		public function response()
		{
			$data   = array();
			foreach ($this->answers as $key => $val)
			{
				if ($key == 'protocol')
				{
					$data[] = $val;
					continue;
				}
				$tmp = array($key, $val);
				if ($key == 'Date')
				{
					$tmp[1] = date('r');
				}
				else if ($key == 'Sec-WebSocket-Accept')
				{
					$tmp[1] = $this->gen();
				}
				$data[] = implode(': ', $tmp);
			}
			foreach (array(self::ORIGIN, self::PROTOCOL, self::VERSION) as $key)
			{
				if (isset($this->options[$key]))
				{
					$data[] = implode(': ', array($key, $this->options[$key]));
				}
			}
			$data[] = self::HEADEOL;
			return  implode(self::HEADEOL, $data);
		}

		private function gen()
		{
			$seckey = $this->options[self::SECKEY];
			$string = $seckey . self::SALTCODE;
			$encode = sha1($string, true);
			$result = base64_encode($encode);
			return $result;
		}
	}

?>
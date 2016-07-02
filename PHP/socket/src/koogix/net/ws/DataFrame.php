<?php
	/**
	 * Websocket数据报文格式
	 *
	 * 
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */

	namespace koogix\net\ws;

	/**
	 * ************************************************************
	 * 第一字节 * **********************
	 * *   PIN    1位
	 * *   RSV    3位
	 * *   OPCODE 4位 
	 * 
	 * 第二字节 * **********************
	 * *   MASK         1位，是否加密数据，默认必须置为1
	 * *   PAYLOAD LEN  7位，数据长度
	 *                       当值为 126 时，后面两字节表示长度（7E）
	 *                       当值为 127 时，后面八字节表示长度（7F）
	 * *****
	 * 表示长度的字节（根据 payload len 来确定）
	 * *****
	 * 
	 * 下一字节 * **********************
	 * *   MASKING-KEY  MASK位为1时，4字节掩码
	 * 
	 * 数据内容 * **********************
	 * *    PAYLOAD DATA     由 PAYLOAD LEN 指定长度
	 * 
	 * **************************************************************
	 */

	/**
	 * WebSocket 数据包辅助类
	 *
	 *
	 */
	class DataFrame
	{
		// ~ opcode
		const OPCODE_CONNECT = 0;		// 连接
		const OPCODE_TEXT    = 1;		// 文本
		const OPCODE_BINARY  = 2;		// 二进制流
		const OPCODE_CLOSE   = 8;		// 断开
		const OPCODE_PING    = 9;		// PING
		const OPCODE_PONG    = 10;		// PONG

		// ~ 掩码位
		const MASK_BIT = 0x80;



		public static function unpack($data, &$options = null)
		{
			$datalen = strlen($data);
			if ($datalen < 2)
			{
				return false;
			}
			$len = (int) (ord($data[1]) & 0x7F);
			$idx = 2;
			$length = 0;
			if ($len == 127)
			{
				$length = (ord($data[2]) << 56)
						| (ord($data[3]) << 48)
						| (ord($data[4]) << 40)
						| (ord($data[5]) << 32)
						| (ord($data[6]) << 24)
						| (ord($data[7]) << 16)
						| (ord($data[8]) <<  8)
						|  ord($data[9]);
				$idx = 10;
			}
			else if ($len == 126)
			{
				$length = (ord($data[2]) << 8)
						 | ord($data[3]);
				$idx = 4;
			}
			else
			{
				$length = $len;
				$idx = 2;
			}
			$options = array
			(
				'isClose'  => ($data[0] & self::OPCODE_CLOSE)  == self::OPCODE_CLOSE,
		 		'isPing'   => ($data[0] & self::OPCODE_PING)   == self::OPCODE_PING,
		 		'isPong'   => ($data[0] & self::OPCODE_PONG)   == self::OPCODE_PONG,
				'isText'   => ($data[0] & self::OPCODE_TEXT)   == self::OPCODE_TEXT,
				'isBinary' => ($data[0] & self::OPCODE_BINARY) == self::OPCODE_BINARY,
				'isMasked' => ($data[1] & self::MASK_BIT) == self::MASK_BIT,
				'length'   => $length,
				'maskcode' => array()
			);
			for ($i = 0; $i < 4; $i++)
			{
				$options['maskcode'][] = ord($data[$idx + $i]);
			}
			$dataoffset = $idx + 4;
			$databuffer = '';
			for ($i = 0; $i < $length; $i++)
			{
				$idx = $dataoffset + $i;
				$bin = ord($data[$idx]) ^ ($options['maskcode'][$i % 4]);
				$databuffer .= $options['isBinary'] ? $bin : chr($bin);
			}
			return $databuffer;
		}

		public static function pack($output, $opcode = false, $isBinary = false)
		{
			for ($i = 0; $i < 4; $i++)
			{
				$mask[] = mt_rand(0, 255);
			}
			$len = strlen($output);
			$buf = array();
			for ($i = 0; $i < $len; $i++)
			{
				if ($isBinary)
				{
					$buf[] = chr($output[$i] ^ ($mask[$i % 4]));
				}
				else
				{
					$buf[] = chr(ord($output[$i]) ^ ($mask[$i % 4]));
				}
			}
			for ($i = 3; $i >= 0; $i--)
			{
				array_unshift($buf, chr($mask[$i]));
			}
			if ($len > 65535)
			{
				array_unshift($buf, chr( $len & 0x00000000000000FF));
				array_unshift($buf, chr(($len & 0x000000000000FF00) >>  8));
				array_unshift($buf, chr(($len & 0x0000000000FF0000) >> 16));
				array_unshift($buf, chr(($len & 0x00000000FF000000) >> 24));
				array_unshift($buf, chr(($len & 0x000000FF00000000) >> 32));
				array_unshift($buf, chr(($len & 0x0000FF0000000000) >> 40));
				array_unshift($buf, chr(($len & 0x00FF000000000000) >> 48));
				array_unshift($buf, chr(($len & 0xFF00000000000000) >> 56));
				array_unshift($buf, chr(0xFF));	// 0x80 | 0x7F
			}
			else if ($len > 127)
			{
				array_unshift($buf, chr( $len & 0x000000FF));
				array_unshift($buf, chr(($len & 0x0000FF00) >>  8));
				array_unshift($buf, chr(($len & 0x00FF0000) >> 16));
				array_unshift($buf, chr(0xFE));	// 0x80 | 0x7E
			}
			else
			{
				array_unshift($buf, chr(self::MASK_BIT | $len));
			}
			if ($opcode === false)
			{
				$opcode = $isBinary ? self::OPCODE_BINARY : self::OPCODE_TEXT;
			}
			array_unshift($buf, chr(0x80 | $opcode));
			return implode('', $buf);
		}


	}

?>
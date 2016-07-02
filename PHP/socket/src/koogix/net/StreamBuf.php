<?php
	/**
	 * 数据缓冲类
	 * 
	 * 用于Session类中读取及发送数据
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */

	namespace koogix\net;

	/**
	 * 数据缓冲
	 *
	 * 用于数据的接收与发送
	 * 其中辅助函数 hexdump() 可以方便输出16进数据查看数据包
	 * 
	 */
	class StreamBuf
	{

		protected $databuf = '';
		protected $datalen = -1;

		/**
		 * 构造函数
		 *
		 * @param string $content 数据内容
		 */
		public function __construct($content)
		{
			$this->databuf = $content;
			$this->datalen = strlen($this->databuf);
		}

		/**
		 * 获取数据内容
		 *
		 * @return string 数据内容
		 */
		public function data()
		{
			return $this->databuf;
		}

		/**
		 * 获取数据长度
		 *
		 * @return number 数据长度
		 */
		public function length()
		{
			return $this->datalen;
		}

		/**
		 * 打印输出16进制数据
		 * 
		 * @return void
		 */
		public function hexdump()
		{
			printf('%s------------- HEX DUMP -----------------', PHP_EOL);
			for ($i = 0; $i < $this->datalen; $i++)
			{
				if ($i % 8 == 0)
				{
					printf(PHP_EOL);
				}
				printf("%02X ", ord($this->databuf[$i]));
			}
			printf('%s------------- FINISHED -----------------%s', PHP_EOL, PHP_EOL);
		}


	}

?>